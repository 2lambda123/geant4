//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4IonisParamMat.cc,v 1.24 2008-06-04 08:27:29 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// 09-07-98, data moved from G4Material, M.Maire
// 18-07-98, bug corrected in ComputeDensityEffect() for gas
// 16-01-01, bug corrected in ComputeDensityEffect() E100eV (L.Urban)
// 08-02-01, fShellCorrectionVector correctly handled (mma)
// 28-10-02, add setMeanExcitationEnergy (V.Ivanchenko)
// 06-09-04, factor 2 to shell correction term (V.Ivanchenko) 
// 10-05-05, add a missing coma in FindMeanExcitationEnergy() - Bug#746 (mma)
// 27-09-07, add computation of parameters for ions (V.Ivanchenko)
// 04-03-08, remove reference to G4NistManager. Add fBirks constant (mma)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#include "G4IonisParamMat.hh"
#include "G4Material.hh"
///#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamMat::G4IonisParamMat(G4Material* material)
  : fMaterial(material)
{
  ComputeMeanParameters();
  ComputeDensityEffect();
  ComputeFluctModel();
  ComputeIonParameters();
  
  fBirks = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// Fake default constructor - sets only member data and allocates memory
//                            for usage restricted to object persistency

G4IonisParamMat::G4IonisParamMat(__void__&)
  : fMaterial(0), fShellCorrectionVector(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4IonisParamMat::ComputeMeanParameters()
{
  // compute mean excitation energy and shell correction vector

  fTaul = (*(fMaterial->GetElementVector()))[0]->GetIonisation()->GetTaul();

  fMeanExcitationEnergy = 0.;
  fLogMeanExcEnergy = 0.;

  size_t nElements = fMaterial->GetNumberOfElements();
  const G4ElementVector* elmVector = fMaterial->GetElementVector();
  const G4double* nAtomsPerVolume = fMaterial->GetVecNbOfAtomsPerVolume();

  const G4String ch = fMaterial->GetChemicalFormula();

  if(ch != "") fMeanExcitationEnergy = FindMeanExcitationEnergy(ch);

  // Chemical formula defines mean excitation energy
  if(fMeanExcitationEnergy > 0.0) {
    fLogMeanExcEnergy = std::log(fMeanExcitationEnergy);

    // Compute average 
  } else {
    for (size_t i=0; i < nElements; i++) {
      const G4Element* elm = (*elmVector)[i];
      fLogMeanExcEnergy += nAtomsPerVolume[i]*elm->GetZ()
	*std::log(elm->GetIonisation()->GetMeanExcitationEnergy());
    }
    fLogMeanExcEnergy /= fMaterial->GetTotNbOfElectPerVolume();
    fMeanExcitationEnergy = std::exp(fLogMeanExcEnergy);
  }

  fShellCorrectionVector = new G4double[3]; //[3]

  for (G4int j=0; j<=2; j++)
  {
    fShellCorrectionVector[j] = 0.;

    for (size_t k=0; k<nElements; k++) {
      fShellCorrectionVector[j] += nAtomsPerVolume[k]
	*(((*elmVector)[k])->GetIonisation()->GetShellCorrectionVector())[j];
    }
    fShellCorrectionVector[j] *= 2.0/fMaterial->GetTotNbOfElectPerVolume();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
                    
void G4IonisParamMat::ComputeDensityEffect()
{
  // Compute parameters for the density effect correction in DE/Dx formula.
  // The parametrization is from R.M. Sternheimer, Phys. Rev.B,3:3681 (1971)

  const G4double Cd2 = 4*pi*hbarc_squared*classic_electr_radius;
  const G4double twoln10 = 2.*std::log(10.);

  G4int icase;
  
  fCdensity = 1. + std::log(fMeanExcitationEnergy*fMeanExcitationEnergy
              /(Cd2*fMaterial->GetTotNbOfElectPerVolume()));

  //
  // condensed materials
  //
  G4State State = fMaterial->GetState();
  
  if ((State == kStateSolid)||(State == kStateLiquid)) {

      const G4double E100eV  = 100.*eV; 
      const G4double ClimiS[] = {3.681 , 5.215 };
      const G4double X0valS[] = {1.0   , 1.5   };
      const G4double X1valS[] = {2.0   , 3.0   };
                                
      if(fMeanExcitationEnergy < E100eV) icase = 0;
         else                            icase = 1;

      if(fCdensity < ClimiS[icase]) fX0density = 0.2;
         else                       fX0density = 0.326*fCdensity-X0valS[icase];

      fX1density = X1valS[icase] ; fMdensity = 3.0;
      
      //special: Hydrogen
      if ((fMaterial->GetNumberOfElements()==1)&&(fMaterial->GetZ()==1.)) {
         fX0density = 0.425; fX1density = 2.0; fMdensity = 5.949;
      }
  }

  //
  // gases
  //
  if (State == kStateGas) { 

      const G4double ClimiG[] = { 10. , 10.5 , 11. , 11.5 , 12.25 , 13.804};
      const G4double X0valG[] = { 1.6 , 1.7 ,  1.8 ,  1.9 , 2.0   ,  2.0 };
      const G4double X1valG[] = { 4.0 , 4.0 ,  4.0 ,  4.0 , 4.0   ,  5.0 };

      icase = 5;
      fX0density = 0.326*fCdensity-2.5 ; fX1density = 5.0 ; fMdensity = 3. ; 
      while((icase > 0)&&(fCdensity < ClimiG[icase])) icase-- ;
      fX0density = X0valG[icase]  ; fX1density = X1valG[icase] ;
      
      //special: Hydrogen
      if ((fMaterial->GetNumberOfElements()==1)&&(fMaterial->GetZ()==1.)) {
         fX0density = 1.837; fX1density = 3.0; fMdensity = 4.754;
      }
      
      //special: Helium
      if ((fMaterial->GetNumberOfElements()==1)&&(fMaterial->GetZ()==2.)) {
         fX0density = 2.191; fX1density = 3.0; fMdensity = 3.297;
      }

      // change parameters if the gas is not in STP.
      // For the correction the density(STP) is needed. 
      // Density(STP) is calculated here : 
      
      G4double Density  = fMaterial->GetDensity();
      G4double Pressure = fMaterial->GetPressure();
      G4double Temp     = fMaterial->GetTemperature();
      
     G4double DensitySTP = Density*STP_Pressure*Temp/(Pressure*STP_Temperature);

      G4double ParCorr = std::log(Density/DensitySTP);
  
      fCdensity  -= ParCorr;
      fX0density -= ParCorr/twoln10;
      fX1density -= ParCorr/twoln10;
  }

  G4double Xa = fCdensity/twoln10;
  fAdensity = twoln10*(Xa-fX0density)
              /std::pow((fX1density-fX0density),fMdensity);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4IonisParamMat::ComputeFluctModel()
{
  // compute parameters for the energy loss fluctuation model

  // need an 'effective Z' ?????
  G4double Zeff = 0.;
  for (size_t i=0;i<fMaterial->GetNumberOfElements();i++) {
     Zeff += (fMaterial->GetFractionVector())[i]
             *((*(fMaterial->GetElementVector()))[i]->GetZ());
  }
  if (Zeff > 2.) fF2fluct = 2./Zeff ;
    else         fF2fluct = 0.;

  fF1fluct         = 1. - fF2fluct;
  fEnergy2fluct    = 10.*Zeff*Zeff*eV;
  fLogEnergy2fluct = std::log(fEnergy2fluct);
  fLogEnergy1fluct = (fLogMeanExcEnergy - fF2fluct*fLogEnergy2fluct)
                     /fF1fluct;
  fEnergy1fluct    = std::exp(fLogEnergy1fluct);
  fEnergy0fluct    = 10.*eV;
  fRateionexcfluct = 0.4;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4IonisParamMat::ComputeIonParameters()
{
  // get elements in the actual material,
  const G4ElementVector* theElementVector = fMaterial->GetElementVector() ;
  const G4double* theAtomicNumDensityVector =
                         fMaterial->GetAtomicNumDensityVector() ;
  const G4int NumberOfElements = fMaterial->GetNumberOfElements() ;

  //  loop for the elements in the material
  //  to find out average values Z, vF, lF
  G4double z = 0.0, vF = 0.0, lF = 0.0, norm = 0.0 ;

  if( 1 == NumberOfElements ) {
    const G4Element* element = (*theElementVector)[0];
    z = element->GetZ();
    vF= element->GetIonisation()->GetFermiVelocity();
    lF= element->GetIonisation()->GetLFactor();

  } else {
    for (G4int iel=0; iel<NumberOfElements; iel++)
      {
        const G4Element* element = (*theElementVector)[iel] ;
        const G4double weight = theAtomicNumDensityVector[iel] ;
        norm += weight ;
        z    += element->GetZ() * weight ;
        vF   += element->GetIonisation()->GetFermiVelocity() * weight ;
        lF   += element->GetIonisation()->GetLFactor() * weight ;
      }
    z  /= norm ;
    vF /= norm ;
    lF /= norm ;
  }  
  fZeff        = z;
  fLfactor     = lF;
  fFermiEnergy = 25.*keV*vF*vF;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4IonisParamMat::SetMeanExcitationEnergy(G4double value)
{
  if(value == fMeanExcitationEnergy || value <= 0.0) return;

  /*
  if (G4NistManager::Instance()->GetVerbose() > 0) 
    G4cout << "G4Material: Mean excitation energy is changed for "
           << fMaterial->GetName()
           << " Iold= " << fMeanExcitationEnergy/eV
           << "eV; Inew= " << value/eV << " eV;"
           << G4endl;
  */
  
  fMeanExcitationEnergy = value;
  fLogMeanExcEnergy = std::log(value);
  ComputeDensityEffect();
  ComputeFluctModel();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4double G4IonisParamMat::FindMeanExcitationEnergy(const G4String& chFormula)
{

  // The data on mean excitation energy for compaunds
  // from "Stopping Powers for Electrons and Positrons"
  // ICRU Report N#37, 1984  (energy in eV)

  const size_t numberOfMolecula = 79 ;
  
  static G4String name[numberOfMolecula] = {

    // gas
    "NH_3",       "C_4H_10",    "CO_2",       "C_2H_6",      "C_7H_16",
    "C_6H_14",    "CH_4",       "NO",         "N_2O",        "C_8H_18",
    "C_5H_12",    "C_3H_8",     "H_2O-Gas", 

    // liquid
    "C_3H_6O",    "C_6H_5NH_2",  "C_6H_6",    "C_4H_9OH",    "CCl_4",    
    "C_6H_5Cl",   "CHCl_3",      "C_6H_12",   "C_6H_4Cl_2",  "C_4Cl_2H_8O", 
    "C_2Cl_2H_4", "(C_2H_5)_2O", "C_2H_5OH",  "C_3H_5(OH)_3","C_7H_16",     
    "C_6H_14",    "CH_3OH",      "C_6H_5NO_2","C_5H_12",     "C_3H_7OH",    
    "C_5H_5N",    "C_8H_8",      "C_2Cl_4",   "C_7H_8",      "C_2Cl_3H",    
    "H_2O",       "C_8H_10",

    //solid
    "C_5H_5N_5",  "C_5H_5N_5O",  "(C_6H_11NO)-nylon",  "C_25H_52", 
    "(C_2H_4)-Polyethylene",     "(C_5H_8O-2)-Polymethil_Methacrylate",   
    "(C_8H_8)-Polystyrene",      "A-150-tissue",       "Al_2O_3",  "CaF_2", 
    "LiF",        "Photo_Emulsion",  "(C_2F_4)-Teflon",  "SiO_2"     

  } ;
    
  static G4double meanExcitation[numberOfMolecula] = {

    53.7,   48.3,  85.0,  45.4,  49.2,
    49.1,   41.7,  87.8,  84.9,  49.5,
    48.2,   47.1,  71.6,

    64.2,   66.2,  63.4,  59.9,  166.3,
    89.1,  156.0,  56.4, 106.5,  103.3, 
   111.9,   60.0,  62.9,  72.6,   54.4,  
    54.0,  67.6,   75.8,  53.6,   61.1,  
    66.2,  64.0,  159.2,  62.5,  148.1,  
    75.0,  61.8,

    71.4,  75.0,   63.9,  48.3,   57.4,
    74.0,  68.7,   65.1, 145.2,  166.,
    94.0, 331.0,   99.1, 139.2 

  } ;

  G4double x = fMeanExcitationEnergy;

  for(size_t i=0; i<numberOfMolecula; i++) {
    if(chFormula == name[i]) {
      x = meanExcitation[i]*eV;
      break;
    }
  }
  return x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamMat::~G4IonisParamMat()
{
  if (fShellCorrectionVector) delete [] fShellCorrectionVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamMat::G4IonisParamMat(const G4IonisParamMat& right)
{
  *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

const G4IonisParamMat& G4IonisParamMat::operator=(const G4IonisParamMat& right)
{
  if (this != &right)
    {
      fMaterial                 = right.fMaterial;
      fMeanExcitationEnergy     = right.fMeanExcitationEnergy;
      fLogMeanExcEnergy         = right.fLogMeanExcEnergy;
      if (fShellCorrectionVector) delete [] fShellCorrectionVector;      
      fShellCorrectionVector    = new G4double[3];             
      fShellCorrectionVector[0] = right.fShellCorrectionVector[0];
      fShellCorrectionVector[1] = right.fShellCorrectionVector[1];
      fShellCorrectionVector[2] = right.fShellCorrectionVector[2];
      fTaul                     = right.fTaul;
      fCdensity                 = right.fCdensity;
      fMdensity                 = right.fMdensity;
      fAdensity                 = right.fAdensity;
      fX0density                = right.fX0density;
      fX1density                = right.fX1density;
      fF1fluct                  = right.fF1fluct;
      fF2fluct                  = right.fF2fluct;
      fEnergy1fluct             = right.fEnergy1fluct;
      fLogEnergy1fluct          = right.fLogEnergy1fluct;      
      fEnergy2fluct             = right.fEnergy2fluct;
      fLogEnergy2fluct          = right.fLogEnergy2fluct;      
      fEnergy0fluct             = right.fEnergy0fluct;
      fRateionexcfluct          = right.fRateionexcfluct;
     } 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4IonisParamMat::operator==(const G4IonisParamMat& right) const
{
  return (this == (G4IonisParamMat*) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4IonisParamMat::operator!=(const G4IonisParamMat& right) const
{
  return (this != (G4IonisParamMat*) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

