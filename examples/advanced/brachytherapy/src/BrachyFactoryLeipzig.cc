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
// Code developed by:
//  S.Guatelli
//
//    *******************************
//    *                             *
//    *    BrachyFactoryLeipzig.cc  *
//    *                             *
//    *******************************
//
// $Id: BrachyFactoryLeipzig.cc,v 1.3 2003-05-09 16:52:08 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "globals.hh"
#include "BrachyFactoryLeipzig.hh"
#include"BrachyPrimaryGeneratorActionIr.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"  
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh" 
#include "BrachyDetectorMessenger.hh"
#include "BrachyDetectorConstructionLeipzig.hh"

BrachyFactoryLeipzig:: BrachyFactoryLeipzig()
{
  pLeipzig=new  BrachyDetectorConstructionLeipzig();
}

BrachyFactoryLeipzig:: ~BrachyFactoryLeipzig()
{
  delete pLeipzig;
}

G4VUserPrimaryGeneratorAction*  BrachyFactoryLeipzig::CreatePrimaryGeneratorAction()
{
  G4VUserPrimaryGeneratorAction* pIridium =new BrachyPrimaryGeneratorActionIr();
  return pIridium;
}

void BrachyFactoryLeipzig::CreateSource(G4VPhysicalVolume* mother)
{
  pLeipzig -> ConstructLeipzig(mother);
}

void BrachyFactoryLeipzig::CleanSource()
{
  ;
}
