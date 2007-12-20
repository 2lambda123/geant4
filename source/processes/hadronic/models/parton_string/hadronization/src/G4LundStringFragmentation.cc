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
// $Id: G4LundStringFragmentation.cc,v 1.8 2007-12-20 15:38:07 vuzhinsk Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -----------------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, Maxim Komogorov, 10-Jul-1998
// -----------------------------------------------------------------------------
#include "G4LundStringFragmentation.hh"
#include "G4FragmentingString.hh"
#include "G4DiQuarks.hh"
#include "G4Quarks.hh"

#include "Randomize.hh"

// Class G4LundStringFragmentation 
//*************************************************************************************

G4LundStringFragmentation::G4LundStringFragmentation()
   {
   MinimalStringMass  = 0.;              // Uzhi
   MinimalStringMass2 = 0.;              // Uzhi
   WminLUND = 1.5*GeV;                   // Uzhi 0.8 1.5
   SmoothParam  = 0.2;                   // Uzhi 
   }

G4LundStringFragmentation::G4LundStringFragmentation(const G4LundStringFragmentation &) : G4VLongitudinalStringDecay()
   {
   }


G4LundStringFragmentation::~G4LundStringFragmentation()
   { 
   }

//*************************************************************************************

const G4LundStringFragmentation & G4LundStringFragmentation::operator=(const G4LundStringFragmentation &)
   {
     throw G4HadronicException(__FILE__, __LINE__, "G4LundStringFragmentation::operator= meant to not be accessable");
     return *this;
   }

int G4LundStringFragmentation::operator==(const G4LundStringFragmentation &right) const
   {
   return !memcmp(this, &right, sizeof(G4LundStringFragmentation));
   }

int G4LundStringFragmentation::operator!=(const G4LundStringFragmentation &right) const
   {
   return memcmp(this, &right, sizeof(G4LundStringFragmentation));
   }

//**************************************************************************************
//--------------------------------------------------------------------------------------

void G4LundStringFragmentation::SetMinimalStringMass(const G4FragmentingString  * const string)  // Uzhi
{
//G4cout<<"In SetMinMass -------------------"<<std::sqrt(string->Mass2())<<G4endl;
//G4cout<<string->GetLeftParton()->GetPDGEncoding()<<G4endl;
//G4cout<<string->GetRightParton()->GetPDGEncoding()<<G4endl;

  G4double EstimatedMass=0.;  
  G4int Number_of_quarks=0;
  G4double Mass_of_light_quark    =140.*MeV;
  G4double Mass_of_heavy_quark    =500.*MeV;
  G4double Mass_of_string_junction=720.*MeV;

  G4int Qleft =std::abs(string->GetLeftParton()->GetPDGEncoding());

  if( Qleft > 1000) 
    {
     Number_of_quarks+=2;
     G4int q1=Qleft/1000;
     if( q1 < 3) {EstimatedMass +=Mass_of_light_quark;}  // Uzhi
     if( q1 > 2) {EstimatedMass +=Mass_of_heavy_quark;}     

     G4int q2=(Qleft/100)%10;
     if( q2 < 3) {EstimatedMass +=Mass_of_light_quark;}  // Uzhi
     if( q2 > 2) {EstimatedMass +=Mass_of_heavy_quark;}
     EstimatedMass +=Mass_of_string_junction; 
    }
  else
    {
     Number_of_quarks++;
     if( Qleft < 3) {EstimatedMass +=Mass_of_light_quark;}  // Uzhi
     if( Qleft > 2) {EstimatedMass +=Mass_of_heavy_quark;} 
    }

  G4int Qright=std::abs(string->GetRightParton()->GetPDGEncoding());

  if( Qright > 1000) 
    {
     Number_of_quarks+=2;
     G4int q1=Qright/1000;
     if( q1 < 3) {EstimatedMass +=Mass_of_light_quark;}  // Uzhi
     if( q1 > 2) {EstimatedMass +=Mass_of_heavy_quark;}     

     G4int q2=(Qright/100)%10;
     if( q2 < 3) {EstimatedMass +=Mass_of_light_quark;}  // Uzhi
     if( q2 > 2) {EstimatedMass +=Mass_of_heavy_quark;} 
     EstimatedMass +=Mass_of_string_junction; 
    }
  else
    {
     Number_of_quarks++;
     if( Qright < 3) {EstimatedMass +=Mass_of_light_quark;} // 
     if( Qright > 2) {EstimatedMass +=Mass_of_heavy_quark;} 
    }

  if(Number_of_quarks==2){EstimatedMass +=100.*MeV;}
  if(Number_of_quarks==3){EstimatedMass += 20.*MeV;}
  if(Number_of_quarks==4){EstimatedMass -=2.*Mass_of_string_junction;
                          if(EstimatedMass <= 1600.*MeV){EstimatedMass-=200.*MeV;}
                          else                          {EstimatedMass+=100.*MeV;}
                                                                                  }
  MinimalStringMass=EstimatedMass;
  SetMinimalStringMass2(EstimatedMass);
//G4cout<<"Out SetMinimalStringMass "<<MinimalStringMass<<G4endl;
}

