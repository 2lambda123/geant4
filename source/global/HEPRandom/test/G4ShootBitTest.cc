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
// $Id: G4ShootBitTest.cc,v 1.5 2001-07-11 10:00:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

// Tests RandFlat::fireBit() member function
// Peter Urban, 5th Sep 1996

#include "G4ios.hh"
#include "g4std/iomanip"
#include "Randomize.hh"
#include "G4Timer.hh"

int main()
{
  HepRandomEngine* e= HepRandom::getTheEngine();
  RandFlat r(*e);
  
  const long NumTest= 10000;  

  G4cout << "Printed value: (relative frequency - probability)" << G4endl;
  G4cout << NumTest << " random numbers" << G4endl << G4endl;

  long sum= 0;
  long i, j;

  for (i=0; i<NumTest; i++) {
    sum+= r.fireBit();
  }
  G4cout << "fireBit                   " << double(sum)/NumTest-0.5 << G4endl;  
  sum= 0;
  for (i=0; i<NumTest; i++) {
    sum+= r.shootBit();
  }
  G4cout << "shootBit                  " << double(sum)/NumTest-0.5 << G4endl;  
  sum= 0;
  for (i=0; i<NumTest; i++) {
    sum+= r.shootBit(e);
  }
  G4cout << "shootBit(engine)          " << double(sum)/NumTest-0.5 << G4endl;  

 
  const long Num= 1000000;
  int iii;  long lll;

  G4Timer theTimer;
  G4cout << G4endl << "Performance test: " << G4endl;

  G4cout << "shootBit() vs G4UniformRand()<0.5: ";
  theTimer.Start();
  for (i=0; i<Num; i++)
    iii = RandFlat::shootBit();
  theTimer.Stop();
  G4cout << theTimer.GetUserElapsed() << " vs ";
  theTimer.Start();
  for (i=0; i<Num; i++)
    G4UniformRand()<0.5;
  theTimer.Stop();
  G4cout << theTimer.GetUserElapsed() << G4endl << G4endl;
  
  return 0;
}



