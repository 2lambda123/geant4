
#ifndef LXePMTHit_h
#define LXePMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

class LXePMTHit : public G4VHit
{
public:
  
  LXePMTHit();
  ~LXePMTHit();
  LXePMTHit(const LXePMTHit &right);

  const LXePMTHit& operator=(const LXePMTHit &right);
  G4int operator==(const LXePMTHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  
  void Draw();
  void Print();

  inline void SetDrawit(G4bool b){drawit=b;}
  inline G4bool GetDrawit(){return drawit;}

  inline void IncPhotonCount(){photons++;}
  inline G4int GetPhotonCount(){return photons;}

  inline void SetPMTNumber(G4int n) { pmtNumber = n; }
  inline G4int GetPMTNumber() { return pmtNumber; }

  inline void SetPMTPhysVol(G4VPhysicalVolume* physVol){this->physVol=physVol;}
  inline G4VPhysicalVolume* GetPMTPhysVol(){return physVol;}

  inline void SetPMTPos(G4double x,G4double y,G4double z){
    pos=G4ThreeVector(x,y,z);
  }
  
  inline G4ThreeVector GetPMTPos(){return pos;}

private:
  G4int pmtNumber;
  G4int photons;
  G4ThreeVector pos;
  G4VPhysicalVolume* physVol;
  G4bool drawit;

};

typedef G4THitsCollection<LXePMTHit> LXePMTHitsCollection;

extern G4Allocator<LXePMTHit> LXePMTHitAllocator;

inline void* LXePMTHit::operator new(size_t){
  void *aHit;
  aHit = (void *) LXePMTHitAllocator.MallocSingle();
  return aHit;
}

inline void LXePMTHit::operator delete(void *aHit){
  LXePMTHitAllocator.FreeSingle((LXePMTHit*) aHit);
}

#endif


