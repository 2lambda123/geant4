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
// $Id: G4ParameterisationPolyhedra.cc,v 1.11 2005-11-02 16:06:32 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4ParameterisationPolyhedra Implementation file
//
// 14.10.03 - P.Arce, Initial version
// 08.04.04 - I.Hrivnacova, Implemented reflection
// --------------------------------------------------------------------

#include "G4ParameterisationPolyhedra.hh"

#include <iomanip>
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ReflectedSolid.hh"
#include "G4Polyhedra.hh"

//--------------------------------------------------------------------------
G4VParameterisationPolyhedra::
G4VParameterisationPolyhedra( EAxis axis, G4int nDiv, G4double width,
                              G4double offset, G4VSolid* msolid,
                              DivisionType divType )
  :  G4VDivisionParameterisation( axis, nDiv, width, offset, divType, msolid )
{
  G4Polyhedra* msol = (G4Polyhedra*)(msolid);
  if (msol->IsGeneric())
  {
    G4Exception("G4VParameterisationPolyhedra::G4VParameterisationPolyhedra()",
                "NotSupported", FatalException,
                "Sorry, generic construct for G4Polyhedra NOT supported.");
  }
  if (msolid->GetEntityType() == "G4ReflectedSolid")
  {
    // Get constituent solid  
    G4VSolid* mConstituentSolid 
       = ((G4ReflectedSolid*)msolid)->GetConstituentMovedSolid();
    msol = (G4Polyhedra*)(mConstituentSolid);
  
    // Get parameters
    G4int   nofSides = msol->GetOriginalParameters()->numSide;
    G4int   nofZplanes = msol->GetOriginalParameters()->Num_z_planes;
    G4double* zValues  = msol->GetOriginalParameters()->Z_values;
    G4double* rminValues  = msol->GetOriginalParameters()->Rmin;
    G4double* rmaxValues  = msol->GetOriginalParameters()->Rmax;

    // Invert z values, 
    // convert radius parameters
    G4double* rminValues2 = new G4double[nofZplanes];
    G4double* rmaxValues2 = new G4double[nofZplanes];
    G4double* zValuesRefl = new G4double[nofZplanes];
    for (G4int i=0; i<nofZplanes; i++)
    {
      rminValues2[i] = rminValues[i] * ConvertRadiusFactor(*msol);
      rmaxValues2[i] = rmaxValues[i] * ConvertRadiusFactor(*msol);
      zValuesRefl[i] = - zValues[i];
    }  
    
    G4Polyhedra* newSolid
      = new G4Polyhedra(msol->GetName(),
                        msol->GetStartPhi(), 
                        msol->GetEndPhi() - msol->GetStartPhi(),
                        nofSides,
                        nofZplanes, zValuesRefl, rminValues2, rmaxValues2);

    delete [] rminValues2;       
    delete [] rmaxValues2;       
    delete [] zValuesRefl;       

    msol = newSolid;
    fmotherSolid = newSolid;
    fReflectedSolid = true;
    fDeleteSolid = true;
  }    
}

//------------------------------------------------------------------------
G4VParameterisationPolyhedra::~G4VParameterisationPolyhedra()
{
}

//--------------------------------------------------------------------------
G4double 
G4VParameterisationPolyhedra::
ConvertRadiusFactor(const G4Polyhedra& phedra) const
{
  G4double phiTotal = phedra.GetEndPhi() - phedra.GetStartPhi();
  G4int nofSides = phedra.GetOriginalParameters()->numSide;
  
  if ( (phiTotal <=0) || (phiTotal > 2*pi+kAngTolerance) )
    phiTotal = 2*pi;
  
  return std::cos(0.5*phiTotal/nofSides);
}  

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraRho::
G4ParameterisationPolyhedraRho( EAxis axis, G4int nDiv,
                               G4double width, G4double offset,
                               G4VSolid* msolid, DivisionType divType )
  :  G4VParameterisationPolyhedra( axis, nDiv, width, offset, msolid, divType )
{

  CheckParametersValidity();
  SetType( "DivisionPolyhedraRho" );

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* original_pars = msol->GetOriginalParameters();

  if( divType == DivWIDTH )
  {
    fnDiv = CalculateNDiv( original_pars->Rmax[0]
                         - original_pars->Rmin[0], width, offset );
  }
  else if( divType == DivNDIV )
  {
    fwidth = CalculateWidth( original_pars->Rmax[0]
                           - original_pars->Rmin[0], nDiv, offset );
  }

#ifdef G4DIVDEBUG
  if( verbose >= 1 )
  {
    G4cout << " G4ParameterisationPolyhedraRho - # divisions " << fnDiv
           << " = " << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }
#endif
}

//------------------------------------------------------------------------
G4ParameterisationPolyhedraRho::~G4ParameterisationPolyhedraRho()
{
}

