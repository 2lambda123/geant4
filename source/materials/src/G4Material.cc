// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Material.cc,v 1.10 2001-05-18 12:35:53 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
//
// 26-06-96, Code uses operators (+=, *=, ++, -> etc.) correctly, P. Urban
// 10-07-96, new data members added by L.Urban
// 12-12-96, new data members added by L.Urban
// 20-01-97, aesthetic rearrangement. RadLength calculation modified.
//           Data members Zeff and Aeff REMOVED (i.e. passed to the Elements).
//           (local definition of Zeff in DensityEffect and FluctModel...)
//           Vacuum defined as a G4State. Mixture flag removed, M.Maire. 
// 29-01-97, State=Vacuum automatically set density=0 in the contructors.
//           Subsequent protections have been put in the calculation of 
//           MeanExcEnergy, ShellCorrectionVector, DensityEffect, M.Maire.
// 11-02-97, ComputeDensityEffect() rearranged, M.Maire.
// 20-03-97, corrected initialization of pointers, M.Maire.
// 28-05-98, the kState=kVacuum has been removed.
//           automatic check for a minimal density, M.Maire 
// 12-06-98, new method AddMaterial() allowing mixture of materials, M.Maire  
// 09-07-98, ionisation parameters removed from the class, M.Maire
// 05-10-98, change names: NumDensity -> NbOfAtomsPerVolume
// 18-11-98, new interface to SandiaTable
// 19-01-99  enlarge tolerance on test of coherence of gas conditions
// 19-07-99, Constructors with chemicalFormula added by V.Ivanchenko
// 16-01-01, Nuclear interaction length, M.Maire
// 12-03-01, G4bool fImplicitElement;
//           copy constructor and assignement operator revised (mma)
// 03-05-01, flux.precision(prec) at begin/end of operator<<
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "g4std/iomanip"


G4MaterialTable G4Material::theMaterialTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// Constructor to create a material from scratch

G4Material::G4Material(const G4String& name, G4double z,
                       G4double a, G4double density, 
                       G4State state, G4double temp, G4double pressure)
