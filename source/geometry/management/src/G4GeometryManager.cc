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
// $Id: G4GeometryManager.cc,v 1.4 2001-07-11 09:59:20 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4GeometryManager
//
// Implementation
//
// History:
// 26.07.95 P.Kent Initial version, incuding optimisation Build

#include "G4GeometryManager.hh"

// Close geometry - perform sanity checks and optionally Build optimisation
// for placed volumes (always built for replicas & parameterised)
// NOTE: Currently no sanity checks
G4bool G4GeometryManager::CloseGeometry(G4bool pOptimise)
{
    if (!fIsClosed)
	{
	    BuildOptimisations(pOptimise);
	    fIsClosed=true;
	}
    return true;
}

void G4GeometryManager::OpenGeometry()
{
     if (fIsClosed)
	 {
 	    DeleteOptimisations();
 	    fIsClosed=false;
 	}
}

// Static class variable: ptr to single instance of class
G4GeometryManager* G4GeometryManager::fgInstance = 0;

G4GeometryManager* G4GeometryManager::GetInstance()
{
    static G4GeometryManager worldManager;
    if (!fgInstance)
	{
	    fgInstance = &worldManager;
	}
    return fgInstance;    
}


// Constructor. Set the geometry to be open
G4GeometryManager::G4GeometryManager() 
{
    fIsClosed=false;
}

//
// Create optimisation info. Build all voxels if allOpts=true
// else only for replicated volumes
//
void G4GeometryManager::BuildOptimisations(const G4bool allOpts)
{
     G4LogicalVolumeStore *Store;
     G4LogicalVolume *volume;
     G4SmartVoxelHeader *head;
     G4int nVolumes,n;
     Store=G4LogicalVolumeStore::GetInstance();
     nVolumes=Store->size();
     for (n=0;n<nVolumes;n++)
	 {
	     volume=(*Store)[n];
// For safety, check if there are any existing voxels and delete before
// replacement
	     head = volume->GetVoxelHeader();
	     if (head) 
		 {
		     delete head;
		     volume->SetVoxelHeader(0);
		 }
	     if ((volume->GetNoDaughters()>=kMinVoxelVolumesLevel1&&allOpts) ||
                  (volume->GetNoDaughters()==1&&
                   volume->GetDaughter(0)->IsReplicated()==true))
		 {
#ifdef G4GEOMETRY_VOXELDEBUG
		     G4cout << "**** G4GeometryManager::BuildOptimisations" << G4endl
			  << "     Examining logical volume name = " << volume->GetName() << G4endl;
#endif
		     head = new G4SmartVoxelHeader(volume);
		     if (head)
			 {
			     volume->SetVoxelHeader(head);
			 }
		     else
			 {
			     G4Exception("G4GeometryManager::BuildOptimisations voxelheader new failed");
			 }
		 }
	     else
		 {
// Don't create voxels for this node
#ifdef G4GEOMETRY_VOXELDEBUG
		     G4cout << "**** G4GeometryManager::BuildOptimisations"
			  << G4endl
			  << "     Skipping logical volume name = "
			  << volume->GetName() << G4endl;
#endif
		 }

	 }
}

// Remove all optimisation info
//
// Process:
//
// Loop over all logical volumes, deleting non-null voxels ptrs

void G4GeometryManager::DeleteOptimisations()
{
     G4LogicalVolumeStore *Store=G4LogicalVolumeStore::GetInstance();
     G4LogicalVolume *volume;
     G4SmartVoxelHeader *head;
     G4int nVolumes,n;
     nVolumes=Store->size();
     for (n=0;n<nVolumes;n++)
	 {
	     volume=(*Store)[n];
	     head=volume->GetVoxelHeader();
	     if (head)
		 {
		     delete head;
		     volume->SetVoxelHeader(0);
		 }
	 }
}










