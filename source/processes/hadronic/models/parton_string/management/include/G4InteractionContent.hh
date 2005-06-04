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
// $Id: G4InteractionContent.hh,v 1.2 2005-06-04 13:47:01 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4InteractionContent_h
#define G4InteractionContent_h 1

// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      ---------------- G4InteractionContent----------------
//             by Gunter Folger, June 1998.
//       class for a storing colliding particles in PartonString Models
// ------------------------------------------------------------

#include "globals.hh"

#include "G4VSplitableHadron.hh"

class G4InteractionContent 
{

  public:

      G4InteractionContent(G4VSplitableHadron *aPrimaryParticipant);

      ~G4InteractionContent();

      int operator==(const G4InteractionContent &right) const;
      int operator!=(const G4InteractionContent &right) const;
      
      G4VSplitableHadron * GetProjectile() const ;
      G4VSplitableHadron * GetTarget() const;

      void SetTarget(G4VSplitableHadron *aTarget);

      G4int GetNumberOfSoftCollisions();
      G4int GetNumberOfHardCollisions();
      void  SetNumberOfSoftCollisions(int);
      void  SetNumberOfHardCollisions(int);
      G4int GetNumberOfDiffractiveCollisions();
      void  SetNumberOfDiffractiveCollisions(int);

      void SplitHadrons();

public:
      G4InteractionContent(){}
      G4InteractionContent(const G4InteractionContent &right);
      const G4InteractionContent & operator=(const G4InteractionContent &right);

  protected:

  private:

      G4VSplitableHadron * theTarget;
      G4VSplitableHadron * theProjectile;
      
      G4int theNumberOfHard;
      G4int theNumberOfSoft;
      G4int theNumberOfDiffractive;
};

// Class G4InteractionContent 

inline G4VSplitableHadron * G4InteractionContent::GetProjectile() const
{
	return theProjectile;
}

inline G4VSplitableHadron * G4InteractionContent::GetTarget() const
{
	return theTarget;
}

inline void G4InteractionContent::SetTarget(G4VSplitableHadron *aTarget)
{
	theTarget = aTarget;
}

inline G4int G4InteractionContent::GetNumberOfSoftCollisions()
{
	return theNumberOfSoft;
}

inline G4int G4InteractionContent::GetNumberOfHardCollisions()
{
	return theNumberOfHard;
}

inline void G4InteractionContent::SetNumberOfSoftCollisions(int nCol)
{
	theNumberOfSoft = nCol;
}

inline void G4InteractionContent::SetNumberOfHardCollisions(int nCol)
{
	theNumberOfHard = nCol;
}

inline G4int G4InteractionContent::GetNumberOfDiffractiveCollisions()
{
	return theNumberOfDiffractive;
}

inline void G4InteractionContent::SetNumberOfDiffractiveCollisions(int nCol)
{
	theNumberOfDiffractive = nCol;
}

inline void G4InteractionContent::SplitHadrons()
{
	if ( theProjectile != NULL ) theProjectile->SplitUp();
	if ( theTarget != NULL ) theTarget->SplitUp();
}

#endif


