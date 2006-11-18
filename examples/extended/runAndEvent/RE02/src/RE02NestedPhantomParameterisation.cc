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
///////////////////////////////////////////////////////////////////////////////
#include "RE02NestedPhantomParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

//=======================================================================
// (RE02NestedPhantomParameterisation)
//
//  (Description)
//     Class for nested parameterisation.
//     This parameterisation handles material and transfomation of voxles.
//
//  T.Aso Created. Nov.2007.
//
////////////////////////////////////////////////////////////////////
RE02NestedPhantomParameterisation::RE02NestedPhantomParameterisation(
						     const G4ThreeVector& voxelSize,
						     G4int nz,
						     std::vector<G4Material*>& mat):
  G4VNestedParameterisation(),fdX(voxelSize.x()),fdY(voxelSize.y()),fdZ(voxelSize.z()),
  fNz(nz),fmat(mat)
{
  // Position of voxels. 
  // x and y positions are already defined in DetectorConstruction 
  // by using replicated volume. Here only we need to define is z positions of voxles.
  fpZ.clear();
  G4double zp;
  for ( G4int iz = 0; iz < fNz; iz++){
    zp = (-fNz+1+2*iz)*fdZ;
    fpZ.push_back(zp);
  }

}

RE02NestedPhantomParameterisation::~RE02NestedPhantomParameterisation(){
  fpZ.clear();
}

//
// Material assignment to geometry.
//
G4Material* RE02NestedPhantomParameterisation::ComputeMaterial(G4VPhysicalVolume* currentVol,
							 const G4int copyNo, 
							 const G4VTouchable* parentTouch)
{
  // Copy number of voxels. 
  // Copy number of X and Y are obtained from replication number.
  // Copy nymber of Z is the copy number of current voxel.
  G4int ix = parentTouch->GetReplicaNumber(0);
  G4int iy = parentTouch->GetReplicaNumber(1);
  G4int iz = copyNo;
  // For demonstration purpose,a couple of materials are chosen alternately.
  G4Material* mat=0;
  if ( ix < 0 && iy < 0 ) return NULL;
  if ( ix%2 == 0 && iy%2 == 0 && iz%2 == 0 ) mat = fmat[0];
  else mat = fmat[1];

  // Set material to the current logical volume.
  G4LogicalVolume* currentLog = currentVol->GetLogicalVolume();
  currentLog->SetMaterial(mat);

  return mat;
}

//
//  Number of Materials
//  Material scanner is required for preparing physics tables and so on before 
//  stating simulation, so that G4 has to know number of materials.
G4int       RE02NestedPhantomParameterisation::GetNumberOfMaterials() const{
  return fmat.size();
}

//
// GetMaterial
//  This is needed for material scanner and realizing geometry.
//
G4Material* RE02NestedPhantomParameterisation::GetMaterial(G4int i) const{
  return fmat[i];
}

//
// Transformation of voxels.
//
void RE02NestedPhantomParameterisation::ComputeTransformation(const G4int copyNo, 
						  G4VPhysicalVolume* physVol)const{
  G4ThreeVector position(0.,0.,fpZ[copyNo]);
  physVol->SetTranslation(position);
}

//
// Dimensions are always same in this RE02 example.
//
void RE02NestedPhantomParameterisation::ComputeDimensions(G4Box& box, 
					       const G4int ,
					       const G4VPhysicalVolume* ) const{
  box.SetXHalfLength(fdX);
  box.SetYHalfLength(fdY);
  box.SetZHalfLength(fdZ);
}
