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
//
#ifndef G4Nucleon_h
#define G4Nucleon_h 1

// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      ---------------- G4Nucleon ----------------
//             by Gunter Folger, May 1998.
//       class for a nucleon (inside a 3D Nucleus)
// ------------------------------------------------------------

#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4VKineticNucleon.hh"

//#include "G4VSplitableHadron.hh"
class G4VSplitableHadron;

class G4Nucleon : public G4VKineticNucleon
{

  public:
      G4Nucleon();
      ~G4Nucleon();

      inline int operator==(const G4Nucleon &right) const;
      inline int operator!=(const G4Nucleon &right) const;
      const G4Nucleon& operator=(const G4Nucleon& right);

  public:

    inline void SetPosition(G4ThreeVector & aPosition) {thePosition = aPosition;}
    virtual inline const G4ThreeVector & GetPosition()  const {return thePosition;}

    inline void SetMomentum(G4LorentzVector & aMomentum) {theMomentum = aMomentum;}
    inline const G4LorentzVector& GetMomentum()  const {return theMomentum;}
    virtual inline const G4LorentzVector & Get4Momentum()  const {return theMomentum;}

    inline void SetBindingEnergy(G4double anEnergy) {theBindingE = anEnergy;}
    inline G4double GetBindingEnergy()  const {return theBindingE;}

    inline void SetParticleType(G4Proton * aProton) {theParticleType = aProton;}
    inline void SetParticleType(G4Neutron *aNeutron){theParticleType = aNeutron;}

    inline  G4ParticleDefinition * GetParticleType() const {return theParticleType;}
    virtual  G4ParticleDefinition* GetDefinition() const {return theParticleType;}
    
    inline void Boost(const G4ThreeVector & beta){ theMomentum.boost(beta); } 
           void Boost(const G4LorentzVector & aMomentum);

    inline void Hit(G4VSplitableHadron * aHit) { theSplitableHadron=aHit;}
    inline void Hit(G4int ) 
    { 
      theSplitableHadron=reinterpret_cast<G4VSplitableHadron *>(1111); 
      // G4cout << "$%$#%@%$#@%@%%% "<<theSplitableHadron<<G4endl;
    }
    inline G4VSplitableHadron * GetSplitableHadron() const { return theSplitableHadron;}
    
    inline G4bool AreYouHit() const 
    { 
      G4bool result = true;
      if (theSplitableHadron==NULL) result = false;
      return result;
    }
  
  private:

    G4ThreeVector thePosition;
    G4LorentzVector theMomentum;
    G4double theBindingE;
    G4ParticleDefinition * theParticleType;
    G4VSplitableHadron * theSplitableHadron;


};

std::ostream & operator << (std::ostream &, const G4Nucleon&);

inline int G4Nucleon::operator==(const G4Nucleon &right) const
{
	return this==&right;
}
inline int G4Nucleon::operator!=(const G4Nucleon &right) const
{
	return this!=&right;
}

inline const G4Nucleon& G4Nucleon::operator=(const G4Nucleon& right)
{
	thePosition=right.GetPosition();
	theMomentum=right.Get4Momentum();
	theBindingE=right.GetBindingEnergy();
	theParticleType=right.GetDefinition();
	theSplitableHadron=right.GetSplitableHadron();

	return *this;
}

#endif