//--------------------------------------------------------------------------------------
void G4LundStringFragmentation::SetMinimalStringMass2(const G4double aValue)                     // Uzhi
{
  MinimalStringMass2=aValue * aValue;
}

//*************************************************************************************
//--------------------------------------------------------------------------------------

G4KineticTrackVector* G4LundStringFragmentation::FragmentString(const G4ExcitedString& theString)
{

//G4cout<<"In FragmentString"<<G4endl;

//    Can no longer modify Parameters for Fragmentation.
	PastInitPhase=true;
	
// 	check if string has enough mass to fragment...
        
        SetMassCut(160.*MeV); // For LightFragmentationTest it is required
                              // that no one pi-meson can be produced

	G4KineticTrackVector * LeftVector=LightFragmentationTest(&theString);
	if ( LeftVector != 0 ) {
//G4cout<<"Return single hadron insted of string"<<G4endl; 
                                return LeftVector;}

//--------------------- The string can fragment -------------------------------	
//--------------- At least two particles can be produced ----------------------

                               LeftVector =new G4KineticTrackVector;
	G4KineticTrackVector * RightVector=new G4KineticTrackVector;

// this should work but its only a semi deep copy. %GF	G4ExcitedString theStringInCMS(theString);

        G4ExcitedString *theStringInCMS=CPExcited(theString);
	G4LorentzRotation toCms=theStringInCMS->TransformToAlignedCms();

	G4bool success=false, inner_sucess=true;
	G4int attempt=0;
	while ( !success && attempt++ < StringLoopInterrupt )
	{
		G4FragmentingString *currentString=new G4FragmentingString(*theStringInCMS);

//G4cout<<"FragmentString cur M2  "<<std::sqrt(currentString->Mass2())<<G4endl;

		std::for_each(LeftVector->begin() , LeftVector->end() , DeleteKineticTrack());
		LeftVector->clear();

		std::for_each(RightVector->begin(), RightVector->end(), DeleteKineticTrack());
		RightVector->clear();
		
		inner_sucess=true;  // set false on failure..
		while (! StopFragmenting(currentString) )
		{  // Split current string into hadron + new string

//			G4FragmentingString *PreviousString=currentString;   // Uzhi

			G4FragmentingString *newString=0;  // used as output from SplitUp...

//G4cout<<"FragmentString to Splitup ===================================="<<G4endl; 
//G4cout<<"++++++++++++++++++++++++++ Enter num--------------------------"<<G4endl;
//G4int Uzhi; G4cin>>Uzhi;                         // Uzhi

			G4KineticTrack * Hadron=Splitup(currentString,newString);
//G4cout<<" Hadron "<<Hadron<<G4endl;

//			if ( Hadron != 0 && IsFragmentable(newString))       // Uzhi
			if ( Hadron != 0 )                                   // Uzhi
			{
			   if ( currentString->GetDecayDirection() > 0 )
				   LeftVector->push_back(Hadron);
       			   else
	  			   RightVector->push_back(Hadron);
			   delete currentString;
			   currentString=newString;
			} /* else {                                // Uzhi
			 // abandon ... start from the beginning
			   if (newString) delete newString;                  // ??? Uzhi local?
			   if (Hadron)    delete Hadron;
//                           currentString = PreviousString;                 // Uzhi
			   inner_sucess=false;
			   break;
			} */                                      // Uzhi 
//                        delete PreviousString;                             // ??? Uzhi local?
		}; 
		// Split current string into 2 final Hadrons
//G4cout<<"FragmentString to SplitLast if inner_sucess#0"<<inner_sucess<<G4endl;
		if ( inner_sucess &&                                         // Uzhi
		     SplitLast(currentString,LeftVector, RightVector) ) 
		{
			success=true;
		}
		delete currentString;
	}
	
	delete theStringInCMS;
	
	if ( ! success )
	{
		std::for_each(LeftVector->begin(), LeftVector->end(), DeleteKineticTrack());
		LeftVector->clear();
		std::for_each(RightVector->begin(), RightVector->end(), DeleteKineticTrack());
		delete RightVector;
		return LeftVector;
	}
		
	// Join Left- and RightVector into LeftVector in correct order.
	while(!RightVector->empty())
	{
	    LeftVector->push_back(RightVector->back());
	    RightVector->erase(RightVector->end()-1);
	}
	delete RightVector;

//G4cout<<"CalculateHadronTimePosition"<<G4endl;

	CalculateHadronTimePosition(theString.Get4Momentum().mag(), LeftVector);

	G4LorentzRotation toObserverFrame(toCms.inverse());

	for(size_t C1 = 0; C1 < LeftVector->size(); C1++)
	{
	   G4KineticTrack* Hadron = LeftVector->operator[](C1);
	   G4LorentzVector Momentum = Hadron->Get4Momentum();
	   Momentum = toObserverFrame*Momentum;
	   Hadron->Set4Momentum(Momentum);
	   G4LorentzVector Coordinate(Hadron->GetPosition(), Hadron->GetFormationTime());
	   Momentum = toObserverFrame*Coordinate;
	   Hadron->SetFormationTime(Momentum.e());
	   G4ThreeVector aPosition(Momentum.vect());
	   Hadron->SetPosition(theString.GetPosition()+aPosition);
	}

//G4cout<<"Out FragmentString"<<G4endl<<G4endl<<G4endl;
	return LeftVector;
		
}

