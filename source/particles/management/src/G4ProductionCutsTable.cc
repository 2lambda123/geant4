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
// $Id: G4ProductionCutsTable.cc,v 1.15 2003-05-19 17:10:26 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// --------------------------------------------------------------
//      GEANT 4 class implementation file/  History:
//    06/Oct. 2002, M.Asai : First implementation
// --------------------------------------------------------------

#include "G4ProductionCutsTable.hh"
#include "G4ProductionCuts.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForPositron.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"

#include "G4ios.hh"
#include "g4std/iomanip"                
#include "g4std/fstream"       



G4ProductionCutsTable* G4ProductionCutsTable::fG4ProductionCutsTable = 0;

G4ProductionCutsTable* G4ProductionCutsTable::GetProductionCutsTable()
{ 
  if(!fG4ProductionCutsTable)
  { fG4ProductionCutsTable = new G4ProductionCutsTable(); }
  return fG4ProductionCutsTable;
}

G4ProductionCutsTable::G4ProductionCutsTable()
: firstUse(true)
{
  for(size_t i=0;i< NumberOfG4CutIndex;i++)
  {
    rangeCutTable.push_back(new G4CutVectorForAParticle);
    energyCutTable.push_back(new G4CutVectorForAParticle);
    rangeDoubleVector[i] = 0;
    energyDoubleVector[i] = 0;
    converters[i] = 0;
  }
  fG4RegionStore = G4RegionStore::GetInstance();
  defaultProductionCuts = new G4ProductionCuts();
}

G4ProductionCutsTable::G4ProductionCutsTable(const G4ProductionCutsTable& )
{;}

G4ProductionCutsTable::~G4ProductionCutsTable()
{
  for(CoupleTableIterator itr=coupleTable.begin();itr!=coupleTable.end();itr++)
  { delete (*itr); }
  coupleTable.clear();
  for(size_t i=0;i< NumberOfG4CutIndex;i++)
  {
    delete rangeCutTable[i];
    delete energyCutTable[i];
    delete converters[i];
    if(rangeDoubleVector[i]!=0) delete [] rangeDoubleVector[i];
    if(energyDoubleVector[i]!=0) delete [] energyDoubleVector[i];
  }
}

