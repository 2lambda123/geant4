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
// $Id: G4UIbatch.cc,v 1.12 2005-05-19 16:11:53 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4UIbatch.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

G4UIbatch::G4UIbatch(const char* fileName,G4UIsession* prevSession) 
 : previousSession(prevSession), macroFileName(fileName),
   openFailed(false)
{
  UImanager = G4UImanager::GetUIpointer();
  UImanager->SetSession(this);
  macroFile.open((char*)fileName);
  if(macroFile.fail())
  {
    G4cerr << "macro file <" << fileName << "> could not open."
         << G4endl;
    openFailed = true;
    commandFailed = true;
  }
  else
  {
    commandFailed = false;
  }
}

G4UIbatch::~G4UIbatch() 
{
  if(!openFailed) macroFile.close();
}

G4UIsession * G4UIbatch::SessionStart() 
{
  if(!openFailed)
  {
    char commandLine[256];
    G4int lineLength = 255;

    while(1)
    {
      macroFile.getline( commandLine, lineLength );
      if( macroFile.bad() )
      {
        G4cout << "Cannot read " << macroFileName << "." << G4endl;
        commandFailed = true;
        break;
      }
      if( macroFile.eof() ) break;
      commandLine[lineLength] = '\0';
      G4String commandString = commandLine;
      G4String nC= commandString.strip(G4String::both);
      if( commandLine[0] == '#')
      { if(G4UImanager::GetUIpointer()->GetVerboseLevel()==2)
        { G4cout << commandLine << G4endl; }
      }
      else if( nC.length() == 0 )
      { continue; }
      else if(nC == "exit")
      { break; }
      else
      { 
        G4int rc = UImanager->ApplyCommand(commandLine);
        if(rc)
        {
          switch(rc) 
          {
          case fCommandNotFound:
            G4cerr << "***** COMMAND NOT FOUND <"
                   << commandLine << "> *****" << G4endl;
            break;
          case fIllegalApplicationState:
            G4cerr << "***** Illegal application state <"
                   << commandLine << "> *****" << G4endl;
            break;
          default:
            G4int pn = rc%100;
            G4cerr << "***** Illegal parameter (" << pn << ") <"
                   << commandLine << "> *****" << G4endl;
          }
          G4cerr << "***** Command ignored *****" << G4endl;
          commandFailed = true;
        }
        if(commandFailed) break;
      }
    }
  }
  return previousSession;
}

void G4UIbatch::PauseSessionStart(G4String Prompt) 
{
  G4cout << "Pause session <" << Prompt << "> start." << G4endl;
  SessionStart();
  G4cout << "Pause session <" << Prompt << "> Terminate." << G4endl;
}


