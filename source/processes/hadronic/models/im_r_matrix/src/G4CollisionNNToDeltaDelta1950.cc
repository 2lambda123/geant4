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
// $Id: G4CollisionNNToDeltaDelta1950.cc,v 1.3 2003-12-12 15:38:22 hpw Exp $ //

#include "globals.hh"
#include "G4CollisionNNToDeltaDelta1950.hh"
#include "G4ConcreteNNToDeltaDeltastar.hh"

G4CollisionNNToDeltaDelta1950::G4CollisionNNToDeltaDelta1950()
{ 
  MakeNNToDeltaDelta<Dm_1950PC, D0_1950PC, Dp_1950PC, Dpp_1950PC, 
                     G4ConcreteNNToDeltaDeltastar>(this);
}



