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
// $Id: G4UIterminal.cc,v 1.19 2003-06-16 16:56:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4Types.hh"
#include <strstream>
#include "G4StateManager.hh"
#include "G4UIcommandTree.hh"
#include "G4UIcommand.hh"
#include "G4UIcommandStatus.hh"
#include "G4UIterminal.hh"
#include "G4UIcsh.hh"

//////////////////////////////////////////////
G4UIterminal::G4UIterminal(G4VUIshell* aShell)
//////////////////////////////////////////////
{
  UI= G4UImanager::GetUIpointer();
  UI-> SetSession(this);
  UI-> SetCoutDestination(this);

  iExit= FALSE;
  iCont= FALSE;

  if(aShell) shell= aShell;
  else shell= new G4UIcsh;
}

/////////////////////////////
G4UIterminal::~G4UIterminal() 
/////////////////////////////
{ 
  if(shell) delete shell;

  if(G4UImanager::GetUIpointer()) {
    UI-> SetSession(NULL);
    UI-> SetCoutDestination(NULL);
  }
}


////////////////////////////////////////////////////
void G4UIterminal::SetPrompt(const G4String& prompt) 
////////////////////////////////////////////////////
{
  shell-> SetPrompt(prompt);
}

/////////////////////////////////////////
G4UIsession* G4UIterminal::SessionStart()
/////////////////////////////////////////
{
  iExit= TRUE;

  G4String newCommand= GetCommand();
  while(iExit){
    ExecuteCommand(newCommand);
    newCommand= GetCommand();
  }
  return NULL;
}

//////////////////////////////////////////////////
void G4UIterminal::PauseSessionStart(G4String msg)
//////////////////////////////////////////////////
{
  iCont= TRUE;

  G4String newCommand= GetCommand(msg);
  while(iCont){
    ExecuteCommand(newCommand);
    newCommand= GetCommand(msg);
  }
}

////////////////////////////////////////////////////
void G4UIterminal::ExecuteCommand(G4String aCommand)
////////////////////////////////////////////////////
{
  if(aCommand.length()<2) return;

  G4int returnVal = UI-> ApplyCommand(aCommand);

  G4int paramIndex = returnVal % 100;
  // 0 - 98 : paramIndex-th parameter is invalid
  // 99     : convination of parameters is invalid
  G4int commandStatus = returnVal - paramIndex;

  G4UIcommand* cmd = 0;
  if(commandStatus!=fCommandSucceeded)
  { cmd = FindCommand(aCommand); }

  switch(commandStatus) {
  case fCommandSucceeded:
    break;
  case fCommandNotFound:
    G4cerr << "command <" << UI->SolveAlias(aCommand) << "> not found" << G4endl;
    if( aCommand.index("@@") != G4String::npos) {
      G4cout << "@@G4UIterminal" << G4endl;
    }
    break;
  case fIllegalApplicationState:
    G4cerr << "illegal application state -- command refused" << G4endl;
    break;
  case fParameterOutOfRange:
    // if(paramIndex<99) {
    //   G4cerr << "Parameter is out of range (index " << paramIndex << ")" << G4endl;
    //   G4cerr << "Allowed range : " << cmd->GetParameter(paramIndex)->GetParameterRange() << G4endl;
    // } else {
    //   G4cerr << "Parameter is out of range" << G4endl;
    //   G4cerr << "Allowed range : " << cmd->GetRange() << G4endl;
    // }
    break;
  case fParameterOutOfCandidates:
    G4cerr << "Parameter is out of candidate list (index " << paramIndex << ")" << G4endl;
    G4cerr << "Candidates : " << cmd->GetParameter(paramIndex)->GetParameterCandidates() << G4endl;
    break;
  case fParameterUnreadable:
    G4cerr << "Parameter is wrong type and/or is not omittable (index " << paramIndex << ")" << G4endl;
    break;
  case fAliasNotFound:
  default:
    G4cerr << "command refused (" << commandStatus << ")" << G4endl;
  }
}

