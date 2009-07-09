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
// $Id: G4ElasticHNScattering.cc,v 1.5 2009-07-09 15:14:09 vuzhinsk Exp $
// ------------------------------------------------------------
//      GEANT 4 class implemetation file
//
//      ---------------- G4ElasticHNScattering --------------
//                   by V. Uzhinsky, March 2008.
//             elastic scattering used by Fritiof model
//	           Take a projectile and a target
//	           scatter the projectile and target
// ---------------------------------------------------------------------


#include "globals.hh"
#include "Randomize.hh"

#include "G4ElasticHNScattering.hh"
#include "G4LorentzRotation.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4VSplitableHadron.hh"
#include "G4ExcitedString.hh"
#include "G4FTFParameters.hh"                            // Uzhi 29.03.08
//#include "G4ios.hh"

G4ElasticHNScattering::G4ElasticHNScattering()          
{
}

G4bool G4ElasticHNScattering::
           ElasticScattering (G4VSplitableHadron *projectile, 
                              G4VSplitableHadron *target,
                              G4FTFParameters    *theParameters) const
{
//G4cout<<"G4ElasticHNScattering::ElasticScattering"<<G4endl;
     G4int    ProjectilePDGcode=projectile->GetDefinition()->GetPDGEncoding(); // Uzhi 24.06.09
     G4ParticleDefinition * ProjectileDefinition = projectile->GetDefinition(); //Uzhi 24.06.09
//   G4int    absProjectilePDGcode=std::abs(ProjectilePDGcode);

	   G4LorentzVector Pprojectile=projectile->Get4Momentum();
           G4double ProjectileRapidity = Pprojectile.rapidity();            // Uzhi 24.06.09

           if(Pprojectile.z() < 0.){ return false;}  // Uzhi 9.07.09  true;}

// -------------------- Projectile parameters -----------------------------------
           G4bool PutOnMassShell=0;

	   G4double M0projectile = Pprojectile.mag();                        

           if(M0projectile < projectile->GetDefinition()->GetPDGMass()) 
             {
              PutOnMassShell=1;
              M0projectile=projectile->GetDefinition()->GetPDGMass();
             }

	   G4double Mprojectile2 = M0projectile * M0projectile;

//           G4double AveragePt2=theParameters->GetSlope(); // Uzhi ???
//           AveragePt2 = AveragePt2 * GeV*GeV;

           G4double AveragePt2=theParameters->GetAvaragePt2ofElasticScattering();

// -------------------- Target parameters ----------------------------------------------
     G4int    TargetPDGcode=target->GetDefinition()->GetPDGEncoding(); // Uzhi 24.06.09
//   G4int    absTargetPDGcode=std::abs(TargetPDGcode);

  	   G4LorentzVector Ptarget=target->Get4Momentum();
           G4double TargetRapidity = Ptarget.rapidity();            // Uzhi 24.06.09
	   G4double M0target = Ptarget.mag();
//G4cout<<" Mp Mt Pt2 "<<M0projectile<<" "<<M0target<<" "<<AveragePt2/GeV/GeV<<G4endl;

           if(M0target < target->GetDefinition()->GetPDGMass()) 
             {
              PutOnMassShell=1;
              M0target=target->GetDefinition()->GetPDGMass();
             }
     
   	   G4double Mtarget2 = M0target * M0target;                      //Ptarget.mag2(); 
                                                                         // for AA-inter.
//        (G4UniformRand() < 0.5))
//        (G4UniformRand() < 0.5*std::exp(-0.5*(ProjectileRapidity - TargetRapidity))))
//
     if((ProjectilePDGcode != TargetPDGcode) && 
        ((ProjectilePDGcode > 1000) && (TargetPDGcode > 1000)) &&  // Uzhi 6.07.09
        (G4UniformRand() < 1.0*std::exp(-0.5*(ProjectileRapidity - TargetRapidity)))) 
       {
        projectile->SetDefinition(target->GetDefinition());
        target->SetDefinition(ProjectileDefinition);                   // Uzhi 24.06.09
       }
//
// Transform momenta to cms and then rotate parallel to z axis;

	   G4LorentzVector Psum;
	   Psum=Pprojectile+Ptarget;

	   G4LorentzRotation toCms(-1*Psum.boostVector());

	   G4LorentzVector Ptmp=toCms*Pprojectile;

	   if ( Ptmp.pz() <= 0. )                                 // Uzhi ???
	   {
	   // "String" moving backwards in  CMS, abort collision !!
           //G4cout << " abort Collision!! " << G4endl;
		   return false; 
	   }
	   		   
	   toCms.rotateZ(-1*Ptmp.phi());
	   toCms.rotateY(-1*Ptmp.theta());
	
	   G4LorentzRotation toLab(toCms.inverse());

	   Pprojectile.transform(toCms);
	   Ptarget.transform(toCms);

// ---------------------- Sampling of transfered Pt ------------------------
	   G4double Pt2;                                                    
           G4double ProjMassT2, ProjMassT;                                  
           G4double TargMassT2, TargMassT;                                  
           G4double PZcms2, PZcms;                                          

           G4double S=Psum.mag2();                                          
//           G4double SqrtS=std::sqrt(S);                                     

	   PZcms2=(S*S+Mprojectile2*Mprojectile2+Mtarget2*Mtarget2-
                                 2*S*Mprojectile2-2*S*Mtarget2-2*Mprojectile2*Mtarget2)/4./S;

           if(PZcms2 < 0.)
           {  // It can be in an interaction with off-shell nuclear nucleon
            if(M0projectile > projectile->GetDefinition()->GetPDGMass()) 
            {  // An attempt to de-excite the projectile
             M0projectile = projectile->GetDefinition()->GetPDGMass();
             Mprojectile2=M0projectile*M0projectile;
             PZcms2=(S*S+Mprojectile2*Mprojectile2+Mtarget2*Mtarget2-
                    2*S*Mprojectile2 - 2*S*Mtarget2 - 2*Mprojectile2*Mtarget2)
                    /4./S;
             if(PZcms2 < 0.){ return true;} // Non succesful attempt
            }
            else // if(M0projectile > projectile->GetDefinition()->GetPDGMass())
            {
             return true;                  // We can do only one - skip the interaction!
            }
           }

           PZcms = std::sqrt(PZcms2);

           if(PutOnMassShell)
           {
              if(Pprojectile.z() > 0.)
              {
                 Pprojectile.setPz( PZcms);
                 Ptarget.setPz(    -PZcms);
              }
              else  // if(Pprojectile.z() > 0.)
              {
                 Pprojectile.setPz(-PZcms);
                 Ptarget.setPz(     PZcms);
              };

              Pprojectile.setE(std::sqrt(Mprojectile2+
                                                      Pprojectile.x()*Pprojectile.x()+
                                                      Pprojectile.y()*Pprojectile.y()+
                                                      PZcms2));
              Ptarget.setE(std::sqrt(    Mtarget2    +
                                                      Ptarget.x()*Ptarget.x()+
                                                      Ptarget.y()*Ptarget.y()+
                                                      PZcms2));
           }

           G4double maxPtSquare = PZcms2;
	   G4LorentzVector Qmomentum;

	   Qmomentum=G4LorentzVector(GaussianPt(AveragePt2,maxPtSquare),0);

	   Pt2=G4ThreeVector(Qmomentum.vect()).mag2();                  
//G4cout<<"Pt2 GeV^2 "<<(Pt2)/GeV/GeV<<G4endl;

           ProjMassT2=Mprojectile2+Pt2;                           
           ProjMassT =std::sqrt(ProjMassT2);                            

           TargMassT2=Mtarget2+Pt2;                               
           TargMassT =std::sqrt(TargMassT2);                            

           PZcms2=(S*S+ProjMassT2*ProjMassT2+                           
                       TargMassT2*TargMassT2-                           
                    2.*S*ProjMassT2-2.*S*TargMassT2-                 
                    2.*ProjMassT2*TargMassT2)/4./S;                  
           if(PZcms2 < 0 ) {PZcms2=0;};                                 
           PZcms =std::sqrt(PZcms2);                                    

	   Pprojectile.setPz( PZcms);  // Uzhi Proj can move backward
	   Ptarget.setPz(    -PZcms);  // Uzhi Proj can move backward

//G4cout << "Qplus / Qminus " << Qplus << " / " << Qminus<<G4endl;
//	     G4cout << "pt2" << pt2 << G4endl;
//	     G4cout << "Qmomentum " << Qmomentum << G4endl;
//	     G4cout << " Masses (P/T) : " << (Pprojectile+Qmomentum).mag() <<
//	   		       " / " << (Ptarget-Qmomentum).mag() << G4endl;

	   Pprojectile += Qmomentum;
	   Ptarget     -= Qmomentum;

//G4cout << "Pprojectile with Q : " << Pprojectile << G4endl;
//G4cout << "Ptarget with Q : " << Ptarget << G4endl;
	
//	   G4cout << "Projectile back: " << toLab * Pprojectile << G4endl;
//	   G4cout << "Target back: " << toLab * Ptarget << G4endl;

// Transform back and update SplitableHadron Participant.
	   Pprojectile.transform(toLab);
	   Ptarget.transform(toLab);

//G4cout << "Pprojectile with Q M: " << Pprojectile<<" "<<  Pprojectile.mag() << G4endl;
//G4cout << "Ptarget     with Q M: " << Ptarget    <<" "<<  Ptarget.mag()     << G4endl;

//G4cout << "Target	 mass  " <<  Ptarget.mag() << G4endl;     
	   		   
//G4cout << "Projectile mass  " <<  Pprojectile.mag() << G4endl; 

           G4double ZcoordinateOfCurrentInteraction = target->GetPosition().z();
// It is assumed that nucleon z-coordinates are ordered on increasing -----------

           G4double betta_z=projectile->Get4Momentum().pz()/projectile->Get4Momentum().e();

           G4double ZcoordinateOfPreviousCollision=projectile->GetPosition().z();
           if(projectile->GetSoftCollisionCount()==0) {
              projectile->SetTimeOfCreation(0.);
              target->SetTimeOfCreation(0.);
              ZcoordinateOfPreviousCollision=ZcoordinateOfCurrentInteraction;
           }
           
           G4ThreeVector thePosition(projectile->GetPosition().x(),
                                     projectile->GetPosition().y(),
                                     ZcoordinateOfCurrentInteraction);
           projectile->SetPosition(thePosition);

           G4double TimeOfPreviousCollision=projectile->GetTimeOfCreation();
           G4double TimeOfCurrentCollision=TimeOfPreviousCollision+ 
                    (ZcoordinateOfCurrentInteraction-ZcoordinateOfPreviousCollision)/betta_z; 

           projectile->SetTimeOfCreation(TimeOfCurrentCollision);
           target->SetTimeOfCreation(TimeOfCurrentCollision);

	   projectile->Set4Momentum(Pprojectile);
	   target->Set4Momentum(Ptarget);

           projectile->IncrementCollisionCount(1);
           target->IncrementCollisionCount(1);

	   return true;
}


