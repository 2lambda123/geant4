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
//
// GEANT4 physics class: G4HadronElasticDataSet -- header file
// F.W. Jones, TRIUMF, 28-JAN-97
//
// Class Description
// Baseline data-set for hadron nucleaus elastic cross-section. This does not 
// need to be registered, but provides part of the general cross-section 
// baseline
// Class Description - End

#ifndef G4HadronElasticDataSet_h
#define G4HadronElasticDataSet_h 1

#include "G4VCrossSectionDataSet.hh"
#include "G4HadronCrossSections.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"


class G4HadronElasticDataSet : public G4VCrossSectionDataSet
{
public:

   G4HadronElasticDataSet()
   {
      theHadronCrossSections = G4HadronCrossSections::Instance();
   }

   ~G4HadronElasticDataSet()
   {
   }

   G4bool IsApplicable(const G4DynamicParticle* aParticle,
                       const G4Element* anElement)
   {
      return theHadronCrossSections->IsApplicable(aParticle, anElement);
   }

   G4double GetCrossSection(const G4DynamicParticle* aParticle,
                            const G4Element* anElement, G4double )
   {
      return theHadronCrossSections->GetElasticCrossSection(aParticle,
                                                              anElement);
   }

   void BuildPhysicsTable(const G4ParticleDefinition&)
   {
   }

   void DumpPhysicsTable(const G4ParticleDefinition&)
   {
   }

private:

   G4HadronCrossSections* theHadronCrossSections;
};

#endif
