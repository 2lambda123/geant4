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
// $Id: G4UserPhysicsListMessenger.hh,v 1.11 2003-03-10 08:04:18 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//---------------------------------------------------------------
//
//  G4UserPhysicsListMessenger.hh
//
//  Class Description:
//    This is a messenger class to interface to exchange information
//    between ParticleUserList and UI.
// --
//  the List of Directory and Commands
// -       
//  /run/particle/   Paricle control commands.
//   Commands : 
//    SetCuts *  Set default cut value
//    dumpList * Dump List of particles in G4VUserPhysicsList.
//    dumpCutValue * Dump cut value information
//    verbose * Set the Verbose level of G4VUserPhysicsList.
//    addProcessManager * add process manager
//    buildPhysicsTable * build physics table
//    storePhysicsTable * store physics table into files
//    retreivePhysicsTable * retreive physics table from files
//    setStoredInAscii * Switch on/off ascii mode in store/retreive Physics Table
//    applyCuts *  Set flag for ApplyCuts 
// ------------------------------------------------------------
//	History
//        first version                   09 Jan. 1998 by H.Kurashige 
//        second version                  24 Jan. 1998 by H.Kurashige 
//        add buildPhysicsTable command   13 Apr. 1999 by H.Kurashige
//        add store/retreivePhysicsTable  08 Nov. 2000 by H.Kurashige
//        add setStoredInAscii command    12 Mar. 2001 by H.Kurashige
//        add applyCuts command         2 Aug. 2001 by H.Kurashige
// ------------------------------------------------------------

#ifndef G4UserPhysicsListMessenger_h
#define G4UserPhysicsListMessenger_h 1

class G4VUserPhysicsList;

class G4VUserPhysicsList;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString; 
class G4UIcommand;

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UserPhysicsListMessenger: public G4UImessenger
{
  private:
  // hide default constructor
    G4UserPhysicsListMessenger(){}

  public:
    G4UserPhysicsListMessenger(G4VUserPhysicsList* pParticleList);
    virtual ~G4UserPhysicsListMessenger();
    
public: // with description
    virtual  void SetNewValue(G4UIcommand * command,G4String newValues);
    virtual  G4String GetCurrentValue(G4UIcommand * command);

  protected:
    G4VUserPhysicsList* thePhysicsList;
    
  private: //commands
    G4UIdirectory *             theDirectory;
    G4UIcmdWithADoubleAndUnit * setCutCmd; 
    G4UIcommand *               setCutRCmd;
    G4UIcmdWithAnInteger *      verboseCmd;
    G4UIcmdWithoutParameter *   dumpListCmd;
    G4UIcmdWithAString *        dumpCutValuesCmd;
    G4UIcmdWithAString *        addProcManCmd;
    G4UIcmdWithAString *        buildPTCmd;
    G4UIcmdWithAString *        storeCmd;
    G4UIcmdWithAString *        retrieveCmd;
    G4UIcmdWithAnInteger *      asciiCmd;
    G4UIcommand          *      applyCutsCmd;
};

#endif


