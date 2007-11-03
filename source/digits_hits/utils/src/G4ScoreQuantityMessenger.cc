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
// $Id: G4ScoreQuantityMessenger.cc,v 1.2 2007-11-03 21:46:49 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ---------------------------------------------------------------------

#include "G4ScoreQuantityMessenger.hh"
#include "G4ScoringManager.hh"
#include "G4VScoringMesh.hh"

#include "G4PSCellCharge3D.hh"
#include "G4PSCellFlux3D.hh"
#include "G4PSPassageCellFlux3D.hh"
#include "G4PSEnergyDeposit3D.hh"
#include "G4PSDoseDeposit3D.hh"
#include "G4PSNofStep3D.hh"
#include "G4PSNofSecondary3D.hh"
//
#include "G4PSTrackLength3D.hh"
#include "G4PSPassageCellCurrent3D.hh"
#include "G4PSPassageTrackLength3D.hh"
#include "G4PSFlatSurfaceCurrent3D.hh"
#include "G4PSFlatSurfaceFlux3D.hh"
#include "G4PSSphereSurfaceCurrent3D.hh"
#include "G4PSSphereSurfaceFlux3D.hh"
#include "G4PSCylinderSurfaceCurrent3D.hh"
#include "G4PSCylinderSurfaceFlux3D.hh"
#include "G4PSNofCollision3D.hh"
#include "G4PSPopulation3D.hh"
#include "G4PSTrackCounter3D.hh"
#include "G4PSTermination3D.hh"

#include "G4SDChargedFilter.hh"
#include "G4SDNeutralFilter.hh"
#include "G4SDKineticEnergyFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4SDParticleWithEnergyFilter.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4Tokenizer.hh"
#include "G4UnitsTable.hh"

G4ScoreQuantityMessenger::G4ScoreQuantityMessenger(G4ScoringManager* SManager)
:fSMan(SManager)
{
  QuantityCommands();
  FilterCommands();
}

void G4ScoreQuantityMessenger::FilterCommands()
{
  G4UIparameter* param;

  //
  // Filter commands 
  filterDir = new G4UIdirectory("/score/filter/");
  filterDir->SetGuidance("Filter for scoring");
  //
  fchargedCmd = new G4UIcmdWithAString("/score/filter/charged",this);
  fchargedCmd->SetGuidance("Charge filter ( charged )");
  fchargedCmd->SetParameterName("fname",false);
  //
  fneutralCmd = new G4UIcmdWithAString("/score/filter/neutral",this);
  fneutralCmd->SetGuidance("Charge filter ( Neutral )");
  fneutralCmd->SetParameterName("fname",false);
  //
  fkinECmd = new G4UIcommand("/score/filter/kinE",this);
  fkinECmd->SetGuidance("Kinetic Energy Filter");
  fkinECmd->SetGuidance("[usage] /score/filter/kinE fname Elow Ehigh unit");
  fkinECmd->SetGuidance("  fname     :(String) Filter Name ");
  fkinECmd->SetGuidance("  Elow      :(Double) Lower edge of kinetic energy");
  fkinECmd->SetGuidance("  Ehigh     :(Double) Higher edge of kinetic energy");
  fkinECmd->SetGuidance("  unit      :(String) unit of given kinetic energy");
  param = new G4UIparameter("fname",'s',false);
  fkinECmd->SetParameter(param);
  param = new G4UIparameter("elow",'d',true);
  param->SetDefaultValue("0.0");
  fkinECmd->SetParameter(param);
  param = new G4UIparameter("ehigh",'d',false);
  fkinECmd->SetParameter(param);
  G4String smax = DtoS(DBL_MAX);
  param->SetDefaultValue(smax);
  param = new G4UIparameter("unit",'s',false);
  param->SetDefaultValue("keV");
  fkinECmd->SetParameter(param);
  //
  fparticleCmd = new G4UIcommand("/score/filter/particle",this);
  fparticleCmd->SetGuidance("Touch particle filter into current quantity");
  fparticleCmd->SetGuidance("[usage] /score/filter/particle fname p0 .. pn");
  fparticleCmd->SetGuidance("  fname     :(String) Filter Name ");
  fparticleCmd->SetGuidance("  p0 .. pn  :(String) particle names");
  param = new G4UIparameter("fname",'s',false);
  fparticleCmd->SetParameter(param);
  param = new G4UIparameter("particlelist",'s',false);
  param->SetDefaultValue("");
  fparticleCmd->SetParameter(param);
  //
  //
  //
  fparticleKinECmd = new G4UIcommand("/score/filter/particleWithKinE",this);
  fparticleKinECmd->SetGuidance("Particle with kinetic energy filter");
  fparticleKinECmd->SetGuidance("[usage] /score/filter/particleWithKinE fname Elow Ehigh unit p0 .. pn");
  fparticleKinECmd->SetGuidance("  fname     :(String) Filter Name ");
  fparticleKinECmd->SetGuidance("  Elow      :(Double) Lower edge of kinetic energy");
  fparticleKinECmd->SetGuidance("  Ehigh     :(Double) Higher edge of kinetic energy");
  fparticleKinECmd->SetGuidance("  unit      :(String) unit of given kinetic energy");
  fparticleKinECmd->SetGuidance("  p0 .. pn  :(String) particle names");
  param = new G4UIparameter("fname",'s',false);
  fparticleKinECmd->SetParameter(param);
  param = new G4UIparameter("elow",'d',false);
  param->SetDefaultValue("0.0");
  fparticleKinECmd->SetParameter(param);
  param = new G4UIparameter("ehigh",'d',true);
  param->SetDefaultValue(smax);
  fparticleKinECmd->SetParameter(param);
  param = new G4UIparameter("unit",'s',true);
  param->SetDefaultValue("keV");
  fparticleKinECmd->SetParameter(param);
  param = new G4UIparameter("particlelist",'s',false);
  param->SetDefaultValue("");
  fparticleKinECmd->SetParameter(param);
  //
  //
}

