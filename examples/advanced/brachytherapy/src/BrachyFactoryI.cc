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
//    *    BrachyFactoryI.cc
//    *                             *
//    *******************************
//
// $Id: BrachyFactoryI.cc,v 1.5 2006-05-12 17:08:06 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "BrachyFactoryI.hh"
#include "BrachyPrimaryGeneratorActionI.hh"
#include "BrachyDetectorConstructionI.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"  
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4RunManager.hh" 

BrachyFactoryI:: BrachyFactoryI()
{
  iodiumSource = new BrachyDetectorConstructionI();
  iodiumPrimaryParticle = new BrachyPrimaryGeneratorActionI();
}

BrachyFactoryI::~BrachyFactoryI()
{
  delete iodiumSource;
}

void  BrachyFactoryI::CreatePrimaryGeneratorAction(G4Event* anEvent)
{ 
  iodiumPrimaryParticle -> GeneratePrimaries(anEvent);
}

void BrachyFactoryI::CreateSource(G4VPhysicalVolume* mother)
{
  iodiumSource -> ConstructIodium(mother);
}

void BrachyFactoryI::CleanSource()
{;}
