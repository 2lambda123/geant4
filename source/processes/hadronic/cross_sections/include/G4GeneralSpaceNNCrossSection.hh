#ifndef G4GeneralSpaceNNCrossSection_h
#define G4GeneralSpaceNNCrossSection_h
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              G4TESAGeneralNNCrossSection.hh
//
// Version:		B.1
// Date:		15/04/04
// Author:		P R Truscott
// Organisation:	QinetiQ Ltd, UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		17191/03/NL/LvH
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 6 October 2003, P R Truscott, QinetiQ Ltd, UK
// Created.
//
// 15 March 2004, P R Truscott, QinetiQ Ltd, UK
// Beta release
//
// 4 June 2004, J.P. Wellisch, CERN
// trivial porting issue to windows.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
// * This  code  implementation  is  the  intellectual   property  of *
// * QinetiQ.   Rights  to use  this  software  are  granted  to  the *
// * European Space Agency  under  the  provisions  of  Clause 39  of *
// * the    Standard    Conditions   for  Contract.   ESA    contract *
// * 17191/03/NL/LvH  provides  the  rights to distribute source code *
// * through  the  Geant4 Collaboration Agreement for general  public *
// * use.  Some elements of this source code may be derived from code *
// * developed   by   other   member   institutes   of   the   Geant4 *
// * Collaboration, and the provision of  their code under the Geant4 *
// * Collaboration is acknowledged.                                   *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Class Description
//
//
// Class Description - End
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "G4VCrossSectionDataSet.hh"
#include "G4ProtonInelasticCrossSection.hh"
#include <iostream>
#include "G4IonProtonCrossSection.hh"
#include "G4TripathiLightCrossSection.hh"
#include "G4TripathiCrossSection.hh"
#include "G4IonsShenCrossSection.hh"
#include "globals.hh"

////////////////////////////////////////////////////////////////////////////////
//
class G4GeneralSpaceNNCrossSection : public G4VCrossSectionDataSet
{
  public:
    G4GeneralSpaceNNCrossSection ();
    ~G4GeneralSpaceNNCrossSection ();

    virtual G4bool IsApplicable(const G4DynamicParticle* theProjectile,
      const G4Element* theTarget);

    virtual G4double GetCrossSection(const G4DynamicParticle* theProjectile,
      const G4Element* theTarget, G4double theTemperature);

    virtual void BuildPhysicsTable(const G4ParticleDefinition&)
      {;}

    virtual void DumpPhysicsTable(const G4ParticleDefinition&)
      {G4cout << "G4GeneralSpaceNNCrossSection: uses formula"<<G4endl;}

   private:

     G4ProtonInelasticCrossSection *protonInelastic;
     G4IonProtonCrossSection       *ionProton;
     G4TripathiLightCrossSection   *TripathiLight;
     G4TripathiCrossSection        *TripathiGeneral;
     G4IonsShenCrossSection        *Shen;
};
////////////////////////////////////////////////////////////////////////////////
//
#endif
