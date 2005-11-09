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
// $Id: G4Isotope.hh,v 1.17 2005-11-09 15:38:43 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// class description
//
// An isotope is a chemical isotope defined by its name,
//                                                 Z: atomic number,
//                                                 N: number of nucleons, 
//                                                 A: mass of a mole (optional).
//
// The class contains as a private static member the table of defined
// isotopes (an ordered vector of isotopes).
//
// Isotopes can be assembled into elements via the G4Element class.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 31.03.05: A becomes optional. Taken from Nist data base by default (mma)  
// 26.02.02: fIndexInTable renewed 
// 14.09.01: fCountUse: nb of elements which use this isotope 
// 13.09.01: stl migration. Suppression of the data member fIndexInTable
// 30.03.01: suppression of the warning message in GetIsotope
// 04.08.98: new method GetIsotope(isotopeName) (mma)
// 17.01.97: aesthetic rearrangement (mma)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef G4ISOTOPE_HH
#define G4ISOTOPE_HH

#include "globals.hh"
#include "G4ios.hh"
#include <vector>

class G4Isotope;
typedef std::vector<G4Isotope*> G4IsotopeTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo......

class G4Isotope
{
 public:  // with description

    // Make an isotope
    G4Isotope(const G4String& name,		//its name
                    G4int     z,		//atomic number
                    G4int     n,		//number of nucleons
                    G4double  a = 0.);		//mass of mole
                    
    virtual ~G4Isotope();

    // Retrieval methods
    G4String GetName()  const {return fName;};    
    G4int    GetZ()     const {return fZ;};
    G4int    GetN()     const {return fN;};
    G4double GetA()     const {return fA;};
    
    G4int GetCountUse() const {return fCountUse;};
    void  increaseCountUse()  {fCountUse++;};
    void  decreaseCountUse()  {fCountUse--;};
    
    static  
    G4Isotope* GetIsotope(G4String name);
    
    static const
    G4IsotopeTable* GetIsotopeTable();
    
    static 
    size_t GetNumberOfIsotopes();
    
    size_t GetIndex() const {return fIndexInTable;};    
    
    friend
    std::ostream& operator<<(std::ostream&, G4Isotope*);
    
    friend
    std::ostream& operator<<(std::ostream&, G4Isotope&);
    
    friend
    std::ostream& operator<<(std::ostream&, G4IsotopeTable);
     
 public:  // without description
 
    G4int operator==(const G4Isotope&) const;
    G4int operator!=(const G4Isotope&) const;

    G4Isotope(__void__&);
      // Fake default constructor for usage restricted to direct object
      // persistency for clients requiring preallocation of memory for
      // persistifiable objects.
   
 private:
     
    G4Isotope(G4Isotope&);
    G4Isotope& operator=(const G4Isotope&);
     
 private:

    G4String fName;              // name of the Isotope
    G4int    fZ;                 // atomic number
    G4int    fN;                 // number of nucleons
    G4double fA;                 // mass of a mole
    
    G4int    fCountUse;          // nb of elements which use this isotope

    static 
    G4IsotopeTable theIsotopeTable;
    
    size_t fIndexInTable;        // index in the Isotope table
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