//----------------------------------------------------------------------------------------

G4bool G4LundStringFragmentation::StopFragmenting(const G4FragmentingString * const string)
{
//G4cout<<"StopFragmenting"<<G4endl;

  SetMinimalStringMass(string);                                           // Uzhi

//G4cout<<"StopFragm MinMass "<<MinimalStringMass<<" String Mass "<<std::sqrt(string->Get4Momentum().mag2())<<G4endl; 

  return (MinimalStringMass + WminLUND)*
             (1 + SmoothParam * (1.-2*G4UniformRand())) >                // Uzhi
                   string->Mass();                         // Uzhi

//       sqr(FragmentationMass(string,&G4HadronBuilder::BuildHighSpin)+MassCut) >   // Uzhi
//       string->Get4Momentum().mag2();                                             // Uzhi
}

//----------------------------------------------------------------------------------

G4bool G4LundStringFragmentation::IsFragmentable(const G4FragmentingString * const string)
{
//G4cout<<"In IsFragmentable"<<G4endl;
  SetMinimalStringMass(string);                                                     // Uzhi
//G4cout<<"Out IsFragmentable MinMass"<<MinimalStringMass<<" String Mass"<<std::sqrt(string->Get4Momentum().mag2())<<G4endl; 
  return sqr(MinimalStringMass + WminLUND) < string->Get4Momentum().mag2();         // Uzhi

//	return sqr(FragmentationMass(string)+MassCut) <                             // Uzhi
//			string->Mass2();                                            // Uzhi
}

