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

// ====================================================================
//
//   H02PythiaInterface.hh
//   $Id: H02PythiaInterface.hh,v 1.2 2003-06-16 16:48:45 gunter Exp $
//
//   A generic interface class with Pythia event generator via HepMC.
//
// ====================================================================
#ifndef H02_PYTHIA_INTERFACE_H
#define H02_PYTHIA_INTERFACE_H

#include "G4HepMCInterface.hh"

namespace HepMC {
  class CBhepevt;
}
class H02PythiaMessenger;

class H02PythiaInterface : public G4HepMCInterface {
protected:
  G4int verbose;
  G4int mpylist;
  HepMC::CBhepevt* hepevtio;

  H02PythiaMessenger* messenger;

  // In default, this is automatic conversion, Pythia->HEPEVT->HepMC, 
  // by HepMC utilities.
  virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
  H02PythiaInterface();
  ~H02PythiaInterface();

  // set/get methods
  void SetVerboseLevel(G4int i);
  G4int GetVerboseLevel() const; 

  void SetPylist(G4int i);
  G4int GetPylist() const; 

  // call pyxxx
  void CallPyinit(G4String frame, G4String beam, 
		  G4String target, G4double win);
  void CallPystat(G4int istat);
  void CallPygive(G4String par);
  void CallPyrget(G4int lun, G4int move);
  void CallPyrset(G4int lun, G4int move);
  void CallPyevnt();
  void CallPylist(G4int mode);
  void CallPyhepc(G4int mode);

  // random numbers operations
  void SetRandomSeed(G4int iseed);
  void PrintRandomStatus(std::ostream& ostr=G4cout) const;

  // setup user parameters (empty in default).
  // Implement your parameters in a delived class if you want.
  virtual void SetUserParameters();

  virtual void Print() const;
};

// ====================================================================
// inline functions
// ====================================================================

inline void H02PythiaInterface::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int H02PythiaInterface::GetVerboseLevel() const
{
  return verbose;
}

inline void H02PythiaInterface::SetPylist(G4int i)
{
  mpylist= i;
}

inline G4int H02PythiaInterface::GetPylist() const
{
  return mpylist;
}

#endif
