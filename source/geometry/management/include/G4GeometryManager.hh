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
// $Id: G4GeometryManager.hh,v 1.8 2003-11-02 14:01:21 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4GeometryManager
//
// Class description:
//
// A class responsible for high level geometrical functions, and for
// high level objects in the geometry subdomain.
// The class is a `singleton', with access via the static method
// G4GeometryManager::GetInstance().
//
// Member data:
//
//   static G4GeometryManager* fgInstance
//     - Ptr to the unique instance of class

// Author:
// 26.07.95 P.Kent Initial version, including optimisation Build
// --------------------------------------------------------------------
#ifndef G4GEOMETRYMANAGER_HH
#define G4GEOMETRYMANAGER_HH

#include <vector>
#include "G4Types.hh"
#include "G4SmartVoxelStat.hh"

class G4GeometryManager
{
  public: // with description
  
    G4bool CloseGeometry(G4bool pOptimise=true, G4bool verbose=false);
      // Close (`lock') the geometry: perform sanity and `completion' checks
      // and optionally [default=yes] build optimisation information.
    
    void OpenGeometry();
      // Open (`unlock') the geometry and remove optimisation information if
      // present.

    G4bool IsGeometryClosed();
      // Return true/false according to state of optimised geoemtry.

    static G4GeometryManager* GetInstance();
      // Return ptr to singleton instance of the class.

  protected:

    G4GeometryManager();

  private:

    void BuildOptimisations(G4bool allOpt, G4bool verbose=false);
    void DeleteOptimisations();
    static void ReportVoxelStats( std::vector<G4SmartVoxelStat> & stats,
                                  G4double totalCpuTime );
    static G4GeometryManager* fgInstance;
    G4bool fIsClosed;
};

#endif
