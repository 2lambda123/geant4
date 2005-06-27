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
// $Id: G4QHBook.hh,v 1.2 2005-06-27 15:33:04 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4QHBook_h
#define G4QHBook_h 1

// ----------------------------------------------------------------------
//      GEANT 4 class header file
//      ---------------- G4QHBook ----------------
//      made for process level tests by Mikhail Kossov - Feb 2005
//      class for booking and filling histograms and ntuples
// ----------------------------------------------------------------------

#include "globals.hh"
#include "G4ios.hh"
#include "G4VParticleChange.hh"
#include "G4DynamicParticle.hh"

class G4QHBook
{
public:
  G4QHBook();                                     // Default Constructor
  ~G4QHBook();                                    // Destructor

  // Specific Modifiers
  void  FillEvt(const G4VParticleChange* hadrons);  // Fill Histos & ntuples for the event

private: 
  G4int         nEvnt;    // Consecutive number of call to fill the histograms 
  std::ofstream histNevt; // 1D histogram, id=1, to store number of calls to fill
  std::ofstream tuplEvtA; // Ntuple, id=25, to be filled once per event (All particles)
  std::ofstream tuplEvtQ; // Ntuple, id=27, to be filled once per event (Quasmon particles)
  std::ofstream tuplIncl; // Ntuple, id=20, to be filled once per track
  std::ofstream tuple3pi; // Ntuple, id=22, to be filled once per event with 3 pions
};

#endif
