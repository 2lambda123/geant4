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
// $Id: G4RegionStore.hh,v 1.7 2005-08-18 16:51:37 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4RegionStore
//
// Class description:
//
// Container for all regiong, with functionality derived from
// std::vector<T>. The class is a `singleton', in that only
// one can exist, and access is provided via the static method
// G4RegionStore::GetInstance().
//
// All regions should be registered with G4RegionStore, and removed on their
// destruction. The underlying container initially has a capacity of 20.
//
// If much additional functionality is added, should consider containment
// instead of inheritance for std::vector<T>
//
// Member data:
//
// static G4RegionStore*
//   - Pointer to the single G4RegionStore

// History:
// 18.09.02 G.Cosmo Initial version
// --------------------------------------------------------------------
#ifndef G4REGIONSTORE_HH
#define G4REGIONSTORE_HH

#include <vector>
#include "G4Types.hh"
#include "G4String.hh"

class G4Region;
class G4VStoreNotifier;
class G4VPhysicalVolume;

class G4RegionStore : public std::vector<G4Region*>
{
  public:  // with description

    static void Register(G4Region* pSolid);
      // Add the region to the collection.
    static void DeRegister(G4Region* pSolid);
      // Remove the region from the collection.
    static G4RegionStore* GetInstance();
      // Get a ptr to the unique G4RegionStore, creating it if necessary.
    static void SetNotifier(G4VStoreNotifier* pNotifier);
      // Assign a notifier for allocation/deallocation of regions.
    static void Clean();
      // Delete all regions from the store except for the world region.

    G4bool IsModified() const;
      // Loops through all regions to verify if a region has been
      // modified. It returns TRUE if just one region is modified.
    void ResetRegionModified();
      // Loops through all regions to reset flag for modification
      // to FALSE. Used by the run manager to notify that the
      // physics table has been updated.

    void UpdateMaterialList(G4VPhysicalVolume* currentWorld);
      // Forces recomputation of material lists in all regions
      // in the store.

    G4Region* GetRegion(const G4String& name, G4bool verbose=true) const;
      // Returns a region through its name specification.

    G4Region* FindOrCreateRegion(const G4String& name);
      // Returns a region through its name specification, if it exists.
      // If it does not exist it will allocate one delegating ownership
      // to the client.

    virtual ~G4RegionStore();
      // Destructor: takes care to delete allocated regions.

    void SetWorldVolume();
      // Set a world volume pointer to a region that belongs to it.
      // Scan over all world volumes.
      // This method should be exclusively used by G4RunManagerKernel.

  protected:

    G4RegionStore();

  private:

    static G4RegionStore* fgInstance;
    static G4VStoreNotifier* fgNotifier;
    static G4bool locked;
};

#endif
