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
// $Id: ExN03CalorHit.hh,v 1.1 2002-05-21 11:39:37 stesting Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN03CalorHit_h
#define ExN03CalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN03CalorHit : public G4VHit
{
 public:

   ExN03CalorHit();
  ~ExN03CalorHit();
   ExN03CalorHit(const ExN03CalorHit&);
   const ExN03CalorHit& operator=(const ExN03CalorHit&);
   int operator==(const ExN03CalorHit&) const;

   inline void* operator new(size_t);
   inline void  operator delete(void*);

   void Draw();
   void Print();
      
 public:
  
   void AddAbs(G4double de, G4double dl) {EdepAbs += de; TrackLengthAbs += dl;};
   void AddGap(G4double de, G4double dl) {EdepGap += de; TrackLengthGap += dl;};
                 
   G4double GetEdepAbs()     { return EdepAbs; };
   G4double GetTrakAbs()     { return TrackLengthAbs; };
   G4double GetEdepGap()     { return EdepGap; };
   G4double GetTrakGap()     { return TrackLengthGap; };
    
 private:
  
   G4double EdepAbs, TrackLengthAbs;
   G4double EdepGap, TrackLengthGap;     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ExN03CalorHit> ExN03CalorHitsCollection;

extern G4Allocator<ExN03CalorHit> ExN03CalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ExN03CalorHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) ExN03CalorHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExN03CalorHit::operator delete(void* aHit)
{
  ExN03CalorHitAllocator.FreeSingle((ExN03CalorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


