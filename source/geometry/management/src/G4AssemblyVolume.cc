//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4AssemblyVolume.cc,v 1.6 2001-07-11 09:59:20 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Class G4AssemblyVolume - implementation
//
// ----------------------------------------------------------------------

#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4AffineTransform.hh"
#include "G4AssemblyVolume.hh"

#include "g4std/strstream"

unsigned int G4AssemblyVolume::fsInstanceCounter = 0;

// Default constructor
//
G4AssemblyVolume::G4AssemblyVolume()
{
  InstanceCountPlus();
  SetImprintsCount( 0 );
}

// Destructor
//
G4AssemblyVolume::~G4AssemblyVolume()
{
  fTriplets.clear();
  
  unsigned int howmany = fPVStore.size();
  if( howmany != 0 )
  {
    for( unsigned int i = 0; i < howmany; i++ )
    {
      G4RotationMatrix* pRotToClean = fPVStore[i]->GetRotation();
      if( pRotToClean != 0 ) delete pRotToClean;
      delete fPVStore[i];
    }
  }
  
  fPVStore.clear();
  InstanceCountMinus();
}

// Add and place the given volume according to the specified
// translation and rotation.
//
void G4AssemblyVolume::AddPlacedVolume( G4LogicalVolume*  pVolume,
                                        G4ThreeVector&    translation,
                                        G4RotationMatrix* pRotation )
{
  G4AssemblyTriplet toAdd( pVolume, translation, pRotation );
  fTriplets.push_back( toAdd );
}

// Add and place the given volume according to the specified transformation
//
void G4AssemblyVolume::AddPlacedVolume( G4LogicalVolume*  pVolume,
                                        G4Transform3D&    transformation )
{
  G4ThreeVector v   = transformation.getTranslation();
  G4RotationMatrix r = transformation.getRotation();
  G4AssemblyTriplet toAdd( pVolume, v, &r );
  fTriplets.push_back( toAdd );
}

/**
 * Create an instance of an assembly volume inside of the specified
 * mother volume. This works analogically to making stamp imprints.
 * This method makes use of the Geant4 affine transformation class.
 * The algorithm is defined as follows:
 * 
 * Having rotation matrix Rm and translation vector Tm to be applied
 * inside the mother and rotation matrix Ra and translation vector Ta
 * to be applied inside the assembly itself for each of the participating
 * volumes the resulting transformation is
 *
 * Tfinal = Ta * Tm
 *
 * where Ta and Tm are constructed as
 *
 *       -1                                     -1
 * Ta = Ra  * Ta           and            Tm = Rm  * Tm
 *
 * which in words means that we create first the affine transformations
 * by inverse rotation matrices and translations for mother and assembly.
 * The resulting final transformation to be applied to each of the
 * participating volumes is their product.
 * IMPORTANT NOTE!
 * The order of multiplication is reversed when comparing to CLHEP 3D
 * transformation matrix(G4Transform3D class).
 */
void G4AssemblyVolume::MakeImprint( G4LogicalVolume*  pMotherLV,
                                    G4ThreeVector&    translationInMother,
                                    G4RotationMatrix* pRotationInMother )
{
  unsigned int        numberOfDaughters = pMotherLV->GetNoDaughters();

  // We start from the first available index
  numberOfDaughters++;

  ImprintsCountPlus();

  for( unsigned int   i = 0; i < fTriplets.size(); i++ )
  {
    // Generate the unique name for the next PV instance
    // The name has format:
    //
    // av_WWW_impr_XXX_YYY_ZZZ
    // where the fields mean:
    // WWW - assembly volume instance number
    // XXX - assembly volume imprint number
    // YYY - the name of a log. volume we want to make a placement of
    // ZZZ - the log. volume index inside the assembly volume
    G4std::strstream pvName;
    pvName << "av_"
           << GetInstanceCount()
           << "_impr_"
           << GetImprintsCount()
           << "_"
           << fTriplets[i].GetVolume()->GetName().c_str()
           << "_pv_"
           << i
           << G4std::ends;

    // Create the transformation in this assembly volume
    G4AffineTransform  Ta( fTriplets[i].GetRotation()->inverse(),
                           fTriplets[i].GetTranslation() );
    // Create the transformation in a mother volume
    G4AffineTransform  Tm( pRotationInMother->inverse(),
                           translationInMother );
    
    // Combine them together
    G4AffineTransform  Tfinal = Ta * Tm;

    // Extract the final absolute transformation inside a mother
    G4RotationMatrix*  pFinalRotation = new G4RotationMatrix(
                                                          Tfinal.NetRotation()
                                                            );
    G4ThreeVector      finalTranslation = Tfinal.NetTranslation();

    // Generate a new physical volume instance inside a mother
    G4VPhysicalVolume* pPlaced = new G4PVPlacement(
                                 pFinalRotation
                                ,finalTranslation
                                ,fTriplets[i].GetVolume()
                                ,pvName.str()
                                ,pMotherLV
                                ,false
                                ,numberOfDaughters + i
                              );
    
    // Register the physical volume created by us so we can delete it later
    fPVStore.push_back( pPlaced );
  }
}

void G4AssemblyVolume::MakeImprint( G4LogicalVolume*  pMotherLV,
                                    G4Transform3D&    transformation )
{
  unsigned int        numberOfDaughters = pMotherLV->GetNoDaughters();

  // We start from the first available index
  numberOfDaughters++;

  for( unsigned int   i = 0; i < fTriplets.size(); i++ )
  {
    // Generate the unique name for the next PV instance
    // The name has format:
    //
    // av_WWW_impr_XXX_YYY_ZZZ
    // where the fields mean:
    // WWW - assembly volume instance number
    // XXX - assembly volume imprint number
    // YYY - the name of a log. volume we want to make a placement of
    // ZZZ - the log. volume index inside the assembly volume
    G4std::strstream pvName;
    pvName << "av_"
           << GetInstanceCount()
           << "_impr_"
           << GetImprintsCount()
           << "_"
           << fTriplets[i].GetVolume()->GetName().c_str()
           << "_pv_"
           << i
           << G4std::ends;

    G4Transform3D Ta( *(fTriplets[i].GetRotation()),
                      fTriplets[i].GetTranslation()
                    );

    G4Transform3D Tfinal = transformation * Ta;
    
//  G4RotationMatrix* pFinalRotation =
//                    new G4RotationMatrix( Tfinal.getRotation().inverse() );
//  G4ThreeVector     finalTranslation = Tfinal.getTranslation();

    // Generate a new physical volume instance inside a mother
    G4VPhysicalVolume* pPlaced = new G4PVPlacement( Tfinal,
                                                    fTriplets[i].GetVolume(),
                                                    pvName.str(),
                                                    pMotherLV,
                                                    false,
                                                    numberOfDaughters + i );

    // Register the physical volume created by us so we can delete it later
    fPVStore.push_back( pPlaced );
  }
}

unsigned int G4AssemblyVolume::GetInstanceCount() const
{
  return G4AssemblyVolume::fsInstanceCounter;
}

void         G4AssemblyVolume::SetInstanceCount( unsigned int value )
{
  G4AssemblyVolume::fsInstanceCounter = value;
}

void         G4AssemblyVolume::InstanceCountPlus()
{
  G4AssemblyVolume::fsInstanceCounter++;
}

void         G4AssemblyVolume::InstanceCountMinus()
{
  G4AssemblyVolume::fsInstanceCounter--;
}