///////////////////////////////////
G4String G4UIterminal::GetCommand(const char* msg)
///////////////////////////////////
{
  G4String newCommand;
  G4String nullString;

  newCommand= shell-> GetCommandLine(msg);

  G4String nC= newCommand.strip(G4String::leading);
  if( nC.length() == 0 ) {
    newCommand= nullString;

  } else if( nC(0) == '#' ) {  
    G4cout << nC << G4endl;
    newCommand= nullString;

  } else if(nC=="ls" || nC(0,3)=="ls " ) {  // list commands
    ListDirectory(nC); 
    newCommand= nullString;

  } else if(nC=="lc" || nC(0,3)=="lc " ) {  // ... by shell
    shell-> ListCommand(nC.remove(0,2)); 
    newCommand= nullString;

  } else if(nC == "pwd") { // show current directory
    G4cout << "Current Command Directory : " 
	   << GetCurrentWorkingDirectory() << G4endl; 
    newCommand= nullString;

  } else if(nC == "cwd") { // ... by shell
    shell-> ShowCurrentDirectory();
    newCommand= nullString;

  } else if(nC == "cd" || nC(0,3) == "cd ") {  // "cd"
    ChangeDirectoryCommand(nC); 
    shell-> SetCurrentDirectory(GetCurrentWorkingDirectory());
    newCommand= nullString;

  } else if(nC == "help" || nC(0,5) == "help ") {  // "help"
    TerminalHelp(nC);
    newCommand= nullString;

  } else if(nC(0) == '?') {   // "show current value of a parameter"
    ShowCurrent(nC);
    newCommand= nullString;

  } else if(nC == "hist" || nC == "history") {     // "hist/history"
    G4int nh= UI-> GetNumberOfHistory();
    for (G4int i=0; i<nh; i++) { 
      G4cout << i << ": " << UI->GetPreviousCommand(i) << G4endl; 
    }
    newCommand= nullString;

  } else if(nC(0) == '!') {   // "!"
    G4String ss= nC(1, nC.length()-1);
    G4int vl;
    const char* tt= ss;
    std::istrstream is((char*)tt);
    is >> vl;
    G4int nh= UI-> GetNumberOfHistory();
    if(vl>=0 && vl<nh) { 
      newCommand= UI-> GetPreviousCommand(vl); 
      G4cout << newCommand << G4endl;
    } else { 
      G4cerr << "history " << vl << " is not found." << G4endl; 
      newCommand= nullString;
    }

  } else if(nC == "exit") {   // "exit"
    if(iCont) { 
      G4cout << "You are now processing RUN." << G4endl;
      G4cout << "Please abort it using \"/run/abort\" command first" << G4endl;
      G4cout << " and use \"continue\" command until the application" 
	     << G4endl;
      G4cout << " becomes to Idle." << G4endl;
    } else {
      iExit= FALSE;
      newCommand= nullString;
    }

  } else if( nC == "cont" || nC == "continue"){     // "cont/continu"
    iCont= FALSE;
    newCommand= nullString;

  } else if( nC.empty() ){ // NULL command
    newCommand= nullString;
    
  } else {
  }

  return ModifyToFullPathCommand(newCommand);
}


//////////////////////////////////////////////////////
G4int G4UIterminal::ReceiveG4cout(G4String coutString)
//////////////////////////////////////////////////////
{
  std::cout << coutString << std::flush;
  return 0;
}

//////////////////////////////////////////////////////
G4int G4UIterminal::ReceiveG4cerr(G4String cerrString)
//////////////////////////////////////////////////////
{
  std::cerr << cerrString << std::flush;
  return 0;
}

///////////////////////////////////////////////
G4bool G4UIterminal::GetHelpChoice(G4int& aInt)
///////////////////////////////////////////////
{
  G4cin >> aInt;
  if(!G4cin.good()){
    G4cin.clear();
    G4cin.ignore(30,'\n');
    return FALSE;
  }
  return TRUE;
}

/////////////////////////////
void G4UIterminal::ExitHelp()
/////////////////////////////
{
  char temp[100];
  G4cin.getline(temp, 100);
}