//----------------------------------------------------------------------------------------

G4bool G4LundStringFragmentation::SplitLast(G4FragmentingString * string,
					     G4KineticTrackVector * LeftVector,
    					     G4KineticTrackVector * RightVector)
{
    //... perform last cluster decay
//G4cout<<"SplitLast String mass "<<string->Mass()<<G4endl;
//G4cout<<string->GetLeftParton()->GetPDGEncoding()<<" "<<G4endl;
//G4cout<<string->GetRightParton()->GetPDGEncoding()<<" "<<G4endl;

G4LorentzVector Str4Mom=string->Get4Momentum();
//G4cout<<"String 4 momentum "<<Str4Mom<<G4endl;

    G4ThreeVector ClusterVel =string->Get4Momentum().boostVector();

    G4double ResidualMass   = string->Mass(); 
//    G4double ClusterMassCut = ClusterMass;

    G4int cClusterInterrupt = 0;

    G4ParticleDefinition * LeftHadron, * RightHadron;
    do
    {
//G4cout<<" Cicle "<<cClusterInterrupt<<" "<< ClusterLoopInterrupt<<G4endl;

        if (cClusterInterrupt++ >= ClusterLoopInterrupt)
        {
          return false;
        }
	G4ParticleDefinition * quark = NULL;
	string->SetLeftPartonStable(); // to query quark contents..

	if (string->DecayIsQuark() && string->StableIsQuark() ) 
	{
	   //... there are quarks on cluster ends
		LeftHadron= QuarkSplitup(string->GetLeftParton(), quark);
	} else {
	   //... there is a Diquark on cluster ends
		G4int IsParticle;

		if ( string->StableIsQuark() ) {
		  IsParticle=(string->GetLeftParton()->GetPDGEncoding()>0) ? -1 : +1; 
		} else {
		  IsParticle=(string->GetLeftParton()->GetPDGEncoding()>0) ? +1 : -1;
		}

      		pDefPair QuarkPair = CreatePartonPair(IsParticle,false);  // no diquarks wanted
      		quark = QuarkPair.second;

      		LeftHadron=hadronizer->Build(QuarkPair.first, string->GetLeftParton());
	}

        RightHadron = hadronizer->Build(string->GetRightParton(), quark);

//G4cout<<"SplitLast Left Right hadrons "<<LeftHadron->GetPDGEncoding()<<" "<<RightHadron->GetPDGEncoding()<<G4endl;
//G4cout<<"SplitLast Left Right hadrons "<<LeftHadron->GetPDGMass()<<" "<<RightHadron->GetPDGMass()<<G4endl;
//G4cout<<"Sum H mass Str Mass "<<LeftHadron->GetPDGMass() + RightHadron->GetPDGMass()<<" "<<ResidualMass<<G4endl;

       //... repeat procedure, if mass of cluster is too low to produce hadrons
       //... ClusterMassCut = 0.15*GeV model parameter
//	if ( quark->GetParticleSubType()== "quark" ) {ClusterMassCut = 0.;}      // Uzhi
//	else {ClusterMassCut = ClusterMass;}                                     // Uzhi

    } 
    while (ResidualMass <= LeftHadron->GetPDGMass() + RightHadron->GetPDGMass());// UzhiVOVA
//    while (ResidualMass <= LeftHadron->GetPDGMass() + RightHadron->GetPDGMass()  + ClusterMassCut); // Uzhi
    //... compute hadron momenta and energies   

    G4LorentzVector  LeftMom, RightMom;
    G4ThreeVector    Pos;

    Sample4Momentum(&LeftMom,  LeftHadron->GetPDGMass(), 
                    &RightMom, RightHadron->GetPDGMass(), 
                    ResidualMass);

    LeftMom.boost(ClusterVel);
    RightMom.boost(ClusterVel);

    LeftVector->push_back(new G4KineticTrack(LeftHadron, 0, Pos, LeftMom));
    RightVector->push_back(new G4KineticTrack(RightHadron, 0, Pos, RightMom));

//G4cout<<"Out SplitLast "<<G4endl;
    return true;

}

