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
// $Id: G4GeometryManager.cc,v 1.16 2005-04-13 15:53:09 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4GeometryManager
//
// Implementation
//
// Author:
// 26.07.95 P.Kent Initial version, including optimisation Build
// --------------------------------------------------------------------

#include <iomanip>
#include "G4Timer.hh"
#include "G4GeometryManager.hh"

#ifdef  G4GEOMETRY_VOXELDEBUG
#include "G4ios.hh"
#endif

// Needed for building optimisations
//
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4SmartVoxelHeader.hh"
#include "voxeldefs.hh"

// ***************************************************************************
// Static class variable: ptr to single instance of class
// ***************************************************************************
//
G4GeometryManager* G4GeometryManager::fgInstance = 0;

// ***************************************************************************
// Constructor. Set the geometry to be open
// ***************************************************************************
//
G4GeometryManager::G4GeometryManager() 
{
  fIsClosed=false;
}

// ***************************************************************************
// Closes geometry - performs sanity checks and optionally builds optimisation
// for placed volumes (always built for replicas & parameterised).
// NOTE: Currently no sanity checks are performed.
// Applies to just a specific subtree if a logical volume is specified.
// ***************************************************************************
//
G4bool G4GeometryManager::CloseGeometry(G4bool pOptimise, G4bool verbose,
                                        G4LogicalVolume* tVolume)
{
   if (!fIsClosed)
  {
    if (tVolume)
    {
      BuildOptimisations(pOptimise, tVolume);
    }
    else
    {
      BuildOptimisations(pOptimise, verbose);
    }
    fIsClosed=true;
  }
  return true;
}

// ***************************************************************************
// Opens the geometry and removes optimisations (optionally, related to just
// the specified logical-volume).
// Applies to just a specific subtree if a logical volume is specified.
// ***************************************************************************
//
void G4GeometryManager::OpenGeometry(G4LogicalVolume* tVolume)
{
  if (fIsClosed)
  {
    DeleteOptimisations(tVolume);
    fIsClosed=false;
  }
}

// ***************************************************************************
// Returns status of geometry
// ***************************************************************************
//
G4bool G4GeometryManager::IsGeometryClosed()
{
  return fIsClosed;
}

// ***************************************************************************
// Returns the instance of the singleton.
// Creates it in case it's called for the first time.
// ***************************************************************************
//
G4GeometryManager* G4GeometryManager::GetInstance()
{
  static G4GeometryManager worldManager;
  if (!fgInstance)
  {
    fgInstance = &worldManager;
  }
  return fgInstance;    
}

// ***************************************************************************
// Creates optimisation info. Builds all voxels if allOpts=true
// otherwise it builds voxels only for replicated volumes.
// ***************************************************************************
//
void G4GeometryManager::BuildOptimisations(G4bool allOpts, G4bool verbose)
{
   G4Timer timer;
   G4Timer allTimer;
   std::vector<G4SmartVoxelStat> stats;
   if (verbose)  { allTimer.Start(); }

   G4LogicalVolumeStore* Store = G4LogicalVolumeStore::GetInstance();
   G4LogicalVolume* volume;
   G4SmartVoxelHeader* head;
 
   for (size_t n=0; n<Store->size(); n++)
   {
     if (verbose) timer.Start();
     volume=(*Store)[n];
     // For safety, check if there are any existing voxels and
     // delete before replacement
     //
     head = volume->GetVoxelHeader();
     delete head;
     volume->SetVoxelHeader(0);
     if (    (volume->IsToOptimise())
          && (volume->GetNoDaughters()>=kMinVoxelVolumesLevel1&&allOpts)
          || ( (volume->GetNoDaughters()==1)
            && (volume->GetDaughter(0)->IsReplicated()==true) ) ) 
     {
#ifdef G4GEOMETRY_VOXELDEBUG
       G4cout << "**** G4GeometryManager::BuildOptimisations" << G4endl
              << "     Examining logical volume name = "
              << volume->GetName() << G4endl;
#endif
       head = new G4SmartVoxelHeader(volume);
       if (head)
       {
         volume->SetVoxelHeader(head);
       }
       else
       {
         G4cerr << "ERROR - Allocation of new VoxelHeader" << G4endl
                << "        for volume " << volume->GetName() << " failed."
                << G4endl;
         G4Exception("G4GeometryManager::BuildOptimisations()", "FatalError",
                     FatalException, "VoxelHeader allocation error.");
       }
       if (verbose)
       {
         timer.Stop();
         stats.push_back( G4SmartVoxelStat( volume, head,
                                            timer.GetSystemElapsed(),
                                            timer.GetUserElapsed() ) );
       }
     }
     else
     {
       // Don't create voxels for this node
#ifdef G4GEOMETRY_VOXELDEBUG
       G4cout << "**** G4GeometryManager::BuildOptimisations" << G4endl
              << "     Skipping logical volume name = " << volume->GetName()
              << G4endl;
#endif
     }
  }
  if (verbose)
  {
     allTimer.Stop();
     ReportVoxelStats( stats, allTimer.GetSystemElapsed()
                            + allTimer.GetUserElapsed() );
  }
}