void G4ProductionCutsTable::UpdateCoupleTable()
{
  if(firstUse)
  {
    if(G4ParticleTable::GetParticleTable()->FindParticle("gamma"))
    { converters[0] = new G4RToEConvForGamma(); }
    if(G4ParticleTable::GetParticleTable()->FindParticle("e-"))
    { converters[1] = new G4RToEConvForElectron(); }
    if(G4ParticleTable::GetParticleTable()->FindParticle("e+"))
    { converters[2] = new G4RToEConvForPositron(); }
    firstUse = false;
  }

  // Reset "used" flags of all couples
  for(CoupleTableIterator CoupleItr=coupleTable.begin();CoupleItr!=coupleTable.end();CoupleItr++)
  { (*CoupleItr)->SetUseFlag(false); }

  // Update Material-Cut-Couple
  typedef G4std::vector<G4Region*>::iterator regionIterator;
  for(regionIterator rItr=fG4RegionStore->begin();rItr!=fG4RegionStore->end();rItr++)
  { 
    ///////////////////if(!((*rItr)->IsModified())) continue;
    G4ProductionCuts* fProductionCut = (*rItr)->GetProductionCuts();
    G4std::vector<G4Material*>::const_iterator mItr = (*rItr)->GetMaterialIterator();
    size_t nMaterial = (*rItr)->GetNumberOfMaterials();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The following part of the code should be removed once all EM processes
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //G4Region* theWorldRegion = *(fG4RegionStore->begin());
    //if((*rItr)==theWorldRegion)
    //{
    //  mItr = G4Material::GetMaterialTable()->begin();
    //  nMaterial = G4Material::GetMaterialTable()->size();
    //}
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The previous part of the code should be removed once all EM processes 
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    for(size_t iMate=0;iMate<nMaterial;iMate++)
    {
      //check if this material cut couple has already been made
      G4bool coupleAlreadyDefined = false;
      G4MaterialCutsCouple* aCouple;
      for(CoupleTableIterator cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++)
      {
        if((*cItr)->GetMaterial()==(*mItr) && (*cItr)->GetProductionCuts()==fProductionCut)
        { 
          coupleAlreadyDefined = true;
          aCouple = *cItr;
          break;
        }
      }
      
      //if this combination is new, cleate and register a couple
      if(!coupleAlreadyDefined)
      {
        aCouple = new G4MaterialCutsCouple((*mItr),fProductionCut);
        coupleTable.push_back(aCouple);
        aCouple->SetIndex(coupleTable.size()-1);
      }

      //Set the couple to the proper logical volumes in that region
      aCouple->SetUseFlag();
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The following part of the code should be removed once all EM processes 
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //  if((*rItr)==theWorldRegion)
    //  {
    //    aCouple->SetUseFlag(false);
    //    G4std::vector<G4Material*>::const_iterator mItr1 = (*rItr)->GetMaterialIterator();
    //    size_t nMaterial1 = (*rItr)->GetNumberOfMaterials();
    //    for(size_t iMate1=0;iMate1<nMaterial1;iMate1++)
    //    {
    //      if((*mItr1)==aCouple->GetMaterial()) aCouple->SetUseFlag();
    //      mItr1++;
    //    }
    //  }
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The previous part of the code should be removed once all EM processes 
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      G4std::vector<G4LogicalVolume*>::iterator rootLVItr
                         = (*rItr)->GetRootLogicalVolumeIterator();
      size_t nRootLV = (*rItr)->GetNumberOfRootVolumes();
      for(size_t iLV=0;iLV<nRootLV;iLV++)
      {
        //Set the couple to the proper logical volumes in that region
        G4LogicalVolume* aLV = *rootLVItr;
        G4Region* aR = *rItr;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The following part of the code should be removed once all EM processes 
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //    if(aR==theWorldRegion) aR = 0;
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // The previous part of the code should be removed once all EM processes 
    // become "Region-aware"
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        ScanAndSetCouple(aLV,aCouple,aR);

        //proceed to the next root logical volume in this region
        rootLVItr++;
      }

      //proceed to next material in this region
      mItr++;
    }
  }

  // Check if sizes of Range/Energy cuts tables are equal to the size of
  // the couple table
  // If new couples are made during the previous procedure, nCouple becomes
  // larger then nTable
  size_t nCouple = coupleTable.size();
  size_t nTable = energyCutTable[0]->size();
  G4bool newCoupleAppears = nCouple>nTable;
  if(newCoupleAppears)
  {
    for(size_t n=nCouple-nTable;n>0;n--)
    {
      for(size_t nn=0;nn< NumberOfG4CutIndex;nn++)
      {
        rangeCutTable[nn]->push_back(-1.);
        energyCutTable[nn]->push_back(-1.);
      }
    }
  }

  // Update RangeEnergy cuts tables
  size_t idx = 0;
  for(CoupleTableIterator cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++)
  {
    G4ProductionCuts* aCut = (*cItr)->GetProductionCuts();
    const G4Material* aMat = (*cItr)->GetMaterial();
    if((*cItr)->IsRecalcNeeded())
    {
      for(size_t ptcl=0;ptcl< NumberOfG4CutIndex;ptcl++)
      {
        G4double rCut = aCut->GetProductionCut(ptcl);
        (*(rangeCutTable[ptcl]))[idx] = rCut;
        // if(converters[ptcl] && (*cItr)->IsUsed())
        if(converters[ptcl])
        { (*(energyCutTable[ptcl]))[idx] = converters[ptcl]->Convert(rCut,aMat); }
        else
        { (*(energyCutTable[ptcl]))[idx] = -1.; }
      }
    }
    idx++;  
  }

  // resize Range/Energy cuts double vectors if new couple is made
  if(newCoupleAppears)
  {
    for(size_t ix=0;ix<NumberOfG4CutIndex;ix++)
    {
      G4double* rangeVOld = rangeDoubleVector[ix];
      G4double* energyVOld = energyDoubleVector[ix];
      if(rangeVOld) delete [] rangeVOld;
      if(energyVOld) delete [] energyVOld;
      rangeDoubleVector[ix] = new G4double[(*(rangeCutTable[ix])).size()];
      energyDoubleVector[ix] = new G4double[(*(energyCutTable[ix])).size()];
    }
  }

  // Update Range/Energy cuts double vectors
  for(size_t ix=0;ix<NumberOfG4CutIndex;ix++)
  {
    for(size_t ixx=0;ixx<(*(rangeCutTable[ix])).size();ixx++)
    {
      rangeDoubleVector[ix][ixx] = (*(rangeCutTable[ix]))[ixx];
      energyDoubleVector[ix][ixx] = (*(energyCutTable[ix]))[ixx];
    }
  }
}

