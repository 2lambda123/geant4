// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: FCALTBEventAction.hh,v 1.2 2002-10-02 19:38:35 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef FCALTBEventAction_h
#define FCALTBEventAction_h 1

#include "G4UserEventAction.hh"
#include "ExN03SteppingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class FCALTBEventAction : public G4UserEventAction
{
  public:
    FCALTBEventAction(ExN03SteppingAction* );
    virtual ~FCALTBEventAction();

  public:
    virtual void   BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void SetDrawFlag   (G4String val)  {drawFlag = val;};
    void SetPrintModulo(G4int    val)  {printModulo = val;};

  private:
    G4int                       calorimeterCollID;                
    G4String                    drawFlag;
    G4int                       printModulo;   
     
    ExN03SteppingAction* StepAction;

  private:
  G4int NTracksOutOfWorld, NSecondaries, Init1, Init2, Init3;
  // G4double OutOfWorldTracks[1000][11], Secondaries[1000][11];  

};

#endif

    
