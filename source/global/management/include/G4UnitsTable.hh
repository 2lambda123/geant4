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
// $Id: G4UnitsTable.hh,v 1.14 2004-01-21 13:17:50 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// -----------------------------------------------------------------
//
//      ------------------- class G4UnitsTable -----------------
//
// 17-05-98: first version, M.Maire
// 13-10-98: Units and symbols printed in fixed length, M.Maire
// 18-01-00: BestUnit for three vector, M.Maire
// 06-03-01: Migrated to STL vectors, G.Cosmo
//
// Class description:
//
// This class maintains a table of Units.
// A Unit has a name, a symbol, a value and belong to a category (i.e. its
// dimensional definition): Length, Time, Energy, etc...
// The Units are grouped by category. The TableOfUnits is a list of categories.
// The class G4BestUnit allows to convert automaticaly a physical quantity
// from its internal value into the most appropriate Unit of the same category.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef G4UnitsTable_HH
#define G4UnitsTable_HH

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

class G4UnitsCategory;
typedef std::vector<G4UnitsCategory*> G4UnitsTable;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4UnitDefinition
{
public:  // with description

    G4UnitDefinition(const G4String& name, const G4String& symbol,
                     const G4String& category, G4double value);
	    
public:  // without description
	    
   ~G4UnitDefinition();
    G4int operator==(const G4UnitDefinition&) const;
    G4int operator!=(const G4UnitDefinition&) const;
    
private:

    G4UnitDefinition(const G4UnitDefinition&);
    G4UnitDefinition& operator=(const G4UnitDefinition&);
   
public:  // with description

    const G4String& GetName()   const {return Name;}
    const G4String& GetSymbol() const {return SymbolName;}
    G4double        GetValue()  const {return Value;}
    
    void          PrintDefinition();
    
    static void BuildUnitsTable();    
    static void PrintUnitsTable();
    
    static G4UnitsTable& GetUnitsTable();

    static G4double GetValueOf (const G4String&);
    static G4String GetCategory(const G4String&);

private:

    G4String Name;            // SI name
    G4String SymbolName;      // SI symbol
    G4double Value;           // value in the internal system of units
    
    static 
    G4UnitsTable theUnitsTable;   // table of Units
    
    
    size_t CategoryIndex;         // category index of this unit
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef std::vector<G4UnitDefinition*> G4UnitsContainer;

class G4UnitsCategory
{
public:  // without description

    G4UnitsCategory(const G4String& name);
   ~G4UnitsCategory();
    G4int operator==(const G4UnitsCategory&) const;
    G4int operator!=(const G4UnitsCategory&) const;
    
private:

    G4UnitsCategory(const G4UnitsCategory&);
    G4UnitsCategory& operator=(const G4UnitsCategory&);
   
public:  // without description

    const G4String&   GetName()      const {return Name;}
    G4UnitsContainer& GetUnitsList()       {return UnitsList;}
    G4int             GetNameMxLen() const {return NameMxLen;}
    G4int             GetSymbMxLen() const {return SymbMxLen;}
    void  UpdateNameMxLen(G4int len) {if (NameMxLen<len) NameMxLen=len;}
    void  UpdateSymbMxLen(G4int len) {if (SymbMxLen<len) SymbMxLen=len;}
    void  PrintCategory();

private:

    G4String          Name;        // dimensional family: Length,Volume,Energy ...
    G4UnitsContainer  UnitsList;   // List of units in this family
    G4int             NameMxLen;   // max length of the units name
    G4int             SymbMxLen;   // max length of the units symbol
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4BestUnit
{
public:  // with description

    G4BestUnit(G4double internalValue, const G4String& category);
    G4BestUnit(const G4ThreeVector& internalValue, const G4String& category);    
      // These constructors convert a physical quantity from its internalValue
      // into the most appropriate unit of the same category.
      // In practice it builds an object VU = (newValue, newUnit)

   ~G4BestUnit();
   
public:  // without description

    G4double*  GetValue()                 {return Value;}
    const G4String&   GetCategory() const {return Category;}
    size_t     GetIndexOfCategory() const {return IndexOfCategory;}
    
public:  // with description 
   
    friend
    std::ostream&  operator<<(std::ostream&,G4BestUnit VU);
      // Default format to print the objet VU above.

private:

    G4double   Value[3];        // value in the internal system of units
    G4int      nbOfVals;        // G4double=1; G4ThreeVector=3
    G4String   Category;        // dimensional family: Length,Volume,Energy ...
    size_t IndexOfCategory;     // position of Category in UnitsTable
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