void G4ProductionCutsTable::SetEnergyRange(G4double lowedge, G4double highedge)
{
  G4VRangeToEnergyConverter::SetEnergyRange(lowedge,highedge);
}

G4double  G4ProductionCutsTable::GetLowEdgeEnergy() const
{
  return G4VRangeToEnergyConverter::GetLowEdgeEnergy();
}

G4double G4ProductionCutsTable::GetHighEdgeEnergy() const
{
  return G4VRangeToEnergyConverter::GetHighEdgeEnergy();
}
 

void G4ProductionCutsTable::ScanAndSetCouple(G4LogicalVolume* aLV,G4MaterialCutsCouple* aCouple,G4Region* aRegion)
{
  //Check whether or not this logical volume belongs to the same region
  if((aRegion!=0) && aLV->GetRegion()!=aRegion) return;

  //Check if this particular volume has a material matched to the couple
  if(aLV->GetMaterial()==aCouple->GetMaterial())
  {
    aLV->SetMaterialCutsCouple(aCouple);
  }

  size_t noDaughters = aLV->GetNoDaughters();
  if(noDaughters==0) return;

  //Loop over daughters with same region
  for(size_t i=0;i<noDaughters;i++)
  {
    G4LogicalVolume* daughterLVol = aLV->GetDaughter(i)->GetLogicalVolume();
    ScanAndSetCouple(daughterLVol,aCouple,aRegion);
  }
}

void G4ProductionCutsTable::DumpCouples() const
{
  G4cout << G4endl;
  G4cout << "========= Table of registered couples ==============================" << G4endl;
  for(CoupleTableIterator cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++)
  {
    G4MaterialCutsCouple* aCouple = (*cItr);
    G4ProductionCuts* aCut = aCouple->GetProductionCuts();
    G4cout << G4endl;
    G4cout << "Index : " << aCouple->GetIndex() 
           << "     used in the geometry : ";
    if(aCouple->IsUsed()) G4cout << "Yes";
    else                  G4cout << "No ";
    G4cout << "     recalculation needed : ";
    if(aCouple->IsRecalcNeeded()) G4cout << "Yes";
    else                          G4cout << "No ";
    G4cout << G4endl;
    G4cout << " Material : " << aCouple->GetMaterial()->GetName() << G4endl;
    G4cout << " Range cuts        : " 
           << " gamma " << G4BestUnit(aCut->GetProductionCut("gamma"),"Length")
           << "    e- " << G4BestUnit(aCut->GetProductionCut("e-"),"Length")
           << "    e+ " << G4BestUnit(aCut->GetProductionCut("e+"),"Length")
           << G4endl;
    G4cout << " Energy thresholds : " ;
    // if(!(aCouple->IsUsed()) || aCouple->IsRecalcNeeded()) G4cout << " is not ready to print";
    if(aCouple->IsRecalcNeeded()) G4cout << " is not ready to print";
    else
    G4cout << " gamma " << G4BestUnit((*(energyCutTable[0]))[aCouple->GetIndex()],"Energy")
           << "    e- " << G4BestUnit((*(energyCutTable[1]))[aCouple->GetIndex()],"Energy")
           << "    e+ " << G4BestUnit((*(energyCutTable[2]))[aCouple->GetIndex()],"Energy");
    G4cout << G4endl;

    if(aCouple->IsUsed())
    {
      G4cout << " Region(s) which use this couple : " << G4endl;
      typedef G4std::vector<G4Region*>::iterator regionIterator;
      for(regionIterator rItr=fG4RegionStore->begin();rItr!=fG4RegionStore->end();rItr++)
      {
	if (IsCoupleUsedInTheRegion(aCouple, *rItr) ){
	  G4cout << "    " << (*rItr)->GetName() << G4endl;
	}
      }
    }
  }
  G4cout << G4endl;
  G4cout << "====================================================================" << G4endl;
  G4cout << G4endl;
}
           

