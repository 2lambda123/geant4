// ********************************************************************
// *                                                                  *
// * cosmicray_charging advanced example for Geant4                   *
// * (adapted simulation of test-mass charging in the LISA mission)   *
// *                                                                  *
// * Henrique Araujo (h.araujo@imperial.ac.uk) & Peter Wass           *
// * Imperial College London                                          *
// *                                                                  *
// ********************************************************************

#ifndef LISARunAction_h
#define LISARunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class LISARunActionMessenger;
class G4Run;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class LISARunAction : public G4UserRunAction {

  public:
    LISARunAction();
   ~LISARunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);


  public:
    inline void SetAutoSeed (const G4bool val)    {autoSeed    = val;}

  private:
    LISARunActionMessenger* runMessenger;

    G4bool autoSeed;

};

#endif

