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
// $Id: TiaraDPSSampledEnergy.hh,v 1.7 2006-06-29 15:43:31 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
//
// Class TiaraDPSSampledEnergy
//

#ifndef TiaraDPSSampledEnergy_hh
#define TiaraDPSSampledEnergy_hh TiaraDPSSampledEnergy_hh

#ifdef G4ANALYSIS_USE

#include "TiaraVSourceEnergyGenerator.hh"
#include <memory>
#include <map>

namespace AIDA {
  class IDataPointSet;
  class ITree;
}

class TiaraDPSSampledEnergy : public TiaraVSourceEnergyGenerator {
public:
  TiaraDPSSampledEnergy(const G4String &eng,
		     G4double minEnergyCut,
		     const G4String &sourceTree,
		     const G4String &nameExt = G4String(""));
  ~TiaraDPSSampledEnergy();
  TiaraDPSSampledEnergy(const TiaraDPSSampledEnergy& rhs);


  virtual G4double GetEnergy();
  virtual TiaraVSourceEnergyGenerator *Clone() const;

  TiaraDPSSampledEnergy& operator=(const TiaraDPSSampledEnergy& rhs);
  
private:

  void getBounds(G4int &cL, G4int &cH, G4double v);

  //std::auto_ptr<AIDA::ITree> fTree;
  // std::auto_ptr<AIDA::IDataPointSet> fSampleDPS;
 // /////
 AIDA::ITree* fTree;
 AIDA::IDataPointSet* fSampleDPS;
  //////

  G4double fMinEnergyCut;
  std::map<int, double> fEnergy_Flux;
  G4double fMaxProb;
  G4double fMinE;
  G4double fMaxE;
};

#endif
#endif
