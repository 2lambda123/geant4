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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4QParticle.hh,v 1.14 2003-09-15 17:11:02 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QParticle ----------------
//             by Mikhail Kossov, Sept 1999.
//  class header for Particles in the CHIPS Model
// ---------------------------------------------------

#ifndef G4QParticle_h
#define G4QParticle_h 1

#include "g4std/iostream"
#include "globals.hh"
#include "G4QDecayChanVector.hh"

class G4QParticle
{
public:
  // Constructors
  G4QParticle();                                // Default Constructor
  G4QParticle(G4int thePDG);                    // PDGCode Constructor
  G4QParticle(const G4QParticle& right);        // Copy Constructor by value
  G4QParticle(G4QParticle* right);              // Copy Constructor by pointer

  ~G4QParticle();                               // Destructor

  // Operators
  const G4QParticle& operator=(const G4QParticle& right);
  G4int              operator==(const G4QParticle& rhs) const;
  G4int              operator!=(const G4QParticle& rhs) const;

  // Selectors
  G4QPDGCode          GetQPDG() const;          // Get a PDG-Particle of the Particle
  G4int               GetPDGCode() const;       // Get a PDG Code of the Particle
  G4int               GetQCode() const;         // Get a Q Code of the Particle
  G4int               GetSpin() const;          // Get 2s+1 of the Particle
  G4int               GetCharge() const;        // Get a Charge of the Particle
  G4int               GetStrange() const;       // Get a Strangeness of the Particle
  G4int               GetBaryNum() const;       // Get a Baryon Number of the Particle
  G4QContent          GetQContent();            // Get Quark Content of the Particle
  G4QDecayChanVector  GetDecayVector();         // Get a Decay Vector for the Particle
  G4double            GetMass();                // Get a mass value for the Particle
  G4double            GetWidth();               // Get a width value for the Particle

  // Modifiers
  G4QDecayChanVector InitDecayVector(G4int Q);  // Init Decay Vector in the CHIPS World by Q code
  void InitPDGParticle(G4int thePDGCode);
  void InitQParticle(G4int theQCode);

  // General
  G4double MinMassOfFragm();                    // Minimal mass of decaing fragments

private:
  // Encapsulated functions

private:
  // the Body
  G4QPDGCode          aQPDG;
  G4QDecayChanVector  aDecay;
  G4QContent          aQuarkCont;               // @@ Secondary (added for acceleration - check)
};

// Not member operators
G4std::ostream&   operator<<(G4std::ostream& lhs, G4QParticle& rhs);
// Not member functions
//----------------------------------------------------------------------------------------

inline G4int G4QParticle::operator==(const G4QParticle& rhs) const {return this==&rhs;}
inline G4int G4QParticle::operator!=(const G4QParticle& rhs) const {return this!=&rhs;}
 
inline G4QPDGCode         G4QParticle::GetQPDG()    const {return aQPDG;}
inline G4int              G4QParticle::GetPDGCode() const {return aQPDG.GetPDGCode();}
inline G4int              G4QParticle::GetSpin()    const {return aQPDG.GetSpin();}
inline G4int              G4QParticle::GetCharge()  const {return aQuarkCont.GetCharge();}
inline G4int              G4QParticle::GetStrange() const {return aQuarkCont.GetStrangeness();}
inline G4int              G4QParticle::GetBaryNum() const {return aQuarkCont.GetBaryonNumber();}
inline G4QContent         G4QParticle::GetQContent()      {return aQuarkCont;}
inline G4QDecayChanVector G4QParticle::GetDecayVector()   {return aDecay;}
inline G4double           G4QParticle::GetMass()          {return aQPDG.GetMass();}
inline G4double           G4QParticle::GetWidth()         {return aQPDG.GetWidth();}

inline G4double G4QParticle::MinMassOfFragm()
{
  G4int nCh=aDecay.size();
  G4double m=GetMass();
  G4double min=m;
  if(nCh)
  {
    min=aDecay[0]->GetMinMass();
    if(nCh>1) for(G4int j=1; j<nCh; j++)
	{
      G4double next=aDecay[j]->GetMinMass();
      if(next<min) min=next;
	}
  }
  G4double w=GetWidth();
  G4double lim=m+.001;
  if(w)   lim-=1.5*w;
  if(min<lim) min=lim;
  return min;
}

#endif