//----------------------------------------------------------------------------------------------------------

void G4LundStringFragmentation::Sample4Momentum(G4LorentzVector* Mom, G4double Mass, G4LorentzVector* AntiMom, G4double AntiMass, G4double InitialMass) 
    {
     G4ThreeVector Pt;                                                      // Uzhi
     G4double MassMt2, AntiMassMt2;                                         // Uzhi
     G4double AvailablePz, AvailablePz2;                                    // Uzhi

//G4cout<<"Sample4Momentum "<<G4endl;
//G4cout<<"Sample4Momentum Mass"<<Mass<<" "<<AntiMass<<" "<<InitialMass<<G4endl; 
                                                                            // Uzhi
    do                                                                      // Uzhi
      {                                                                     // Uzhi
       Pt=SampleQuarkPt(); Pt.setZ(0); G4double Pt2=Pt.mag2();              // Uzhi

//G4cout<<"Sample4Momentum Pt x y "<<Pt.getX()<<" "<<Pt.getY()<<G4endl;

       MassMt2    =     Mass *     Mass + Pt2;                              // Uzhi
       AntiMassMt2= AntiMass * AntiMass + Pt2;                              // Uzhi

//G4cout<<"Mts "<<MassMt2<<" "<<AntiMassMt2<<" "<<InitialMass*InitialMass<<G4endl;

       AvailablePz2= sqr(InitialMass*InitialMass - MassMt2 - AntiMassMt2) - 
                     4.*MassMt2*AntiMassMt2;                                // Uzhi
      }                                                                     // Uzhi
    while(AvailablePz2 < 0.);                                               // Uzhi
                                                                            // Uzhi
    AvailablePz2 /=(4.*InitialMass*InitialMass);                            // Uzhi
                                                                            // Uzhi
    AvailablePz = std::sqrt(AvailablePz2);                                  // Uzhi

//G4cout<<"AvailablePz "<<AvailablePz<<G4endl;
 

    G4double Px=Pt.getX();                                                  // Uzhi
    G4double Py=Pt.getY();                                                  // Uzhi

//if(Mass > AntiMass){AvailablePz=-AvailablePz;}  // May30                   // Uzhi

    Mom->setPx(Px); Mom->setPy(Py); Mom->setPz(AvailablePz);                // Uzhi
    Mom->setE(std::sqrt(MassMt2+AvailablePz2));                             // Uzhi

//G4cout<<" 1 part "<<Px<<"  "<<Py<<" "<<AvailablePz<<" "<<std::sqrt(MassMt2+AvailablePz2)<<G4endl;

                                                                            // Uzhi
    AntiMom->setPx(-Px); AntiMom->setPy(-Py); AntiMom->setPz(-AvailablePz); // Uzhi
    AntiMom->setE (std::sqrt(AntiMassMt2+AvailablePz2));                    // Uzhi

//G4cout<<" 2 part "<<-Px<<"  "<<-Py<<" "<<-AvailablePz<<" "<<std::sqrt(AntiMassMt2+AvailablePz2)<<G4endl;

// Maybe it must be inversed!                                               // Uzhi
/*                                                                          // Uzhi
    G4double r_val = sqr(InitialMass*InitialMass - Mass*Mass - AntiMass*AntiMass) - sqr(2.*Mass*AntiMass);
    G4double Pabs = (r_val > 0.)? std::sqrt(r_val)/(2.*InitialMass) : 0;

    //... sample unit vector       
    G4double pz = 1. - 2.*G4UniformRand();  
    G4double st     = std::sqrt(1. - pz * pz)*Pabs;
    G4double phi    = 2.*pi*G4UniformRand();
    G4double px = st*std::cos(phi);
    G4double py = st*std::sin(phi);
    pz *= Pabs;
    
    Mom->setPx(px); Mom->setPy(py); Mom->setPz(pz);
    Mom->setE(std::sqrt(Pabs*Pabs + Mass*Mass));

    AntiMom->setPx(-px); AntiMom->setPy(-py); AntiMom->setPz(-pz);
    AntiMom->setE (std::sqrt(Pabs*Pabs + AntiMass*AntiMass));
*/                                                                           // Uzhi
//G4cout<<"Out Sample4Momentum "<<G4endl;
    }

