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
// $Id: G4AttCheck.cc,v 1.7 2005-09-16 01:04:43 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4AttCheck.hh"

#include "globals.hh"

#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4UIcommand.hh"

G4AttCheck::G4AttCheck
(const std::vector<G4AttValue>* values,
 const std::map<G4String,G4AttDef>* definitions):
  fpValues(values),
  fpDefinitions(definitions)
{
  if (fFirst) {  // Initialise static containers.
    fFirst = false;

    // Legal Unit Category Types...
    fUnitCategories.insert("Length");
    fUnitCategories.insert("Energy");
    fUnitCategories.insert("Time");
    fUnitCategories.insert("Electric charge");

    // Corresponding Standard Units...
    fStandardUnits["Length"] = "m";
    fStandardUnits["Energy"] = "MeV";
    fStandardUnits["Time"] = "ns";
    fStandardUnits["Electric charge"] = "e+";

    // Legal Categories...
    fCategories.insert("Bookkeeping");
    fCategories.insert("Draw");
    fCategories.insert("Physics");
    fCategories.insert("PickAction");
    fCategories.insert("Association");

    // Legal units...
    fUnits.insert("G4BestUnit");
    // ...plus any legal unit symbol ("MeV", "km", etc.)...
    G4UnitsTable& units = G4UnitDefinition::GetUnitsTable();
    for (size_t i = 0; i < units.size(); ++i) {
      if (fUnitCategories.find(units[i]->GetName()) !=
	  fUnitCategories.end()) {
	//G4cout << units[i]->GetName() << G4endl;
	G4UnitsContainer& container = units[i]->GetUnitsList();
	for (size_t j = 0; j < container.size(); ++j) {
	  //G4cout << container[j]->GetName() << ' '
	  //       << container[j]->GetSymbol() << G4endl;
	  fUnits.insert(container[j]->GetSymbol());
	}
      }
    }

    // Legal Value Types...
    fValueTypes.insert("G4String");
    fValueTypes.insert("G4int");
    fValueTypes.insert("G4double");
    fValueTypes.insert("G4ThreeVector");
  }
}

G4AttCheck::~G4AttCheck() {}

G4bool G4AttCheck::fFirst = true;

std::set<G4String> G4AttCheck::fUnitCategories;

std::map<G4String,G4String> G4AttCheck::fStandardUnits;

std::set<G4String> G4AttCheck::fCategories;

std::set<G4String> G4AttCheck::fUnits;

std::set<G4String> G4AttCheck::fValueTypes;