// Store cuts and material information in files under the specified directory.
G4bool  G4ProductionCutsTable::StoreCutsTable(const G4String& dir, 
					      G4bool          ascii)
{
  if (!StoreMaterialInfo(dir, ascii)) return false;
  if (!StoreMaterialCutsCoupleInfo(dir, ascii)) return false;
  if (!StoreCutsInfo(dir, ascii)) return false;

  return true;
}
  
G4bool  G4ProductionCutsTable::RetrieveCutsTable(const G4String& dir,
						 G4bool          ascii)
{
  if (!CheckForRetrieveCutsTable(dir, ascii)) return false;
  if (!RetrieveCutsInfo(dir, ascii)) return false;

  return true;
}

// check stored material and cut values are consistent with the current detector setup. 
G4bool G4ProductionCutsTable::CheckForRetrieveCutsTable(const G4String& directory, 
							G4bool          ascii)
{
  if (!CheckMaterialInfo(directory, ascii)) return false;
  if (!CheckMaterialCutsCoupleInfo(directory, ascii)) return false;
  return true;
}
  
// Store material information in files under the specified directory.
G4bool  G4ProductionCutsTable::StoreMaterialInfo(const G4String& directory, 
						 G4bool          ascii)
{
  const G4String fileName = directory + "/" + "material.dat";
  const G4String key = "MATERIAL-V2.0";
  G4std::ofstream fOut;  

  // open output file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii ) 
    fOut.open(fileName,G4std::ios::out|G4std::ios::binary);
  else 
#endif
    fOut.open(fileName,G4std::ios::out);

  
  // check if the file has been opened successfully 
  if (!fOut) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutsTable::StoreMaterialInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  const G4MaterialTable* matTable = G4Material::GetMaterialTable(); 
  // number of materials in the table
  G4int numberOfMaterial = matTable->size();

 if (ascii) {
    /////////////// ASCII mode  /////////////////
    // key word
    fOut  << key << G4endl;
    
    // number of materials in the table
    fOut  << numberOfMaterial << G4endl;
    
    fOut.setf(G4std::ios::scientific);
  
    // material name and density
    for (size_t idx=0; G4int(idx)<numberOfMaterial; ++idx){
      fOut << G4std::setw(FixedStringLengthForStore) << ((*matTable)[idx])->GetName();
      fOut << G4std::setw(FixedStringLengthForStore) << ((*matTable)[idx])->GetDensity()/(g/cm3) << G4endl;
    }
    
    fOut.unsetf(G4std::ios::scientific);

  } else {
    /////////////// Binary mode  /////////////////
    char temp[FixedStringLengthForStore];
    size_t i;

    // key word
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=key[i];
    fOut.write(temp, FixedStringLengthForStore);

    // number of materials in the table
    fOut.write( (char*)(&numberOfMaterial), sizeof (G4int));
    
    // material name and density
    for (size_t imat=0; G4int(imat)<numberOfMaterial; ++imat){
      G4String name =  ((*matTable)[imat])->GetName();
      G4double density = ((*matTable)[imat])->GetDensity();
      for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
      for (i=0; i<name.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=name[i];
      fOut.write(temp, FixedStringLengthForStore);
      fOut.write( (char*)(&density), sizeof (G4double));
    }    
   }    

  fOut.close();
  return true;
}

// check stored material is consistent with the current detector setup. 
G4bool  G4ProductionCutsTable::CheckMaterialInfo(const G4String& directory, 
						 G4bool          ascii)
{
  const G4String fileName = directory + "/" + "material.dat";
  const G4String key = "MATERIAL-V2.0";
  G4std::ifstream fIn;  

  // open input file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii )
    fIn.open(fileName,G4std::ios::in|G4std::ios::binary);
  else
#endif
    fIn.open(fileName,G4std::ios::in);

  // check if the file has been opened successfully 
  if (!fIn) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutsTable::CheckMaterialInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  char temp[FixedStringLengthForStore];

  // key word
  G4String keyword;    
  if (ascii) {
    fIn >> keyword;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
  }
  if (key!=keyword) {
#ifdef G4VERBOSE  
    G4cout << "G4ProductionCutsTable::CheckMaterialInfo  ";
    G4cout << " Wrong key word " << key ;
    G4cout <<"( !=  "<< keyword << ")" <<G4endl;
#endif
    return false;
  }

  const G4MaterialTable* matTable = G4Material::GetMaterialTable(); 
  G4int numberOfMaterial = matTable->size();
  // number of materials in the table
  G4int nmat;
  if (ascii) {
    fIn >> nmat;
  } else {
    fIn.read( (char*)(&nmat), sizeof (G4int));
  }
  if (nmat!=numberOfMaterial) {
#ifdef G4VERBOSE  
    G4cout << "G4ProductionCutsTable::CheckMaterialInfo  ";
    G4cout << "Number of material is inconsistent "<< G4endl;
#endif
    return false;
  }

  // list of material
  for (G4int idx=0; idx<numberOfMaterial ; ++idx){
    // check eof
    if(fIn.eof()) {
#ifdef G4VERBOSE  
      G4cout << "G4ProductionCutsTable::CheckMaterialInfo  ";
      G4cout << " encountered End of File" << G4endl;
#endif   
      fIn.close();
      return false;
    }

    // check material name and density
    char name[FixedStringLengthForStore];
    double density;
    if (ascii) {
      fIn >> name >> density;
      density *= (g/cm3);
      
    } else {
      fIn.read(name, FixedStringLengthForStore);
      fIn.read((char*)(&density), sizeof (G4double));
    }
    if (fIn.fail()) {
#ifdef G4VERBOSE  
      G4cout << "G4ProductionCutsTable::CheckMaterialInfo  ";
      G4cout << " Bad data format " << G4endl;
#endif   
      fIn.close();
      return false;
    }
    G4double ratio = abs(density/((*matTable)[idx])->GetDensity() );
    if ( (name != ((*matTable)[idx])->GetName()) || (0.999>ratio) || (ratio>1.001) ){
#ifdef G4VERBOSE  
      G4cout << "G4ProductionCutsTable::CheckMaterialInfo  ";
      G4cout << " Inconsistent material name or density" << G4endl;;
      G4cout << G4std::setw(40) << name;
      G4cout << G4std::setw(20) << G4std::setiosflags(G4std::ios::scientific) << density << G4endl;
      G4cout << G4std::resetiosflags(G4std::ios::scientific);
#endif   
      fIn.close();
      return false;
    }
  }

  fIn.close();
  return true;

}
  
// Store materialCutsCouple information in files under the specified directory.
G4bool  G4ProductionCutsTable::StoreMaterialCutsCoupleInfo(const G4String& directory, 
				    G4bool          ascii)
{  
  const G4String fileName = directory + "/" + "couple.dat";
  const G4String key = "COUPLE-V2.0";
  G4std::ofstream fOut;  
  char temp[FixedStringLengthForStore];

  // open output file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii ) 
    fOut.open(fileName,G4std::ios::out|G4std::ios::binary);
  else 
#endif
    fOut.open(fileName,G4std::ios::out);
  
  
  // check if the file has been opened successfully 
  if (!fOut) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutsTable::StoreMaterialCutsCoupleInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  G4int numberOfCouples = coupleTable.size();
  if (ascii) {
    /////////////// ASCII mode  /////////////////
    // key word
    fOut << G4std::setw(FixedStringLengthForStore) <<  key << G4endl;
    
    // number of couples in the table
    fOut  << numberOfCouples << G4endl;
  } else {
    /////////////// Binary mode  /////////////////
    // key word
    size_t i;
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=key[i];
    fOut.write(temp, FixedStringLengthForStore);
    
    // number of couples in the table   
    fOut.write( (char*)(&numberOfCouples), sizeof (G4int));
  }

 
  // Loop over all couples
  CoupleTableIterator cItr;
  for (cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++){
    G4MaterialCutsCouple* aCouple = (*cItr);
    G4int index = aCouple->GetIndex(); 
    // cut value
    G4ProductionCuts* aCut = aCouple->GetProductionCuts();
    G4double cutValues[NumberOfG4CutIndex];
    for (size_t idx=0; idx <NumberOfG4CutIndex; idx++) {
      cutValues[idx] = aCut->GetProductionCut(idx);
    }
    // material/region info
    G4String materialName = aCouple->GetMaterial()->GetName();
    G4String regionName = "NONE";
    if (aCouple->IsUsed()){
      typedef G4std::vector<G4Region*>::iterator regionIterator;
      for(regionIterator rItr=fG4RegionStore->begin();rItr!=fG4RegionStore->end();rItr++){
	if (IsCoupleUsedInTheRegion(aCouple, *rItr) ){
	  regionName = (*rItr)->GetName();
	  break;
	}
      }
    }

    if (ascii) {
      /////////////// ASCII mode  /////////////////
      // index number
      fOut  << index << G4endl; 
  
      // material name 
      fOut << G4std::setw(FixedStringLengthForStore) << materialName<< G4endl;
  
      // region name 
      fOut << G4std::setw(FixedStringLengthForStore) << regionName<< G4endl;

      fOut.setf(G4std::ios::scientific);
      // cut values
      for (size_t idx=0; idx< NumberOfG4CutIndex; idx++) {
	fOut << G4std::setw(FixedStringLengthForStore) << cutValues[idx]/(mm) << G4endl;
      }
      fOut.unsetf(G4std::ios::scientific);

    } else {
      /////////////// Binary mode  /////////////////
      // index
      fOut.write( (char*)(&index), sizeof (G4int));
    
      // material name
      size_t i;
      for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
      for (i=0; i<materialName.length() && i<FixedStringLengthForStore-1; ++i) {
	temp[i]=materialName[i];
      }
      fOut.write(temp, FixedStringLengthForStore);

      // region name
      for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
      for (i=0; i<regionName.length() && i<FixedStringLengthForStore-1; ++i) {
	temp[i]=regionName[i];
      }
      fOut.write(temp, FixedStringLengthForStore);

      // cut values
      for (size_t idx=0; idx< NumberOfG4CutIndex; idx++) {
	 fOut.write( (char*)(&(cutValues[idx])), sizeof (G4double));
      }    
    }
   }    

  fOut.close();
  return true;
}


// check stored materialCutsCouple is consistent with the current detector setup. 
G4bool  G4ProductionCutsTable::CheckMaterialCutsCoupleInfo(const G4String& directory,
							   G4bool          ascii )
{
  const G4String fileName = directory + "/" + "couple.dat";
  const G4String key = "COUPLE-V2.0";
  G4std::ifstream fIn;  

  // open input file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii )
    fIn.open(fileName,G4std::ios::in|G4std::ios::binary);
  else
#endif
    fIn.open(fileName,G4std::ios::in);

  // check if the file has been opened successfully 
  if (!fIn) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  char temp[FixedStringLengthForStore];

   // key word
  G4String keyword;    
  if (ascii) {
    fIn >> keyword;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
  }
  if (key!=keyword) {
#ifdef G4VERBOSE  
    G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
    G4cout << " Wrong key word " << key ;
    G4cout <<"( !=  "<< keyword << ")" <<G4endl;
#endif
    return false;
  }

  // numberOfCouples
  G4int numberOfCouples;    
  if (ascii) {
    fIn >> numberOfCouples;
  } else {
    fIn.read( (char*)(&numberOfCouples), sizeof (G4int));
  }
  if ( size_t(numberOfCouples) != coupleTable.size()) {
#ifdef G4VERBOSE  
    G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
    G4cout << "Number of couples is inconsistent "<< G4endl;
#endif
    return false;
  }
 
   // Loop over all couples
  CoupleTableIterator cItr;
  for (cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++){
    char name[FixedStringLengthForStore];
    G4MaterialCutsCouple* aCouple = (*cItr);
    G4int index; 
    if (ascii) {
      fIn >> index;
    } else {
      fIn.read( (char*)(&index), sizeof (G4int));
    }
    if ( index !=  aCouple->GetIndex() ) {
#ifdef G4VERBOSE  
      G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
      G4cout << "Index of couples is inconsistent "<< index << G4endl;
#endif
      return false;
    }
   
    if (ascii) {
      fIn >> name;
    } else {
      fIn.read(name, FixedStringLengthForStore);
    }
    if ( name !=  aCouple->GetMaterial()->GetName() ) {
#ifdef G4VERBOSE  
      G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
      G4cout << "MaterialName is inconsistent ";
      G4cout << "[" << index << "]  " << name << G4endl;
#endif
      return false;
    }

    if (ascii) {
      fIn >> name;
    } else {
     fIn.read(name, FixedStringLengthForStore);
    }
    if (!aCouple->IsUsed()){
      if ( name !=  "NONE" ) {
#ifdef G4VERBOSE  
	G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
	G4cout << "Region Name is inconsistent ";
	G4cout << "[" << index << "]  " << name << G4endl;
#endif
	return false;
      }
    } else {
      G4Region* fRegion = fG4RegionStore->GetRegion(name);
      if (!IsCoupleUsedInTheRegion(aCouple, fRegion) ){
#ifdef G4VERBOSE  
	G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
	G4cout << "Region Name is inconsistent ";
	G4cout << "[" << index << "]  " << name << G4endl;
#endif
	return false;
      }
    }
    
    // cut value
    G4ProductionCuts* aCut = aCouple->GetProductionCuts();
    G4double cutValues[NumberOfG4CutIndex];
    for (size_t idx=0; idx< NumberOfG4CutIndex; idx++) {
      if (ascii) {
	fIn >>  cutValues[idx];
        cutValues[idx] *= (mm);
      } else {
	fIn.read( (char*)(&(cutValues[idx])), sizeof (G4double));
      }
      G4double ratio =  cutValues[idx]/aCut->GetProductionCut(idx);
      if ((0.999>ratio) || (ratio>1.001) ){
#ifdef G4VERBOSE  
	G4cout << "G4ProductionCutTable::CheckMaterialCutsCoupleInfo ";
	G4cout << "CutValue for " << idx << " is inconsistent ";
	G4cout << "[" << index << "]  " << G4endl;
#endif
	return false;
      }
    }
  }
  return true;
}
 

// Store cut values information in files under the specified directory.
G4bool   G4ProductionCutsTable::StoreCutsInfo(const G4String& directory, 
					      G4bool          ascii)
{
  const G4String fileName = directory + "/" + "cut.dat";
  const G4String key = "CUT-V2.0";
  G4std::ofstream fOut;  
  char temp[FixedStringLengthForStore];
  
  // open output file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii ) 
    fOut.open(fileName,G4std::ios::out|G4std::ios::binary);
  else 
#endif
    fOut.open(fileName,G4std::ios::out);
  
  
  // check if the file has been opened successfully 
  if (!fOut) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutsTable::StoreCutsInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }

  G4int numberOfCouples = coupleTable.size();
  if (ascii) {
    /////////////// ASCII mode  /////////////////
    // key word
    fOut  << key << G4endl;
    
    // number of couples in the table
    fOut  << numberOfCouples << G4endl;
  } else {
    /////////////// Binary mode  /////////////////
    // key word
    size_t i;
    for (i=0; i<FixedStringLengthForStore; ++i) temp[i] = '\0'; 
    for (i=0; i<key.length() && i<FixedStringLengthForStore-1; ++i) temp[i]=key[i];
    fOut.write(temp, FixedStringLengthForStore);
    
    // number of couples in the table   
    fOut.write( (char*)(&numberOfCouples), sizeof (G4int));
  }

  
  for (size_t idx=0; idx <NumberOfG4CutIndex; idx++) {
    const G4std::vector<G4double>* fRange  = GetRangeCutsVector(idx);
    const G4std::vector<G4double>* fEnergy = GetEnergyCutsVector(idx);
    size_t i=0;
    // Loop over all couples
    CoupleTableIterator cItr;
    for (cItr=coupleTable.begin();cItr!=coupleTable.end();cItr++, i++){      
      if (ascii) {
        /////////////// ASCII mode  /////////////////
        fOut.setf(G4std::ios::scientific);
        fOut << G4std::setw(20) << (*fRange)[i]/mm  ;
        fOut << G4std::setw(20) << (*fEnergy)[i]/keV << G4endl;
        fOut.unsetf(G4std::ios::scientific);
      } else {
	/////////////// Binary mode  /////////////////
	G4double cut =  (*fRange)[i];
	fOut.write((char*)(&cut), sizeof (G4double));
	cut =  (*fEnergy)[i];
	fOut.write((char*)(&cut), sizeof (G4double));
      }
    }
  }
  fOut.close();
  return true;
}
  
