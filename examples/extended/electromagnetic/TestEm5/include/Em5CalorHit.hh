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
// $Id: Em5CalorHit.hh,v 1.6 2003-06-06 15:55:48 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Em5CalorHit_h
#define Em5CalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Em5CalorHit : public G4VHit
{
 public:

   Em5CalorHit();
  ~Em5CalorHit();
   Em5CalorHit(const Em5CalorHit&);
   const Em5CalorHit& operator=(const Em5CalorHit&);

   inline void* operator new(size_t);
   inline void  operator delete(void*);

   void Print();
      
 public:
  
   void AddAbs(G4double de, G4double dl) {EdepAbs += de; TrackLengthAbs += dl;};
   void AddGap(G4double de, G4double dl) {EdepGap += de; TrackLengthGap += dl;};
                 
   G4double GetEdepAbs()     {return EdepAbs;};
   G4double GetTrakAbs()     {return TrackLengthAbs;};
   G4double GetEdepGap()     {return EdepGap; };
   G4double GetTrakGap()     {return TrackLengthGap;};
     
 private:
  
   G4double EdepAbs, TrackLengthAbs;
   G4double EdepGap, TrackLengthGap;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<Em5CalorHit> Em5CalorHitsCollection;

extern G4Allocator<Em5CalorHit> Em5CalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* Em5CalorHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) Em5CalorHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void Em5CalorHit::operator delete(void* aHit)
{
  Em5CalorHitAllocator.FreeSingle((Em5CalorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