//---------------------------------------------------------------------
void G4ParameterisationPolyhedraRho::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraRho::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along R will be done with a width "
           << "different for each solid section." << G4endl
           << "          WIDTH will not be used !" << G4endl;
  }
  if( foffset != 0. )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraRho::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along  R will be done with a width "
           << "different for each solid section." << G4endl
           << "          OFFSET will not be used !" << G4endl;
  }
}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraRho::GetMaxParameter() const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* original_pars = msol->GetOriginalParameters();
  return original_pars->Rmax[0] - original_pars->Rmin[0];
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraRho::
ComputeTransformation( const G4int, G4VPhysicalVolume* physVol ) const
{
  //----- translation 
  G4ThreeVector origin(0.,0.,0.);

  //----- set translation 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix: unit

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraRho " << G4endl
           << " foffset: " << foffset/deg
           << " - fwidth: " << fwidth/deg << G4endl;
  }
#endif

  ChangeRotMatrix( physVol );

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraRho "
           << G4endl
           << " Position: " << origin
           << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
#endif
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraRho::
ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                   const G4VPhysicalVolume* ) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );
  G4int nZplanes = origparamMother->Num_z_planes;

  G4double width = 0.;
  for( G4int ii = 0; ii < nZplanes; ii++ )
  {
    width = CalculateWidth( origparamMother->Rmax[ii]
                          - origparamMother->Rmin[ii], fnDiv, foffset );
    origparam.Rmin[ii] = origparamMother->Rmin[ii]+foffset+width*copyNo;
    origparam.Rmax[ii] = origparamMother->Rmin[ii]+foffset+width*(copyNo+1);
  }

  phedra.SetOriginalParameters(&origparam); // copy values & transfer pointers
  phedra.Reset();                           // reset to new solid parameters

#ifdef G4DIVDEBUG
  if( verbose >= -2 )
  {
    G4cout << "G4ParameterisationPolyhedraRho::ComputeDimensions()" << G4endl
           << "-- Parametrised phedra copy-number: " << copyNo << G4endl;
    phedra.DumpInfo();
  }
#endif
}

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraPhi::
G4ParameterisationPolyhedraPhi( EAxis axis, G4int nDiv,
                               G4double width, G4double offset,
                               G4VSolid* msolid, DivisionType divType )
  :  G4VParameterisationPolyhedra( axis, nDiv, width, offset, msolid, divType )
{ 
  CheckParametersValidity();
  SetType( "DivisionPolyhedraPhi" );

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4double deltaPhi = msol->GetEndPhi() - msol->GetStartPhi();

  if( divType == DivWIDTH )
  {
    fnDiv = CalculateNDiv( deltaPhi, width, offset );
  }
  else if( divType == DivNDIV )
  {
    fwidth = CalculateWidth( deltaPhi, nDiv, offset );
  }

#ifdef G4DIVDEBUG
  if( verbose >= 1 )
  {
    G4cout << " G4ParameterisationPolyhedraPhi - # divisions " << fnDiv
           << " = " << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }
#endif
}

//------------------------------------------------------------------------
G4ParameterisationPolyhedraPhi::~G4ParameterisationPolyhedraPhi()
{
}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraPhi::GetMaxParameter() const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  return msol->GetEndPhi() - msol->GetStartPhi();
}

//---------------------------------------------------------------------
void G4ParameterisationPolyhedraPhi::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraPhi::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along PHI will be done splitting "
           << "in the defined numSide." << G4endl
           << "          WIDTH will not be used !" << G4endl;
  }
  if( foffset != 0. )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraPhi::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along PHI will be done splitting "
           << "in the defined numSide." << G4endl
           << "          OFFSET will not be used !" << G4endl;
  }

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();

  if( origparamMother->numSide != fnDiv )
  { 
    G4cerr << "ERROR - "
           << "G4ParameterisationPolyhedraPhi::CheckParametersValidity()"
           << G4endl
           << "        Division along PHI will be done splitting in the defined"
           << G4endl
           << "        numSide, i.e, the number of division would be :"
           << "        " << origparamMother->numSide
           << " instead of " << fnDiv << " !"
           << G4endl; 
    G4Exception("G4ParameterisationPolyhedraPhi::CheckParametersValidity()",
                "IllegalConstruct", FatalException,
                "Not supported configuration.");
  }
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraPhi::
ComputeTransformation( const G4int copyNo, G4VPhysicalVolume *physVol ) const
{
  //----- translation 
  G4ThreeVector origin(0.,0.,0.); 
  //----- set translation 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix (so that all volumes point to the centre)
  G4double posi = copyNo*fwidth;

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraPhi - position: " << posi/deg
           << G4endl
           << " copyNo: " << copyNo
           << " - fwidth: " << fwidth/deg << G4endl;
  }
#endif

  ChangeRotMatrix( physVol, -posi );

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraPhi " << copyNo
           << G4endl
           << " Position: " << origin << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
