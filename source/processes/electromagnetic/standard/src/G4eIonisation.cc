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
// $Id: G4eIonisation.cc,v 1.28 2003-04-26 02:06:38 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//--------------- G4eIonisation physics process --------------------------------
//                by Laszlo Urban, 20 March 1997
//------------------------------------------------------------------------------
//
// 07-04-98 remove 'tracking cut' of the ionizing particle, mma
// 04-09-98 new methods SetBining() PrintInfo()
// 07-09-98 Cleanup
// 02-02-99 correction inDoIt , L.Urban
// 10-02-00 modifications , new e.m. structure, L.Urban
// 28-05-01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 09-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 13-08-01 new function ComputeRestrictedMeandEdx()  (mma)
// 17-09-01 migration of Materials to pure STL (mma)
// 21-09-01 completion of RetrievePhysicsTable() (mma)
// 29-10-01 all static functions no more inlined (mma)
// 07-11-01 particleMass and Charge become local variables
// 26-03-02 change access to cuts in BuildLossTables (V.Ivanchenko)
// 16-01-03 Migrade to cut per region (V.Ivanchenko)
// 08-04-03 finalRange is region aware (V.Ivanchenko)
//------------------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4eIonisation.hh"
#include "G4UnitsTable.hh"
#include "G4ProductionCutsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eIonisation::LowerBoundLambda = 1.*keV;
G4double G4eIonisation::UpperBoundLambda = 100.*TeV;
G4int    G4eIonisation::NbinLambda = 100;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4eIonisation::G4eIonisation(const G4String& processName)
   : G4VeEnergyLoss(processName),
     theMeanFreePathTable(NULL)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4eIonisation::~G4eIonisation()
{
     if (theMeanFreePathTable)
       {theMeanFreePathTable->clearAndDestroy(); delete theMeanFreePathTable;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::SetLowerBoundLambda(G4double val)
     {LowerBoundLambda = val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::SetUpperBoundLambda(G4double val)
     {UpperBoundLambda = val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::SetNbinLambda(G4int n)
     {NbinLambda = n;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eIonisation::GetLowerBoundLambda()
        {return LowerBoundLambda;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eIonisation::GetUpperBoundLambda()
        {return UpperBoundLambda;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4eIonisation::GetNbinLambda()
     {return NbinLambda;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::BuildPhysicsTable(const G4ParticleDefinition& aParticleType)
{
  if( !CutsWhereModified() && theLossTable) return;

  LowestKineticEnergy  = GetLowerBoundEloss();
  HighestKineticEnergy = GetUpperBoundEloss();
  TotBin               = GetNbinEloss();

  BuildLossTable(aParticleType);

  if (&aParticleType==G4Electron::Electron())
    {
     RecorderOfElectronProcess[CounterOfElectronProcess] = (*this).theLossTable;
     CounterOfElectronProcess++;
    }
  else
    {
     RecorderOfPositronProcess[CounterOfPositronProcess] = (*this).theLossTable;
     CounterOfPositronProcess++;
    }

  BuildLambdaTable(aParticleType);

  BuildDEDXTable(aParticleType);

  if (&aParticleType==G4Electron::Electron()) PrintInfoDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::BuildLossTable(const G4ParticleDefinition& aParticleType)
{

  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  if (theLossTable) {theLossTable->clearAndDestroy(); delete theLossTable;}
  theLossTable = new G4PhysicsTable(numOfCouples);

  secondaryEnergyCuts = theCoupleTable->GetEnergyCutsVector(1);

  //  loop for materials
  //
  for (size_t J=0; J<numOfCouples; J++)
   {

     G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
                         LowestKineticEnergy, HighestKineticEnergy, TotBin);

     const G4MaterialCutsCouple* couple = theCoupleTable->GetMaterialCutsCouple(J);
     const G4Material* material= couple->GetMaterial();

     // get  electron cut in kinetic energy for the material
     G4double DeltaThreshold = SecondaryEnergyThreshold(J);


     // now comes the loop for the kinetic energy values
     //
      for (G4int i = 0; i < TotBin; i++)
         {
          G4double dEdx = ComputeRestrictedMeandEdx(aParticleType,
	                                           aVector->GetLowEdgeEnergy(i),
	                                           material,
	                                           DeltaThreshold);
      if(1 < verboseLevel) {
        G4cout << "Material= " << material->GetName()
               << "   E(MeV)= " << aVector->GetLowEdgeEnergy(i)/MeV
               << "  dEdx(MeV/mm)= " << dEdx*mm/MeV
               << G4endl;
      }
          aVector->PutValue(i,dEdx);
         }
      theLossTable->insert(aVector);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::BuildLambdaTable(const G4ParticleDefinition& aParticleType)
{

  if(0 < verboseLevel) {
    G4cout << "G4eIonisation::BuildLambdaTable() for process "
           << GetProcessName() << " and particle "
           << aParticleType.GetParticleName() << G4endl;
  }

  //create table
  //
  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  if (theMeanFreePathTable)
    { theMeanFreePathTable->clearAndDestroy(); delete theMeanFreePathTable;}

  theMeanFreePathTable = new G4PhysicsTable(numOfCouples);

  secondaryEnergyCuts = theCoupleTable->GetEnergyCutsVector(1);

  // loop for materials

  for (size_t J=0 ; J < numOfCouples; J++)
    {

      G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
               LowerBoundLambda, UpperBoundLambda, NbinLambda);

     // compute the (macroscopic) cross section first
      const G4MaterialCutsCouple* couple = theCoupleTable->GetMaterialCutsCouple(J);
      const G4Material* material= couple->GetMaterial();

      // get  electron cut in kinetic energy for the material
      G4double DeltaThreshold = SecondaryEnergyThreshold(J);

      const G4ElementVector* theElementVector = material->GetElementVector();
      const G4double* NbOfAtomsPerVolume = material->GetVecNbOfAtomsPerVolume();
      G4int NumberOfElements = material->GetNumberOfElements();

      for (G4int i = 0 ; i < NbinLambda ; i++)
        {
	  G4double LowEdgeEnergy = aVector->GetLowEdgeEnergy(i);
	  G4double SIGMA = 0.;
          for (G4int iel=0; iel<NumberOfElements; iel++ )
             {
              SIGMA += NbOfAtomsPerVolume[iel]*
                       ComputeCrossSectionPerAtom(aParticleType,
                                                  LowEdgeEnergy,
                               (*theElementVector)[iel]->GetZ(),
                                                DeltaThreshold);
             }

          // mean free path = 1./macroscopic cross section
          G4double Value = SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX;
          aVector->PutValue(i, Value);
        }
     theMeanFreePathTable->insert(aVector);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eIonisation::ComputeRestrictedMeandEdx (
                                 const G4ParticleDefinition& aParticleType,
                                 G4double KineticEnergy,
				 const G4Material* material,
				 G4double DeltaThreshold)
{
 // calculate the dE/dx due to the ionization process (Geant4 internal units)
 // Seltzer-Berger formula
 //
 G4double particleMass = aParticleType.GetPDGMass();

 G4double ElectronDensity = material->GetElectronDensity();
 G4double Eexc = material->GetIonisation()->GetMeanExcitationEnergy();
 Eexc  /= particleMass; G4double Eexcm2 = Eexc*Eexc;

 // for the lowenergy extrapolation
 G4double Zeff = material->GetTotNbOfElectPerVolume()/
                 material->GetTotNbOfAtomsPerVolume();
 G4double Th = 0.25*sqrt(Zeff)*keV;
 G4double Tsav = 0.;
 if (KineticEnergy < Th) {Tsav = KineticEnergy; KineticEnergy = Th;}
 
 G4double tau = KineticEnergy/particleMass;
 G4double gamma = tau + 1., gamma2 = gamma*gamma, bg2 = tau*(tau+2.);
 G4double beta2 = bg2/gamma2;

 G4double Tmax,d,dEdx;

 // electron
 if (&aParticleType==G4Electron::Electron())
   {
     Tmax = KineticEnergy/2.;
     d = G4std::min(DeltaThreshold, Tmax)/particleMass;
     dEdx = log(2.*(tau+2.)/Eexcm2)-1.-beta2
            + log((tau-d)*d)+tau/(tau-d)
            + (0.5*d*d+(2.*tau+1.)*log(1.-d/tau))/gamma2;
   }
   
 else        //positron
   {
     Tmax = KineticEnergy;  
     d = G4std::min(DeltaThreshold, Tmax)/particleMass;
     G4double d2=d*d/2., d3=d*d*d/3., d4=d*d*d*d/4.;
     G4double y=1./(1.+gamma);
     dEdx = log(2.*(tau+2.)/Eexcm2)+log(tau*d)
            - beta2*(tau+2.*d-y*(3.*d2+y*(d-d3+y*(d2-tau*d3+d4))))/tau;
   } 

 //density correction
 G4double Cden   = material->GetIonisation()->GetCdensity();
 G4double Mden   = material->GetIonisation()->GetMdensity();
 G4double Aden   = material->GetIonisation()->GetAdensity();
 G4double X0den  = material->GetIonisation()->GetX0density();
 G4double X1den  = material->GetIonisation()->GetX1density();
  
 const G4double twoln10 = 2.*log(10.); 
 G4double  x = log(bg2)/twoln10;
 G4double delta;
 if (x < X0den) delta = 0.;
 else          {delta = twoln10*x - Cden;
                if (x < X1den) delta += Aden*pow((X1den-x),Mden);
               } 

 //now you can compute the total ionization loss
 dEdx -= delta;
 dEdx *= twopi_mc2_rcl2*ElectronDensity/beta2;
 if (dEdx <= 0.) dEdx = 0.;
   
 // low energy ?
 const G4double Tl = 0.2*keV; 
 if (Tsav > 0.)
   {
    if (Tsav >= Tl) dEdx *= sqrt(KineticEnergy/Tsav);
    else            dEdx *= sqrt(KineticEnergy*Tsav)/Tl;
   }
 return dEdx;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eIonisation::ComputeCrossSectionPerAtom(
                                 const G4ParticleDefinition& aParticleType,
                                 G4double KineticEnergy,
                                 G4double AtomicNumber ,
                                 G4double DeltaThreshold)
{
  // calculates the cross section per atom (Geant4 internal units) 
  //(it is called for elements , AtomicNumber = Z )

  G4double particleMass = aParticleType.GetPDGMass();
  G4double TotalEnergy = KineticEnergy + particleMass;

  G4double betasquare = KineticEnergy*(TotalEnergy+particleMass)
                       /(TotalEnergy*TotalEnergy);
  G4double gamma = TotalEnergy/particleMass, gamma2 = gamma*gamma;
  G4double x=DeltaThreshold/KineticEnergy, x2 = x*x;

  G4double MaxKineticEnergyTransfer;
  if (&aParticleType==G4Electron::Electron())
                      MaxKineticEnergyTransfer = 0.5*KineticEnergy;
  else                MaxKineticEnergyTransfer =     KineticEnergy;


  G4double TotalCrossSection = 0.;
  if (MaxKineticEnergyTransfer > DeltaThreshold)
    {
      if (&aParticleType==G4Electron::Electron())   //Moller (e-e-) scattering
        {
          TotalCrossSection  = (gamma-1.)*(gamma-1.)*(0.5-x)/gamma2 + 1./x
                             - 1./(1.-x)-(2.*gamma-1.)*log((1.-x)/x)/gamma2;
          TotalCrossSection /= betasquare;
        }
      else                                         //Bhabha (e+e-) scattering
        {
          G4double y=1./(1.+gamma), y2 =y*y, y12=1.-2.*y;
          G4double b1=2.-y2, b2=y12*(3.+y2), b4=y12*y12*y12, b3=b4+y12*y12;
          TotalCrossSection = (1./x-1.)/betasquare+b1*log(x)+b2*(1.-x)
                            - b3*(1.-x2)/2.+b4*(1.-x2*x)/3.;
        }
      TotalCrossSection *= (twopi_mc2_rcl2*AtomicNumber/KineticEnergy);
     }
 return TotalCrossSection ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* G4eIonisation::PostStepDoIt( const G4Track& trackData,
                                                const G4Step&  stepData)
{
  aParticleChange.Initialize(trackData);

  const G4MaterialCutsCouple* couple = trackData.GetMaterialCutsCouple();
  const G4DynamicParticle*  aParticle = trackData.GetDynamicParticle();

  G4double particleMass = aParticle->GetDefinition()->GetPDGMass();
  G4double Charge       = aParticle->GetDefinition()->GetPDGCharge();
  G4double KineticEnergy = aParticle->GetKineticEnergy();
  G4double TotalEnergy = KineticEnergy + particleMass;
  G4double Psquare = KineticEnergy*(TotalEnergy+particleMass);
  G4double TotalMomentum = sqrt(Psquare);
  G4ParticleMomentum ParticleDirection = aParticle->GetMomentumDirection();

  // get kinetic energy cut for the electron
  G4double DeltaThreshold = SecondaryEnergyThreshold(couple->GetIndex());

  // some kinematics
  G4double MaxKineticEnergyTransfer;
  if (Charge < 0.) MaxKineticEnergyTransfer = 0.5*KineticEnergy;
  else             MaxKineticEnergyTransfer =     KineticEnergy;

  // sampling kinetic energy of the delta ray

  if (MaxKineticEnergyTransfer <= DeltaThreshold)
     // pathological case (should not happen, there is no change at all)
     return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);


  // normal case
  G4double cc,y,y2,c2,b0,b1,b2,b3,b4,x,x1,grej,grejc;

  G4double tau = KineticEnergy/particleMass;
  G4double gamma = tau+1., gamma2=gamma*gamma;
  G4double xc = DeltaThreshold/KineticEnergy, xc1=1.-xc;

  if (Charge < 0.)  // Moller (e-e-) scattering
    {
      b1=4./(9.*gamma2-10.*gamma+5.);
      b2=tau*tau*b1; b3=(2.*gamma2+2.*gamma-1.)*b1;
      cc=1.-2.*xc;
      do {
           x    = xc/(1.-cc*G4UniformRand()); x1 = 1.-x;
           grej = b2*x*x-b3*x/x1+b1*gamma2/(x1*x1);
         } while (G4UniformRand()>grej);
    }
  else             // Bhabha (e+e-) scattering
    {
      y=1./(gamma+1.); y2=y*y; cc=1.-2.*y;
      b1=2.-y2; b2=cc*(3.+y2);
      c2=cc*cc; b4=c2*cc; b3=c2+b4;
      b0=gamma2/(gamma2-1.);
      grejc=(((b4*xc-b3)*xc+b2)*xc-b1)*xc+b0;
      do {
           x    = xc/(1.-xc1*G4UniformRand());
           grej = ((((b4*x-b3)*x+b2)*x-b1)*x+b0)/grejc;
         } while (G4UniformRand()>grej);
    }

    G4double DeltaKineticEnergy = x * KineticEnergy;

  // protection :do not produce a secondary with 0. kinetic energy !
  if (DeltaKineticEnergy <= 0.)
     return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);

  G4double DeltaTotalMomentum = sqrt(DeltaKineticEnergy*(DeltaKineticEnergy +
                                                      2.*electron_mass_c2 ));

  G4double costheta = DeltaKineticEnergy * (TotalEnergy + electron_mass_c2)
                      /(DeltaTotalMomentum * TotalMomentum);

  if (costheta < -1.) costheta = -1.;
  if (costheta > +1.) costheta = +1.;

  //  direction of the delta electron

  G4double phi = twopi * G4UniformRand();
  G4double sintheta = sqrt((1.+costheta)*(1.-costheta));
  G4double dirx = sintheta*cos(phi), diry = sintheta*sin(phi), dirz = costheta;

  G4ThreeVector DeltaDirection(dirx,diry,dirz);
  DeltaDirection.rotateUz(ParticleDirection);

  // create G4DynamicParticle object for delta ray

  G4DynamicParticle* theDeltaRay = new G4DynamicParticle;
  theDeltaRay->SetKineticEnergy( DeltaKineticEnergy );
  theDeltaRay->SetMomentumDirection(
                   DeltaDirection.x(),DeltaDirection.y(),DeltaDirection.z());
  theDeltaRay->SetDefinition(G4Electron::Electron());

  // fill aParticleChange
  // changed energy and momentum of the actual particle
  G4double finalKineticEnergy = KineticEnergy - DeltaKineticEnergy;

  G4double Edep = 0.;

  if (finalKineticEnergy > MinKineticEnergy)
    {
      G4double finalPx = TotalMomentum*ParticleDirection.x()
                        - DeltaTotalMomentum*DeltaDirection.x();
      G4double finalPy = TotalMomentum*ParticleDirection.y()
                        - DeltaTotalMomentum*DeltaDirection.y();
      G4double finalPz = TotalMomentum*ParticleDirection.z()
                        - DeltaTotalMomentum*DeltaDirection.z();
      G4double finalMomentum =
                sqrt(finalPx*finalPx+finalPy*finalPy+finalPz*finalPz);
      finalPx /= finalMomentum;
      finalPy /= finalMomentum;
      finalPz /= finalMomentum;

      aParticleChange.SetMomentumChange(finalPx, finalPy, finalPz);
    }
  else
    {
      Edep = finalKineticEnergy;
      finalKineticEnergy = 0.;
      if (Charge < 0.) aParticleChange.SetStatusChange(fStopAndKill);
      else             aParticleChange.SetStatusChange(fStopButAlive);
    }

  aParticleChange.SetEnergyChange(finalKineticEnergy);
  aParticleChange.SetNumberOfSecondaries(1);
  aParticleChange.AddSecondary(theDeltaRay);
  aParticleChange.SetLocalEnergyDeposit(Edep);

  return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4eIonisation::StorePhysicsTable(G4ParticleDefinition* particle,
				              const G4String& directory,
				              G4bool          ascii)
{
  G4String filename;

  // store stopping power table
  filename = GetPhysicsTableFileName(particle,directory,"StoppingPower",ascii);
  if ( !theLossTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theLossTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }
  // store mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  if ( !theMeanFreePathTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }

  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to store the PhysicsTables in "
         << directory << G4endl;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4eIonisation::RetrievePhysicsTable(G4ParticleDefinition* particle,
					         const G4String& directory,
				                 G4bool          ascii)
{
  // delete theLossTable and theMeanFreePathTable
  if (theLossTable != 0) {
    theLossTable->clearAndDestroy();
    delete theLossTable;
  }
  if (theMeanFreePathTable != 0) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }

  // get bining from EnergyLoss
  LowestKineticEnergy  = GetLowerBoundEloss();
  HighestKineticEnergy = GetUpperBoundEloss();
  TotBin               = GetNbinEloss();

  G4String filename;
  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  secondaryEnergyCuts = theCoupleTable->GetEnergyCutsVector(0);

  // retreive stopping power table
  filename = GetPhysicsTableFileName(particle,directory,"StoppingPower",ascii);
  theLossTable = new G4PhysicsTable(numOfCouples);
  if ( !theLossTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theLossTable->RetrievePhysicsTable in " << filename
           << G4endl;
    return false;
  }

  // retreive mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  theMeanFreePathTable = new G4PhysicsTable(numOfCouples);
  if ( !theMeanFreePathTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->RetrievePhysicsTable in " << filename
           << G4endl;
    return false;
  }

  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to retrieve the PhysicsTables from "
         << directory << G4endl;

  if (particle==G4Electron::Electron())
    {
     RecorderOfElectronProcess[CounterOfElectronProcess] = (*this).theLossTable;
     CounterOfElectronProcess++;
    }
  else
    {
     RecorderOfPositronProcess[CounterOfPositronProcess] = (*this).theLossTable;
     CounterOfPositronProcess++;
    }
 	 
  BuildDEDXTable(*particle);
  	 
  return true;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eIonisation::PrintInfoDefinition()
{
  G4String comments = "delta cross sections from Moller+Bhabha. "
            "Good description from 1 KeV to 100 GeV.\n"
            "        delta ray energy sampled from  differential Xsection.";
                     
  G4cout << G4endl << GetProcessName() << ":  " << comments
         << "\n        PhysicsTables from "
	 << G4BestUnit(LowerBoundLambda,"Energy")
         << " to " << G4BestUnit(UpperBoundLambda,"Energy") 
         << " in " << NbinLambda << " bins."
	 << "\n        Step function: finalRange(mm)= " << finalRange
	 << ",  dRoverRange= " << dRoverRange 
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
