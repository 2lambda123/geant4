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
// $Id: G4EnergyLossTables.cc,v 1.20 2003-03-13 16:19:02 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
// first version created by P.Urban , 06/04/1998
// modifications + "precise" functions added by L.Urban , 27/05/98
// modifications , TOF functions , 26/10/98, L.Urban
// cache mechanism in order to gain time, 11/02/99, L.Urban
// bug fixed , 12/04/99 , L.Urban
// 10.11.99: moved from RWT hash dictionary to STL map, G.Barrand, M.Maire
// 27.09.01 L.Urban , bug fixed (negative energy deposit)
// 26.10.01 all static functions moved from .icc files (mma)
// 15.01.03 Add interfaces required for "cut per region" (V.Ivanchenko)
// 12.03.03 Add warnings to obsolete interfaces (V.Ivanchenko)
// -------------------------------------------------------------------

#include "G4EnergyLossTables.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4RegionStore.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EnergyLossTablesHelper G4EnergyLossTables::t  ;
const G4ParticleDefinition* G4EnergyLossTables::lastParticle = 0;
G4double G4EnergyLossTables::QQPositron = eplus*eplus ;
G4double G4EnergyLossTables::Chargesquare ;
G4int    G4EnergyLossTables::oldIndex = -1 ;
G4double G4EnergyLossTables::rmin = 0. ;
G4double G4EnergyLossTables::rmax = 0. ;
G4double G4EnergyLossTables::Thigh = 0. ;
G4int    G4EnergyLossTables::let_counter = 0;
G4int    G4EnergyLossTables::let_max_num_warnings = 2;