// Retrieve cut values information in files under the specified directory.
G4bool   G4ProductionCutsTable::RetrieveCutsInfo(const G4String& directory,
						 G4bool          ascii)
{
  const G4String fileName = directory + "/" + "cut.dat";
  const G4String key = "CUT-V2.0";
  G4std::ifstream fIn;  

  // open input file //
#ifdef G4USE_STD_NAMESPACE
  if (!ascii )
    fIn.open(fileName,G4std::ios::in|G4std::ios::binary);
  else
#endif
    fIn.open(fileName,G4std::ios::in);

  // check if the file has been opened successfully 
  if (!fIn) {
#ifdef G4VERBOSE  
    G4cerr << "G4ProductionCutTable::RetreiveCutsInfo  ";
    G4cerr << " Can not open file " << fileName << G4endl;
#endif
    return false;
  }
  
  char temp[FixedStringLengthForStore];

   // key word
  G4String keyword;    
  if (ascii) {
    fIn >> keyword;
  } else {
    fIn.read(temp, FixedStringLengthForStore);
    keyword = (const char*)(temp);
  }
  if (key!=keyword) {
#ifdef G4VERBOSE  
    G4cout << "G4ProductionCutTable::RetreiveCutsInfo ";
    G4cout << " Wrong key word " << key ;
    G4cout <<"( !=  "<< keyword << ")" <<G4endl;
#endif
    return false;
  }

  // numberOfCouples
  G4int numberOfCouples;    
  if (ascii) {
    fIn >> numberOfCouples;
  } else {
    fIn.read( (char*)(&numberOfCouples), sizeof (G4int));
  }

  for (size_t idx=0; G4int(idx) <NumberOfG4CutIndex; idx++) {
    G4CutVectorForAParticle* fRange  = rangeCutTable[idx];
    G4CutVectorForAParticle* fEnergy = energyCutTable[idx];
    fRange->clear();
    fEnergy->clear();

    // Loop over all couples
    for (size_t i=0; G4int(i)< numberOfCouples; i++){      
      G4double rcut, ecut;
      if (ascii) {
	fIn >> rcut >> ecut;
	rcut *= mm;
	ecut *= keV;
      } else {
	fIn.read((char*)(&rcut), sizeof (G4double));
	fIn.read((char*)(&ecut), sizeof (G4double));
      }
      fRange->push_back(rcut);
      fEnergy->push_back(ecut);
    }
  }
  return true;
}







