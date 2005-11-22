//
// 05-11-21 NeutronHP or Low Energy Parameterization Models 
//          Implemented by T. Koi (SLAC/SCCS)
//          If NeutronHP data do not available for an element, then Low Energy 
//          Parameterization models handle the interactions of the element.
//

#ifndef G4NeutronHPorLElasticData_h
#define G4NeutronHPorLElasticData_h 1

// Class Description
// Cross-section data set for a high precision (based on evaluated data
// libraries) description of neutron elastic scattering below 20 MeV; 
// To be used in your physics list in case you need this physics.
// In this case you want to register an object of this class with 
// the corresponding process.
// Class Description - End

#include "G4NeutronHPChannel.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicsTable.hh"

#include <set>

class G4NeutronHPorLElasticData : public G4VCrossSectionDataSet
{
   public:
   
   G4NeutronHPorLElasticData();
   
   G4NeutronHPorLElasticData( G4NeutronHPChannel* , std::set< G4String >* );

   ~G4NeutronHPorLElasticData();
   
   G4bool IsApplicable(const G4DynamicParticle*, const G4Element*);

   G4double GetCrossSection(const G4DynamicParticle*, const G4Element*, G4double aT);

   void BuildPhysicsTable(const G4ParticleDefinition&);

   void DumpPhysicsTable(const G4ParticleDefinition&);
   
   private:
   
   G4PhysicsTable* theCrossSections;

   private:
     G4NeutronHPChannel* theElasticChannel;
     std::set< G4String >* unavailable_elements;
};

#endif