// ***************************************************************************
// Creates optimisation info for the specified volumes subtree.
// ***************************************************************************
//
void G4GeometryManager::BuildOptimisations(G4bool allOpts,
                                           G4LogicalVolume* tVolume)
{
   G4SmartVoxelHeader* head = tVolume->GetVoxelHeader();
   delete head;
   tVolume->SetVoxelHeader(0);
   if (    (tVolume->IsToOptimise())
        && (tVolume->GetNoDaughters()>=kMinVoxelVolumesLevel1&&allOpts)
        || ( (tVolume->GetNoDaughters()==1)
          && (tVolume->GetDaughter(0)->IsReplicated()==true) ) ) 
   {
     head = new G4SmartVoxelHeader(tVolume);
     if (head)
     {
       tVolume->SetVoxelHeader(head);
     }
     else
     {
       G4cerr << "ERROR - Allocation of new VoxelHeader" << G4endl
              << "        for volume " << tVolume->GetName() << " failed."
              << G4endl;
       G4Exception("G4GeometryManager::BuildOptimisations()", "FatalError",
                   FatalException, "VoxelHeader allocation error.");
     }
   }
   else
   {
     // Don't create voxels for this node
#ifdef G4GEOMETRY_VOXELDEBUG
     G4cout << "**** G4GeometryManager::BuildOptimisations" << G4endl
            << "     Skipping logical volume name = " << volume->GetName()
            << G4endl;
#endif
   }
}

// ***************************************************************************
// Removes all optimisation info.
// Loops over all logical volumes, deleting non-null voxels pointers,
// or removes optimisation info for a subtree, if a LV is specified.
// ***************************************************************************
//
void G4GeometryManager::DeleteOptimisations(G4LogicalVolume* tVolume)
{
  if (tVolume)
  {
    delete tVolume->GetVoxelHeader();
    tVolume->SetVoxelHeader(0);
  }
  else
  {
    G4LogicalVolumeStore* Store = G4LogicalVolumeStore::GetInstance();
    G4LogicalVolume* volume;
    for (size_t n=0; n<Store->size(); n++)
    {
      volume=(*Store)[n];
      delete volume->GetVoxelHeader();
      volume->SetVoxelHeader(0);
    }
  }
}

// ***************************************************************************
// Reports statistics on voxel optimisation when closing geometry.
// ***************************************************************************
//
void
G4GeometryManager::ReportVoxelStats( std::vector<G4SmartVoxelStat> & stats,
                                     G4double totalCpuTime )
{
  G4cout << "G4GeometryManager::ReportVoxelStats -- Voxel Statistics"
         << G4endl << G4endl;
 
  //
  // Get total memory use
  //
  G4int i, nStat = stats.size();
  G4long totalMemory = 0;
 
  for( i=0;i<nStat;++i )  { totalMemory += stats[i].GetMemoryUse(); }
 
  G4cout << "    Total memory consumed for geometry optimisation:   "
         << totalMemory/1024 << " kByte" << G4endl;
  G4cout << "    Total CPU time elapsed for geometry optimisation: " 
         << std::setprecision(2) << totalCpuTime << " seconds" << G4endl;
 
  //
  // First list: sort by total CPU time
  //
  std::sort( stats.begin(), stats.end(), G4SmartVoxelStat::ByCpu() );
         
  G4int nPrint = nStat > 10 ? 10 : nStat;

  if (nPrint)
  {
    G4cout << "\n    Voxelisation: top CPU users:" << G4endl;
    G4cout << "    Percent   Total CPU    System CPU       Memory  Volume\n"
           << "    -------   ----------   ----------     --------  ----------"
           << G4endl;
    //         12345678901.234567890123.234567890123.234567890123k .
  }

  for(i=0;i<nPrint;++i)
  {
    G4double total = stats[i].GetTotalTime();
    G4double system = stats[i].GetSysTime();
    G4double perc = 0.0;

    if (system < 0) { system = 0.0; }
    if ((total < 0) || (totalCpuTime < perMillion))
      { total = 0; }
    else
      { perc = total*100/totalCpuTime; }

    G4cout << std::setprecision(2) 
           << std::setiosflags(std::ios::fixed|std::ios::right)
           << std::setw(11) << perc
           << std::setw(13) << total
           << std::setw(13) << system
           << std::setw(13) << (stats[i].GetMemoryUse()+512)/1024
           << "k " << std::setiosflags(std::ios::left)
           << stats[i].GetVolume()->GetName()
           << std::resetiosflags(std::ios::floatfield|std::ios::adjustfield)
           << std::setprecision(6)
           << G4endl;
  }
 
  //
  // Second list: sort by memory use
  //
  std::sort( stats.begin(), stats.end(), G4SmartVoxelStat::ByMemory() );
 
  if (nPrint)
  {
    G4cout << "\n    Voxelisation: top memory users:" << G4endl;
    G4cout << "    Percent     Memory      Heads    Nodes   Pointers    Total CPU    Volume\n"
           << "    -------   --------     ------   ------   --------   ----------    ----------"
           << G4endl;
    //         12345678901.2345678901k .23456789.23456789.2345678901.234567890123.   .
  }

  for(i=0;i<nPrint;++i)
  {
    G4long memory = stats[i].GetMemoryUse();
    G4double totTime = stats[i].GetTotalTime();
    if (totTime < 0) { totTime = 0.0; }

    G4cout << std::setprecision(2) 
           << std::setiosflags(std::ios::fixed|std::ios::right)
           << std::setw(11) << G4double(memory*100)/G4double(totalMemory)
           << std::setw(11) << memory/1024 << "k "
           << std::setw( 9) << stats[i].GetNumberHeads()
           << std::setw( 9) << stats[i].GetNumberNodes()
           << std::setw(11) << stats[i].GetNumberPointers()
           << std::setw(13) << totTime << "    "
           << std::setiosflags(std::ios::left)
           << stats[i].GetVolume()->GetName()
           << std::resetiosflags(std::ios::floatfield|std::ios::adjustfield)
           << std::setprecision(6)
           << G4endl;
  }
}
