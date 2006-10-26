//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4SafetyHelper.hh,v 1.2 2006-10-26 10:07:06 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4SafetyHelper
//
// Class description:
//
// This class is a helper for physics processes which require 
// knowledge of the safety, and the step size for the 'mass' geometry

// First version:  J. Apostolakis,  July 5th, 2006
// --------------------------------------------------------------------

#ifndef G4SAFETYHELPER_HH
#define G4SAFETYHELPER_HH 1

#include <vector>
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class G4Navigator;
class G4PathFinder;

class G4SafetyHelper
{
  public: // with description

   G4SafetyHelper(); 
   ~G4SafetyHelper();
     //
     // Constructor and destructor

   void InitialiseNavigator();
     //
     // Check for new navigator for tracking, and reinitialise pointer

   G4double ComputeMassStep( const G4ThreeVector &position, 
                             const G4ThreeVector &direction,
                                   G4double  &newSafety );
     //
     // Return step for mass geometry

   G4double ComputeSafety( const G4ThreeVector& pGlobalPoint ); 
     //
     // Return safety for all geometries

   void ReLocateWithinVolume(const G4ThreeVector& pGlobalPoint );
     //
     // Relocate the point in the volume of interest

private:

   G4PathFinder* fpPathFinder;
   G4Navigator*  fpMassNavigator;
   G4int         fMassNavigatorId;
};

#endif
