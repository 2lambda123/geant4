#include "RunAction.hh"

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4ProcessManager.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
:detector(det), primary(kin)
{ 
  asciiFileName="Sandia.out";
  std::ofstream asciiFile(asciiFileName, std::ios::app);
  if(asciiFile.is_open()) {
    asciiFile << " FMR (z/r0)    ||       J(MeV/g/cm2)" << G4endl << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  // get particle energy 
  G4double energy   = primary->GetParticleGun()->GetParticleEnergy();

  // get name of material 1, 2 & 3
  matName1     = detector->GetAbsorber1Material()->GetName();
  matName2     = detector->GetAbsorber2Material()->GetName();
  matName3     = detector->GetAbsorber3Material()->GetName();

// Medium 1
  if (matName1=="G4_Ta") // Tantalum 
  { 
  density1 =16.654*g/cm3; 
  if(energy==1.033*MeV)      MFP1 = 0.788*g/cm2  ; // MFP1, MFP2 &  MFP3 values were taken from SANDIA Report
  else if(energy==1.000*MeV) MFP1 = 0.763*g/cm2  ;
  else if(energy==0.521*MeV) MFP1 = 0.339*g/cm2 ;
  else if(energy==0.500*MeV) MFP1 = 0.325*g/cm2  ;
  else if(energy==0.314*MeV) MFP1 = 0.167*g/cm2  ;
  else if(energy==0.300*MeV) MFP1 = 0.160*g/cm2  ;
  }
  else if (matName1=="G4_Mo") // Molybdenum 
  { 
  density1 =10.22*g/cm3; 
  if(energy==0.5*MeV)      MFP1 = 0.281*g/cm2  ;
  }
  else if(matName1=="G4_Al")//Aluminium
  { 
  density1 = 2.699*g/cm3;
  if(energy==1.033*MeV)      MFP1 = 0.569*g/cm2  ;
  else if(energy==1.000*MeV) MFP1 = 0.551*g/cm2  ;
  else if(energy==0.521*MeV) MFP1 = 0.234*g/cm2  ;
  else if(energy==0.314*MeV) MFP1 = 0.113*g/cm2 ;
  }
  else if(matName1=="G4_Au")//Gold
  { 
  density1 = 19.32*g/cm3;
  if(energy==1.000*MeV)      MFP1 = 0.772*g/cm2  ;
  }


// Medium 2
  if (matName2=="G4_Ta") // Tantalum 
  { 
  density2 =16.654*g/cm3; 
  if(energy==1.033*MeV)      MFP2 = 0.788*g/cm2  ;
  else if(energy==1.000*MeV) MFP2 = 0.763*g/cm2  ;
  else if(energy==0.521*MeV) MFP2 = 0.339*g/cm2 ;
  else if(energy==0.500*MeV) MFP2 = 0.325*g/cm2  ;
  else if(energy==0.314*MeV) MFP2 = 0.167*g/cm2  ;
  else if(energy==0.300*MeV) MFP2 = 0.160*g/cm2  ;
  }
  else if (matName2=="G4_Mo") // Molybdenum 
  { 
  density2 =10.22*g/cm3; 
  if(energy==0.5*MeV)      MFP2 = 0.281*g/cm2  ;
  }
  else if(matName2=="G4_Al")//Aluminium
  { 
  density2 = 2.699*g/cm3;
  if(energy==1.033*MeV)      MFP2 = 0.569*g/cm2  ;
  else if(energy==1.000*MeV) MFP2 = 0.551*g/cm2  ;
  else if(energy==0.521*MeV) MFP2 = 0.234*g/cm2  ;
  else if(energy==0.314*MeV) MFP2 = 0.113*g/cm2 ;
  }
  else if(matName2=="G4_Au")//Gold
  { 
  density2 = 19.32*g/cm3;
  if(energy==1.000*MeV)      MFP2 = 0.772*g/cm2  ;
  }

// Medium 3
  if (matName3=="G4_Ta") // Tantalum 
  { 
  density3 =16.654*g/cm3; 
  if(energy==1.033*MeV)      MFP3 = 0.788*g/cm2  ;
  else if(energy==1.000*MeV) MFP3 = 0.763*g/cm2  ;
  else if(energy==0.521*MeV) MFP3 = 0.339*g/cm2 ;
  else if(energy==0.500*MeV) MFP3 = 0.325*g/cm2  ;
  else if(energy==0.314*MeV) MFP3 = 0.167*g/cm2  ;
  else if(energy==0.300*MeV) MFP3 = 0.160*g/cm2  ;
  }
  else if (matName3=="G4_Mo") // Molybdenum 
  { 
  density3 =10.22*g/cm3; 
  if(energy==0.5*MeV)      MFP3 = 0.281*g/cm2  ;
  }
  else if(matName3=="G4_Al")//Aluminium
  { 
  density3 = 2.699*g/cm3;
  if(energy==1.033*MeV)      MFP3 = 0.569*g/cm2  ;
  else if(energy==1.000*MeV) MFP3 = 0.551*g/cm2  ;
  else if(energy==0.521*MeV) MFP3 = 0.234*g/cm2  ;
  else if(energy==0.314*MeV) MFP3 = 0.113*g/cm2 ;
  }
  else if(matName3=="G4_Au")//Gold
  { 
  density3 = 19.32*g/cm3;
  if(energy==1.000*MeV)      MFP3 = 0.772*g/cm2  ;
  }

  //initialize EnergyDeposit per Layer
  for (G4int k=0; k<detector->GetNbOfLayersOfMedium1(); k++)   energyDeposit1[k] = 0.0*MeV;   
  for (G4int k=0; k<detector->GetNbOfLayersOfMedium2(); k++)   energyDeposit2[k] = 0.0*MeV;   
  for (G4int k=0; k<detector->GetNbOfLayersOfMedium3(); k++)   energyDeposit3[k] = 0.0*MeV;   

  //initialize EnergyDeposit per Medium
  energyDepositRun1 = 0.*MeV;
  energyDepositRun2 = 0.*MeV;
  energyDepositRun3 = 0.*MeV;
 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void RunAction::EndOfRunAction(const G4Run* aRun)
{
 
  G4bool entering = false;

 G4double NumbrOfEvents = double (aRun->GetNumberOfEvent());
 G4double totalAbsEnergy = (energyDepositRun1/MeV)+(energyDepositRun2/MeV)+(energyDepositRun3/MeV);

G4double thick11=detector->GetAbsorber1Thickness();
G4double thick12=double (detector->GetNbOfLayersOfMedium1());
G4double LayerTh1 = thick11/thick12;
G4double deltaX1 = LayerTh1*density1/MFP1;

G4double thick21=detector->GetAbsorber2Thickness();
G4double thick22=double (detector->GetNbOfLayersOfMedium2());
G4double LayerTh2 = thick21/thick22;
G4double deltaX2 = LayerTh2*density2/MFP2;

G4double thick31=detector->GetAbsorber3Thickness();
G4double thick32=double (detector->GetNbOfLayersOfMedium3());
G4double LayerTh3 = thick31/thick32;
G4double deltaX3 = LayerTh3*density3/MFP3;
G4cout<<thick21/cm<<"  "<<thick22/cm<<"  "<<LayerTh2/cm<<"  "<<G4endl;

G4cout<<" ----------------------------------------------------------"<<G4endl;
G4cout<<" ----------------  RUN SUMMARY ----------------------------"<<G4endl;
G4cout<<" ----------------------------------------------------------"<<G4endl;
if(matName1!="G4_Galactic")
{
G4cout<<"             Medium 1 ==>   "<<matName1<<G4endl;
G4cout<<"\t FMR (z/r0)    ||       J  "<<G4endl;
entering = true;  
// Write to file
  if(entering)
    {
    std::ofstream asciiFile(asciiFileName, std::ios::app);
    if(asciiFile.is_open()) 
	{
     asciiFile<<"       Medium 1 ==>   "<<matName1<<G4endl;

    for (G4int k=1; k<=thick12; k++)
      {normalizedvalue1[k] =(energyDeposit1[k]/MeV)/(NumbrOfEvents*deltaX1*(MFP1/(g/cm2))); 
       G4cout<<"\t   "<<deltaX1*k<<"\t          "<<normalizedvalue1[k]<<G4endl;
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << deltaX1*k;
            asciiFile << "           ";
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << normalizedvalue1[k]
		<< G4endl;      
          }
	   asciiFile.close();
         }
      G4cout<<"\n Deposit energy (MeV) = "<<(energyDepositRun1/MeV)/NumbrOfEvents<<G4endl;
    }
}                
entering = false;  

G4cout<<" ----------------------------------------------------------"<<G4endl;
if(matName2!="G4_Galactic"){

G4cout<<"               Medium 2 ==>   "<<matName2<<G4endl;
G4cout<<"\t FMR (z/r0)    ||       J  "<<G4endl;
entering = true;  
// Write to file
  if(entering)
    {
    std::ofstream asciiFile(asciiFileName, std::ios::app);
    if(asciiFile.is_open()) 
	{
     asciiFile<<G4endl;
     asciiFile<<"       Medium 2 ==>   "<<matName2<<G4endl;
          
     for (G4int k=1; k<=thick22; k++)
       {normalizedvalue2[k] =(energyDeposit2[k]/MeV)/(NumbrOfEvents*deltaX2*(MFP2/(g/cm2)));
        G4cout<<"\t   "<<(deltaX1*thick12)+(deltaX2*k)<<"\t          "<<normalizedvalue2[k]<<G4endl;
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << (deltaX1*thick12)+(deltaX2*k);
            asciiFile << "           ";
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << normalizedvalue2[k]
		<< G4endl;      
          }
	   asciiFile.close();
         }
      G4cout<<"\n Deposit energy (MeV) = "<<(energyDepositRun2/MeV)/NumbrOfEvents<<G4endl;
    }

}                
entering = false;  

G4cout<<" ----------------------------------------------------------"<<G4endl;

if(matName3!="G4_Galactic"){
G4cout<<"               Medium 3 ==>   "<<matName3<<G4endl;
G4cout<<"\t FMR (z/r0)    ||       J  "<<G4endl;
entering = true;  
// Write to file
  if(entering)
    {
    std::ofstream asciiFile(asciiFileName, std::ios::app);
    if(asciiFile.is_open()) 
	{
     asciiFile<<G4endl;
     asciiFile<<"       Medium 3 ==>   "<<matName3<<G4endl;
          
    for (G4int k=1; k<=thick32; k++)
      {normalizedvalue3[k] =(energyDeposit3[k]/MeV)/(NumbrOfEvents*deltaX3*(MFP3/(g/cm2))); 
       G4cout<<"\t   "<<(deltaX1*thick12)+(deltaX2*thick22)+(deltaX3*k)<<"\t          "<<normalizedvalue3[k]<<G4endl;
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << (deltaX1*thick12)+(deltaX2*thick22)+(deltaX3*k);
            asciiFile << "           ";
            asciiFile << std::setiosflags(std::ios::fixed)
		<< std::setprecision(5)
		<< std::setiosflags(std::ios::right)
		<< std::setw(10);
            asciiFile << normalizedvalue3[k]
		<< G4endl;      
          }
	   asciiFile.close();


        }
      G4cout<<"\n Deposit energy (MeV) = "<<(energyDepositRun3/MeV)/(NumbrOfEvents*1.0)<<G4endl;
    }

}                
entering = false;  


G4cout<<" ----------------------------------------------------------"<<G4endl;

G4cout<<" Total Deposit energy (MeV) = "<<(totalAbsEnergy/MeV)/(NumbrOfEvents*1.0)<<G4endl;

G4cout<<" ----------------------------------------------------------"<<G4endl;
G4cout<<" ----------------  END OF RUN SUMMARY ---------------------"<<G4endl;
G4cout<<" ----------------------------------------------------------"<<G4endl;


}

