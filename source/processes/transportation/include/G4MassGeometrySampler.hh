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
// $Id: G4MassGeometrySampler.hh,v 1.6 2003-11-26 14:51:48 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4MassGeometrySampler
//
// Class description:
// This class inherits from G4VSampler. It is used for scoring and 
// importance sampling in the tracking (mass) geometry.
// See also the description in G4VSampler.hh.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4MassGeometrySampler_hh
#define G4MassGeometrySampler_hh G4MassGeometrySampler_hh

#include "G4Types.hh"
#include "G4String.hh"
#include "G4VSampler.hh"
#include "G4VSamplerConfigurator.hh"

class G4MImportanceConfigurator;
class G4MWeightWindowConfigurator;
class G4MScoreConfigurator;
class G4WeightCutOffConfigurator;
class G4VGCellFinder;

class G4MassGeometrySampler : public G4VSampler
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
  G4WeightCutOffConfigurator *fWeightCutOffConfigurator;
  G4VIStore *fIStore;
  G4MWeightWindowConfigurator *fMWeightWindowConfigurator;
  G4VWeightWindowStore *fWWStore;
  G4bool fIsConfigured;
  G4Configurators fConfigurators;
};
  
#endif

