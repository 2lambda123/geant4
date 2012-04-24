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
// $Id: G4Element.cc,v 1.36 2010-10-25 07:58:15 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 26-06-96: Code uses operators (+=, *=, ++, -> etc.) correctly, P. Urban
// 09-07-96: new data members added by L.Urban
// 17-01-97: aesthetic rearrangement, M.Maire
// 20-01-97: Compute Tsai's formula for the rad length, M.Maire
// 21-01-97: remove mixture flag, M.Maire
// 24-01-97: ComputeIonisationParameters(). 
//           new data member: fTaul, M.Maire
// 29-01-97: Forbidden to create Element with Z<1 or N<Z, M.Maire
// 20-03-97: corrected initialization of pointers, M.Maire
// 28-04-98: atomic subshell binding energies stuff, V. Grichine  
// 09-07-98: Ionisation parameters removed from the class, M.Maire
// 16-11-98: name Subshell -> Shell; GetBindingEnergy() (mma)
// 09-03-01: assignement operator revised (mma)
// 02-05-01: check identical Z in AddIsotope (marc)
// 03-05-01: flux.precision(prec) at begin/end of operator<<
// 13-09-01: suppression of the data member fIndexInTable
// 14-09-01: fCountUse: nb of materials which use this element
// 26-02-02: fIndexInTable renewed
// 30-03-05: warning in GetElement(elementName)
// 15-11-05: GetElement(elementName, G4bool warning=true) 
// 17-10-06: Add fNaturalAbandances (V.Ivanchenko)
// 27-07-07: improve destructor (V.Ivanchenko) 
// 18-10-07: move definition of material index to ComputeDerivedQuantities (VI) 
// 25.10.11: new scheme for G4Exception  (mma)
// 05-03-12: always create isotope vector (V.Ivanchenko) 
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Element.hh"
#include "G4AtomicShells.hh"
#include "G4NistManager.hh"
#include <iomanip>
#include <sstream>

G4ElementTable G4Element::theElementTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Constructor to Generate an element from scratch

