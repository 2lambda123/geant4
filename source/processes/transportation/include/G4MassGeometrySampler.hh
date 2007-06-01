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
// $Id: G4MassGeometrySampler.hh,v 1.8 2007-06-01 07:53:26 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4MassGeometrySampler
//
// Class description:
// This class inherits from G4VSampler8. It is used for scoring and 
// importance sampling in the tracking (mass) geometry.
// See also the description in G4VSampler8.hh.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4MassGeometrySampler_hh
#define G4MassGeometrySampler_hh G4MassGeometrySampler_hh

#include "G4Types.hh"
#include "G4String.hh"
#include "G4VSampler8.hh"
#include "G4VSamplerConfigurator8.hh"

class G4MImportanceConfigurator;
class G4MWeightWindowConfigurator;
class G4MScoreConfigurator;
class G4WeightCutOffConfigurator8;
class G4VGCellFinder;

class G4MassGeometrySampler : public G4VSampler8
{

public:  // with description
 
  explicit G4MassGeometrySampler(const G4String &particlename);
  virtual ~G4MassGeometrySampler();

  virtual void PrepareScoring(G4VScorer *Scorer);
  virtual void PrepareImportanceSampling(G4VIStore *istore,
                                         const G4VImportanceAlgorithm 
                                         *ialg);
  virtual void PrepareWeightRoulett(G4double wsurvive, 
                                    G4double wlimit,
                                    G4double isource);
  
  virtual void PrepareWeightWindow(G4VWeightWindowStore *wwstore,
                                   G4VWeightWindowAlgorithm *wwAlg,
                                   G4PlaceOfAction placeOfAction);

  virtual void Configure();

  virtual void ClearSampling();
  virtual G4bool IsConfigured() const;
    
private:

  G4MassGeometrySampler(const G4MassGeometrySampler &);
  G4MassGeometrySampler &
  operator=(const G4MassGeometrySampler &);

private:

  G4String fParticleName;
  G4MImportanceConfigurator *fMImportanceConfigurator;
  G4MScoreConfigurator *fMScoreConfigurator;
  G4VGCellFinder *fGCellFinder;
  G4WeightCutOffConfigurator8 *fWeightCutOffConfigurator8;
  G4VIStore *fIStore;
  G4MWeightWindowConfigurator *fMWeightWindowConfigurator;
  G4VWeightWindowStore *fWWStore;
  G4bool fIsConfigured;
  G4Configurators fConfigurators;
};
  
#endif

