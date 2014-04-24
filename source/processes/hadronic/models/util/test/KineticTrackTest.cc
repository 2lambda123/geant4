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
// $Id$
//
// ------------------------------------------------------------
//      GEANT 4 file
//
//
//             by Gunter Folger, June 2004.
//       class exercising G4KineticTrack class.
// ------------------------------------------------------------

#include "G4ParticleDefinition.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4KineticTrack.hh"
#include "G4KineticTrackVector.hh"
#include "G4ParticleTable.hh"

class KTVsum4momentum
{
  private: 
     G4LorentzVector sum;
  public:
     KTVsum4momentum() : sum(0) {}
     void operator () (G4KineticTrack * kt)
     {
        sum += (kt)->Get4Momentum();
     }
     operator G4LorentzVector()
     {
        return sum;
     }
};

void PrintKTVector(G4KineticTrackVector * ktv, std::string comment);

int main()
{

   G4ShortLivedConstructor ShortLived;
   ShortLived.ConstructParticle();
   G4int Encoding(32214);
   G4ParticleDefinition* N1720 = G4ParticleTable::GetParticleTable()->
                                 FindParticle(Encoding);
   if (N1720 == 0)
   {
     G4cout << "Particle with encoding "<<Encoding<<" does not exist!!!"<<G4endl;
     exit(-1);
   }
   G4double formationTime(0);
   G4ThreeVector position(0);
   G4double mass(1298);
   G4LorentzVector momentum(mass);
   G4KineticTrack kt(N1720,formationTime,position,momentum) ;

   for ( G4int i=0; i<100000; ++i)
   {
        G4KineticTrackVector * secs=kt.Decay();
	G4LorentzVector sum=std::for_each((*secs).begin(), (*secs).end(),
				   KTVsum4momentum());
				   
	if ( (sum.vect()-momentum.vect()).mag2() > 1e-16 ||
	     abs(sum.e()-mass) > 1e-10 )
	{
	     std::cout << " momentum sum : " << sum << std::endl;
	     PrintKTVector( secs, "decay products");
	}     
				   
	
   }

} 
	
	
//----------------------------------------------------------------------------
void PrintKTVector(G4KineticTrackVector * ktv, std::string comment)
//----------------------------------------------------------------------------
{
  if (comment.size() > 0 ) G4cout << comment << G4endl;
  G4cout << "  vector: " << ktv << ", number of tracks: " << ktv->size()
	 << G4endl;
  std::vector<G4KineticTrack *>::iterator i;
  G4int count;

  for(count = 0, i = ktv->begin(); i != ktv->end(); ++i, ++count)
  {
    G4KineticTrack * kt = *i;
    G4cout << "  track n. " << count << ", id: " << kt << G4endl;
    G4ThreeVector pos = kt->GetPosition();
    G4LorentzVector mom = kt->Get4Momentum();
    G4LorentzVector tmom = kt->GetTrackingMomentum();
    G4ParticleDefinition * definition = kt->GetDefinition();
    G4cout << "    definition: " << definition->GetPDGEncoding() << " pos: "
	   << 1/fermi*pos << " R: " << 1/fermi*pos.mag() << " 4mom: "
	   << 1/MeV*mom <<"Tr_mom" <<  1/MeV*tmom << " P: " << 1/MeV*mom.vect().mag() 
	   << " M: " << 1/MeV*mom.mag() << G4endl;
    G4cout <<"trackstatus: "<<kt->GetState()<<G4endl;
  }
}