G4bool G4AttCheck::Check(const G4String& leader) const {
  // Check only.  Silent unless error - then G4cerr.  Returns error.
  G4bool error = false;
  static G4int iError = 0;
  G4bool print = false;
  if (iError < 10 || iError%100 == 0) {
    print = true;
  }
  using namespace std;
  if (!fpValues) return error;  // A null values vector is a valid situation.
  if (!fpDefinitions) {
    ++iError;
    error = true;
    if (print) {
      G4cerr <<
	"\n*******************************************************";
      if (leader != "") {
	G4cerr << '\n' << leader;
      }
      G4cerr <<
	"\nERROR " << iError << ": Null definitions map<G4String,G4AttDef>*"
	"\n*******************************************************"
	     << G4endl;
      return error;
    }
  }
  vector<G4AttValue>::const_iterator iValue;
  for (iValue = fpValues->begin(); iValue != fpValues->end(); ++iValue) {
    const G4String& valueName = iValue->GetName();
    const G4String& value = iValue->GetValue();
    map<G4String,G4AttDef>::const_iterator iDef =
      fpDefinitions->find(valueName);
    if (iDef == fpDefinitions->end()) {
      ++iError;
      error = true;
      if (print) {
	G4cerr <<
	  "\n*******************************************************";
	if (leader != "") {
	  G4cerr << '\n' << leader;
	}
	G4cerr <<
	  "\nERROR " << iError << ": No G4AttDef for G4AttValue \""
	       <<  valueName << "\": " << value <<
	  "\n*******************************************************"
	       << G4endl;
      }
    } else {
      const G4String& category = iDef->second.GetCategory();
      const G4String& extra = iDef->second.GetExtra();
      const G4String& valueType = iDef->second.GetValueType();
      if (fCategories.find(category) == fCategories.end()) {
	++iError;
	error = true;
	if (print) {
	  G4cerr <<
	    "\n*******************************************************";
	  if (leader != "") {
	    G4cerr << '\n' << leader;
	  }
	  G4cerr <<
	    "\nERROR " << iError << ": Illegal Category Field \""
		 << category << "\" for G4AttValue \""
		 << valueName << "\": " << value <<
	    "\n  Possible Categories:";
	  set<G4String>::iterator i;
	  for (i = fCategories.begin(); i != fCategories.end(); ++i) {
	    G4cerr << ' ' << *i;
	  }
	  G4cerr <<
	    "\n*******************************************************"
		 << G4endl;
	}
      }
      if(category == "Physics" && fUnits.find(extra) == fUnits.end()) {
	++iError;
	error = true;
	if (print) {
	  G4cerr <<
	    "\n*******************************************************";
	  if (leader != "") {
	    G4cerr << '\n' << leader;
	  }
	  G4cerr <<
	    "\nERROR " << iError << ": Illegal Extra field \""
		 << extra << "\" for G4AttValue \""
		 << valueName << "\": " << value <<
	    "\n  Possible Extra fields if Category==\"Physics\":\n    ";
	  set<G4String>::iterator i;
	  for (i = fUnits.begin(); i != fUnits.end(); ++i) {
	    G4cerr << ' ' << *i;
	  }
	  G4cerr <<
	    "\n*******************************************************"
		 << G4endl;
	}
      }
      if (fValueTypes.find(valueType) == fValueTypes.end()) {
	++iError;
	error = true;
	if (print) {
	  G4cerr <<
	    "\n*******************************************************";
	  if (leader != "") {
	    G4cerr << '\n' << leader;
	  }
	  G4cerr <<
	    "\nERROR " << iError << ": Illegal Value Type field \""
		 << valueType << "\" for G4AttValue \""
		 << valueName << "\": " << value <<
	    "\n  Possible Value Types:";
	  set<G4String>::iterator i;
	  for (i = fValueTypes.begin(); i != fValueTypes.end(); ++i) {
	    G4cerr << ' ' << *i;
	  }
	  G4cerr <<
	    "\n*******************************************************"
	       << G4endl;
	}
      }
    }
  }
  return error;
}

std::ostream& operator<< (std::ostream& os, const G4AttCheck& ac) {
  using namespace std;
  vector<G4AttValue>::const_iterator iValue;
  for (iValue = ac.fpValues->begin(); iValue != ac.fpValues->end(); ++iValue) {
    const G4String& valueName = iValue->GetName();
    const G4String& value = iValue->GetValue();
    map<G4String,G4AttDef>::const_iterator iDef =
      ac.fpDefinitions->find(valueName);
    G4bool error = false;
    if (iDef == ac.fpDefinitions->end()) {
      error = true;
      os << "ERROR: No G4AttDef for G4AttValue \""
	 << valueName << "\": " << value << endl;
    } else {
      const G4String& category = iDef->second.GetCategory();
      const G4String& extra = iDef->second.GetExtra();
      const G4String& valueType = iDef->second.GetValueType();
      if (ac.fCategories.find(category) == ac.fCategories.end()) {
	error = true;
	os <<
	  "ERROR: Illegal Category Field \"" << category
	   << "\" for G4AttValue \"" << valueName << "\": " << value <<
	  "\n  Possible Categories:";
	set<G4String>::iterator i;
	for (i = ac.fCategories.begin(); i != ac.fCategories.end(); ++i) {
	  os << ' ' << *i;
	}
	os << endl;
      }
      if(category == "Physics" && ac.fUnits.find(extra) == ac.fUnits.end()) {
	error = true;
	os <<
	  "ERROR: Illegal Extra field \""<< extra << "\" for G4AttValue \""
	   << valueName << "\": " << value <<
	  "\n  Possible Extra fields if Category==\"Physics\":\n    ";
	set<G4String>::iterator i;
	for (i = ac.fUnits.begin(); i != ac.fUnits.end(); ++i) {
	  os << ' ' << *i;
	}
	os << endl;
      }
      if (ac.fValueTypes.find(valueType) == ac.fValueTypes.end()) {
	error = true;
	os <<
	  "ERROR: Illegal Value Type field \"" << valueType
	   << "\" for G4AttValue \"" << valueName << "\": " << value <<
	  "\n  Possible Value Types:";
	set<G4String>::iterator i;
	for (i = ac.fValueTypes.begin(); i != ac.fValueTypes.end(); ++i) {
	  os << ' ' << *i;
	}
	os << endl;
      }
    }
    if (!error) {
      os << iDef->second.GetDesc() << ": "
	 << iValue->GetValue();
      if (iDef->second.GetCategory() == "Physics") {
	os << " ("
	   << iDef->second.GetExtra() << ")";
      }
      os << endl;
    }
  }
  return os;
}

