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
// $Id: G4UnitsTable.cc,v 1.18 2003-06-06 16:17:17 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... 
//
// 17-05-98: first version, M.Maire
// 05-08-98: angstrom,microbarn,picobarn,petaelectronvolt, M.Maire
// 13-10-98: units and symbols printed in fixed length, M.Maire
// 01-03-01: parsec, M.Maire
// 06-03-01: migration to STL vectors, G.Cosmo
// 06-05-02: BestUnit operator<<  flux instead of G4cout (mma)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
 
#include "G4UnitsTable.hh"

#include <iomanip>

G4UnitsTable      G4UnitDefinition::theUnitsTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitDefinition::G4UnitDefinition(G4String name, G4String symbol,
                                   G4String category, G4double value)
  : Name(name),SymbolName(symbol),Value(value)				   
{
    //
    //does the Category objet already exist ?
    size_t nbCat = theUnitsTable.size();
    size_t i = 0;
    while ((i<nbCat)&&(theUnitsTable[i]->GetName()!=category)) i++;
    if (i == nbCat) theUnitsTable.push_back( new G4UnitsCategory(category));
    CategoryIndex = i;
    //
    //insert this Unit in the Unitstable
    (theUnitsTable[CategoryIndex]->GetUnitsList()).push_back(this);
    
    //update string max length for name and symbol
    theUnitsTable[i]->UpdateNameMxLen((G4int)name.length());
    theUnitsTable[i]->UpdateSymbMxLen((G4int)symbol.length());
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitDefinition::~G4UnitDefinition()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitDefinition::G4UnitDefinition(G4UnitDefinition& right)
{
    *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitDefinition& G4UnitDefinition::operator=(const G4UnitDefinition& right)
{
  if (this != &right)
    {
      Name          = right.Name;
      SymbolName    = right.SymbolName;
      Value         = right.Value;
      CategoryIndex = right.CategoryIndex;
    }
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4int G4UnitDefinition::operator==(const G4UnitDefinition& right) const
{
  return (this == (G4UnitDefinition *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4int G4UnitDefinition::operator!=(const G4UnitDefinition &right) const
{
  return (this != (G4UnitDefinition *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitsTable& G4UnitDefinition::GetUnitsTable()
{
  return theUnitsTable;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4double G4UnitDefinition::GetValueOf(G4String string)
{
  if(theUnitsTable.size()==0) BuildUnitsTable();
  G4String name,symbol;
  for (size_t i=0;i<theUnitsTable.size();i++)
     { G4UnitsContainer& units = theUnitsTable[i]->GetUnitsList();
       for (size_t j=0;j<units.size();j++)
          { name=units[j]->GetName(); symbol=units[j]->GetSymbol();
            if(string==name||string==symbol) 
               return units[j]->GetValue();
          }
     }
  G4cout << "Warning from G4UnitDefinition::GetValueOf(" << string << ")."
       << " The unit " << string << " does not exist in UnitsTable."
       << " Return Value = 0." << G4endl;     
  return 0.;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
G4String G4UnitDefinition::GetCategory(G4String string)
{
  if(theUnitsTable.size()==0) BuildUnitsTable();
  G4String name,symbol;
  for (size_t i=0;i<theUnitsTable.size();i++)
     { G4UnitsContainer& units = theUnitsTable[i]->GetUnitsList();
       for (size_t j=0;j<units.size();j++)
          { name=units[j]->GetName(); symbol=units[j]->GetSymbol();
            if(string==name||string==symbol) 
               return theUnitsTable[i]->GetName();
          }
     }
  G4cout << "Warning from G4UnitDefinition::GetCategory(" << string << ")."
       << " The unit " << string << " does not exist in UnitsTable."
       << " Return category = None" << G4endl;
  name = "None";     
  return name;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
void G4UnitDefinition::PrintDefinition()
{
  G4int nameL = theUnitsTable[CategoryIndex]->GetNameMxLen();
  G4int symbL = theUnitsTable[CategoryIndex]->GetSymbMxLen();
  G4cout << std::setw(nameL) << Name << " (" 
         << std::setw(symbL) << SymbolName << ") = " << Value << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
void G4UnitDefinition::BuildUnitsTable()
{
 //Length
 new G4UnitDefinition(    "parsec","pc"      ,"Length",parsec); 
 new G4UnitDefinition( "kilometer","km"      ,"Length",kilometer);
 new G4UnitDefinition(     "meter","m"       ,"Length",meter);
 new G4UnitDefinition("centimeter","cm"      ,"Length",centimeter); 
 new G4UnitDefinition("millimeter","mm"      ,"Length",millimeter);
 new G4UnitDefinition("micrometer","mum"     ,"Length",micrometer);
 new G4UnitDefinition( "nanometer","nm"      ,"Length",nanometer);
 new G4UnitDefinition(  "angstrom","Ang"     ,"Length",angstrom);    
 new G4UnitDefinition(     "fermi","fm"      ,"Length",fermi);
 
 //Surface
 new G4UnitDefinition( "kilometer2","km2"    ,"Surface",kilometer2);
 new G4UnitDefinition(     "meter2","m2"     ,"Surface",meter2);
 new G4UnitDefinition("centimeter2","cm2"    ,"Surface",centimeter2); 
 new G4UnitDefinition("millimeter2","mm2"    ,"Surface",millimeter2);
 new G4UnitDefinition(       "barn","barn"   ,"Surface",barn);
 new G4UnitDefinition(  "millibarn","mbarn"  ,"Surface",millibarn);   
 new G4UnitDefinition(  "microbarn","mubarn" ,"Surface",microbarn);
 new G4UnitDefinition(   "nanobarn","nbarn"  ,"Surface",nanobarn);
 new G4UnitDefinition(   "picobarn","pbarn"  ,"Surface",picobarn);
 
 //Volume
 new G4UnitDefinition( "kilometer3","km3"    ,"Volume",kilometer3);
 new G4UnitDefinition(     "meter3","m3"     ,"Volume",meter3);
 new G4UnitDefinition("centimeter3","cm3"    ,"Volume",centimeter3); 
 new G4UnitDefinition("millimeter3","mm3"    ,"Volume",millimeter3);

 //Angle
 new G4UnitDefinition(     "radian","rad"    ,"Angle",radian);
 new G4UnitDefinition("milliradian","mrad"   ,"Angle",milliradian); 
 new G4UnitDefinition(  "steradian","sr"     ,"Angle",steradian);
 new G4UnitDefinition(     "degree","deg"    ,"Angle",degree);
 
 //Time
 new G4UnitDefinition(     "second","s"      ,"Time",second);
 new G4UnitDefinition("millisecond","ms"     ,"Time",millisecond);
 new G4UnitDefinition("microsecond","mus"    ,"Time",microsecond);
 new G4UnitDefinition( "nanosecond","ns"     ,"Time",nanosecond);
 new G4UnitDefinition( "picosecond","ps"     ,"Time",picosecond);
 
 //Frequency
 new G4UnitDefinition(    "hertz","Hz"       ,"Frequency",hertz);
 new G4UnitDefinition("kilohertz","kHz"      ,"Frequency",kilohertz);
 new G4UnitDefinition("megahertz","MHz"      ,"Frequency",megahertz);
 
 //Electric charge
 new G4UnitDefinition(  "eplus","e+"         ,"Electric charge",eplus);
 new G4UnitDefinition("coulomb","C"          ,"Electric charge",coulomb); 
 
 //Energy
 new G4UnitDefinition(    "electronvolt","eV" ,"Energy",electronvolt);
 new G4UnitDefinition("kiloelectronvolt","keV","Energy",kiloelectronvolt);
 new G4UnitDefinition("megaelectronvolt","MeV","Energy",megaelectronvolt);
 new G4UnitDefinition("gigaelectronvolt","GeV","Energy",gigaelectronvolt);
 new G4UnitDefinition("teraelectronvolt","TeV","Energy",teraelectronvolt);
 new G4UnitDefinition("petaelectronvolt","PeV","Energy",petaelectronvolt);
 new G4UnitDefinition(           "joule","J"  ,"Energy",joule);
 
 //Mass
 new G4UnitDefinition("milligram","mg","Mass",milligram);
 new G4UnitDefinition(     "gram","g" ,"Mass",gram);
 new G4UnitDefinition( "kilogram","kg","Mass",kilogram);
 
 //Volumic Mass
 new G4UnitDefinition( "g/cm3", "g/cm3","Volumic Mass", g/cm3);
 new G4UnitDefinition("mg/cm3","mg/cm3","Volumic Mass",mg/cm3);
 new G4UnitDefinition("kg/m3", "kg/m3", "Volumic Mass",kg/m3);
 
 //Power
 new G4UnitDefinition("watt","W","Power",watt);
 
 //Force
 new G4UnitDefinition("newton","N","Force",newton);
 
 //Pressure
 new G4UnitDefinition(    "pascal","Pa" ,"Pressure",pascal);
 new G4UnitDefinition(       "bar","bar","Pressure",bar); 
 new G4UnitDefinition("atmosphere","atm","Pressure",atmosphere);
 
 //Electric current
 new G4UnitDefinition(     "ampere","A"  ,"Electric current",ampere);
 new G4UnitDefinition("milliampere","mA" ,"Electric current",milliampere);
 new G4UnitDefinition("microampere","muA","Electric current",microampere);
 new G4UnitDefinition( "nanoampere","nA" ,"Electric current",nanoampere);   
 
 //Electric potential
 new G4UnitDefinition(    "volt","V" ,"Electric potential",volt); 
 new G4UnitDefinition("kilovolt","kV","Electric potential",kilovolt);
 new G4UnitDefinition("megavolt","MV","Electric potential",megavolt);
 
 //Magnetic flux
 new G4UnitDefinition("weber","Wb","Magnetic flux",weber);
 
 //Magnetic flux density
 new G4UnitDefinition(    "tesla","T" ,"Magnetic flux density",tesla);
 new G4UnitDefinition("kilogauss","kG","Magnetic flux density",kilogauss);
 new G4UnitDefinition(    "gauss","G" ,"Magnetic flux density",gauss);
 
 //Temperature
 new G4UnitDefinition("kelvin","K","Temperature",kelvin);
 
 //Amount of substance
 new G4UnitDefinition("mole","mol","Amount of substance",mole);
 
 //Activity
 new G4UnitDefinition("becquerel","Bq","Activity",becquerel);
 new G4UnitDefinition(    "curie","Ci","Activity",curie);
 
 //Dose
 new G4UnitDefinition("gray","Gy","Dose",gray);                          
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
void G4UnitDefinition::PrintUnitsTable()
{
  G4cout << "\n          ----- The Table of Units ----- \n";
  for(size_t i=0;i<theUnitsTable.size();i++)
      theUnitsTable[i]->PrintCategory();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
   
G4UnitsCategory::G4UnitsCategory(G4String name)
:Name(name),NameMxLen(0),SymbMxLen(0)
{
    UnitsList = *(new G4UnitsContainer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitsCategory::~G4UnitsCategory()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitsCategory::G4UnitsCategory(G4UnitsCategory& right)
{
    *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4UnitsCategory& G4UnitsCategory::operator=(const G4UnitsCategory& right)
{
  if (this != &right)
    {
      Name      = right.Name;
      UnitsList = right.UnitsList;
      NameMxLen = right.NameMxLen;
      SymbMxLen = right.SymbMxLen;
    }
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4int G4UnitsCategory::operator==(const G4UnitsCategory& right) const
{
  return (this == (G4UnitsCategory *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4int G4UnitsCategory::operator!=(const G4UnitsCategory &right) const
{
  return (this != (G4UnitsCategory *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
void G4UnitsCategory::PrintCategory()
{
  G4cout << "\n  category: " << Name << G4endl;
  for(size_t i=0;i<UnitsList.size();i++)
      UnitsList[i]->PrintDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
       
G4BestUnit::G4BestUnit(G4double value,G4String category)
{
 // find the category
    G4UnitsTable& theUnitsTable = G4UnitDefinition::GetUnitsTable();
    size_t nbCat = theUnitsTable.size();
    size_t i = 0;
    while
     ((i<nbCat)&&(theUnitsTable[i]->GetName()!=category)) i++;
    if (i == nbCat) 
       { G4cout << " G4BestUnit: the category " << category 
                << " does not exist !!" << G4endl;
         G4Exception("Missing unit category !");
       }  
  //
    IndexOfCategory = i;
    nbOfVals = 1;
    Value[0] = value; Value[1] = 0.; Value[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
       
G4BestUnit::G4BestUnit(const G4ThreeVector& value,G4String category)
{
 // find the category
    G4UnitsTable& theUnitsTable = G4UnitDefinition::GetUnitsTable();
    size_t nbCat = theUnitsTable.size();
    size_t i = 0;
    while
     ((i<nbCat)&&(theUnitsTable[i]->GetName()!=category)) i++;
    if (i == nbCat) 
       { G4cerr << " G4BestUnit: the category " << category 
                << " does not exist." << G4endl;
         G4Exception("Unit category not existing !");
       }  
  //
    IndexOfCategory = i;
    nbOfVals = 3;
    Value[0] = value.x();
    Value[1] = value.y();
    Value[2] = value.z();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4BestUnit::~G4BestUnit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
std::ostream& operator<<(std::ostream& flux, G4BestUnit a)
{
  G4UnitsTable& theUnitsTable = G4UnitDefinition::GetUnitsTable();
  G4UnitsContainer& List = theUnitsTable[a.IndexOfCategory]
                           ->GetUnitsList();
  G4int len = theUnitsTable[a.IndexOfCategory]->GetSymbMxLen();
                           
  G4int    ksup(-1), kinf(-1);
  G4double umax(0.), umin(DBL_MAX);
  G4double rsup(DBL_MAX), rinf(0.);

  //for a ThreeVector, choose the best unit for the biggest value 
  G4double value = std::max(std::max(fabs(a.Value[0]),fabs(a.Value[1])),
                              fabs(a.Value[2]));

  for (size_t k=0; k<List.size(); k++)
     {
       G4double unit = List[k]->GetValue();
            if (value==DBL_MAX) {if(unit>umax) {umax=unit; ksup=k;}}
       else if (value<=DBL_MIN) {if(unit<umin) {umin=unit; kinf=k;}}
       
       else { G4double ratio = value/unit;
              if ((ratio>=1.)&&(ratio<rsup)) {rsup=ratio; ksup=k;}
              if ((ratio< 1.)&&(ratio>rinf)) {rinf=ratio; kinf=k;}
	    } 
     }
	 
  G4int index=ksup; if(index==-1) index=kinf; if(index==-1) index=0;
  
  for (G4int j=0; j<a.nbOfVals; j++) 
     {flux << a.Value[j]/(List[index]->GetValue()) << " ";}

  std::ios::fmtflags oldform = flux.flags();

  flux.setf(std::ios::left,std::ios::adjustfield);
  flux << std::setw(len) << List[index]->GetSymbol();       
  flux.flags(oldform);

  return flux;
}       

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
        
