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

//  The main program for G4DiffElasticHadrNucleus class

#include  "G4Proton.hh"
#include  "G4DynamicParticle.hh"
#include  "G4DiffElasticHadrNucleus.hh"
#include  "Randomize.hh"
#include  "G4ParticleChange.hh"
#include  "G4Track.hh"
#include  "G4Nucleus.hh"
#include  "G4IonTable.hh"
#include  "globals.hh"
#include  "G4ParticleTable.hh"
#include  "G4NucleiProperties.hh"
#include  "iostream.h"

 int main()

 {
        G4double CrossSection, ElasticCrossSec, 
                 InelasticCrossSec, Energy, Momentum,
                 Tkin, aS, Ecm, Pcm, TetCM, Pi1;

                      Pi1 = 3.1416;

        G4double      px = 0;
        G4double      py = 0;
        G4double      pz = 1000;        //  MeV
        G4ThreeVector aVector(px, py, pz);

        G4Proton                 * aProton = G4Proton::Proton();
        G4DiffElasticHadrNucleus * aCrossSection = 
                                    new G4DiffElasticHadrNucleus;
        G4DynamicParticle        * aParticle = new G4DynamicParticle;
   
        aParticle->SetDefinition(aProton);

        G4double N = 28, Z = 14;

        G4Nucleus *   aNucleus = new G4Nucleus;

        aNucleus->SetParameters( N, Z);
        G4double hMass = 938.;          //  MeV

           for(G4int i1=1; i1<10; i1+=10)
             {
                 Tkin = i1*1100;        //  MeV
                 Momentum = std::sqrt(std::pow(Tkin+hMass,2)-
                   std::pow(hMass,2));

                 aVector.setZ(Momentum);
                 aParticle->SetMomentum(aVector);

       G4double  MomGeV   = Momentum/1000;      //  GeV
       G4double  TkinGeV  = Tkin/1000;          //  GeV
       G4double  hMassGeV = hMass/1000;         //  GeV

                 aS  = 2*hMassGeV*N*(TkinGeV+hMassGeV)+
                                hMassGeV*hMassGeV*(1+N*N);       //GeV^2
                 Ecm = (aS-(N*N-1)*hMassGeV*hMassGeV)/2/std::sqrt(aS);// GeV  
                 Pcm = std::sqrt(Ecm*Ecm-hMassGeV*hMassGeV);          // GeV

  aCrossSection->GetNucleusParameters(aNucleus);

       G4double dQ2 = 75/aCrossSection->R1/
                         aCrossSection->R1/98;                   //Gev^2

//  G4cout << " Nucleus  "<<N<<"  R1, R2, p "
 //  << aCrossSection->R1<<"  " << aCrossSection->R2<<"  "
//   << aCrossSection->Pnucl<<G4endl;

//  G4cout << " Tkin "<<TkinGeV << " Mom " << MomGeV 
//         << " Pcm " << Pcm << endl;

               for(G4int i2=1; i2<100; i2++)
                  {
                    G4double Q2 = dQ2*(i2-1)*1000*1000+0.00001; // MeV

             CrossSection = aCrossSection->HadrNuclDifferCrSec(
                          aParticle, aNucleus, Q2);

              TetCM = std::acos(1-Q2/1000/1000/2/Pcm/Pcm)*180/Pi1;

//  G4cout << i1  <<" "<<  MomGeV <<" "  <<  Q2/1000 << " "
//       << CrossSection*Pcm*Pcm/Pi1 << endl;

//    G4cout <<" Mom "<<MomGeV<<" Ecm "<<Ecm<<" Pcm "<<Pcm<<" Q2 "<<
//              Q2/1000/1000 << " Tet " << 
//               TetCM << " Cr.-Sec. "<< CrossSection   << endl;

   G4cout<<Q2/1000/1000<<"    "<<TetCM<<"    "<<CrossSection<<G4endl;

                   }  //  i2
               }      //  i1
 
     delete aParticle;
     return 0;
  }           

