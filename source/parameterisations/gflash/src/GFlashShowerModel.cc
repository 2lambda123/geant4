//E.Barberio & Joanna Weng 

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4NeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4NeutrinoTau.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4AntiNeutrinoTau.hh"
#include "G4PionZero.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "geomdefs.hh"

#include "GFlashShowerModel.hh"
#include "GFlashHomoShowerParamterisation.hh"
#include "GFlashEnergySpot.hh"


GFlashShowerModel::GFlashShowerModel(G4String modelName, G4LogicalVolume* envelope)
: G4VFastSimulationModel(modelName, envelope)
{
	FlagParamType           = 0;
	FlagParticleContainment = 1;  
	StepInX0 = 0.1;
	Messenger       = new GFlashShowerModelMessenger(this);
}

GFlashShowerModel::GFlashShowerModel(G4String modelName)
: G4VFastSimulationModel(modelName)
{
	FlagParamType           =1;
	FlagParticleContainment = 1;  
	StepInX0 = 0.1; 
	Messenger       = new GFlashShowerModelMessenger(this); 
}


GFlashShowerModel::~GFlashShowerModel()
{
	Messenger       = new GFlashShowerModelMessenger(this);
}

G4bool GFlashShowerModel::IsApplicable(const G4ParticleDefinition& particleType)
{ 
	return 
	&particleType == G4Electron::ElectronDefinition() ||
	&particleType == G4Positron::PositronDefinition(); 
}

/*********************************************************************/
/* Checks whether conditions of fast parametrisation  are fullfilled */
/**********************************************************************/

G4bool GFlashShowerModel::ModelTrigger(const G4FastTrack & fastTrack )

{
	G4bool select = false;
	if(FlagParamType != 0)                  
	{
		G4double  ParticleEnergy = fastTrack.GetPrimaryTrack()->GetKineticEnergy(); 
		G4ParticleDefinition &ParticleType = *(fastTrack.GetPrimaryTrack()->GetDefinition()); 
		if(ParticleEnergy > PBound->GetMinEneToParametrise(ParticleType) ||
			ParticleEnergy < PBound->GetMaxEneToParametrise(ParticleType) )
		{
			///check conditions depending on particle flavour
			Parametrisation->GenerateLongitudinalProfile(ParticleEnergy); // performance to be optimized @@@@@@@
			select     = CheckParticleDefAndContainment(fastTrack);	
			if (select) EnergyStop= PBound->GetEneToKill(ParticleType);
		}
	}
	return select; 
}


G4bool GFlashShowerModel::CheckParticleDefAndContainment(const G4FastTrack& fastTrack)
{  
	G4bool filter=false;
	G4ParticleDefinition * ParticleType = fastTrack.GetPrimaryTrack()->GetDefinition(); 
	
	if(  ParticleType == G4Electron::ElectronDefinition() || 
		ParticleType == G4Positron::PositronDefinition() )
	{
		filter=true;
		if(FlagParticleContainment == 1)  
		{
			filter=CheckContainment(fastTrack); 
		}
	}
	return filter;  
}

G4bool GFlashShowerModel::CheckContainment(const G4FastTrack& fastTrack)
{
	//Note: typedef Hep3Vector G4ThreeVector;
	
	G4bool filter=false;
	//track informations
	G4ThreeVector DirectionShower = fastTrack.GetPrimaryTrackLocalDirection();
	G4ThreeVector InitialPositionShower = fastTrack.GetPrimaryTrackLocalPosition();
	
	G4ThreeVector OrthoShower, CrossShower; 
	//Returns orthogonal vector 
	OrthoShower = DirectionShower.orthogonal();
	// Shower in direction perpendicular to OrthoShower and DirectionShower
	CrossShower = DirectionShower.cross(OrthoShower);
	
	G4double  R     = Parametrisation->GetAveR90();
	G4double  Z     = Parametrisation->GetAveT90();
	G4int CosPhi[4] = {1,0,-1,0};
	G4int SinPhi[4] = {0,1,0,-1};
	
	G4ThreeVector Position;
	G4int NlateralInside=0;
	//pointer to soild we're in
	G4VSolid *SolidCalo = fastTrack.GetEnvelopeSolid();
	for(int i=0; i<4 ;i++)
	{
		// polar coordinates
		Position = InitialPositionShower       + 
		Z*DirectionShower           +
		R*CosPhi[i]*OrthoShower     +
		R*SinPhi[i]*CrossShower     ;
		
		if(SolidCalo->Inside(Position) != kOutside) 
			NlateralInside=NlateralInside++;
	}
	
	//chose to parametrise or flag when all inetc...
	if(NlateralInside==4) filter=true;
	// std::cout << " points =   " <<NlateralInside << std::endl;
	return filter;
}


void GFlashShowerModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep)
{
	// parametrise electrons
	if(fastTrack.GetPrimaryTrack()->GetDefinition() == G4Electron::ElectronDefinition() || 
		fastTrack.GetPrimaryTrack()->GetDefinition() == G4Positron::PositronDefinition() ) 
	ElectronDoIt(fastTrack,fastStep);
}

void GFlashShowerModel::ElectronDoIt(const G4FastTrack& fastTrack,  G4FastStep& fastStep)
{
	// std::cout<<"--- ElectronDoit --- "<<std::endl;
	
	fastStep.KillPrimaryTrack();
	fastStep.SetPrimaryTrackPathLength(0.0);
	fastStep.SetTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());
	feSpotList.clear();
	
	//-----------------------------
	// Get track parameters 
	//-----------------------------	
	//E,vect{p} and t,vec(x)
	G4double Energy = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
	
	// axis of the shower, in global reference frame:
	G4ThreeVector DirectionShower = fastTrack.GetPrimaryTrack()->GetMomentumDirection();
	G4ThreeVector OrthoShower, CrossShower;
	OrthoShower = DirectionShower.orthogonal();
	CrossShower = DirectionShower.cross(OrthoShower);
	
	//--------------------------------
	///Generate longitudinal profile
	//--------------------------------
	Parametrisation->GenerateLongitudinalProfile(Energy); // performane iteration @@@@@@@
	
	///Initialisation of long. loop variables
	G4VSolid *SolidCalo = fastTrack.GetEnvelopeSolid();
	G4ThreeVector pos   = fastTrack.GetPrimaryTrackLocalPosition();
	G4ThreeVector start  = fastTrack.GetPrimaryTrack()->GetPosition();
	G4ThreeVector dir   = fastTrack.GetPrimaryTrackLocalDirection();
	G4double Bound      = SolidCalo->DistanceToOut(pos,dir); 
	
	G4double Dz       = 0.00;     
	G4double ZEndStep = 0.00;
	
	G4double EnergyNow        = Energy;
	G4double EneIntegral      = 0.00;   
	G4double LastEneIntegral  = 0.00;   
	G4double DEne             = 0.00;
	
	G4double NspIntegral      = 0.00;   
	G4double LastNspIntegral  = 0.00;   
	G4double DNsp             = 0.00;
	
	// starting point of the shower:
	G4ThreeVector PositionShower  = fastTrack.GetPrimaryTrack()->GetPosition();
	G4ThreeVector NewPositionShower    = PositionShower;   
	G4double      StepLenght           = 0.00;
	
	G4int NSpotDeposited =0;
	
	//--------------------------
	/// Begin Longitudinal Loop
	//-------------------------
	
	do
	{  
		//determine step size=min(1Xo,next boundary)
		G4double stepLength = StepInX0*Parametrisation->GetX0();
		if(Bound < stepLength)
		{ 
			Dz    = Bound;
			Bound = 0.00;
		}
		else
		{ 
			Dz    = stepLength;
			Bound = Bound-Dz;
		}
		ZEndStep=ZEndStep+Dz;
		
		// Determine Energy Release in Step
		if(EnergyNow > EnergyStop)
		{
			LastEneIntegral  = EneIntegral;
			EneIntegral      = Parametrisation->IntegrateEneLongitudinal(ZEndStep);
			DEne             = std::min( EnergyNow, (EneIntegral-LastEneIntegral)*Energy);
			LastNspIntegral  = NspIntegral;
			NspIntegral      = Parametrisation->IntegrateNspLongitudinal(ZEndStep);
			DNsp             = std::max(1., floor( (NspIntegral-LastNspIntegral)*Parametrisation->GetNspot() ) );
		}
		// end of the shower
		else
		{    
			DEne             = EnergyNow;
			DNsp             = std::max(1., floor( (1.- NspIntegral)*Parametrisation->GetNspot() ));
		} 
		EnergyNow  = EnergyNow - DEne;
		
		//  apply sampling fluctuation
		// G4double DEneSampling = Parametrisation->ApplySampling(DEne,Energy);
		
		//move particle in the middle of the step
		StepLenght        = StepLenght + Dz/2.00;  
		NewPositionShower = NewPositionShower + 
		StepLenght*DirectionShower;
		StepLenght        = Dz/2.00;
		
		//generate spots & hits:
		for (int i = 0; i < DNsp; i++)
		{ 
			NSpotDeposited=NSpotDeposited++;
			GFlashEnergySpot Spot;      
			if (i == 0)
			{
				Spot.SetStart(start);
			}
			
			//Spot energy: the same for all spots
			Spot.SetEnergy( DEne / DNsp );
			G4double PhiSpot = Parametrisation->GeneratePhi(); // phi of spot
			G4double RSpot   = Parametrisation->GenerateRadius(i,Energy,ZEndStep-Dz/2.); // radius of spot
			///check reference-> may be need to introduce rot matrix @@@
			//Position: equally spaced in z
			
			G4ThreeVector SpotPosition = NewPositionShower                         +
			Dz/DNsp*DirectionShower*(i+1/2.-DNsp/2.)  +
			RSpot*cos(PhiSpot)*OrthoShower            +  
			RSpot*sin(PhiSpot)*CrossShower;      
			Spot.SetPosition(SpotPosition);
			
			// Record the Spot:
			feSpotList.push_back(Spot);
			// Spot.Print();
			//Generate Hits of this spot      
			HMaker->make(Spot);
		}
	}
	while(EnergyNow > 0.0 && Bound> 0.0);     
	
	//---------------
	/// End Loop
	//------------- 
	
}

