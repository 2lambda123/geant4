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
//
// $Id: G4StringModel.cc,v 1.1 2003-10-07 11:26:00 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4StringModel
#include "G4StringModel.hh"

G4StringModel::G4StringModel()
{
	the3DNucleus=NULL;
	theStringFragmentationModel=NULL;
	theGenerator=NULL;
}

G4StringModel::G4StringModel(const G4StringModel &) : G4VHighEnergyGenerator()
{
}


G4StringModel::~G4StringModel()
{
}


const G4StringModel & G4StringModel::operator=(const G4StringModel &)
{
  G4Exception("G4StringModel::operator= meant to not be accessable");
  return *this;
}


int G4StringModel::operator==(const G4StringModel &) const
{
  return 0;
}

int G4StringModel::operator!=(const G4StringModel &) const
{
  return 1;
}
