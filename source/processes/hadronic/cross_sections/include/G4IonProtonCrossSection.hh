#ifndef G4IonProtonCrossSection_h
#define G4IonProtonCrossSection_h

#include "globals.hh"
#include "G4Proton.hh"
// Class Description
// Cross-sections for ion proton scattering up to 20 GeV, getting the low
// energy threshold behaviour right.
// H.P. Wellisch (TRIUMF), D. Axen (British Columbia U.). 1996. 
// Published in Phys.Rev.C54:1329-1332,1996 
// Class Description - End

#include "G4VCrossSectionDataSet.hh"
#include "G4ProtonInelasticCrossSection.hh"

class G4IonProtonCrossSection : public G4VCrossSectionDataSet
{
   public:

   virtual
   G4bool IsApplicable(const G4DynamicParticle* aPart, const G4Element*anEle)
   {
     G4bool result = false;
     if(( anEle->GetN()<1.1) &&
        ( aPart->GetKineticEnergy()/aPart->GetDefinition()->GetBaryonNumber()<20*GeV&&
	  aPart->GetDefinition()->GetBaryonNumber()>4)
       ) result = true;
     return result;
   }

   virtual
   G4double GetCrossSection(const G4DynamicParticle* aPart, 
                            const G4Element*anEle, G4double aTemperature)
   {
     G4ProtonInelasticCrossSection theForward;
     G4double result = theForward.GetCrossSection(aPart->GetKineticEnergy(),
                                                  aPart->GetDefinition()->GetBaryonNumber(),
						  aPart->GetDefinition()->GetPDGCharge());
     return result;					  
   }

   virtual
   void BuildPhysicsTable(const G4ParticleDefinition&)
   {}

   virtual
   void DumpPhysicsTable(const G4ParticleDefinition&)
   {G4cout << "G4IonProtonCrossSection: uses formula"<<endl;}

};

#endif
