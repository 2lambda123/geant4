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
// $Id: G4PSDoseDeposit_RegNav.hh,v 1.1 2008-11-27 21:55:27 arce Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4PSDoseDeposit_RegNav_h
#define G4PSDoseDeposit_RegNav_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   Primitive scorer class for scoring dose deposit in the geometry volume.
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////
class G4PhantomParameterisation;
class G4EnergyLossForExtrapolator;
class G4VPhysicalVolume;

class G4PSDoseDeposit_RegNav : public G4VPrimitiveScorer
{
 public: // with description
      G4PSDoseDeposit_RegNav(G4String name, G4int depth=0);
      virtual ~G4PSDoseDeposit_RegNav();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      G4int HCID;
      G4THitsMap<G4double>* EvtMap;

  G4PhantomParameterisation* GetPhantomParam(G4bool mustExist);
  G4bool IsPhantomVolume( G4VPhysicalVolume* pv );

  G4PhantomParameterisation* thePhantomParam;
 G4EnergyLossForExtrapolator* theElossExt;
};
#endif