// --------- private methods ----------------------

G4ThreeVector G4ElasticHNScattering::GaussianPt(G4double AveragePt2, G4double maxPtSquare) const
{            //  @@ this method is used in FTFModel as well. Should go somewhere common!
	
	G4double Pt2;
        Pt2 = -AveragePt2 * std::log(1. + G4UniformRand() * 
                (std::exp(-maxPtSquare/AveragePt2)-1.)); 
	
	G4double Pt=std::sqrt(Pt2);
	G4double phi=G4UniformRand() * twopi;
	
	return G4ThreeVector (Pt*std::cos(phi), Pt*std::sin(phi), 0.);    
}

G4ElasticHNScattering::G4ElasticHNScattering(const G4ElasticHNScattering &)
{
	throw G4HadronicException(__FILE__, __LINE__, "G4ElasticHNScattering copy contructor not meant to be called");
}


G4ElasticHNScattering::~G4ElasticHNScattering()
{
}


const G4ElasticHNScattering & G4ElasticHNScattering::operator=(const G4ElasticHNScattering &)
{
	throw G4HadronicException(__FILE__, __LINE__, "G4ElasticHNScattering = operator meant to be called");
	return *this;
}


int G4ElasticHNScattering::operator==(const G4ElasticHNScattering &) const
{
	throw G4HadronicException(__FILE__, __LINE__, "G4ElasticHNScattering == operator meant to be called");
	return false;
}

int G4ElasticHNScattering::operator!=(const G4ElasticHNScattering &) const
{
	throw G4HadronicException(__FILE__, __LINE__, "G4ElasticHNScattering != operator meant to be called");
	return true;
}