/* void GFlashShowerModel::GammaDoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep)
{ 
	
	if( fastTrack.GetPrimaryTrack()->GetKineticEnergy() > EnergyStop )
		return;
	
	//deposita in uno spot unico l'energia 
	//con andamento exp decrescente. 
	
	// Kill the particle to be parametrised
	fastStep.KillPrimaryTrack();
	fastStep.SetPrimaryTrackPathLength(0.0);
	fastStep.SetTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());
	//other settings????
	feSpotList.clear();   
	//-----------------------------
	// Get track parameters 
	//-----------------------------
	//E,vect{p} and t,vec(x)
	G4double Energy = 
	fastTrack.GetPrimaryTrack()->GetKineticEnergy();
	// axis of the shower, in global reference frame:
	G4ThreeVector DirectionShower 
	= fastTrack.GetPrimaryTrack()->GetMomentumDirection();
	// starting point of the shower:
	G4ThreeVector PositionShower 
	= fastTrack.GetPrimaryTrack()->GetPosition();
	
	//G4double DEneSampling = Parametrisation->ApplySampling(Energy,Energy);
	//if(DEneSampling <= 0.00) DEneSampling=Energy;  
	
	if(Energy > 0.0)
	{
		G4double dist = Parametrisation->GenerateExponential(Energy);      
		
		GFlashEnergySpot Spot;
		Spot.SetEnergy( Energy );
		G4ThreeVector SpotPosition = PositionShower + dist*DirectionShower;  
		Spot.SetPosition(SpotPosition);
		
		// Record the Spot:
		feSpotList.push_back(Spot);
		
		//Generate Hits of this spot      
		HMaker->make(Spot);
	}
	
}

void GFlashShowerModel::NeutrinoDoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep)
{ 
	
	if( fastTrack.GetPrimaryTrack()->GetKineticEnergy() > EnergyStop )
		return;
	
	// Kill the particle to be parametrised
	fastStep.KillPrimaryTrack();
	// fastStep.SetPrimaryTrackPathLength(0.0);
	//fastStep.SetTotalEnergyDeposited(0.0);
}
*/
