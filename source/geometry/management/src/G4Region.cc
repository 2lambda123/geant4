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
// $Id: G4Region.cc,v 1.7 2003-03-24 10:17:54 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4Region Implementation

#include "G4RegionStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Region.hh"
#include "G4VPVParameterisation.hh"

// *******************************************************************
// Constructor:
//  - Adds self to region Store
// *******************************************************************
//
G4Region::G4Region(const G4String& pName)
  : fName(pName), fRegionMod(true), fCut(0)
{
  G4RegionStore* rStore = G4RegionStore::GetInstance();
  if (rStore->GetRegion(pName,false))
  {
    G4cerr << "WARNING - G4Region::G4Region()" << G4endl
           << "          Region " << pName << " already existing in store !"
           << G4endl
           << "          The region has NOT been registered !" << G4endl;
  }
  else
  {
    rStore->Register(this);
  }
}

// *******************************************************************
// Destructor:
//  - Removes self from region Store
// *******************************************************************
//
G4Region::~G4Region()
{
  G4RegionStore::GetInstance()->DeRegister(this);
}

// *******************************************************************
// ScanVolumeTree:
//  - Scans recursively the 'lv' logical volume tree, retrieves
//    and places all materials in the list.
//  - The boolean flag 'region' identifies if the volume tree must
//    have region reset (false) or if the current region must be
//    associated to the logical volume 'lv' and its tree (true).
// *******************************************************************
//
void G4Region::ScanVolumeTree(G4LogicalVolume* lv, G4bool region)
{
  // If logical volume is going to become a region, add 
  // its material to the list if not already present
  //
  G4Region* currentRegion = 0;
  size_t noDaughters = lv->GetNoDaughters();
  G4Material* volMat = lv->GetMaterial();
  G4MaterialList::iterator pos;
  if (region)
  {
    currentRegion = this;
    pos = G4std::find(fMaterials.begin(),fMaterials.end(),volMat);
    if (pos == fMaterials.end())
    {
      fMaterials.push_back(volMat);
      fRegionMod = true;
    }
  }

  // Set the LV region to be either the current region or NULL,
  // according to the boolean selector
  //
  lv->SetRegion(currentRegion);

  // Stop recursion here if no further daughters are involved
  //
  if(noDaughters==0) return;

  G4VPhysicalVolume* daughterPVol = lv->GetDaughter(0);
  if (daughterPVol->IsParameterised())
  {
    // Adopt special treatment in case of parameterised volumes,
    // where parameterisation involves a new material scan
    //
    G4VPVParameterisation* pParam = daughterPVol->GetParameterisation();
    size_t repNo = daughterPVol->GetMultiplicity();
    for (register size_t rep=0; rep<repNo; rep++)
    {
      volMat = pParam->ComputeMaterial(rep, daughterPVol);
      pos = G4std::find(fMaterials.begin(),fMaterials.end(),volMat);
      if (pos == fMaterials.end())
      {
        fMaterials.push_back(volMat);
        fRegionMod = true;
      }
    }
    G4LogicalVolume* daughterLVol = daughterPVol->GetLogicalVolume();
    ScanVolumeTree(daughterLVol, region);
  }
  else
  {
    for (register size_t i=0; i<noDaughters; i++)
    {
      G4LogicalVolume* daughterLVol = lv->GetDaughter(i)->GetLogicalVolume();
      if (!daughterLVol->IsRootRegion())
      {
        // Set daughter's LV to be a region and store materials in
        // the materials list, if the LV is not already a root region
        //
        ScanVolumeTree(daughterLVol, region);
      }
    }
  }
}

// *******************************************************************
// AddRootLogicalVolume:
//  - Adds a root logical volume and sets its daughters flags as
//    regions. It also recomputes the materials list for the region.
// *******************************************************************
//
void G4Region::AddRootLogicalVolume(G4LogicalVolume* lv)
{
  // Check the logical volume is not already in the list
  //
  G4RootLVList::iterator pos;
  pos = G4std::find(fRootVolumes.begin(),fRootVolumes.end(),lv);
  if (pos == fRootVolumes.end())
  {
    // Insert the root volume in the list and set it as root region
    //
    fRootVolumes.push_back(lv);
    lv->SetRegionRootFlag(true);
  }

  // Scan recursively the tree of daugther volumes and set regions
  //
  ScanVolumeTree(lv, true);

  // Set region as modified
  //
  fRegionMod = true;
}

// *******************************************************************
// RemoveRootLogicalVolume:
//  - Removes a root logical volume and resets its daughters flags as
//    regions. It also recomputes the materials list for the region.
// *******************************************************************
//
void G4Region::RemoveRootLogicalVolume(G4LogicalVolume* lv)
{
  // Find and remove logical volume from the list
  //
  G4RootLVList::iterator pos;
  pos = G4std::find(fRootVolumes.begin(),fRootVolumes.end(),lv);
  if (pos != fRootVolumes.end())
  {
    fRootVolumes.erase(pos);
    lv->SetRegionRootFlag(false);
  }

  // Scan recursively the tree of daugther volumes and reset regions
  //
  ScanVolumeTree(lv, false);

  // Update the materials list
  //
  UpdateMaterialList();

  // Set region as modified
  //
  fRegionMod = true;
}

// *******************************************************************
// ClearMaterialList:
//  - Clears the material list.
// *******************************************************************
//
void G4Region::ClearMaterialList()
{
  fMaterials.clear();
}

// *******************************************************************
// UpdateMaterialList:
//  - computes material list looping through
//    each root logical volume in the region.
// *******************************************************************
//
void G4Region::UpdateMaterialList()
{
  // Reset the materials list
  //
  ClearMaterialList();

  // Loop over the root logical volumes and rebuild the list
  // of materials from scratch
  //
  G4RootLVList::iterator pLV;
  for (pLV=fRootVolumes.begin(); pLV!=fRootVolumes.end(); pLV++)
  {
    ScanVolumeTree(*pLV, true);
  }
}