:fName(name)		       
{
    InitializePointers();
    
    if (density < universe_mean_density)
       { G4cerr << "--- Warning from G4Material::G4Material()"
              << " define a material with density=0 is not allowed. \n"
              << " The material " << name << " will be constructed with the"
              << " default minimal density: " << universe_mean_density/(g/cm3) 
              << "g/cm3" << G4endl;
         density = universe_mean_density;
       } 

    fDensity  = density;
    fState    = state;
    fTemp     = temp;
    fPressure = pressure;
    fChemicalFormula = " ";

    // Initialize theElementVector allocating one
    // element corresponding to this material
    maxNbComponents  = fNumberOfComponents = fNumberOfElements = 1;
    fImplicitElement = true;
    theElementVector    = new G4ElementVector(1);
    theElementVector[0] = new G4Element(name, " ", z, a);
    fMassFractionVector = new G4double[1];
    fMassFractionVector[0] = 1. ;

    if (fState == kStateUndefined)
      {
       if (fDensity > kGasThreshold) fState = kStateSolid;
       else                          fState = kStateGas;
      }

    ComputeDerivedQuantities();

    // Store in the table of Materials
    theMaterialTable.insert(this);
    fIndexInTable = theMaterialTable.index(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// Constructor to create a material from a List of constituents
// (elements and/or materials)  added with AddElement or AddMaterial

G4Material::G4Material(const G4String& name, G4double density, G4int nComponents,
                       G4State state, G4double temp, G4double pressure)
:fName(name)		       
{
 
    InitializePointers();
    
    if (density < universe_mean_density)
      {G4cerr << "--- Warning from G4Material::G4Material()"
            << " define a material with density=0 is not allowed. \n"
            << " The material " << name << " will be constructed with the"
            << " default minimal density: " << universe_mean_density/(g/cm3) 
            << "g/cm3" << G4endl;
       density = universe_mean_density;
      }
        
    fDensity  = density;
    fState    = state;
    fTemp     = temp;
    fPressure = pressure;
    fChemicalFormula = " ";
    
    maxNbComponents     = nComponents;
    fNumberOfComponents = fNumberOfElements = 0;
    fImplicitElement    = false;
    theElementVector    = new G4ElementVector(maxNbComponents);
    
    if (fState == kStateUndefined) 
      {
       if (fDensity > kGasThreshold) fState = kStateSolid;
       else                          fState = kStateGas;
      }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// Constructor to create a material with chemical formula from scratch

G4Material::G4Material(const G4String& name, const G4String& chFormula, 
                       G4double z, G4double a, G4double density, 
                       G4State state, G4double temp, G4double pressure)
:fName(name),fChemicalFormula(chFormula)
{
    InitializePointers();
    
    if (density < universe_mean_density)
       { G4cerr << "--- Warning from G4Material::G4Material()"
              << " define a material with density=0 is not allowed. \n"
              << " The material " << name << " will be constructed with the"
              << " default minimal density: " << universe_mean_density/(g/cm3) 
              << "g/cm3" << G4endl;
         density = universe_mean_density;
       } 

    fDensity  = density;
    fState    = state;
    fTemp     = temp;
    fPressure = pressure;

    // Initialize theElementVector allocating one
    // element corresponding to this material
    maxNbComponents  = fNumberOfComponents = fNumberOfElements = 1;
    fImplicitElement = true;
    theElementVector    = new G4ElementVector(1);
    theElementVector[0] = new G4Element(name, " ", z, a);
    fMassFractionVector = new G4double[1];
    fMassFractionVector[0] = 1. ;

    if (fState == kStateUndefined)
      {
       if (fDensity > kGasThreshold) fState = kStateSolid;
       else                          fState = kStateGas;
      }

    ComputeDerivedQuantities();

    // Store in the table of Materials
    theMaterialTable.insert(this);
    fIndexInTable = theMaterialTable.index(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// Constructor to create a material with chemical formula from a List 
// of constituents (elements and/or materials)  added with AddElement 
// or AddMaterial

G4Material::G4Material(const G4String& name, const G4String& chFormula, 
                       G4double density, G4int nComponents,
                       G4State state, G4double temp, G4double pressure)
:fName(name),fChemicalFormula(chFormula)
{
 
    InitializePointers();
    
    if (density < universe_mean_density)
      {G4cerr << "--- Warning from G4Material::G4Material()"
            << " define a material with density=0 is not allowed. \n"
            << " The material " << name << " will be constructed with the"
            << " default minimal density: " << universe_mean_density/(g/cm3) 
            << "g/cm3" << G4endl;
       density = universe_mean_density;
      }
        
    fDensity  = density;
    fState    = state;
    fTemp     = temp;
    fPressure = pressure;
    
    maxNbComponents     = nComponents;
    fNumberOfComponents = fNumberOfElements = 0;
    fImplicitElement    = false;
    theElementVector    = new G4ElementVector(maxNbComponents);
    
    if (fState == kStateUndefined) 
      {
       if (fDensity > kGasThreshold) fState = kStateSolid;
       else                          fState = kStateGas;
      }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// AddElement -- composition by atom count

void G4Material::AddElement(G4Element* element, G4int nAtoms)
{   
    // initialization
    if ( fNumberOfElements == 0 ) {
       fAtomsVector        = new G4int   [maxNbComponents];
       fMassFractionVector = new G4double[maxNbComponents];
    }

    // filling ...
    if ( G4int(fNumberOfElements) < maxNbComponents ) {
       (*theElementVector)[fNumberOfElements] = element;
       fAtomsVector       [fNumberOfElements] = nAtoms;
       fNumberOfComponents = ++fNumberOfElements;
    }
    else
       G4Exception
      ("ERROR!!! - Attempt to add more than the declared number of elements.");

    // filled.
    if ( G4int(fNumberOfElements) == maxNbComponents ) {     
       // compute proportion by mass
       size_t i=0;
       G4double Zmol(0.), Amol(0.);
       for (i=0;i<fNumberOfElements;i++) {
         Zmol +=  fAtomsVector[i]*(*theElementVector)[i]->GetZ();
         Amol +=  fAtomsVector[i]*(*theElementVector)[i]->GetA();
       }
       for (i=0;i<fNumberOfElements;i++) {
         fMassFractionVector[i] = fAtomsVector[i]*(*theElementVector)[i]->GetA()/Amol;
       }

       ComputeDerivedQuantities();

       // Store in the static Table of Materials
       theMaterialTable.insert(this);
       fIndexInTable = theMaterialTable.index(this);
     }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// AddElement -- composition by fraction of mass

void G4Material::AddElement(G4Element* element, G4double fraction)
{
    // if fAtomsVector is non-NULL, complain. Apples and oranges.  $$$
    if (fAtomsVector) {
        G4cerr << "This material is already being defined via elements by"
             << "atoms." << G4endl;
        G4Exception ("You are mixing apples and oranges ...");
    }
         
    // initialization
    if (fNumberOfComponents == 0) {
        fMassFractionVector = new G4double[100];
    }

    // filling ...
    if (G4int(fNumberOfComponents) < maxNbComponents) {
        size_t el = 0;
        while ((el<fNumberOfElements)&&(element!=(*theElementVector)[el])) el++;
        if (el<fNumberOfElements) fMassFractionVector[el] += fraction;
        else {
          if(el>=theElementVector->length()) theElementVector->resize(el+1);
          (*theElementVector)[el] = element;
          fMassFractionVector[el] = fraction;
          fNumberOfElements ++;
        }
        fNumberOfComponents++;  
    }
    else
       G4Exception
      ("ERROR!!! - Attempt to add more than the declared number of components.");

    // filled.
    if (G4int(fNumberOfComponents) == maxNbComponents) {
       // check sum of weights -- OK?
       G4double wtSum(0.0);
       for (size_t i=0;i<fNumberOfElements;i++) { wtSum +=  fMassFractionVector[i]; }
       if (abs(1.-wtSum) > perThousand) {
         G4cerr << "WARNING !! - Fractional masses do not sum to 1 :the Delta is > 0.001"
              << "( the weights are NOT renormalized; the results may be wrong)" 
              << G4endl;
       }

       ComputeDerivedQuantities();

       // Store in the static Table of Materials
       theMaterialTable.insert(this);
       fIndexInTable = theMaterialTable.index(this);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// AddMaterial -- composition by fraction of mass

void G4Material::AddMaterial(G4Material* material, G4double fraction)
{
    // if fAtomsVector is non-NULL, complain. Apples and oranges.  $$$
    if (fAtomsVector) {
        G4cerr << "This material is already being defined via elements by"
             << "atoms." << G4endl;
        G4Exception ("You are mixing apples and oranges ...");
    }
    // initialization
    if (fNumberOfComponents == 0) {
        fMassFractionVector = new G4double[100];
    }

    // filling ...
    if (G4int(fNumberOfComponents) < maxNbComponents) {
       for (size_t elm=0; elm < material->GetNumberOfElements(); elm++)
          { G4Element* element = (*(material->GetElementVector()))[elm];
            size_t el = 0;
            while ((el<fNumberOfElements)&&(element!=(*theElementVector)[el])) el++;
            if (el<fNumberOfElements) fMassFractionVector[el] += fraction
                                                  *(material->GetFractionVector())[elm];
            else {
              if(el>=theElementVector->length()) theElementVector->resize(el+1);
              (*theElementVector)[el] = element;
              fMassFractionVector[el] = fraction*(material->GetFractionVector())[elm];
              fNumberOfElements ++;
            }
          } 
        fNumberOfComponents++;  
    }
    else
       G4Exception
      ("ERROR!!! - Attempt to add more than the declared number of components.");

    // filled.
    if (G4int(fNumberOfComponents) == maxNbComponents) {
       // check sum of weights -- OK?
       G4double wtSum(0.0);
       for (size_t i=0;i<fNumberOfElements;i++)
        { wtSum +=  fMassFractionVector[i]; }
       if (abs(1.-wtSum) > perThousand) {
         G4cerr << "WARNING !! - Fractional masses do not sum to 1 :the Delta is > 0.001"
              << "( the weights are NOT renormalized; the results may be wrong)" 
              << G4endl;
       }

       ComputeDerivedQuantities();

       // Store in the static Table of Materials
       theMaterialTable.insert(this);
       fIndexInTable = theMaterialTable.index(this);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4Material::ComputeDerivedQuantities()
{
  // Header routine to compute various properties of material.
  // 

  // Atoms density vector, Electrons density
  G4double Zi, Ai;
  TotNbOfAtomsPerVolume = 0.;
  if (VecNbOfAtomsPerVolume) delete [] VecNbOfAtomsPerVolume;
  VecNbOfAtomsPerVolume = new G4double[fNumberOfElements];
  TotNbOfElectPerVolume = 0.;
  for (size_t i=0;i<fNumberOfElements;i++) {
     Zi = (*theElementVector)[i]->GetZ();
     Ai = (*theElementVector)[i]->GetA();
     VecNbOfAtomsPerVolume[i] = Avogadro*fDensity*fMassFractionVector[i]/Ai;
     TotNbOfAtomsPerVolume += VecNbOfAtomsPerVolume[i];
     TotNbOfElectPerVolume += VecNbOfAtomsPerVolume[i]*Zi;
   }

   //for gas, check coherence of the state conditions
   if (fState == kStateGas) {
      G4double ratio = TotNbOfAtomsPerVolume*k_Boltzmann*fTemp/fPressure;
      if ((ratio<0.1)||(ratio>10.)) {
         G4cerr << "---warning from G4Material-- The state conditions of the gas: " 
              << fName << " are not consistent."
              << "\n density  = "    << fDensity/(mg/cm3)     << " mg/cm3"
              << "\t pressure = "    << fPressure/atmosphere  << " atmosphere"
              << "\t temperature = " << fTemp/kelvin          << " kelvin"
              << "\n rho*(T/P) would be of the order of: " 
              << (fDensity/(TotNbOfAtomsPerVolume*k_Boltzmann))/((mg/cm3)*(kelvin/atmosphere))
              << " (mg/cm3)*(kelvin/atmosphere).  The energy loss calculation maybe be affected \n";
        }
     }
         
   ComputeRadiationLength();
   ComputeNuclearInterLength();

   if (fIonisation) delete fIonisation;
   fIonisation  = new G4IonisParamMat(this);
   if (fSandiaTable) delete fSandiaTable;
   fSandiaTable = new G4SandiaTable(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4Material::ComputeRadiationLength()
{
  G4double radinv = 0.0 ;
  for (size_t i=0;i<fNumberOfElements;i++) {
     radinv += VecNbOfAtomsPerVolume[i]*((*theElementVector)[i]->GetfRadTsai()); 
   }
  fRadlen = (radinv <= 0.0 ? DBL_MAX : 1./radinv);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4Material::ComputeNuclearInterLength()
{
  const G4double lambda0 = 35*g/cm2;
  G4double NILinv = 0.0;
  for (size_t i=0;i<fNumberOfElements;i++) {
     NILinv +=
     VecNbOfAtomsPerVolume[i]*pow(((*theElementVector)[i]->GetN()),2./3.); 
   }
  NILinv *= amu/lambda0; 
  fNuclInterLen = (NILinv <= 0.0 ? DBL_MAX : 1./NILinv);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

void G4Material::InitializePointers()
{
    theElementVector         = NULL;
    fMassFractionVector      = NULL;
    fAtomsVector             = NULL;
    fMaterialPropertiesTable = NULL;
    
    VecNbOfAtomsPerVolume    = NULL;
    fIonisation              = NULL;
    fSandiaTable             = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4Material::~G4Material()
{
  if (fImplicitElement)       delete    ((*theElementVector)[0]);
  if (theElementVector)       delete    theElementVector;
  if (fMassFractionVector)    delete [] fMassFractionVector;
  if (fAtomsVector)           delete [] fAtomsVector;
  if (VecNbOfAtomsPerVolume)  delete [] VecNbOfAtomsPerVolume;
  if (fIonisation)            delete    fIonisation;
  if (fSandiaTable)           delete    fSandiaTable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4Material::G4Material(const G4Material& right)
{
    InitializePointers();
    *this = right;
        
    // Store this new material in the table of Materials
    theMaterialTable.insert(this);
    fIndexInTable = theMaterialTable.index(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

const G4Material& G4Material::operator=(const G4Material& right)
{
  if (this != &right)
    {
      fName                    = right.fName;
      fChemicalFormula         = right.fChemicalFormula;
      fDensity                 = right.fDensity;
      fState                   = right.fState;
      fTemp                    = right.fTemp;
      fPressure                = right.fPressure;
           
      if (fImplicitElement)    delete    ((*theElementVector)[0]);      
      if (theElementVector)    delete    theElementVector;
      if (fMassFractionVector) delete [] fMassFractionVector;
      if (fAtomsVector)        delete [] fAtomsVector;
      
      maxNbComponents          = right.maxNbComponents;
      fNumberOfComponents      = right.fNumberOfComponents;
      fNumberOfElements        = right.fNumberOfElements;     
      fImplicitElement         = right.fImplicitElement;
      
      if (fImplicitElement) {
        G4double z = (*right.theElementVector)[0]->GetZ();
	G4double a = (*right.theElementVector)[0]->GetA();
        theElementVector       = new G4ElementVector(1);
	theElementVector[0]    = new G4Element(fName," ",z,a);
	fMassFractionVector    = new G4double[1];
	fMassFractionVector[0] = 1.;
      } else {		
        theElementVector       = new G4ElementVector(fNumberOfElements);
        fMassFractionVector    = new G4double[fNumberOfElements];     
        for (size_t i=0; i<fNumberOfElements; i++) {
           (*theElementVector)[i]= (*right.theElementVector)[i];
           fMassFractionVector[i]= right.fMassFractionVector[i];
        }
      }
      
      if (right.fAtomsVector) { 
        fAtomsVector       = new G4int[fNumberOfElements];
        for (size_t i=0; i<fNumberOfElements; i++)              
           fAtomsVector[i] = right.fAtomsVector[i];
      }
      	   
      fMaterialPropertiesTable = right.fMaterialPropertiesTable;
      
      ComputeDerivedQuantities();      
      fIndexInTable = right.fIndexInTable;
     } 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4Material::operator==(const G4Material& right) const
{
  return (this == (G4Material *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4Material::operator!=(const G4Material& right) const
{
  return (this != (G4Material *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....


G4std::ostream& operator<<(G4std::ostream& flux, G4Material* material)
{
  long mode = flux.setf(G4std::ios::fixed,G4std::ios::floatfield);
  long prec = flux.precision(3);
  
  flux
    << " Material: "      << G4std::setw(8) <<  material->fName
    << " " << material->fChemicalFormula << " "
    << "  density: "     << G4std::setw(6) << G4std::setprecision(3)  
                          << G4BestUnit(material->fDensity,"Volumic Mass") 
    << "  temperature: " << G4std::setw(6) << G4std::setprecision(2)  
                          << (material->fTemp)/kelvin << " K"
    << "  pressure: "    << G4std::setw(6) << G4std::setprecision(2)   
                          << (material->fPressure)/atmosphere << " atm"
    << "  RadLength: "   << G4std::setw(7)  << G4std::setprecision(3)  
                          << G4BestUnit(material->fRadlen,"Length");

  for (size_t i=0; i<material->fNumberOfElements; i++)
  flux 
    << "\n   ---> " << (*(material->theElementVector))[i] 
    << "  fractionMass: " << G4std::setw(6)<< G4std::setprecision(2) 
                          << (material->fMassFractionVector[i])/perCent << " %" 
    << "  Abundance "     << G4std::setw(6)<< G4std::setprecision(2) 
                          << 100*(material->VecNbOfAtomsPerVolume[i])/
                                 (material->TotNbOfAtomsPerVolume)
                          << " %";

  flux.precision(prec);    
  flux.setf(mode,G4std::ios::floatfield);
            
  return flux;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

 G4std::ostream& operator<<(G4std::ostream& flux, G4Material& material)
{
  flux << &material;        
  return flux;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
     
G4std::ostream& operator<<(G4std::ostream& flux, G4MaterialTable MaterialTable)
{
 //Dump info for all known materials
   flux << "\n***** Table : Nb of materials = " << MaterialTable.length() 
        << " *****\n" << G4endl;
        
   for (size_t i=0; i<MaterialTable.length(); i++) flux << MaterialTable[i] 
                                                       << G4endl << G4endl;

   return flux;
}      
