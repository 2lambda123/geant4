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
// $Id: OlapManager.hh,v 1.3 2003-06-16 16:49:19 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
// OlapManager
//
// Author: Martin Liendl - Martin.Liendl@cern.ch
//
// --------------------------------------------------------------
//
#ifndef OlapManager_h
#define OlapManager_h

#include <vector>
#include <map>
#include <set>

#include "globals.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "OlapNotify.hh"
#include "OlapEventAction.hh" // class OlapInfo!

class OlapManagerMessenger;
class OlapDetConstr;
class OlapVolTree;
class G4RunManager;
class G4GeoNav;
class OlapGui;
class CMSPrimaryGeneratorAction;
class OlapGenerator;

class OlapManager
{
public:
   ~OlapManager();
   static OlapManager * GetOlapManager();
   void TriggerRun();
   void TriggerFull(G4int=10);  
   G4VPhysicalVolume * GetNewWorld();
   G4VPhysicalVolume * GetFullWorld();
   //G4bool SetNextWorld(G4int=1);
   //G4bool SetPrevWorld(G4int=1); 
   void SetGrid(G4int,G4int,G4int);
   void SetDelta(G4double d) { delta = d;}
   G4double Delta() { return delta;}
   G4bool Next();
   void ListLV(const G4String &);
   void GotoLV(const G4String &);
   void ChangeLV(const G4String &);
   void SetNewWorld(G4LogicalVolume*);
   void SetRotation(G4double theta, G4double phi, G4double alpha);
   G4LogicalVolume* GetOriginalWorld();
   void PwdLV();
   void LsLV();
   void TestVolTree();
   G4int GetNrLVs();
   //! interrupts olap-processing (only during 'triggerFull');
   // next trigger/triggerFull will continue!
   void Interrupt() { interrupt=true ; } 
   void registerNotification(OlapNotify*n) { theNotifs.insert(n); }
   void notifyNewWorld(G4LogicalVolume*);
   void notifyOlaps(const std::vector<OlapInfo*> &);
   void deRegisterNotification(OlapNotify*n) { ; }
   // FIXME: handle completly different in next redesign!
   std::map<G4LogicalVolume *, G4bool> NoOlapMap; 

private:

   OlapManager();
   
   OlapManagerMessenger * theMessenger;
   OlapDetConstr * theDet;

   OlapGenerator * olapGenerator;
   CMSPrimaryGeneratorAction * cmsGenerator;   
   G4double delta;
   G4int eventsPerRun;
   G4int lvPos;
   G4bool polyMode;
   G4RunManager * theRunManager;
   G4LogicalVolumeStore * theLVStore;
   std::vector<G4LogicalVolume*>::iterator theLVit;
   std::vector<G4LogicalVolume*> theLVs;
   static OlapManager * theInstance;  
   G4GeoNav * theGeoNav;
   std::set<OlapNotify*> theNotifs;
   G4bool interrupt;
};
#endif
