#include "G4QMDNucleus.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4NucleiProperties.hh"

#include <numeric>

G4QMDNucleus::G4QMDNucleus()
{
   G4QMDParameters* parameters = G4QMDParameters::GetInstance();
   hbc = parameters->Get_hbc();
}



G4QMDNucleus::~G4QMDNucleus()
{
   ;
}


G4LorentzVector G4QMDNucleus::Get4Momentum()
{
   G4LorentzVector p( 0 );
   std::vector< G4QMDParticipant* >::iterator it; 
   for ( it = participants.begin() ; it != participants.end() ; it++ ) 
      p += (*it)->Get4Momentum();   

   return p;
}



G4int G4QMDNucleus::GetMassNumber()
{

   G4int A = 0; 
   std::vector< G4QMDParticipant* >::iterator it; 
   for ( it = participants.begin() ; it != participants.end() ; it++ ) 
   {
      if ( (*it)->GetDefinition() == G4Proton::Proton() 
        || (*it)->GetDefinition() == G4Neutron::Neutron() ) 
         A++; 
   }

   return A;
}



G4int G4QMDNucleus::GetAtomicNumber()
{
   G4int Z = 0; 
   std::vector< G4QMDParticipant* >::iterator it; 
   for ( it = participants.begin() ; it != participants.end() ; it++ ) 
   {
      if ( (*it)->GetDefinition() == G4Proton::Proton() ) 
         Z++; 
   }
   return Z;
}



G4double G4QMDNucleus::GetNuclearMass()
{

   G4double mass = G4NucleiPropertiesTable::GetNuclearMass( GetAtomicNumber() , GetMassNumber() );
   
   if ( mass == 0.0 )
   {

      G4int Z = GetAtomicNumber();
      G4int A = GetMassNumber();
      G4int N = A - Z;

// Weizsacker-Bethe 

      G4double Av = 16*MeV; 
      G4double As = 17*MeV; 
      G4double Ac = 0.7*MeV; 
      G4double Asym = 23*MeV; 

      G4double BE = Av * A 
                  - As * std::pow ( G4double ( A ) , 2.0/3.0 ) 
                  - Ac * Z*Z/std::pow ( G4double ( A ) , 1.0/3.0 )
                  - Asym * ( N - Z )* ( N - Z ) / A; 

      mass = Z * G4Proton::Proton()->GetPDGMass() 
           + N * G4Neutron::Neutron()->GetPDGMass()
           - BE;

   }

   return mass;
}



void G4QMDNucleus::CalEnergyAndAngularMomentumInCM()
{

   //G4cout << "CalEnergyAndAngularMomentumInCM " << this->GetAtomicNumber() << " " << GetMassNumber() << G4endl;

   G4double gamma = Get4Momentum().gamma();
   G4ThreeVector beta = Get4Momentum().v()/ Get4Momentum().e();

   G4ThreeVector pcm0( 0.0 ) ;

   G4int n = GetTotalNumberOfParticipant();
   pcm.resize( n );

   for ( G4int i= 0; i < n ; i++ ) 
   {
      G4ThreeVector p_i = GetParticipant( i )->GetMomentum();

      G4double trans = gamma / ( gamma + 1.0 ) * p_i * beta; 
      pcm[i] = p_i - trans*beta;

      pcm0 += pcm[i];
   }

   pcm0 = pcm0 / double ( n );

   //G4cout << "pcm0 " << pcm0 << G4endl;

   for ( G4int i= 0; i < n ; i++ ) 
   {
      pcm[i] += -pcm0;
      //G4cout << "pcm " << i << " " << pcm[i] << G4endl;
   }


   G4double tmass = 0;
   G4ThreeVector rcm0( 0.0 ) ;
   rcm.resize( n );
   es.resize( n );

   for ( G4int i= 0; i < n ; i++ ) 
   {
      G4ThreeVector ri = GetParticipant( i )->GetPosition();
      G4double trans = gamma / ( gamma + 1.0 ) * ri * beta; 

      es[i] = std::sqrt ( std::pow ( GetParticipant( i )->GetMass() , 2 ) + pcm[i]*pcm[i] );

      rcm[i] = ri + trans*beta;

      rcm0 += rcm[i]*es[i];

      tmass += es[i];
   }

   rcm0 = rcm0/tmass;

   for ( G4int i= 0; i < n ; i++ ) 
   {
      rcm[i] += -rcm0;
      //G4cout << "rcm " << i << " " << rcm[i] << G4endl;
   }

// Angluar momentum

   G4ThreeVector rl ( 0.0 ); 
   for ( G4int i= 0; i < n ; i++ ) 
   {
      rl += rcm[i].cross ( pcm[i] );
   }

   jj = int ( std::sqrt ( rl*rl / hbc ) + 0.5 );


// kinetic energy per nucleon in CM

   G4double totalMass = 0.0;
   for ( G4int i= 0; i < n ; i++ ) 
   {
      // following two lines are equivalent
      //totalMass += GetParticipant( i )->GetDefinition()->GetPDGMass()/GeV;
      totalMass += GetParticipant( i )->GetMass();
   }

   kineticEnergyPerNucleon = ( std::accumulate ( es.begin() , es.end() , 0.0 ) - totalMass )/n;

// Total (not per nucleion ) Binding Energy 
   bindingEnergy =  ( std::accumulate ( es.begin() , es.end() , 0.0 ) -totalMass ) + potentialEnergy;

   //G4cout << "KineticEnergyPerNucleon in GeV " << kineticEnergyPerNucleon << G4endl;
   //G4cout << "KineticEnergySum in GeV " << std::accumulate ( es.begin() , es.end() , 0.0 ) - totalMass << G4endl;
   //G4cout << "PotentialEnergy in GeV " << potentialEnergy << G4endl;
   //G4cout << "BindingEnergy in GeV " << bindingEnergy << G4endl;
   //G4cout << "G4BindingEnergy in GeV " << G4NucleiPropertiesTable::GetBindingEnergy( GetAtomicNumber() , GetMassNumber() )/GeV << G4endl;

   excitationEnergy = bindingEnergy + G4NucleiPropertiesTable::GetBindingEnergy( GetAtomicNumber() , GetMassNumber() )/GeV;
   //G4cout << "excitationEnergy in GeV " << excitationEnergy << G4endl;
   if ( excitationEnergy < 0 ) excitationEnergy = 0.0; 

}