//-----------------------------------------------------------------------------

G4LorentzVector * G4LundStringFragmentation::SplitEandP(G4ParticleDefinition * pHadron,
	G4FragmentingString * string, G4FragmentingString * newString)
{       
//G4cout<<"SplitEandP "<<G4endl;
//G4cout<<"SplitEandP string mass "<<string->Mass()<<G4endl;   
//G4cout<<string->GetLeftParton()->GetPDGEncoding()<<" "<<G4endl;
//G4cout<<string->GetRightParton()->GetPDGEncoding()<<" "<<G4endl;
//G4cout<<G4endl;
//G4cout<<newString->GetLeftParton()->GetPDGEncoding()<<" "<<G4endl;
//G4cout<<newString->GetRightParton()->GetPDGEncoding()<<" "<<G4endl;

       G4LorentzVector String4Momentum=string->Get4Momentum();
       G4double StringMT2=string->Get4Momentum().mt2();
//G4cout<<"StringMt2 "<<StringMT2<<G4endl;

       G4double HadronMass = pHadron->GetPDGMass();
//G4cout<<"Hadron mass "<<HadronMass<<" "<<pHadron->GetParticleName()<<G4endl;   

       SetMinimalStringMass(newString);                                         // Uzhi
 
       String4Momentum.setPz(0.);
       G4ThreeVector StringPt=String4Momentum.vect();
//G4cout<<"StringPt "<<StringPt<<G4endl<<G4endl;

//G4cout<<"Min string mass "<<MinimalStringMass<<G4endl;

// calculate and assign hadron transverse momentum component HadronPx and HadronPy
       G4ThreeVector thePt;
       thePt=SampleQuarkPt();

       G4ThreeVector HadronPt = thePt +string->DecayPt();
       HadronPt.setZ(0);
//G4cout<<"Hadron Pt"<<HadronPt<<G4endl;

       G4ThreeVector RemSysPt = StringPt - HadronPt;
//G4cout<<"RemSys Pt"<<RemSysPt<<G4endl;

       //...  sample z to define hadron longitudinal momentum and energy
       //... but first check the available phase space

       G4double HadronMassT2 = sqr(HadronMass) + HadronPt.mag2();
       G4double ResidualMassT2=sqr(MinimalStringMass) + RemSysPt.mag2(); // Uzhi

//G4cout<<"Mt h res str "<<std::sqrt(HadronMassT2)<<" "<<std::sqrt(ResidualMassT2)<<" srt mass"<<StringMT2<<G4endl;

        G4double Pz2 = (sqr(StringMT2 - HadronMassT2 - ResidualMassT2) -             // Uzhi
                                      4*HadronMassT2 * ResidualMassT2)/4./StringMT2; // Uzhi

//G4cout<<"Pz**2 "<<Pz2<<G4endl;

        if(Pz2 < 0 ) {return 0;}          // have to start all over!            // Uzhi

       //... then compute allowed z region  z_min <= z <= z_max 
 
       G4double Pz = std::sqrt(Pz2);                                           // Uzhi
       G4double zMin = (std::sqrt(HadronMassT2+Pz2) - Pz)/std::sqrt(StringMT2);  // Uzhi
       G4double zMax = (std::sqrt(HadronMassT2+Pz2) + Pz)/std::sqrt(StringMT2);  // Uzhi

//G4cout<<"Zmin max "<<zMin<<" "<<zMax<<G4endl;               // Uzhi

       if (zMin >= zMax) return 0;		// have to start all over!
	
       G4double z = GetLightConeZ(zMin, zMax,
		       string->GetDecayParton()->GetPDGEncoding(), pHadron,
		       HadronPt.x(), HadronPt.y());      
       
       //... now compute hadron longitudinal momentum and energy
       // longitudinal hadron momentum component HadronPz

        HadronPt.setZ(0.5* string->GetDecayDirection() *
			(z * string->LightConeDecay() - 
			 HadronMassT2/(z * string->LightConeDecay())));

        G4double HadronE  = 0.5* (z * string->LightConeDecay() + 
				  HadronMassT2/(z * string->LightConeDecay()));

       G4LorentzVector * a4Momentum= new G4LorentzVector(HadronPt,HadronE);

//G4cout<<"Hadron Pt"<<HadronPt<<G4endl;
//G4cout<<"Out of SplitEandP Pz E "<<HadronPt.getZ()<<" "<<HadronE<<G4endl;

       return a4Momentum;
}

