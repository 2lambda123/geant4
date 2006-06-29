//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// File name:     RadmonPhysicsList.hh
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPhysicsList.hh,v 1.6 2006-06-29 16:17:29 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Implementation of the G4VUserPhysicsList
//

#ifndef   RADMONPHYSICSLIST_HH
 #define  RADMONPHYSICSLIST_HH

 // Include files
 #include "globals.hh"
 #include "G4VUserPhysicsList.hh"
 #include "RadmonVLayoutObserver.hh"
 #include <list>
 
 // Forward declaration
 class RadmonVPhysicsLayout;
 class RadmonVSubPhysicsListFactory;
 class RadmonVSubPhysicsList;
 class RadmonPhysicsSteppingAction;

 class RadmonPhysicsList : public G4VUserPhysicsList, public RadmonVLayoutObserver
 {
  public:
                                                RadmonPhysicsList(RadmonVPhysicsLayout * layout, RadmonVSubPhysicsListFactory * factory);
   virtual                                     ~RadmonPhysicsList();

   virtual void                                 OnLayoutChange(void);

   virtual void                                 ConstructParticle(void);
   virtual void                                 ConstructProcess(void);
   virtual void                                 SetCuts(void);

  private:
  // Private methods
   void                                         Destruct(void);
   void                                         CheckUpdate(void);
   void                                         UpdateProcessManagers(void);

  // Hidden constructors and operators
                                                RadmonPhysicsList();
                                                RadmonPhysicsList(const RadmonPhysicsList & copy);
   RadmonPhysicsList &                          operator=(const RadmonPhysicsList & copy);

  // Private data types
   typedef std::list<RadmonVSubPhysicsList *>   SubPhysiscsLists;

  // Private attributes
   RadmonVPhysicsLayout *                       physicsLayout;
   RadmonVSubPhysicsListFactory *               subPhysicsListFactory;
    
   SubPhysiscsLists                             subPhysiscsLists;

   RadmonPhysicsSteppingAction *                steppingAction;
   
   G4bool                                       initializationMethodsCalled;
   G4bool                                       changed;
 };
#endif /* RADMONPHYSICSLIST_HH */
