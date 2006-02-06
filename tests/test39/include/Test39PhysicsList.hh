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
//    ---------- fake Test39PhysicsList header -------
//    Created by Mikhail Kossov, 7 Jan 2005 
//
// **********************************************************************

#ifndef Test39PhysicsList_h
#define Test39PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class Test39PhysicsList: public G4VUserPhysicsList
{
public:
  Test39PhysicsList();
  ~Test39PhysicsList();

protected:
  // Construct particle and physics process
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

public:
  // Set/Get cut values 
  void      SetCutForGamma(G4double);
  void      SetCutForElectron(G4double);
  void      SetCutForProton(G4double);           
  G4double  GetCutForGamma() const;
  G4double  GetCutForElectron() const;
  G4double  GetCutForProton() const;
    
protected:
  // these methods Construct particles 
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();
  void ConstructAllShortLiveds();

protected:
  // these methods Construct physics processes and register them
  void ConstructGeneral();
  void ConstructEM();

private:
  G4double cutForGamma;
  G4double cutForElectron; 
  G4double cutForProton;
};

#endif







