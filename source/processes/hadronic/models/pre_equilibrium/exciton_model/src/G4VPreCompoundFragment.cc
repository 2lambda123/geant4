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
// $Id: G4VPreCompoundFragment.cc,v 1.1 2003-08-26 18:54:54 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// by V. Lara
 
#include "G4VPreCompoundFragment.hh"
#include "G4PreCompoundParameters.hh"

G4VPreCompoundFragment::
G4VPreCompoundFragment(const G4VPreCompoundFragment & right)
{
  theA = right.theA;
  theZ = right.theZ;
  theRestNucleusA = right.theRestNucleusA;
  theRestNucleusZ = right.theRestNucleusZ;
  theCoulombBarrier = right.theCoulombBarrier;
  theCoulombBarrierPtr = right.theCoulombBarrierPtr;
  theMaximalKineticEnergy = right.theMaximalKineticEnergy;
  theEmissionProbability = right.theEmissionProbability;
  theMomentum = right.theMomentum;
  theFragmentName = right.theFragmentName;
  theStage = right.theStage;
}

G4VPreCompoundFragment::
G4VPreCompoundFragment(const G4double anA,
		       const G4double aZ, 
		       G4VCoulombBarrier* aCoulombBarrier,
		       const G4String & aName):
  theA(anA),theZ(aZ), 
  theRestNucleusA(0.0),theRestNucleusZ(0.0),theCoulombBarrier(0.0),
  theCoulombBarrierPtr(aCoulombBarrier),
  theBindingEnergy(0.0), theMaximalKineticEnergy(-1.0),
  theEmissionProbability(0.0), theMomentum(0.0,0.0,0.0,0.0),
  theFragmentName(aName),theStage(0)
{}



G4VPreCompoundFragment::~G4VPreCompoundFragment()
{
}


const G4VPreCompoundFragment & G4VPreCompoundFragment::
operator= (const G4VPreCompoundFragment & right)
{
  if (this != &right) {
    theA = right.theA;
    theZ = right.theZ;
    theRestNucleusA = right.theRestNucleusA;
    theRestNucleusZ = right.theRestNucleusZ;
    theCoulombBarrier = right.theCoulombBarrier;
    theCoulombBarrierPtr = right.theCoulombBarrierPtr;
    theMaximalKineticEnergy = right.theMaximalKineticEnergy;
    theEmissionProbability = right.theEmissionProbability;
    theMomentum = right.theMomentum;
    theFragmentName = right.theFragmentName;
    theStage = right.theStage;
  }
  return *this;
}

G4int G4VPreCompoundFragment::operator==(const G4VPreCompoundFragment & right) const
{
  return (this == (G4VPreCompoundFragment *) &right);
}

G4int G4VPreCompoundFragment::operator!=(const G4VPreCompoundFragment & right) const
{
  return (this != (G4VPreCompoundFragment *) &right);
}


std::ostream& 
operator << (std::ostream &out, const G4VPreCompoundFragment &theFragment)
{
  out << &theFragment;
  return out; 
}


std::ostream& 
operator << (std::ostream &out, const G4VPreCompoundFragment *theFragment)
{
  std::ios::fmtflags old_floatfield = out.flags();
  out.setf(std::ios::floatfield);
    
  out 
    << "PreCompound Model Emitted Fragment: A = " 
    << std::setprecision(3) << theFragment->theA 
    << ", Z = " << std::setprecision(3) << theFragment->theZ;
    out.setf(std::ios::scientific,std::ios::floatfield);
    //   out
    //     << ", U = " << theFragment->theExcitationEnergy/MeV 
    //     << " MeV" << endl
    //     << "          P = (" 
    //     << theFragment->theMomentum.x()/MeV << ","
    //     << theFragment->theMomentum.y()/MeV << ","
    //     << theFragment->theMomentum.z()/MeV 
    //     << ") MeV   E = " 
    //     << theFragment->theMomentum.t()/MeV << " MeV";
    
    out.setf(old_floatfield,std::ios::floatfield);
    
    return out;
}


void G4VPreCompoundFragment::
Initialize(const G4Fragment & aFragment)
{
  theRestNucleusA = aFragment.GetA() - theA;
  theRestNucleusZ = aFragment.GetZ() - theZ;

  if ((theRestNucleusA < theRestNucleusZ) ||
      (theRestNucleusA < theA) ||
      (theRestNucleusZ < theZ)) 
    {
      // In order to be sure that emission probability will be 0.
      theMaximalKineticEnergy = 0.0;
      return;
    }
  
  
  // Calculate Coulomb barrier
  theCoulombBarrier = theCoulombBarrierPtr->
    GetCoulombBarrier(static_cast<G4int>(theRestNucleusA),static_cast<G4int>(theRestNucleusZ),
		      aFragment.GetExcitationEnergy());
  
  // Compute Binding Energies for fragments 
  // (needed to separate a fragment from the nucleus)
  
  theBindingEnergy = G4NucleiProperties::GetMassExcess(static_cast<G4int>(theA),static_cast<G4int>(theZ)) +
    G4NucleiProperties::GetMassExcess(static_cast<G4int>(theRestNucleusA),static_cast<G4int>(theRestNucleusZ)) -
    G4NucleiProperties::GetMassExcess(static_cast<G4int>(aFragment.GetA()),static_cast<G4int>(aFragment.GetZ()));
  
  // Compute Maximal Kinetic Energy which can be carried by fragments after separation
  G4double m = aFragment.GetMomentum().m();
  G4double rm = GetRestNuclearMass();
  G4double em = GetNuclearMass();
  theMaximalKineticEnergy = ((m - rm)*(m + rm) + em*em)/(2.0*m) - em;
  // - theCoulombBarrier;
  
  return;
}




