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
// Class Description:
// The user has a possibility to define and to fill his histograms in this class.
// Class Description - end
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Test17RunAction_h
#define Test17RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4Proton.hh"
#include "G4Alpha.hh"
#include "G4IonC12.hh"
#include "G4IonAr40.hh"
#include "G4IonFe56.hh"
#include "G4Electron.hh"
#include "g4std/iostream"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//class Test17RunMessenger;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Test17RunAction : public G4UserRunAction
{
public: // Without description

    Test17RunAction();
   ~Test17RunAction();

public: // With description
 
    void BeginOfRunAction(const G4Run*);

    void   EndOfRunAction(const G4Run*);

public: // Without description

    void CountEvent();
    void CountParticles(G4double,G4double);
    void AddEP(G4double,G4double);
    void AddEdeps(G4double Eabs); 
    void AddTrackLength(G4double tlabs); 
    void AddnStepsCharged(G4double ns);
    void AddnStepsNeutral(G4double ns);

    void AddTrRef(G4double tr,G4double ref);

    void FillEn(G4double e) {kinEnergy0 = e;};
    void FillDef(const G4ParticleDefinition* p) {part0 = p;};
    void FillThBack(G4double Th);
    void FillR(G4double R);
    void FillTt(G4double Tt);
    void FillTb(G4double Tt);
    void FillTsec(G4double T);
    void FillGammaSpectrum(G4double E);
    void FillNbOfSteps(G4double nstep);
    void Fillvertexz(G4double z);

    void SetnbinStep(G4int nbin);
    void SetSteplow(G4double Slow);
    void SetStephigh(G4double Shigh);

    void SetnbinEn(G4int nbin);
    void SetEnlow(G4double Elow);
    void SetEnhigh(G4double Enhigh);

    void SetnbinTt(G4int nbin);
    void SetTtlow(G4double Ttlow);
    void SetTthigh(G4double Tthigh);

    void SetnbinTb(G4int nbin);
    void SetTblow(G4double Tblow);
    void SetTbhigh(G4double Tbhigh);

    void SetnbinTsec(G4int nbin);
    void SetTseclow(G4double Tlow);
    void SetTsechigh(G4double Thigh);

    void SetnbinTh(G4int nbin);
    void SetThlow(G4double Thlow);
    void SetThhigh(G4double Thhigh);

    void SetnbinThBack(G4int nbin);
    void SetThlowBack(G4double Thlow);
    void SetThhighBack(G4double Thhigh);

    void SetnbinR(G4int nbin);
    void SetRlow(G4double Rlow);
    void SetRhigh(G4double Rhigh);

    void SetnbinGamma(G4int nbin);
    void SetElowGamma(G4double Elow);
    void SetEhighGamma(G4double Ehigh);

    void Setnbinzvertex(G4int nbin);
    void Setzlow(G4double z);
    void Setzhigh(G4double z);
    void SaveToTuple(G4String parname,G4double val);
    void SaveToTuple(G4String parname,G4double val,G4double defval);
    void SaveEvent();

  private:


    G4double LowestEnergy,HighestEnergy;
    G4int TotBin;

    G4ParticleDefinition* theProton ;
    const G4Electron* theElectron ;

    G4double EnergySumAbs,EnergySquareSumAbs;
    G4double tlSumAbs,tlsquareSumAbs;
    G4double nStepSumCharged,nStepSum2Charged ;
    G4double nStepSumNeutral,nStepSum2Neutral ;
    G4double TotNbofEvents;
    G4double TotNbofEvents0;
    G4double SumCharged,Sum2Charged,SumNeutral,Sum2Neutral;
    G4double Selectron,Spositron;

    G4double Transmitted,Reflected ;

    G4double entryStep,underStep,overStep,distStep[200];
    G4double Steplow,Stephigh,dStep;
    G4int    nbinStep;
    G4double entryEn,underEn,overEn,distEn[200];
    G4double Enlow,Enhigh,dEn;
    G4int    nbinEn;
    G4double entryTt,underTt,overTt,distTt[200];
    G4double Ttlow,Tthigh,dTt;
    G4int    nbinTt;
    G4double Ttmean,Tt2mean;
    G4double entryTb,underTb,overTb,distTb[200];
    G4double Tblow,Tbhigh,dTb;
    G4int    nbinTb;
    G4double Tbmean,Tb2mean;
    G4double entryTsec,underTsec,overTsec,distTsec[200];
    G4double Tseclow,Tsechigh,dTsec;
    G4int    nbinTsec;
    G4double entryTh,underTh,overTh,distTh[200];
    G4double Thlow,Thhigh,dTh;
    G4int    nbinTh;
    G4double entryThback,underThback,overThback,distThback[200];
    G4double Thlowback,Thhighback,dThback;
    G4int    nbinThback;
    G4double entryR ,underR ,overR ,distR[200];
    G4double Rlow,Rhigh,dR;
    G4int    nbinR;
    G4double Rmean,R2mean;
    G4double entryGamma,underGamma,overGamma,distGamma[200];
    G4double ElowGamma,EhighGamma,dEGamma;
    G4int nbinGamma ;
    G4double entryvertexz,undervertexz,oververtexz,distvertexz[200];
    G4double zlow,zhigh,dz;
    G4int nbinvertexz;
    G4double kinEnergy0;
    const G4ParticleDefinition* part0;
 
    //Test17RunMessenger* runMessenger ;

};

#endif