G4ScoreQuantityMessenger::~G4ScoreQuantityMessenger()
{
    delete         quantityDir;
    delete         qTouchCmd;
    //
    delete    qCellChgCmd;
    delete    qCellFluxCmd;
    delete    qPassCellFluxCmd;
    delete    qeDepCmd;
    delete    qdoseDepCmd;
    delete    qnOfStepCmd;
    delete    qnOfSecondaryCmd;
    //
    delete          qTrackLengthCmd;
    delete          qPassCellCurrCmd;
    delete          qPassTrackLengthCmd;
    delete          qFlatSurfCurrCmd;
    delete          qFlatSurfFluxCmd;
    delete          qSphereSurfCurrCmd;
    delete          qSphereSurfFluxCmd;
    delete          qCylSurfCurrCmd;
    delete          qCylSurfFluxCmd;
    delete          qNofCollisionCmd;
    delete          qPopulationCmd;
    delete          qTrackCountCmd;
    delete          qTerminationCmd;
    //
    delete   filterDir;
    delete   fchargedCmd;
    delete   fneutralCmd;
    delete   fkinECmd;
    delete   fparticleCmd;
    delete   fparticleKinECmd;
}

void G4ScoreQuantityMessenger::SetNewValue(G4UIcommand * command,G4String newVal)
{
      // Tokens
      G4TokenVec token;
      FillTokenVec(newVal,token);
      //
      // Get Current Mesh
      //
      G4VScoringMesh* mesh = fSMan->GetCurrentMesh();
      //
      // Commands for Current Mesh
      if ( mesh ){
          if(command==qTouchCmd) {
              mesh->SetCurrentPrimitiveScorer(newVal);
	  } else if(command== qCellChgCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      mesh->SetPrimitiveScorer(new G4PSCellCharge3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qCellFluxCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal)) { 
	      mesh->SetPrimitiveScorer(new G4PSCellFlux3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qPassCellFluxCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal))  {
	      mesh->SetPrimitiveScorer(new G4PSPassageCellFlux3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command==qeDepCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal))  {
	      mesh->SetPrimitiveScorer(new G4PSEnergyDeposit3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qdoseDepCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal))  {
	      mesh->SetPrimitiveScorer(new G4PSDoseDeposit3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qnOfStepCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal))  {
	      mesh->SetPrimitiveScorer(new G4PSNofStep3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qnOfSecondaryCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal))  {
	      mesh->SetPrimitiveScorer(new G4PSNofSecondary3D(newVal));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qTrackLengthCmd) {
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSTrackLength3D* ps = new G4PSTrackLength3D(token[0]);
	      ps->Weighted(StoB(token[1]));
	      ps->MultiplyKineticEnergy(StoB(token[2]));
	      ps->DivideByVelocity(StoB(token[3]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qPassCellCurrCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSPassageCellCurrent* ps = new G4PSPassageCellCurrent3D(token[0]);
	      ps->Weighted(StoB(token[1]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qPassTrackLengthCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSPassageTrackLength* ps = new G4PSPassageTrackLength3D(token[0]);
	      ps->Weighted(StoB(token[1]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qFlatSurfCurrCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSFlatSurfaceCurrent3D* ps = 
		new G4PSFlatSurfaceCurrent3D(token[0],StoI(token[1]));
	      ps->Weighted(StoB(token[2]));
	      ps->DivideByArea(StoB(token[3]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qFlatSurfFluxCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      mesh->SetPrimitiveScorer(
				       new G4PSFlatSurfaceFlux3D(token[0],StoI(token[1])));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qSphereSurfCurrCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSSphereSurfaceCurrent3D* ps = 
		new G4PSSphereSurfaceCurrent3D(token[0],StoI(token[1]));
	      ps->Weighted(StoB(token[2]));
	      ps->DivideByArea(StoB(token[3]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
	  } else if(command== qSphereSurfFluxCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      mesh->SetPrimitiveScorer(
				       new G4PSSphereSurfaceFlux3D(token[0], StoI(token[1])));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qCylSurfCurrCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSCylinderSurfaceCurrent3D* ps = 
		new G4PSCylinderSurfaceCurrent3D(token[0],StoI(token[1]));
	      ps->Weighted(StoB(token[2]));
	      ps->DivideByArea(StoB(token[3]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qCylSurfFluxCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      mesh->SetPrimitiveScorer(
				       new G4PSCylinderSurfaceFlux3D(token[0], StoI(token[1])));
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qNofCollisionCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSNofCollision3D* ps =new G4PSNofCollision3D(token[0]); 
	      ps->Weighted(StoB(token[1]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qPopulationCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSPopulation3D* ps =new G4PSPopulation3D(token[0]); 
	      ps->Weighted(StoB(token[1]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qTrackCountCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSTrackCounter3D* ps =new G4PSTrackCounter3D(token[0],StoI(token[1])); 
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }
          } else if(command== qTerminationCmd){
	    if(!mesh->FindPrimitiveScorer(newVal)) {
	      G4PSTermination3D* ps =new G4PSTermination3D(token[0]); 
	      ps->Weighted(StoB(token[1]));
	      mesh->SetPrimitiveScorer(ps);
	    } else {
	      G4cout << " Quantity name, \"" << newVal << "\", is already existing." << G4endl;
	      mesh->SetNullToCurrentPrimitiveScorer();
	    }

	    //
	    // Filters 
	    // 
	  }else if(command== fchargedCmd){
	    if(!mesh->IsCurrentPrimitiveScorerNull()) {
	      mesh->SetFilter(new G4SDChargedFilter(token[0])); 
	    } else {
	      G4cout << " Filter, \"" << token[0] << "\", is not registered." << G4endl;
	    }
          }else if(command== fneutralCmd){
	    if(!mesh->IsCurrentPrimitiveScorerNull()) {
	      mesh->SetFilter(new G4SDNeutralFilter(token[0])); 
	    } else {
	      G4cout << " Filter, \"" << token[0] << "\", is not registered." << G4endl;
	    }
          }else if(command== fkinECmd){
	    if(!mesh->IsCurrentPrimitiveScorerNull()) {
	      G4String& name = token[0];
	      G4double elow  = StoD(token[1]);
	      G4double ehigh = StoD(token[2]);
	      mesh->SetFilter(new G4SDKineticEnergyFilter(name,elow,ehigh));
	    } else {
	      G4cout << " Filter, \"" << token[0] << "\", is not registered." << G4endl;
	    }
          }else if(command== fparticleKinECmd){
	    if(!mesh->IsCurrentPrimitiveScorerNull()) {
	      FParticleWithEnergyCommand(mesh,token); 
	    } else {
	      G4cout << " Filter, \"" << token[0] << "\", is not registered." << G4endl;
	    }
	  } else if(command==fparticleCmd) {
	    if(!mesh->IsCurrentPrimitiveScorerNull()) {
	      FParticleCommand(mesh,token);
	    } else {
	      G4cout << " Filter, \"" << token[0] << "\", is not registered." << G4endl;
	    }
	  }
      }else{
///////////////////	  G4Exception("G4ScroingMessenger:: Current Mesh has not opened. Error!");
        G4cerr << "No mesh is currently open. Open/create a mesh first. Command ignored." << G4endl;
      }
}

G4String G4ScoreQuantityMessenger::GetCurrentValue(G4UIcommand * /*command*/)
{
  G4String val;
//  if(command==verboseCmd)
//  { val = verboseCmd->ConvertToString(fSMan->GetVerboseLevel()); }

  return val;
}

void G4ScoreQuantityMessenger::FillTokenVec(G4String newValues, G4TokenVec& token){

    G4Tokenizer next(newValues);
    G4String val;
    while ( !(val = next()).isNull() ) {
	//if ( val.first('"')==0 ) {
	//     val.remove(0,1);
	//}
	//if ( (val.last('"')) == (G4int)(val.length()-1) ){
	//    val.remove(val.length()-1,1);
	//}
	token.push_back(val);

//      G4cout << "@GetToken:"
//	       << val
//	       << "::"
//	       << G4endl;
    }
}


void G4ScoreQuantityMessenger::FParticleCommand(G4VScoringMesh* mesh, G4TokenVec& token){
    //
    // Filter name
    G4String name = token[0];
    //
    // particle list
    std::vector<G4String> pnames;
    for ( G4int i = 1; i<(G4int)token.size(); i++){
	pnames.push_back(token[i]);
    }
    //
    // Attach Filter
    mesh->SetFilter(new G4SDParticleFilter(name,pnames));
}    

void G4ScoreQuantityMessenger::FParticleWithEnergyCommand(G4VScoringMesh* mesh,G4TokenVec& token){
    G4String& name = token[0];
    G4double  elow = StoD(token[1]);
    G4double  ehigh= StoD(token[2]);
    G4double  unitVal = G4UnitDefinition::GetValueOf(token[3]);
    G4SDParticleWithEnergyFilter* filter = 
	new G4SDParticleWithEnergyFilter(name,elow*unitVal,ehigh*unitVal);
    for ( G4int i = 4; i < (G4int)token.size(); i++){
	filter->add(token[i]);
    }
    mesh->SetFilter(filter);
}
 
