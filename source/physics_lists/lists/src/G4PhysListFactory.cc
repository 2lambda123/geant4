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
// $Id: G4PhysListFactory.cc,v 1.10 2009-11-25 13:10:51 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:  G4PhysListFactory
//
// Author: 21 April 2008 V. Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

#include "G4PhysListFactory.hh"
#include "FTFP.hh"
#include "FTFP_BERT.hh"
#include "FTFP_BERT_EMV.hh"
#include "FTFP_BERT_EMX.hh"
#include "FTFP_BERT_TRV.hh"
#include "FTF_BIC.hh"
#include "LBE.hh"
#include "LHEP.hh"
#include "LHEP_EMV.hh"
#include "QBBC.hh"
#include "QGSC.hh"
#include "QGSC_BERT.hh"
#include "QGSC_CHIPS.hh"
#include "QGSC_EMV.hh"
#include "QGSC_QGSC.hh"
#include "QGSP.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_EMV.hh"
#include "QGSP_BERT_EMX.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BERT_TRV.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_EMY.hh"
#include "QGSP_BIC_HP.hh"
#include "QGSP_FTFP_BERT.hh"
#include "QGS_BIC.hh"
#include "QGSP_INCL_ABLA.hh"

G4PhysListFactory::G4PhysListFactory() 
{
  defName = "QGSP_BERT";
  nlists = 31;
  G4String s[31] = {
    "FTFP","FTFP_BERT","FTFP_BERT_EMV","FTFP_BERT_EMX","FTFP_BERT_TRV","FTF_BIC",
    "LBE","LHEP","LHEP_EMV",
    "QBBBG","QBBC","QBBCG","QBBCF","QBBC_HP",
    "QGSC","QGSC_BERT","QGSC_CHIPS","QGSC_EMV","QGSC_QGSC",
    "QGSP","QGSP_BERT","QGSP_BERT_EMV","QGSP_BERT_EMX","QGSP_BERT_HP",
    "QGSP_BERT_TRV","QGSP_BIC","QGSP_BIC_EMY","QGSP_BIC_HP",
    "QGSP_FTFP_BERT","QGS_BIC", "QGSP_INCL_ABLA"};

  for(size_t i=0; i<nlists; i++) {
    listnames.push_back(s[i]);
  }
}

G4PhysListFactory::~G4PhysListFactory()
{}

G4VModularPhysicsList* G4PhysListFactory::ReferencePhysList()
{
  // instantiate PhysList by environment variable "PHYSLIST"
  G4String name = "";
  char* path = getenv("PHYSLIST");
  if (path) {
    name = G4String(path);
  } else {
    name = defName;
    G4cout << "### G4PhysListFactory WARNING: "
	   << " environment variable PHYSLIST is not defined"
	   << G4endl
	   << "    Default Physics Lists " << name 
	   << " is instantiated" 
	   << G4endl;
  }
  return GetReferencePhysList(name);
}

G4VModularPhysicsList* G4PhysListFactory::GetReferencePhysList(
        const G4String& name)
{
  G4VModularPhysicsList* p = 0;
  if     (name == "FTFP") {p = new FTFP();}
  else if(name == "FTFP_BERT") {p = new FTFP_BERT();}
  else if(name == "FTFP_BERT_EMV") {p = new FTFP_BERT_EMV();}
  else if(name == "FTFP_BERT_EMX") {p = new FTFP_BERT_EMX();}
  else if(name == "FTFP_BERT_TRV") {p = new FTFP_BERT_TRV();}
  else if(name == "FTF_BIC") {p = new FTF_BIC();}
  else if(name == "LBE") {p = new LBE();}
  else if(name == "LHEP") {p = new LHEP();}
  else if(name == "LHEP_EMV") {p = new LHEP_EMV();}
  else if(name == "QBBC") {p = new QBBC();}
  else if(name == "QBBC_HP") {p = new QBBC(1,"QBBC_HP");}
  else if(name == "QBBC_XGG") {p = new QBBC(1,"QBBC_XGG");}
  else if(name == "QBBC_XGGSN") {p = new QBBC(1,"QBBC_XGGSN");}
  else if(name == "QGSC") {p = new QGSC();}
  else if(name == "QGSC_BERT") {p = new QGSC_BERT();}
  else if(name == "QGSC_CHIPS") {p = new QGSC_CHIPS();}
  else if(name == "QGSC_EMV") {p = new QGSC_EMV();}
  else if(name == "QGSC_QGSC") {p = new QGSC_QGSC();}
  else if(name == "QGSP") {p = new QGSP();}
  else if(name == "QGSP_BERT") {p = new QGSP_BERT();}
  else if(name == "QGSP_BERT_EMV") {p = new QGSP_BERT_EMV();}
  else if(name == "QGSP_BERT_EMX") {p = new QGSP_BERT_EMX();}
  else if(name == "QGSP_BERT_HP") {p = new QGSP_BERT_HP();}
  else if(name == "QGSP_BERT_TRV") {p = new QGSP_BERT_TRV();}
  else if(name == "QGSP_BIC") {p = new QGSP_BIC();}
  else if(name == "QGSP_BIC_EMY") {p = new QGSP_BIC_EMY();}
  else if(name == "QGSP_BIC_HP") {p = new QGSP_BIC_HP();}
  else if(name == "QGSP_FTFP_BERT") {p = new QGSP_FTFP_BERT();}
  else if(name == "QGS_BIC") {p = new QGS_BIC();}
  else if(name == "QGSP_INCL_ABLA") {p = new QGSP_INCL_ABLA();}
  else {
    G4cout << "### G4PhysListFactory WARNING: "
	   << "PhysicsList " << name << " is not known"
	   << G4endl
	   << "    Default Physics Lists " << defName
	   << " is instantiated" 
	   << G4endl;
    p = new QGSP_BERT();
  }
  return p;
}
  
G4bool G4PhysListFactory::IsReferencePhysList(const G4String& name)
{
  G4bool res = false;
  for(size_t i=0; i<nlists; i++) {
    if(name == listnames[i]) {
      res = true;
      break;
    }
  }
  return res;
}

const std::vector<G4String>& 
G4PhysListFactory::AvailablePhysLists() const
{
  return listnames;
}

