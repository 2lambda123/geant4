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
// $Id: G4DiffractiveExcitation.hh,v 1.1 2003-10-07 11:25:59 hpw Exp $

#ifndef G4DiffractiveExcitation_h
#define G4DiffractiveExcitation_h 1
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      ---------------- G4DiffractiveExcitation --------------
//             by Gunter Folger, October 1998.
//      diffractive Excitation used by strings models
//	Take a projectile and a target
//	excite the projectile and target
// ------------------------------------------------------------

#include "globals.hh"
class G4VSplitableHadron;
class G4ExcitedString;
#include "G4ThreeVector.hh"

class G4DiffractiveExcitation 
{

  public:

      G4DiffractiveExcitation(G4double sigmaPt=0.6*GeV, G4double minExtraMass=250*MeV,G4double x0mass=250*MeV);
      virtual ~G4DiffractiveExcitation();

      virtual G4bool ExciteParticipants (G4VSplitableHadron *aPartner, G4VSplitableHadron * bPartner) const;
      virtual G4ExcitedString * String(G4VSplitableHadron * aHadron, G4bool isProjectile) const;
      
//      void SetPtWidth(G4double aValue) { widthOfPtSquare = aValue*aValue; }
//      void SetExtraMass(G4double aValue) { minExtraMass = aValue; }
//      void SetMinimumMass(G4double aValue) { minmass = aValue; }


  private:

      G4DiffractiveExcitation(const G4DiffractiveExcitation &right);
      
      G4double ChooseX(G4double Xmin, G4double Xmax) const;
      G4ThreeVector GaussianPt(G4double widthSquare, G4double maxPtSquare) const;
      
      const G4DiffractiveExcitation & operator=(const G4DiffractiveExcitation &right);
      int operator==(const G4DiffractiveExcitation &right) const;
      int operator!=(const G4DiffractiveExcitation &right) const;

  private:
// Model Parameters:
	const G4double widthOfPtSquare;	// width^2 of pt for string excitation
	const G4double minExtraMass;	// minimum excitation mass 
	const G4double minmass;	// mean pion transverse mass; used for Xmin 
};

#endif
