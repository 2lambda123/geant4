//
// File name:     RadmonPrimaryGeneratorAction.cc
// Creation date: Oct 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPrimaryGeneratorAction.cc,v 1.2 2005-11-10 08:11:26 capra Exp $
// Tag:           $Name: not supported by cvs2svn $
//

// Include files
#include "RadmonPrimaryGeneratorAction.hh"
#include "RadmonVGeneratorLayout.hh"
#include "RadmonVGeneratorsFactory.hh"
#include "RadmonVGenerator.hh"
#include "Randomize.hh"
#include "G4Geantino.hh"

#include "G4UnitsTable.hh"

                                                RadmonPrimaryGeneratorAction :: RadmonPrimaryGeneratorAction(RadmonVGeneratorLayout * layout, RadmonVGeneratorsFactory * factory)
:
 particlesGun(1),
 generatorLayout(layout),
 generatorsFactory(factory),
 needUpdate(true)
{
 if (layout==0)
  G4Exception("RadmonPrimaryGeneratorAction::RadmonPrimaryGeneratorAction: layout==0.");

 if (factory==0)
  G4Exception("RadmonPrimaryGeneratorAction::RadmonPrimaryGeneratorAction: factory==0.");

 layout->AttachObserver(this);
}
 
 
 
                                                RadmonPrimaryGeneratorAction :: ~RadmonPrimaryGeneratorAction()
{
 generatorLayout->DetachObserver(this);
  
 CleanUp();

 delete generatorsFactory;
}





void                                            RadmonPrimaryGeneratorAction :: GeneratePrimaries(G4Event * anEvent)
{
 if (needUpdate)
 {
  Update();
  needUpdate=false;
 }
 
 G4ThreeVector zero(0., 0. ,0.);
 particlesGun.SetParticlePosition(zero);
 particlesGun.SetParticleTime(0.);
 particlesGun.SetParticleDefinition(G4Geantino::GeantinoDefinition());
 particlesGun.SetParticleEnergy(0.*MeV);
 particlesGun.SetParticleMomentumDirection(G4ParticleMomentum(0., 0., 1.));
 particlesGun.SetParticlePolarization(zero);
 particlesGun.SetParticleCharge(0.);
 particlesGun.SetNumberOfParticles(1);
 
 const RadmonVGeneratorLayout * const layout(generatorLayout);

 G4String sourceLabel;
 G4int i(layout->GetNSources());
 G4double total(0.);

 while (i>0)
 {
  i--;
  
  sourceLabel=layout->GetSourceLabel(i);
  total+=layout->GetRelativeSourceIntensity(sourceLabel);
 }
 
 if (total<=0.)
 {
  particlesGun.GeneratePrimaryVertex(anEvent);
  return;
 }

 total*=G4UniformRand();
 
 G4double intensity;
 
 i=layout->GetNSources();
 while (i>0)
 {
  i--;
  
  sourceLabel=layout->GetSourceLabel(i);
  intensity=layout->GetRelativeSourceIntensity(sourceLabel);
  if (total < intensity)
   break;
  
  total-=intensity;
 }
 
 const G4int n(layout->GetNSourceAlgorithms(sourceLabel));
 
 for (i=0; i<n; i++)
 {
  G4String algorithmLabel(layout->GetSourceAlgorithmLabel(sourceLabel, i));

  GeneratorsMap::iterator j(generatorsMap.find(MakeKey(sourceLabel, algorithmLabel)));
  
  if (j!=generatorsMap.end())
   if (j->second)
   {
    j->second->ConvolveParticleGun(particlesGun);
   }
 }

// G4cout << G4BestUnit(particlesGun.GetParticlePosition(), "Length") << ", " << particlesGun.GetParticleDefinition()->GetParticleName() << ", " << G4BestUnit(particlesGun.GetParticleMomentumDirection()*particlesGun.GetParticleEnergy(), "Energy") << G4endl;

 particlesGun.GeneratePrimaryVertex(anEvent);
}





void                                            RadmonPrimaryGeneratorAction :: OnLayoutChange()
{
 needUpdate=true; 
}





inline void                                     RadmonPrimaryGeneratorAction :: Update(void)
{
 CleanUp();
 generatorsMap.clear();
 
 const RadmonVGeneratorLayout * const layout(generatorLayout);
 
 G4int i(layout->GetNSources());
 
 while (i>0)
 {
  i--;
  
  G4String sourceLabel(layout->GetSourceLabel(i));
  
  G4int j(layout->GetNSourceAlgorithms(sourceLabel));
  
  while (j>0)
  {
   j--;
   
   G4String algorithmLabel(layout->GetSourceAlgorithmLabel(sourceLabel, j));
   
   RadmonVGenerator * generator(generatorsFactory->GetGenerator(layout->GetSourceAlgorithmType(sourceLabel, algorithmLabel)));
   
   if (generator)
   {
    generatorsMap[MakeKey(sourceLabel, algorithmLabel)]=generator;

    G4int k(layout->GetSourceAlgorithmNAttributes(sourceLabel, algorithmLabel));
    
    while (k>0)
    {
     k--;
     
     G4String attribute(layout->GetSourceAlgorithmAttributeName(sourceLabel, algorithmLabel, k));
     G4String value(layout->GetSourceAlgorithmAttribute(sourceLabel, algorithmLabel, attribute));
    
     generator->SetGeneratorAttribute(attribute, value);
    }
   }
  }
 }
}



inline void                                     RadmonPrimaryGeneratorAction :: CleanUp(void)
{
 GeneratorsMap::iterator i(generatorsMap.begin());
 const GeneratorsMap::iterator end(generatorsMap.end());
 
 while (i!=end)
 {
  delete i->second;
  i++;
 }
}



inline G4String                                 RadmonPrimaryGeneratorAction :: MakeKey(const G4String & source, const G4String & algorithm)
{
 G4String key(source);
 key+="|==|";
 key+=algorithm;

 return key;      
}
