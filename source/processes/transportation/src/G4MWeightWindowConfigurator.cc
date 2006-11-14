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
// $Id: G4MWeightWindowConfigurator.cc,v 1.6 2006-11-14 09:11:18 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4MWeightWindowConfigurator
//
// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------

#include "G4MWeightWindowConfigurator.hh"
#include "G4WeightWindowAlgorithm.hh"
#include "G4MassWeightWindowProcess.hh"

G4MWeightWindowConfigurator::
G4MWeightWindowConfigurator(const G4String &particlename,
                            G4VWeightWindowStore &wwstore,
                            const G4VWeightWindowAlgorithm *wwAlg,
                            G4PlaceOfAction placeOfAction)
  : fPlacer(particlename),
    fWeightWindowStore(wwstore),
    fDeleteWWalg( ( ! wwAlg) ),
    fWWalgorithm(( (fDeleteWWalg) ? 
                   new G4WeightWindowAlgorithm(5,3,5) : wwAlg)),
    fMassWeightWindowProcess(0),
    fPlaceOfAction(placeOfAction)
{
}

G4MWeightWindowConfigurator::~G4MWeightWindowConfigurator()
{  
  if (fMassWeightWindowProcess)
  {
    fPlacer.RemoveProcess(fMassWeightWindowProcess);
    delete fMassWeightWindowProcess;
  }
  if (fDeleteWWalg)
  {
    delete fWWalgorithm;
  }
}

void
G4MWeightWindowConfigurator::Configure(G4VSamplerConfigurator *preConf)
{
  const G4VTrackTerminator *terminator = 0;
  if (preConf)
  {
    terminator = preConf->GetTrackTerminator();
  };

  fMassWeightWindowProcess = 
    new G4MassWeightWindowProcess(*fWWalgorithm, 
                                  fWeightWindowStore, 
                                  terminator,
                                  fPlaceOfAction);
  fPlacer.AddProcessAsSecondDoIt(fMassWeightWindowProcess);
}

const G4VTrackTerminator *
G4MWeightWindowConfigurator::GetTrackTerminator() const 
{
  return fMassWeightWindowProcess;
}