G4EnergyLossTables::helper_map G4EnergyLossTables::dict;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EnergyLossTablesHelper::G4EnergyLossTablesHelper(
  const G4PhysicsTable* aDEDXTable,
  const G4PhysicsTable* aRangeTable,
  const G4PhysicsTable* anInverseRangeTable,
  const G4PhysicsTable* aLabTimeTable,
  const G4PhysicsTable* aProperTimeTable,
  G4double aLowestKineticEnergy,
  G4double aHighestKineticEnergy,
  G4double aMassRatio,
  G4int aNumberOfBins)
  :
  theDEDXTable(aDEDXTable), theRangeTable(aRangeTable),
  theInverseRangeTable(anInverseRangeTable),
  theLabTimeTable(aLabTimeTable),
  theProperTimeTable(aProperTimeTable),
  theLowestKineticEnergy(aLowestKineticEnergy),
  theHighestKineticEnergy(aHighestKineticEnergy),
  theMassRatio(aMassRatio),
  theNumberOfBins(aNumberOfBins)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EnergyLossTablesHelper::G4EnergyLossTablesHelper()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EnergyLossTables::Register(
  const G4ParticleDefinition* p,
  const G4PhysicsTable* tDEDX,
  const G4PhysicsTable* tRange,
  const G4PhysicsTable* tInverseRange,
  const G4PhysicsTable* tLabTime,
  const G4PhysicsTable* tProperTime,
  G4double lowestKineticEnergy,
  G4double highestKineticEnergy,
  G4double massRatio,
  G4int NumberOfBins)
{
  dict[p]= G4EnergyLossTablesHelper(tDEDX, tRange,tInverseRange,
                    tLabTime,tProperTime,lowestKineticEnergy,
		    highestKineticEnergy, massRatio,NumberOfBins);

  t = GetTables(p) ;    // important for cache !!!!!
  lastParticle = p ;
  Chargesquare = (p->GetPDGCharge())*(p->GetPDGCharge())/
                  QQPositron ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4PhysicsTable* G4EnergyLossTables::GetDEDXTable(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) return 0;
  return (*it).second.theDEDXTable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4PhysicsTable* G4EnergyLossTables::GetRangeTable(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) return 0;
  return (*it).second.theRangeTable;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4PhysicsTable* G4EnergyLossTables::GetInverseRangeTable(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) return 0;
  return (*it).second.theInverseRangeTable;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4PhysicsTable* G4EnergyLossTables::GetLabTimeTable(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) return 0;
  return (*it).second.theLabTimeTable;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4PhysicsTable* G4EnergyLossTables::GetProperTimeTable(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) return 0;
  return (*it).second.theProperTimeTable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EnergyLossTablesHelper G4EnergyLossTables::GetTables(
  const G4ParticleDefinition* p)
{
  helper_map::iterator it;
  if((it=dict.find(p))==dict.end()) {
    G4Exception("G4EnergyLossTables::GetTables: table not found!");
    exit(1);
  }
  return (*it).second;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetDEDX(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                   QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double dEdx;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     dEdx =(*dEdxTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut)
           *sqrt(scaledKineticEnergy/t.theLowestKineticEnergy);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     dEdx = (*dEdxTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut);

  } else {

    dEdx = (*dEdxTable)(materialIndex)->GetValue(
	       scaledKineticEnergy,isOut);

  }

  return dEdx*Chargesquare;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetLabTime(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* labtimeTable= t.theLabTimeTable;

  const G4double parlowen=0.4 , ppar=0.5-parlowen ;
  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double time;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     time = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
            (*labtimeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     time = (*labtimeTable)(materialIndex)->GetValue(
              t.theHighestKineticEnergy,isOut);

  } else {

    time = (*labtimeTable)(materialIndex)->GetValue(
               scaledKineticEnergy,isOut);

  }

  return time/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetDeltaLabTime(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergyStart,
    G4double KineticEnergyEnd,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* labtimeTable= t.theLabTimeTable;

  const G4double parlowen=0.4 , ppar=0.5-parlowen ;
  const G4double dToverT = 0.05 , facT = 1. -dToverT ;
  G4double timestart,timeend,deltatime,dTT;
  G4bool isOut;

  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergyStart*t.theMassRatio;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     timestart = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
                (*labtimeTable)(materialIndex)->GetValue(
                t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     timestart = (*labtimeTable)(materialIndex)->GetValue(
                t.theHighestKineticEnergy,isOut);

  } else {

    timestart = (*labtimeTable)(materialIndex)->GetValue(
                scaledKineticEnergy,isOut);

  }

  dTT = (KineticEnergyStart - KineticEnergyEnd)/KineticEnergyStart ;

  if( dTT < dToverT )
    scaledKineticEnergy = facT*KineticEnergyStart*t.theMassRatio;
  else
    scaledKineticEnergy = KineticEnergyEnd*t.theMassRatio;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     timeend = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
                (*labtimeTable)(materialIndex)->GetValue(
                t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     timeend = (*labtimeTable)(materialIndex)->GetValue(
                t.theHighestKineticEnergy,isOut);

  } else {

    timeend = (*labtimeTable)(materialIndex)->GetValue(
                scaledKineticEnergy,isOut);

  }

  deltatime = timestart - timeend ;

  if( dTT < dToverT )
    deltatime *= dTT/dToverT;

  return deltatime/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetProperTime(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* propertimeTable= t.theProperTimeTable;

  const G4double parlowen=0.4 , ppar=0.5-parlowen ;
  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double time;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     time = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
            (*propertimeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     time = (*propertimeTable)(materialIndex)->GetValue(
              t.theHighestKineticEnergy,isOut);

  } else {
   
    time = (*propertimeTable)(materialIndex)->GetValue(
               scaledKineticEnergy,isOut);

  }

  return time/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetDeltaProperTime(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergyStart,
    G4double KineticEnergyEnd,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* propertimeTable= t.theProperTimeTable;

  const G4double parlowen=0.4 , ppar=0.5-parlowen ;
  const G4double dToverT = 0.05 , facT = 1. -dToverT ;
  G4double timestart,timeend,deltatime,dTT;
  G4bool isOut;

  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergyStart*t.theMassRatio;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     timestart = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
                (*propertimeTable)(materialIndex)->GetValue(
                t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     timestart = (*propertimeTable)(materialIndex)->GetValue(
                t.theHighestKineticEnergy,isOut);

  } else {

    timestart = (*propertimeTable)(materialIndex)->GetValue(
                scaledKineticEnergy,isOut);

  }

  dTT = (KineticEnergyStart - KineticEnergyEnd)/KineticEnergyStart ;

  if( dTT < dToverT )
    scaledKineticEnergy = facT*KineticEnergyStart*t.theMassRatio;
  else
    scaledKineticEnergy = KineticEnergyEnd*t.theMassRatio;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     timeend = exp(ppar*log(scaledKineticEnergy/t.theLowestKineticEnergy))*
                (*propertimeTable)(materialIndex)->GetValue(
                t.theLowestKineticEnergy,isOut);


  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     timeend = (*propertimeTable)(materialIndex)->GetValue(
                t.theHighestKineticEnergy,isOut);

  } else {

    timeend = (*propertimeTable)(materialIndex)->GetValue(
                scaledKineticEnergy,isOut);

  }

  deltatime = timestart - timeend ;

  if( dTT < dToverT )
    deltatime *= dTT/dToverT ;

  return deltatime/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetRange(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* rangeTable= t.theRangeTable;
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double Range;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

    Range = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)*
            (*rangeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

    Range = (*rangeTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut)+
            (scaledKineticEnergy-t.theHighestKineticEnergy)/
            (*dEdxTable)(materialIndex)->GetValue(
              t.theHighestKineticEnergy,isOut);

  } else {

    Range = (*rangeTable)(materialIndex)->GetValue(
	       scaledKineticEnergy,isOut);

  }

  return Range/(Chargesquare*t.theMassRatio);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetPreciseEnergyFromRange(
                                     const G4ParticleDefinition *aParticle,
                                           G4double range,
                                     const G4Material *aMaterial)
// it returns the value of the kinetic energy for a given range
{
  CPRWarning();
  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;
  const G4PhysicsTable*  inverseRangeTable= t.theInverseRangeTable;

  G4double scaledrange,scaledKineticEnergy ;
  G4bool isOut ;

  G4int materialIndex = aMaterial->GetIndex() ;

  if(materialIndex != oldIndex)
  {
    oldIndex = materialIndex ;
    rmin = (*inverseRangeTable)(materialIndex)->
                              GetLowEdgeEnergy(0) ;
    rmax = (*inverseRangeTable)(materialIndex)->
                   GetLowEdgeEnergy(t.theNumberOfBins-2) ;
    Thigh = (*inverseRangeTable)(materialIndex)->
                              GetValue(rmax,isOut) ;
  }

  scaledrange = range*Chargesquare*t.theMassRatio ;

  if(scaledrange < rmin)
  {
    scaledKineticEnergy = t.theLowestKineticEnergy*
                   scaledrange*scaledrange/(rmin*rmin) ;
  }
  else
  {
    if(scaledrange < rmax)
    {
      scaledKineticEnergy = (*inverseRangeTable)(materialIndex)->
                              GetValue( scaledrange,isOut) ;
    }
    else
    {
      scaledKineticEnergy = Thigh +
                      (scaledrange-rmax)*
                      (*dEdxTable)(materialIndex)->
                                 GetValue(Thigh,isOut) ;
    }
  }

  return scaledKineticEnergy/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 G4double G4EnergyLossTables::GetPreciseDEDX(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = aMaterial->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double dEdx;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     dEdx = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)
            *(*dEdxTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     dEdx = (*dEdxTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut);

  } else {

      dEdx = (*dEdxTable)(materialIndex)->GetValue(
                          scaledKineticEnergy,isOut) ;

  }

  return dEdx*Chargesquare;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 G4double G4EnergyLossTables::GetPreciseRangeFromEnergy(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4Material *aMaterial)
{
  CPRWarning();
  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* rangeTable= t.theRangeTable;
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = aMaterial->GetIndex();

  G4double Thighr = t.theHighestKineticEnergy*t.theLowestKineticEnergy/
                   (*rangeTable)(materialIndex)->
                   GetLowEdgeEnergy(1) ;

  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double Range;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

    Range = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)*
            (*rangeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>Thighr) {

    Range = (*rangeTable)(materialIndex)->GetValue(
	      Thighr,isOut)+
            (scaledKineticEnergy-Thighr)/
            (*dEdxTable)(materialIndex)->GetValue(
              Thighr,isOut);

  } else {

     Range = (*rangeTable)(materialIndex)->GetValue(
                       scaledKineticEnergy,isOut) ;

  }

  return Range/(Chargesquare*t.theMassRatio);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetDEDX(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4MaterialCutsCouple *couple)
{
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                   QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = couple->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double dEdx;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     dEdx =(*dEdxTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut)
           *sqrt(scaledKineticEnergy/t.theLowestKineticEnergy);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     dEdx = (*dEdxTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut);

  } else {

    dEdx = (*dEdxTable)(materialIndex)->GetValue(
	       scaledKineticEnergy,isOut);

  }

  return dEdx*Chargesquare;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetRange(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4MaterialCutsCouple *couple)
{
  if(aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle ;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* rangeTable= t.theRangeTable;
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = couple->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double Range;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

    Range = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)*
            (*rangeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

    Range = (*rangeTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut)+
            (scaledKineticEnergy-t.theHighestKineticEnergy)/
            (*dEdxTable)(materialIndex)->GetValue(
              t.theHighestKineticEnergy,isOut);

  } else {

    Range = (*rangeTable)(materialIndex)->GetValue(
	       scaledKineticEnergy,isOut);

  }

  return Range/(Chargesquare*t.theMassRatio);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetPreciseEnergyFromRange(
                                     const G4ParticleDefinition *aParticle,
                                           G4double range,
                                     const G4MaterialCutsCouple *couple)
// it returns the value of the kinetic energy for a given range
{
  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;
  const G4PhysicsTable*  inverseRangeTable= t.theInverseRangeTable;

  G4double scaledrange,scaledKineticEnergy ;
  G4bool isOut ;

  G4int materialIndex = couple->GetIndex() ;

  if(materialIndex != oldIndex)
  {
    oldIndex = materialIndex ;
    rmin = (*inverseRangeTable)(materialIndex)->
                              GetLowEdgeEnergy(0) ;
    rmax = (*inverseRangeTable)(materialIndex)->
                   GetLowEdgeEnergy(t.theNumberOfBins-2) ;
    Thigh = (*inverseRangeTable)(materialIndex)->
                              GetValue(rmax,isOut) ;
  }

  scaledrange = range*Chargesquare*t.theMassRatio ;

  if(scaledrange < rmin)
  {
    scaledKineticEnergy = t.theLowestKineticEnergy*
                   scaledrange*scaledrange/(rmin*rmin) ;
  }
  else
  {
    if(scaledrange < rmax)
    {
      scaledKineticEnergy = (*inverseRangeTable)(materialIndex)->
                              GetValue( scaledrange,isOut) ;
    }
    else
    {
      scaledKineticEnergy = Thigh +
                      (scaledrange-rmax)*
                      (*dEdxTable)(materialIndex)->
                                 GetValue(Thigh,isOut) ;
    }
  }

  return scaledKineticEnergy/t.theMassRatio ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetPreciseDEDX(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4MaterialCutsCouple *couple)
{

  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = couple->GetIndex();
  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double dEdx;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

     dEdx = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)
            *(*dEdxTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>t.theHighestKineticEnergy) {

     dEdx = (*dEdxTable)(materialIndex)->GetValue(
	      t.theHighestKineticEnergy,isOut);

  } else {

      dEdx = (*dEdxTable)(materialIndex)->GetValue(
                          scaledKineticEnergy,isOut) ;

  }

  return dEdx*Chargesquare;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4EnergyLossTables::GetPreciseRangeFromEnergy(
    const G4ParticleDefinition *aParticle,
    G4double KineticEnergy,
    const G4MaterialCutsCouple *couple)
{
  if( aParticle != lastParticle)
  {
    t= GetTables(aParticle);
    lastParticle = aParticle;
    Chargesquare = (aParticle->GetPDGCharge())*
                   (aParticle->GetPDGCharge())/
                    QQPositron ;
    oldIndex = -1 ;
  }
  const G4PhysicsTable* rangeTable= t.theRangeTable;
  const G4PhysicsTable*  dEdxTable= t.theDEDXTable;

  G4int materialIndex = couple->GetIndex();

  G4double Thighr = t.theHighestKineticEnergy*t.theLowestKineticEnergy/
                   (*rangeTable)(materialIndex)->
                   GetLowEdgeEnergy(1) ;

  G4double scaledKineticEnergy = KineticEnergy*t.theMassRatio;
  G4double Range;
  G4bool isOut;

  if (scaledKineticEnergy<t.theLowestKineticEnergy) {

    Range = sqrt(scaledKineticEnergy/t.theLowestKineticEnergy)*
            (*rangeTable)(materialIndex)->GetValue(
              t.theLowestKineticEnergy,isOut);

  } else if (scaledKineticEnergy>Thighr) {

    Range = (*rangeTable)(materialIndex)->GetValue(
	      Thighr,isOut)+
            (scaledKineticEnergy-Thighr)/
            (*dEdxTable)(materialIndex)->GetValue(
              Thighr,isOut);

  } else {

     Range = (*rangeTable)(materialIndex)->GetValue(
                       scaledKineticEnergy,isOut) ;

  }

  return Range/(Chargesquare*t.theMassRatio);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EnergyLossTables::CPRWarning()
{
  if (let_counter <  let_max_num_warnings) {

//    G4cout << G4endl;
//    G4cout << "##### G4EnergyLossTable WARNING: The obsolete interface is used" << G4endl;
//    G4cout << "##### Please, substitute G4Material by G4MaterialCutsCouple" << G4endl;
//    G4cout << "##### Obsolete interface will be removed soon" << G4endl;
//    G4cout << G4endl;
    let_counter++;
    if ((G4RegionStore::GetInstance())->size() > 1) {
     G4Exception("G4EnergyLossTables:: More than 1 region - table cannot be accessed");
     exit(1);
   }

  } else if (let_counter == let_max_num_warnings) {

    G4cout << "##### G4EnergyLossTable WARNING closed" << G4endl;
    let_counter++;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

