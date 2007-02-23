//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4PhantomBuilder.hh"
#include "G4VBodyFactory.hh"
#include "G4MIRDBodyFactory.hh"
#include "G4ORNLFemaleBodyFactory.hh"
#include "G4ORNLMaleBodyFactory.hh"
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4PhantomBuilder::G4PhantomBuilder(): model("MIRD")
{  
  // sex can be "female" or "male"
  body = 0;
  motherVolume = 0;
  headVolume = 0;
  trunkVolume = 0;
  leftLegVolume =0;
  rightLegVolume =0;
  maleGenitaliaVolume = 0;  
}

G4PhantomBuilder::~G4PhantomBuilder()
{
} 
void G4PhantomBuilder::BuildTrunk(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (motherVolume == 0)
    G4Exception("The world volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  motherVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  trunkVolume = body -> CreateOrgan("Trunk", motherVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildLeftLeg(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (motherVolume == 0)
    G4Exception("The world volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  motherVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  leftLegVolume = body -> CreateOrgan("LeftLeg", motherVolume, colourName, solidVis, sensitivity);
}
void G4PhantomBuilder::BuildRightLeg(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (motherVolume == 0)
    G4Exception("The world volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  motherVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  rightLegVolume = body -> CreateOrgan("RightLeg", motherVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildLeftLegBone(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (leftLegVolume == 0)
    G4Exception("The left leg volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  leftLegVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("LeftLegBone", leftLegVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildRightLegBone(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
    G4Exception("The right leg volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " << rightLegVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("RightLegBone", rightLegVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildLeftArmBone(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
    G4Exception("The world volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  trunkVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("LeftArmBone" ,trunkVolume,colourName,solidVis, sensitivity);
}
void G4PhantomBuilder::BuildRightArmBone(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  trunkVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("RightArmBone",trunkVolume,colourName,solidVis, sensitivity);
}

void G4PhantomBuilder::BuildHead(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (motherVolume == 0)
    G4Exception("The mother volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  motherVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  headVolume = body -> CreateOrgan("Head",motherVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildSkull(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (headVolume == 0)
    G4Exception("The head volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  headVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan( "Skull",headVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildUpperSpine(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (headVolume == 0)
    G4Exception("The head volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  headVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("UpperSpine",headVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildMiddleLowerSpine(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");
  
  G4cout <<"MotherVolume: " <<  trunkVolume -> GetName()<< G4endl;
  G4cout << "sensitivity : "<< sensitivity << G4endl; 
  body -> CreateOrgan("MiddleLowerSpine",trunkVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildPelvis(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan( "Pelvis",trunkVolume, 
			colourName, solidVis, sensitivity);
}
/*
void G4PhantomBuilder::BuildScapulae(G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

   body -> CreateScapulae(trunkVolume, sensitivity);
}

void G4PhantomBuilder::BuildClavicles(G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

   body -> CreateClavicles(trunkVolume, sensitivity);
}
*/

void G4PhantomBuilder::BuildBrain(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
 if (headVolume == 0)
   G4Exception("The head volume is missing !!!!!");

    body -> CreateOrgan("Brain",headVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildHeart(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  //  if (trunkVolume == 0)
//    G4Exception("The trunk volume is missing !!!!!");

//    body -> CreateHeart(trunkVolume, sensitivity);
}

void G4PhantomBuilder::BuildLeftLung(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("LeftLung",trunkVolume,colourName,solidVis, sensitivity);
}

void G4PhantomBuilder::BuildRightLung(const G4String& colourName, G4bool solidVis, G4bool sensitivity )
{ 
   if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("RightLung",trunkVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildStomach(const G4String& colourName, G4bool solidVis, G4bool sensitivity )
{ 
  if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("Stomach",trunkVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildRibCage(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("RibCage",trunkVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildSpleen(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("Spleen", trunkVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildUpperLargeIntestine(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("UpperLargeIntestine",trunkVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildLowerLargeIntestine(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

   body -> CreateOrgan("LowerLargeIntestine", trunkVolume, colourName,solidVis, sensitivity);
}
/*
void G4PhantomBuilder::BuildEsophagus(G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

   body -> CreateEsophagus(trunkVolume, sensitivity);
}
*/
void G4PhantomBuilder::BuildLeftKidney(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
 if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("LeftKidney", trunkVolume,colourName, solidVis, sensitivity);
}
void G4PhantomBuilder::BuildRightKidney(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
    G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("RightKidney",trunkVolume,colourName, solidVis, sensitivity);
}

/*
void G4PhantomBuilder::BuildAdrenal(G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

   body -> CreateAdrenal(trunkVolume, sensitivity);
}
*/
void G4PhantomBuilder::BuildLiver(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   // if (trunkVolume == 0)
//    G4Exception("The trunk volume is missing !!!!!");

//    body -> CreateLiver(trunkVolume, sensitivity);
}
void G4PhantomBuilder::BuildPancreas(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
   if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("Pancreas",trunkVolume,colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildUrinaryBladder(const G4String& colourName, G4bool solidVis, G4bool sensitivity)
{ 
  if (trunkVolume == 0)
   G4Exception("The trunk volume is missing !!!!!");

    body -> CreateOrgan("UrinaryBladder",trunkVolume, colourName, solidVis, sensitivity);
}

void G4PhantomBuilder::BuildThyroid(const G4String& colourName, G4bool solidVis, G4bool sensitivity )
{ 
   // if (headVolume == 0)
//    G4Exception("The trunk volume is missing !!!!!");

//    body -> CreateThyroid(headVolume, sensitivity);
}


G4VPhysicalVolume* G4PhantomBuilder::GetPhantom()
{
  return motherVolume;
}

void G4PhantomBuilder::SetMotherVolume(G4VPhysicalVolume* mother)
{
  motherVolume = mother;
}


void G4PhantomBuilder::SetModel(G4String modelFlag)
{
  model = modelFlag;

  if(model=="MIRD" || model =="MIX") body = new G4MIRDBodyFactory();
  if(model=="ORNLFemale") body = new G4ORNLFemaleBodyFactory();
  if(model=="ORNLMale") body = new G4ORNLMaleBodyFactory();
}

