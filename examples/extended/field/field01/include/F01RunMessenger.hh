
// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: F01RunMessenger.hh,v 1.1 2001-03-27 16:21:29 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef F01RunMessenger_h
#define F01RunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class F01RunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class F01RunMessenger: public G4UImessenger
{
  public:

   F01RunMessenger(F01RunAction* );
  ~F01RunMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:

   F01RunAction*              runAction;
   
   G4UIdirectory*             plotDir;   

   G4UIcmdWithAString*        sethistNameCmd;

   G4UIcmdWithAnInteger*      setnbinStepCmd; 
   G4UIcmdWithADouble*        setSteplowCmd; 
   G4UIcmdWithADouble*        setStephighCmd; 

   G4UIcmdWithAnInteger*      setnbinEnCmd; 
   G4UIcmdWithADoubleAndUnit* setEnlowCmd; 
   G4UIcmdWithADoubleAndUnit* setEnhighCmd; 

   G4UIcmdWithAnInteger*      setnbinGammaCmd; 
   G4UIcmdWithADoubleAndUnit* setElowGammaCmd; 
   G4UIcmdWithADoubleAndUnit* setEhighGammaCmd; 

   G4UIcmdWithAnInteger*      setnbinTtCmd; 
   G4UIcmdWithADoubleAndUnit* setTtlowCmd; 
   G4UIcmdWithADoubleAndUnit* setTthighCmd; 

   G4UIcmdWithAnInteger*      setnbinTbCmd; 
   G4UIcmdWithADoubleAndUnit* setTblowCmd; 
   G4UIcmdWithADoubleAndUnit* setTbhighCmd; 

   G4UIcmdWithAnInteger*      setnbinTsecCmd; 
   G4UIcmdWithADoubleAndUnit* setTseclowCmd; 
   G4UIcmdWithADoubleAndUnit* setTsechighCmd; 

   G4UIcmdWithAnInteger*      setnbinRCmd; 
   G4UIcmdWithADoubleAndUnit* setRlowCmd; 
   G4UIcmdWithADoubleAndUnit* setRhighCmd; 

   G4UIcmdWithAnInteger*      setnbinThCmd; 
   G4UIcmdWithADoubleAndUnit* setThlowCmd; 
   G4UIcmdWithADoubleAndUnit* setThhighCmd; 

   G4UIcmdWithAnInteger*      setnbinThbackCmd; 
   G4UIcmdWithADoubleAndUnit* setThlowbackCmd; 
   G4UIcmdWithADoubleAndUnit* setThhighbackCmd; 

   G4UIcmdWithAnInteger*      setnbinzvertexCmd; 
   G4UIcmdWithADoubleAndUnit* setzlowCmd; 
   G4UIcmdWithADoubleAndUnit* setzhighCmd;
   
   G4UIdirectory*             RndmDir;
   G4UIcmdWithAnInteger*      RndmSaveCmd;    
   G4UIcmdWithAString*        RndmReadCmd;    
 
};

#endif

