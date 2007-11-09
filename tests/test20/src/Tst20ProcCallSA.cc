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
#include "Tst20ProcCallSA.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"

Tst20ProcCallSA::Tst20ProcCallSA()
{ }

Tst20ProcCallSA::~Tst20ProcCallSA()
{
  print();
}

void Tst20ProcCallSA::execute(const G4Step* aStep)
{
  G4Track* theTrack = aStep->GetTrack();
  if(theTrack->GetNextVolume() == 0 ) return;  
  G4String particleType = theTrack->GetDefinition()->GetParticleName();
     
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint(); 
  G4String processName = postStepPoint->GetProcessDefinedStep()->GetProcessName();

  G4Material* material = postStepPoint->GetMaterial();
  G4String materialName = material->GetName();
     
  G4String index = processName + G4String(" for ") + particleType + G4String(" in ") + materialName;
  calls[index]++; 
}

void Tst20ProcCallSA::print()
{
  for(intMapIter iCall=calls.begin(); iCall!=calls.end(); iCall++)
    {
      G4cout << (*iCall).first << " : " << (*iCall).second << " calls" << G4endl;
    }
}

