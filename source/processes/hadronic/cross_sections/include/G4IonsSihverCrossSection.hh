#ifndef G4IonsSihverCrossSection_h
#define G4IonsSihverCrossSection_h
//
// Class Description
// Implementation of formulas 
// Sihver et al. Phys. Rev. C 47 1225 (1993); 
// Total Reaction Cross Section for Nucleus-nucles reactions.
//    Energy independent   
//    Valid for 100>MeV/nucleon 
// Class Description - End
//

#include "globals.hh"
#include "G4Proton.hh"

#include "G4VCrossSectionDataSet.hh"

class G4IonsSihverCrossSection : public G4VCrossSectionDataSet
{
   public:
      G4IonsSihverCrossSection ():
         square_r0 ( ( 1.36 * fermi ) * ( 1.36 * fermi ) )
      {
      }
   
   virtual
   G4bool IsApplicable(const G4DynamicParticle* aDP, const G4Element*)
   {
      G4int BaryonNumber = aDP->GetDefinition()->GetBaryonNumber();
      G4double KineticEnergy = aDP->GetKineticEnergy(); 
      if ( KineticEnergy / BaryonNumber >= 100*MeV && BaryonNumber > 1 ) 
         return true;
      return false;
   }

   virtual
   G4double GetCrossSection(const G4DynamicParticle*, 
                            const G4Element*, G4double aTemperature);

   virtual
   void BuildPhysicsTable(const G4ParticleDefinition&)
   {}

   virtual
   void DumpPhysicsTable(const G4ParticleDefinition&) 
   {G4cout << "tG4GIonCrossSection: uses formula"<<G4endl;}

   private:
      const G4double square_r0;

};

#endif