//-----------------------------------------------------------------------------------------

//G4double G4LundStringFragmentation::GetLightConeZ(G4double zmin, G4double zmax,        // Uzhi
//                                                  G4int ,  G4ParticleDefinition* pHadron, // Uzhi
G4double G4LundStringFragmentation::GetLightConeZ(G4double zmin, G4double zmax, 
                                                  G4int,  G4ParticleDefinition* pHadron, // Uzhi
G4double Px, G4double Py)
    {
    const G4double  alund = 0.7/GeV/GeV;                 // Uzhi may 2007 0.7 Std

//    If blund get restored, you MUST adapt the calculation of zOfMaxyf.
//    const G4double  blund = 1;

    G4double z, yf;
    G4double Mass = pHadron->GetPDGMass();
    G4int PartonEncoding=pHadron->GetPDGEncoding();
    
    G4double Mt2 = Px*Px + Py*Py + Mass*Mass;
    G4double zOfMaxyf=alund*Mt2/(alund*Mt2 + 1.);
    G4double maxYf=(1-zOfMaxyf)/zOfMaxyf * std::exp(-alund*Mt2/zOfMaxyf);

//    G4double zOfMaxyf=zmin;                                              // Vova
                    //alund*Mt2/(alund*Mt2 + 1.);
//    G4double maxYf=(1-zOfMaxyf)*(1-zOfMaxyf);                            // Vova
                 //(1-zOfMaxyf)/zOfMaxyf * std::exp(-alund*Mt2/zOfMaxyf);

    G4double N=1.5;                                                 // Uzhi
    G4double OverN=1./N;                                           // Uzhi
    G4double ZminN=std::pow(zmin,N);                               // Uzhi
    G4double ZmaxN=std::pow(zmax,N);                               // Uzhi
    G4double Brac=ZmaxN-ZminN;                                     // Uzhi

//G4cout<<" ZminN ZmaxN Brac Code "<<ZminN<<" "<< ZmaxN<<" "<<Brac<<" "<<PartonEncoding<<G4endl;

    if(std::abs(PartonEncoding) < 1000)                            // Uzhi
      {                                                            // Uzhi q or q-bar
//G4cout<<" quark "<<G4endl; // Vova
       do                                                          // Uzhi 
         {
          z = zmin + G4UniformRand()*(zmax-zmin);
//        yf = std::pow(1. - z, blund)/z*std::exp(-alund*Mt2/z);
	  yf = (1-z)/z * std::exp(-alund*Mt2/z);
//	  yf = (1-z)*(1-z);                                               // Vova
         } 
       while (G4UniformRand()*maxYf > yf); 
      }                                                            // Uzhi
    else                                                           // Uzhi
      {                                                            // Uzhi qq or qq-bar
 //G4cout<<"Di-quark"<<G4endl; // Vova
//       z = std::pow(Brac * G4UniformRand() + ZminN, OverN);        // Uzhi
         z=(zmin+zmax)/2.;
      };                                                           // Uzhi
//
//G4cout<<" test z "<<std::pow(2.,3.)<<" "<<z<<G4endl; // Vova
    return z;
    }