G4Element::G4Element(const G4String& name, const G4String& symbol,
                     G4double zeff, G4double aeff)
  : fName(name), fSymbol(symbol)		     
{
  G4int iz = (G4int)zeff;
  if (zeff<1.) {
    G4ExceptionDescription ed;
    ed << "Fail to create G4Element " << name 
       << " Z= " << zeff << " < 1 !" << G4endl;
    G4Exception ("G4Element::G4Element()", "mat011",  FatalException, ed);
    return;
  }
  if (std::abs(zeff - iz) > perMillion) {
    G4ExceptionDescription ed;
    ed << "G4Element Warning:  " << name << " Z= " << zeff 
       << " A= " << aeff/(g/mole) << G4endl; 
    G4Exception("G4Element::G4Element()", "mat017", JustWarning, ed);
  }

  InitializePointers();

  fZeff   = zeff;
  fNeff   = aeff/(g/mole);
  fAeff   = aeff;

  if(fNeff < 1.0) fNeff = 1.0;

  if (fNeff < zeff) {
    G4ExceptionDescription ed;
    ed << "Fail to create G4Element " << name 
       << " with Z= " << zeff << "  N= " << fNeff 
       << "   N < Z is not allowed" << G4endl;
    G4Exception ("G4Element::G4Element()", "mat012",  FatalException, ed);
    return;
  }
   
  fNbOfAtomicShells      = G4AtomicShells::GetNumberOfShells(iz);
  fAtomicShells          = new G4double[fNbOfAtomicShells];
  fNbOfShellElectrons    = new G4int[fNbOfAtomicShells];

  AddNaturalIsotopes();

  for (G4int i=0;i<fNbOfAtomicShells;i++) 
  {
    fAtomicShells[i] = G4AtomicShells::GetBindingEnergy(iz, i);
    fNbOfShellElectrons[i] = G4AtomicShells::GetNumberOfElectrons(iz, i);
  }
  ComputeDerivedQuantities();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Constructor to Generate element from a List of 'nIsotopes' isotopes, added
// via AddIsotope

G4Element::G4Element(const G4String& name,
                     const G4String& symbol, G4int nIsotopes)
  : fName(name),fSymbol(symbol)
{
  InitializePointers();

  size_t n = size_t(nIsotopes);

  if(0 == nIsotopes) {
    AddNaturalIsotopes();
  } else {
    theIsotopeVector         = new G4IsotopeVector(n,0);
    fRelativeAbundanceVector = new G4double[nIsotopes];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Add an isotope to the element

void G4Element::AddIsotope(G4Isotope* isotope, G4double abundance)
{
  if (theIsotopeVector == 0) {
    G4ExceptionDescription ed;
    ed << "Fail to add Isotope to G4Element " << fName 
       << " with Z= " << fZeff << "  N= " << fNeff << G4endl;
    G4Exception ("G4Element::AddIsotope()", "mat013",  FatalException, ed);
    return;
  }
  G4int iz = isotope->GetZ();

  // filling ...
  if ( fNumberOfIsotopes < theIsotopeVector->size() ) {
    // check same Z
    if (fNumberOfIsotopes==0) { fZeff = G4double(iz); }
    else if (G4double(iz) != fZeff) { 
      G4ExceptionDescription ed;
      ed << "Fail to add Isotope Z= " << iz << " to G4Element " << fName 
	 << " with different Z= " << fZeff << fNeff 
	 << G4endl;
      G4Exception ("G4Element::AddIsotope()", "mat014",  FatalException, ed);
      return;
    }
    //Z ok   
    fRelativeAbundanceVector[fNumberOfIsotopes] = abundance;
    (*theIsotopeVector)[fNumberOfIsotopes] = isotope;
    ++fNumberOfIsotopes;
    isotope->increaseCountUse();
  } else {
    G4ExceptionDescription ed;
    ed << "Fail to add Isotope Z= " << iz << " to G4Element " << fName 
       << " - more isotopes than declaired " << G4endl;
    G4Exception ("G4Element::AddIsotope()", "mat015",  FatalException, ed);
    return;
  }

  // filled.
  if ( fNumberOfIsotopes == theIsotopeVector->size() ) {
    // Compute Neff, Aeff
    G4double wtSum=0.0;

    fNeff = fAeff = 0.0;
    for (size_t i=0;i<fNumberOfIsotopes;i++) {
      fAeff +=  fRelativeAbundanceVector[i]*(*theIsotopeVector)[i]->GetA();
      wtSum +=  fRelativeAbundanceVector[i];
    }
    fAeff  /= wtSum;
    fNeff   = fAeff/(g/mole);

    if(wtSum != 1.0) {
      for(size_t i=0; i<fNumberOfIsotopes; ++i) { 
	fRelativeAbundanceVector[i] /= wtSum; 
      }
    }
      
    fNbOfAtomicShells = G4AtomicShells::GetNumberOfShells(iz);
    fAtomicShells     = new G4double[fNbOfAtomicShells];
    fNbOfShellElectrons = new G4int[fNbOfAtomicShells];

    for ( G4int j = 0; j < fNbOfAtomicShells; j++ ) 
    {
      fAtomicShells[j]       = G4AtomicShells::GetBindingEnergy(iz, j);
      fNbOfShellElectrons[j] = G4AtomicShells::GetNumberOfElectrons(iz, j);
    }         
    ComputeDerivedQuantities();

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4Element::InitializePointers()
{
  theIsotopeVector = 0;
  fRelativeAbundanceVector = 0;
  fAtomicShells = 0;
  fNbOfShellElectrons = 0;
  fIonisation = 0;
  fNumberOfIsotopes = 0;
  fNaturalAbandances = false;

  // add initialisation of all remaining members
  fZeff = 0;
  fNeff = 0;
  fAeff = 0;
  fNbOfAtomicShells = 0; 
  fCountUse = 0;
  fIndexZ = 0;
  fIndexInTable = 0;
  fNaturalAbandances = false;
  fCoulomb = 0.0;
  fRadTsai = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Fake default constructor - sets only member data and allocates memory
//                            for usage restricted to object persistency

G4Element::G4Element( __void__& )
  : fZeff(0), fNeff(0), fAeff(0)
{
  InitializePointers();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element::~G4Element()
{
  //  G4cout << "### Destruction of element " << fName << " started" <<G4endl;

  if (theIsotopeVector)         { delete theIsotopeVector; }
  if (fRelativeAbundanceVector) { delete [] fRelativeAbundanceVector; }
  if (fAtomicShells)            { delete [] fAtomicShells; }
  if (fNbOfShellElectrons)      { delete [] fNbOfShellElectrons; }
  if (fIonisation)              { delete    fIonisation; }
  
  //remove this element from theElementTable
  theElementTable[fIndexInTable] = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4Element::ComputeDerivedQuantities()
{
  // some basic functions of the atomic number

  // Store in table
  theElementTable.push_back(this);
  fIndexInTable = theElementTable.size() - 1;
      
  // check if elements with same Z already exist
  fIndexZ = 0; 
  for (size_t J=0 ; J<fIndexInTable ; J++) {
    if (theElementTable[J]->GetZ() == fZeff) { ++fIndexZ; }
  }
  //nb of materials which use this element
  fCountUse = 0;

  // Radiation Length
  ComputeCoulombFactor();
  ComputeLradTsaiFactor(); 

  // parameters for energy loss by ionisation 
  if (fIonisation) { delete fIonisation; }  
  fIonisation = new G4IonisParamElm(fZeff);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4Element::ComputeCoulombFactor()
{
  //
  //  Compute Coulomb correction factor (Phys Rev. D50 3-1 (1994) page 1254)

  const G4double k1 = 0.0083 , k2 = 0.20206 ,k3 = 0.0020 , k4 = 0.0369 ;

  G4double az2 = (fine_structure_const*fZeff)*(fine_structure_const*fZeff);
  G4double az4 = az2 * az2;

  fCoulomb = (k1*az4 + k2 + 1./(1.+az2))*az2 - (k3*az4 + k4)*az4;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4Element::ComputeLradTsaiFactor()
{
  //
  //  Compute Tsai's Expression for the Radiation Length
  //  (Phys Rev. D50 3-1 (1994) page 1254)

  const G4double Lrad_light[]  = {5.31  , 4.79  , 4.74 ,  4.71} ;
  const G4double Lprad_light[] = {6.144 , 5.621 , 5.805 , 5.924} ;
  
  const G4double logZ3 = std::log(fZeff)/3.;

  G4double Lrad, Lprad;
  G4int iz = (G4int)(fZeff+0.5) - 1 ;
  if (iz <= 3) { Lrad = Lrad_light[iz] ;  Lprad = Lprad_light[iz] ; }
    else { Lrad = std::log(184.15) - logZ3 ; Lprad = std::log(1194.) - 2*logZ3;}

  fRadTsai = 4*alpha_rcl2*fZeff*(fZeff*(Lrad-fCoulomb) + Lprad); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4Element::AddNaturalIsotopes()
{
  G4int Z = G4int(fZeff + 0.5);
  G4NistManager* nist = G4NistManager::Instance();
  G4int n = nist->GetNumberOfNistIsotopes(Z);
  G4int N0 = nist->GetNistFirstIsotopeN(Z);
  fNumberOfIsotopes = 0;
  for(G4int i=0; i<n; ++i) {
    if(nist->GetIsotopeAbundance(Z, N0+i) > 0.0) { ++fNumberOfIsotopes; }
  }
  theIsotopeVector         = new G4IsotopeVector(fNumberOfIsotopes,0);
  fRelativeAbundanceVector = new G4double[fNumberOfIsotopes];
  G4int idx = 0;
  G4double xsum = 0.0;
  for(G4int i=0; i<n; ++i) {
    G4int N = N0 + i;
    G4double x = nist->GetIsotopeAbundance(Z, N); 
    if(x > 0.0) {
      std::ostringstream s;
      s << fName << N;
      (*theIsotopeVector)[idx] = new G4Isotope(s.str(),Z, N, 0.0, 0);
      fRelativeAbundanceVector[idx] = x;
      xsum += x;
      ++idx;
    }
  }
  if(xsum != 0.0 && xsum != 1.0) {
    for(G4int i=0; i<idx; ++i) { fRelativeAbundanceVector[i] /= xsum; }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4Element::GetAtomicShell(G4int i) const
{
  if (i<0 || i>=fNbOfAtomicShells) {
    G4ExceptionDescription ed;
    ed << "Invalid argument " << i << " in for G4Element " << fName 
       << " with Z= " << fZeff 
       << " and Nshells= " << fNbOfAtomicShells  
       << G4endl;
    G4Exception("G4Element::GetAtomicShell()", "mat016", FatalException, ed);
    return 0.0;
  }
  return fAtomicShells[i];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4Element::GetNbOfShellElectrons(G4int i) const
{
  if (i<0 || i>=fNbOfAtomicShells) {
    G4ExceptionDescription ed;
    ed << "Invalid argument " << i << " for G4Element " << fName 
       << " with Z= " << fZeff 
       << " and Nshells= " << fNbOfAtomicShells  
       << G4endl;
    G4Exception("G4Element::GetNbOfShellElectrons()", "mat016", FatalException, ed);
    return 0;
  }
  return fNbOfShellElectrons[i];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4ElementTable* G4Element::GetElementTable()
{
  return &theElementTable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

size_t G4Element::GetNumberOfElements()
{
  return theElementTable.size();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element* G4Element::GetElement(G4String elementName, G4bool warning)
{  
  // search the element by its name 
  for (size_t J=0 ; J<theElementTable.size() ; J++)
   {
     if (theElementTable[J]->GetName() == elementName)
       return theElementTable[J];
   }
   
  // the element does not exist in the table
  if (warning) {
    G4cout << "\n---> warning from G4Element::GetElement(). The element: "
	   << elementName << " does not exist in the table. Return NULL pointer."
	   << G4endl;
  }   
  return 0;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element::G4Element(G4Element& right)
{
  InitializePointers();
  *this = right;

  // Store this new element in table and set the index
  theElementTable.push_back(this);
  fIndexInTable = theElementTable.size() - 1; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4Element& G4Element::operator=(const G4Element& right)
{
  if (this != &right)
    {
      fName                    = right.fName;
      fSymbol                  = right.fSymbol;
      fZeff                    = right.fZeff;
      fNeff                    = right.fNeff;
      fAeff                    = right.fAeff;
      
      if (fAtomicShells) delete [] fAtomicShells;      
      fNbOfAtomicShells = right.fNbOfAtomicShells; 
      fAtomicShells     = new G4double[fNbOfAtomicShells];

      if (fNbOfShellElectrons) delete [] fNbOfShellElectrons;
      fNbOfAtomicShells   = right.fNbOfAtomicShells;            
      fNbOfShellElectrons = new G4int[fNbOfAtomicShells];

      for ( G4int i = 0; i < fNbOfAtomicShells; i++ ) 
      {     
         fAtomicShells[i]     = right.fAtomicShells[i];
         fNbOfShellElectrons[i] = right.fNbOfShellElectrons[i];
      } 
      if (theIsotopeVector) delete theIsotopeVector;
      if (fRelativeAbundanceVector) delete [] fRelativeAbundanceVector;
	      	 
      fNumberOfIsotopes        = right.fNumberOfIsotopes;
      if (fNumberOfIsotopes > 0)
        {
	 theIsotopeVector         = new G4IsotopeVector(fNumberOfIsotopes,0);
	 fRelativeAbundanceVector = new G4double[fNumberOfIsotopes];
	 for (size_t i=0;i<fNumberOfIsotopes;i++)
	    {
             (*theIsotopeVector)[i]      = (*right.theIsotopeVector)[i];
             fRelativeAbundanceVector[i] = right.fRelativeAbundanceVector[i];
	    }
	}   
      ComputeDerivedQuantities();
     } 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4Element::operator==(const G4Element& right) const
{
  return (this == (G4Element*) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4Element::operator!=(const G4Element& right) const
{
  return (this != (G4Element*) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::ostream& operator<<(std::ostream& flux, G4Element* element)
{
  std::ios::fmtflags mode = flux.flags();
  flux.setf(std::ios::fixed,std::ios::floatfield);
  G4long prec = flux.precision(3);
  
  flux
    << " Element: " << element->fName   << " (" << element->fSymbol << ")"
    << "   Z = " << std::setw(4) << std::setprecision(1) <<  element->fZeff 
    << "   N = " << std::setw(5) << std::setprecision(1) <<  element->fNeff
    << "   A = " << std::setw(6) << std::setprecision(2)
                 << (element->fAeff)/(g/mole) << " g/mole";
   
  for (size_t i=0; i<element->fNumberOfIsotopes; i++)
  flux 
    << "\n   ---> " << (*(element->theIsotopeVector))[i] 
    << "   abundance: " << std::setw(6) << std::setprecision(2) 
    << (element->fRelativeAbundanceVector[i])/perCent << " %";
    
  flux.precision(prec);        
  flux.setf(mode,std::ios::floatfield);         
  return flux;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 std::ostream& operator<<(std::ostream& flux, G4Element& element)
{
  flux << &element;        
  return flux;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
     
std::ostream& operator<<(std::ostream& flux, G4ElementTable ElementTable)
{
 //Dump info for all known elements
   flux << "\n***** Table : Nb of elements = " << ElementTable.size() 
        << " *****\n" << G4endl;
        
   for (size_t i=0; i<ElementTable.size(); i++) flux << ElementTable[i] 
						     << G4endl << G4endl;

   return flux;
}
      
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
