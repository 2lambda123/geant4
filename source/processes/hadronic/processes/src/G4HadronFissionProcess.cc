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
// F.W. Jones, TRIUMF, 03-DEC-96
// 
// This is a prototype of a low-energy fission process.
// Currently it is based on the GHEISHA routine FISSIO,
// and conforms fairly closely to the original Fortran.
// Note: energy is in MeV and momentum is in MeV/c.
//
// 27-MAR-97 F.W.Jones: first version for Alpha release
// 20-JUN-97 F.W.Jones: added check for zero cross section
//
// 19-MAY-98 FWJ: variant G4HadronFission process for
// G4CrossSectionDataSet/DataStore class design.
// 29-JUN-98 FWJ: default data set G4HadronCrossSections
//


#include "G4HadronFissionProcess.hh"

//unsigned 
//G4HadronicFissionProcessHashFun(const G4ParticleDefinition& aParticleDefinition)
//{
//   return aParticleDefinition.GetParticleName().hash();
//}

G4HadronFissionProcess::
G4HadronFissionProcess(const G4String& processName) : 
   G4HadronicProcess(processName)
{
   AddDataSet(new G4HadronFissionDataSet);
}

G4HadronFissionProcess::~G4HadronFissionProcess()
{
}
 

void 
G4HadronFissionProcess::BuildThePhysicsTable(G4ParticleDefinition& aParticleType)
{
   if (!G4HadronicProcess::GetCrossSectionDataStore()) {
      G4Exception("G4HadronFissionProcess", "007", FatalException,
                  "no cross section data set");
      return;
   }
   G4HadronicProcess::GetCrossSectionDataStore()->BuildPhysicsTable(aParticleType);
}


G4double 
G4HadronFissionProcess::GetMicroscopicCrossSection(const G4DynamicParticle* aParticle,
                                       const G4Element* anElement, G4double aTemp)
{
   // gives the microscopic cross section in GEANT4 internal units
   if (!G4HadronicProcess::GetCrossSectionDataStore()) {
      G4Exception("G4HadronFissionProcess", "007", FatalException,
                  "no cross section data Store");
      return DBL_MIN;
   }
   return G4HadronicProcess::GetCrossSectionDataStore()->GetCrossSection(aParticle, anElement, aTemp);
}

void
G4HadronFissionProcess::
DumpPhysicsTable(const G4ParticleDefinition& aParticleType)
{
   if (!G4HadronicProcess::GetCrossSectionDataStore()) {
      G4Exception("G4HadronFissionProcess", "111", JustWarning,
                  "no cross section data set");
      return;
   }
   G4HadronicProcess::GetCrossSectionDataStore()->DumpPhysicsTable(aParticleType);
}
