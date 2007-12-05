///////////////////////////////////////////////////////////////////////////////
#include "DicomNestedPhantomParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

DicomNestedPhantomParameterisation::DicomNestedPhantomParameterisation(
						     const G4ThreeVector& voxelSize,
						     std::vector<G4Material*>& mat):
  G4VNestedParameterisation(),fdX(voxelSize.x()),fdY(voxelSize.y()),fdZ(voxelSize.z()),fMaterials(mat)
{
  // Position of voxels. 
  // x and y positions are already defined in DetectorConstruction 
  // by using replicated volume. Here only we need to define is z positions of voxes.
}

DicomNestedPhantomParameterisation::~DicomNestedPhantomParameterisation()
{
}

void DicomNestedPhantomParameterisation::SetNoVoxel( size_t nx, size_t ny, size_t nz )
{
  fnX = nx;
  fnY = ny;
  fnZ = nz;
}

//
// Material assignment to geometry.
//
G4Material* DicomNestedPhantomParameterisation::ComputeMaterial(G4VPhysicalVolume* ,
							 const G4int copyNoZ, 
							 const G4VTouchable* parentTouch)
{
  if(parentTouch==0) return fMaterials[0]; // protection for initialization and vis at idle state
  // Copy number of voxels. 
  // Copy number of X and Y are obtained from replication number.
  // Copy nymber of Z is the copy number of current voxel.
  G4int ix = parentTouch->GetReplicaNumber(0);
  G4int iy = parentTouch->GetReplicaNumber(1);
  G4int iz = copyNoZ;

  G4int copyNo = ix + fnZ*iy + fnX*fnY*iz;

  size_t matIndex = GetMaterialIndex(copyNo);

  return fMaterials[ matIndex ];

}

//------------------------------------------------------------------
size_t DicomNestedPhantomParameterisation::
GetMaterialIndex( size_t copyNo ) const
{
  return *(fMaterialIndices+copyNo);
}

//
//  Number of Materials
//  Material scanner is required for preparing physics tables and so on before 
//  stating simulation, so that G4 has to know number of materials.
G4int       DicomNestedPhantomParameterisation::GetNumberOfMaterials() const{
  return fMaterials.size();
}

//
// GetMaterial
//  This is needed for material scanner and realizing geometry.
//
G4Material* DicomNestedPhantomParameterisation::GetMaterial(G4int i) const{
  return fMaterials[i];
}

//
// Transformation of voxels.
//
void DicomNestedPhantomParameterisation::ComputeTransformation(const G4int copyNo, 
						  G4VPhysicalVolume* physVol)const{
  G4ThreeVector position(0.,0.,(2*copyNo+1)*fdZ - fdZ*fnZ);

  physVol->SetTranslation(position);
}

//
// Dimensions are always same in this RE02 example.
//
void DicomNestedPhantomParameterisation::ComputeDimensions(G4Box& box, 
					       const G4int ,
					       const G4VPhysicalVolume* ) const{
  box.SetXHalfLength(fdX);
  box.SetYHalfLength(fdY);
  box.SetZHalfLength(fdZ);
}