#endif
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraPhi::
ComputeDimensions( G4Polyhedra& phedra, const G4int,
                   const G4VPhysicalVolume* ) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );

  origparam.numSide = 1;
  origparam.Start_angle = origparamMother->Start_angle;
  origparam.Opening_angle = fwidth;

  phedra.SetOriginalParameters(&origparam);  // copy values & transfer pointers
  phedra.Reset();                            // reset to new solid parameters

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << "G4ParameterisationPolyhedraPhi::ComputeDimensions():" << G4endl;
    phedra.DumpInfo();
  }
#endif
}

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraZ::
G4ParameterisationPolyhedraZ( EAxis axis, G4int nDiv,
                             G4double width, G4double offset,
                             G4VSolid* msolid, DivisionType divType )
  :  G4VParameterisationPolyhedra( axis, nDiv, width, offset, msolid, divType )
{ 
  CheckParametersValidity();
  SetType( "DivisionPolyhedraZ" );

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  
  if( divType == DivWIDTH )
    {
    fnDiv =
      CalculateNDiv( origparamMother->Z_values[origparamMother->Num_z_planes-1]
                     - origparamMother->Z_values[0] , width, offset );
  }
  else if( divType == DivNDIV )
    {
    fwidth =
      CalculateNDiv( origparamMother->Z_values[origparamMother->Num_z_planes-1]
                     - origparamMother->Z_values[0] , nDiv, offset );
  }
  
#ifdef G4DIVDEBUG
  if( verbose >= 1 )
    {
    G4cout << " G4ParameterisationPolyhedraZ - # divisions " << fnDiv << " = "
           << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }
#endif
}

//---------------------------------------------------------------------
G4ParameterisationPolyhedraZ::~G4ParameterisationPolyhedraZ()
{
}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraZ::GetMaxParameter() const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  return std::abs (origparamMother->Z_values[origparamMother->Num_z_planes-1]
             -origparamMother->Z_values[0]);
}

//---------------------------------------------------------------------
void G4ParameterisationPolyhedraZ::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraZ::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along Z will be done splitting "
           << "in the defined z_planes." << G4endl
           << "          WIDTH will not be used !" << G4endl;
  }

  if( foffset != 0. )
  {
    G4cerr << "WARNING - "
           << "G4ParameterisationPolyhedraZ::CheckParametersValidity()"
           << G4endl
           << "          Solid " << msol->GetName() << G4endl
           << "          Division along Z will be done splitting "
           << "in the defined z_planes." << G4endl
           << "          OFFSET will not be used !" << G4endl;
  }

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();

  if( origparamMother->Num_z_planes-1 != fnDiv )
  { 
    G4cerr << "ERROR - "
           << "G4ParameterisationPolyhedraZ::CheckParametersValidity()"
           << G4endl
           << "        Division along Z will be done splitting in the defined"
           << G4endl
           << "        z_planes, i.e, the number of division would be :"
           << "        " << origparamMother->Num_z_planes-1
           << " instead of " << fnDiv << " !"
           << G4endl; 
    G4Exception("G4ParameterisationPolyhedraZ::CheckParametersValidity()",
                "IllegalConstruct", FatalException,
                "Not supported configuration.");
  }
}

//---------------------------------------------------------------------
void
G4ParameterisationPolyhedraZ::
ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(GetMotherSolid());

  //----- set translation: along Z axis
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4double posi = (origparamMother->Z_values[copyNo]
                   + origparamMother->Z_values[copyNo+1])/2;
  G4ThreeVector origin(0.,0.,posi); 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix: unit

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraZ - position: " << posi << G4endl
           << " copyNo: " << copyNo << " - foffset: " << foffset/deg
           << " - fwidth: " << fwidth/deg << G4endl;
  }
#endif

  ChangeRotMatrix( physVol );

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraZ "
           << copyNo << G4endl
           << " Position: " << origin << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
#endif
}

//---------------------------------------------------------------------
void
G4ParameterisationPolyhedraZ::
ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                   const G4VPhysicalVolume* ) const
{
  // only for mother number of planes = 2!!
  //
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );

  G4double posi = (origparamMother->Z_values[copyNo]
                   + origparamMother->Z_values[copyNo+1])/2;

  origparam.Num_z_planes = 2;
  origparam.Z_values[0] = origparamMother->Z_values[copyNo] - posi;
  origparam.Z_values[1] = origparamMother->Z_values[copyNo+1] - posi;
  origparam.Rmin[0] = origparamMother->Rmin[copyNo];
  origparam.Rmin[1] = origparamMother->Rmin[copyNo+1];
  origparam.Rmax[0] = origparamMother->Rmax[copyNo];
  origparam.Rmax[1] = origparamMother->Rmax[copyNo+1];

  phedra.SetOriginalParameters(&origparam);  // copy values & transfer pointers
  phedra.Reset();                            // reset to new solid parameters

#ifdef G4DIVDEBUG
  if( verbose >= 2 )
  {
    G4cout << "G4ParameterisationPolyhedraZ::ComputeDimensions()" << G4endl
           << "-- Parametrised phedra copy-number: " << copyNo << G4endl;
    phedra.DumpInfo();
  }
#endif
}

