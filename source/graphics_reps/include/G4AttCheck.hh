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
// $Id: G4AttCheck.hh,v 1.2 2005-03-26 22:37:14 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef G4ATTCHECK_HH
#define G4ATTCHECK_HH

// Class Description:
//
// @class G4AttCheck
//
// @brief Checks G4AttValue's and their corresponding G4AttDef map.
//
// Usage (a): G4AttCheck(values,definitions);
//    or (b): G4cout << G4AttCheck(values,definitions) << G4endl;
//
// For further details, see the HepRep home page at http://heprep.freehep.org
//  
// @author J.Allison
// @author J.Perl
// Class Description - End:

#include "globals.hh"

#include <set>
#include <vector>
#include <map>
#include <iostream>

class G4AttValue;
class G4AttDef;

class G4AttCheck {
public:
  G4AttCheck
  (const std::vector<G4AttValue>* values,
   const std::map<G4String,G4AttDef>* definitions);
  ~G4AttCheck();
  const std::vector<G4AttValue>* GetAttValues() const {
    return fpValues;
  }
  const std::map<G4String,G4AttDef>* GetAttDefs() const {
    return fpDefinitions;
  }
  void Check() const;  // Check only.  Silent unless error - then G4cerr.
  G4AttCheck Standard() const;  // Returns standard versions on the heap.
  friend std::ostream& operator<< (std::ostream&, const G4AttCheck&);
private:
  void AddValuesAndDefs
  (std::vector<G4AttValue>* pValues,
   std::map<G4String,G4AttDef>* pDefinitions,
   const G4String& oldName,
   const G4String& name,
   const G4String& value,
   const G4String& valueType,
   const G4String& description = "") const;  // Utility function for Standard.
  static std::set<G4String> fUnitCategories;  // Set of legal unit categories.
  static std::map<G4String,G4String> fStandardUnits;  // Standard units.
  static std::set<G4String> fValueTypes;      // Set of legal value types.
  const std::vector<G4AttValue>* fpValues;
  const std::map<G4String,G4AttDef>* fpDefinitions;
};

#endif //G4ATTCHECK_HH
