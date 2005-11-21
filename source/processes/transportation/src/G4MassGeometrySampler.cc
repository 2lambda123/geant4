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
// $Id: G4MassGeometrySampler.cc,v 1.12 2005-11-21 21:46:53 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// G4MassGeometrySampler.cc
//
// ----------------------------------------------------------------------

#include "G4MassGeometrySampler.hh"

#include "G4VIStore.hh"
#include "G4WeightWindowStore.hh"
#include "G4VScorer.hh"

#include "G4MScoreConfigurator.hh"
#include "G4MImportanceConfigurator.hh"
#include "G4MWeightWindowConfigurator.hh"
#include "G4WeightCutOffConfigurator.hh"
#include "G4MassGCellFinder.hh"

G4MassGeometrySampler::
G4MassGeometrySampler(const G4String &particlename)
  : fParticleName(particlename),
    fMImportanceConfigurator(0),
    fMScoreConfigurator(0),
    fGCellFinder(0),
    fWeightCutOffConfigurator(0),
    fIStore(0),
    fMWeightWindowConfigurator(0),
    fWWStore(0),
    fIsConfigured(false)
{
}

G4MassGeometrySampler::~G4MassGeometrySampler()
{
  ClearSampling();
}

void G4MassGeometrySampler::ClearSampling()
{
  if (fMImportanceConfigurator)
  {
    delete fMImportanceConfigurator;
    fMImportanceConfigurator = 0;
  }
  if (fMWeightWindowConfigurator)
  {
    delete fMWeightWindowConfigurator;
    fMWeightWindowConfigurator = 0;
  }
  if (fMScoreConfigurator)
  {
    delete fMScoreConfigurator;
    fMScoreConfigurator = 0;
  }
  if (fWeightCutOffConfigurator)
  {
    delete fWeightCutOffConfigurator;
    fWeightCutOffConfigurator = 0;
  }
  if (fGCellFinder)
  {
    delete fGCellFinder;
    fGCellFinder = 0;
  }
  fIStore = 0;
  fConfigurators.clear();
  fIsConfigured = false;
}

G4bool G4MassGeometrySampler::IsConfigured() const
{
  G4bool isconf = false;
  if (fIsConfigured)
  {
   G4cout << "WARNING - G4MassGeometrySampler::IsConfigured()"
          << "          Some initalization exists, use ClearSampling()"
          << "          before a new initialization !" << G4endl;
   isconf = true;
  }
  return isconf;
}

void G4MassGeometrySampler::PrepareScoring(G4VScorer *scorer)
{
  fMScoreConfigurator = new G4MScoreConfigurator(fParticleName, *scorer);
  if (!fMScoreConfigurator)
  {
    G4Exception("G4MassGeometrySampler::PrepareScoring()",
                "FatalError", FatalException,
                "Failed allocation of G4MScoreConfigurator !");
  }
}

void
G4MassGeometrySampler::PrepareImportanceSampling(G4VIStore *istore,
                                           const G4VImportanceAlgorithm  *ialg)
{
  fIStore = istore;

  fMImportanceConfigurator =
    new G4MImportanceConfigurator(fParticleName, *fIStore, ialg);
  if (!fMImportanceConfigurator)
  {
    G4Exception("G4MassGeometrySampler::PrepareImportanceSampling()",
                "FatalError", FatalException,
                "Failed allocation of G4MImportanceConfigurator !");
  }
}

void
G4MassGeometrySampler::PrepareWeightRoulett(G4double wsurvive, 
                                            G4double wlimit,
                                            G4double isource)
{
  fGCellFinder = new G4MassGCellFinder;
  if (!fGCellFinder)
  {
    G4Exception("G4MassGeometrySampler::PrepareWeightRoulett()",
                "FatalError", FatalException,
                "Failed allocation of G4MassGCellFinder !");
  }
  
  fWeightCutOffConfigurator = 
    new G4WeightCutOffConfigurator(fParticleName,
                                   wsurvive,
                                   wlimit,
                                   isource,
                                   fIStore,
                                   *fGCellFinder);
  if (!fWeightCutOffConfigurator)
  {
    G4Exception("G4MassGeometrySampler::PrepareWeightRoulett()",
                "FatalError", FatalException,
                "Failed allocation of G4WeightCutOffConfigurator !");
  }
}

void
G4MassGeometrySampler::PrepareWeightWindow(G4VWeightWindowStore *wwstore,
                                           G4VWeightWindowAlgorithm *wwAlg,
                                           G4PlaceOfAction placeOfAction)
{

  fWWStore = wwstore;
  
  fMWeightWindowConfigurator =
    new G4MWeightWindowConfigurator(fParticleName,
                                    *fWWStore,
                                    wwAlg,
                                    placeOfAction);
}

void G4MassGeometrySampler::Configure()
{
  if (!IsConfigured())
  {
    fIsConfigured = true;

    if (fMScoreConfigurator)
    {
      fConfigurators.push_back(fMScoreConfigurator);
    }
    if (fMImportanceConfigurator)
    {
      fConfigurators.push_back(fMImportanceConfigurator);
    }
    if (fMWeightWindowConfigurator)
    {
      fConfigurators.push_back(fMWeightWindowConfigurator);
    }
    
    G4VSamplerConfigurator *preConf = 0;
    for (G4Configurators::iterator it = fConfigurators.begin();
         it != fConfigurators.end(); it++)
    {
      G4VSamplerConfigurator *currConf =*it;
      currConf->Configure(preConf);
      preConf = *it;
    }
    if (fWeightCutOffConfigurator)
    {
      fWeightCutOffConfigurator->Configure(0);
    }
  }
  return;
}