void G4AttCheck::AddValuesAndDefs
(std::vector<G4AttValue>* standardValues,
 std::map<G4String,G4AttDef>* standardDefinitions,
 const G4String& oldName,
 const G4String& name,
 const G4String& value,
 const G4String& extra,
 const G4String& description) const {
  // Add new G4AttDeff...
  standardValues->push_back(G4AttValue(name,value,""));
  // Copy original G4AttDef...
  (*standardDefinitions)[name] = fpDefinitions->find(oldName)->second;
  // ...and make appropriate changes...
  (*standardDefinitions)[name].SetName(name);
  (*standardDefinitions)[name].SetExtra(extra);
  if (description != "") (*standardDefinitions)[name].SetDesc(description);
}

G4bool G4AttCheck::Standard
(std::vector<G4AttValue>* standardValues,
 std::map<G4String,G4AttDef>* standardDefinitions) const {
  // Places standard versions in provided vector and map and returns error.
  using namespace std;
  G4bool error = false;
  vector<G4AttValue>::const_iterator iValue;
  for (iValue = fpValues->begin(); iValue != fpValues->end(); ++iValue) {
    const G4String& valueName = iValue->GetName();
    const G4String& value = iValue->GetValue();
    map<G4String,G4AttDef>::const_iterator iDef =
      fpDefinitions->find(valueName);
    if (iDef == fpDefinitions->end()) {
      error = true;
    } else {
      const G4String& category = iDef->second.GetCategory();
      const G4String& extra = iDef->second.GetExtra();
      const G4String& valueType = iDef->second.GetValueType();
      if (fCategories.find(category) == fCategories.end() ||
	  (category == "Physics" && fUnits.find(extra) == fUnits.end()) ||
	  fValueTypes.find(valueType) == fValueTypes.end()) {
	error = true;
      } else {
	if (category != "Physics") {  // Simply copy...
	  standardValues->push_back(*iValue);
	  (*standardDefinitions)[valueName] =
	    fpDefinitions->find(valueName)->second;
	} else {
	  G4String valueAndUnit;
	  G4String unit;
	  if (extra == "G4BestUnit") {
	    valueAndUnit = value;
	    valueAndUnit = valueAndUnit.strip();
	    unit = valueAndUnit.substr(valueAndUnit.rfind(' ')+1);
	  } else {
	    valueAndUnit = value + ' ' + extra;
	    unit = extra;
	  }
	  G4String unitCategory = G4UnitDefinition::GetCategory(unit);
	  if (fUnitCategories.find(unitCategory) != fUnitCategories.end()) {
	    G4String standardUnit = fStandardUnits[unitCategory];
	    G4double valueOfUnit = G4UnitDefinition::GetValueOf(standardUnit);
	    G4String extra = iDef->second.GetExtra();
	    if (valueType == "G4ThreeVector") {
	      G4ThreeVector internalValue =
		G4UIcommand::ConvertToDimensioned3Vector(valueAndUnit);
	      AddValuesAndDefs
		(standardValues,standardDefinitions,
		 valueName,valueName+"-X",
		 G4UIcommand::ConvertToString(internalValue.x()/valueOfUnit),
		 standardUnit,
		 fpDefinitions->find(valueName)->second.GetDesc()+"-X");
	      AddValuesAndDefs
		(standardValues,standardDefinitions,
		 valueName,valueName+"-Y",
		 G4UIcommand::ConvertToString(internalValue.y()/valueOfUnit),
		 standardUnit,
		 fpDefinitions->find(valueName)->second.GetDesc()+"-Y");
	      AddValuesAndDefs
		(standardValues,standardDefinitions,
		 valueName,valueName+"-Z",
		 G4UIcommand::ConvertToString(internalValue.z()/valueOfUnit),
		 standardUnit,
		 fpDefinitions->find(valueName)->second.GetDesc()+"-Z");
	    } else {
	      G4double internalValue =
		G4UIcommand::ConvertToDimensionedDouble(valueAndUnit);
	      AddValuesAndDefs
		(standardValues,standardDefinitions,
		 valueName,valueName,
		 G4UIcommand::ConvertToString(internalValue/valueOfUnit),
		 standardUnit);
	    }
	  }
	}
      }
    }
  }
  return error;
}
