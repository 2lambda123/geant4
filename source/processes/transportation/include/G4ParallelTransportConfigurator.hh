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
// $Id: G4ParallelTransportConfigurator.hh,v 1.6 2007-06-01 07:53:26 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4ParallelTransportConfigurator
//
// Class description:
// This class builds and places the G4ParallelTransport.
// If the object is deleted the process is removed from the 
// process list.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------

#ifndef G4ParallelTransportConfigurator_hh
#define G4ParallelTransportConfigurator_hh G4ParallelTransportConfigurator_hh

#include "G4Types.hh"
#include "G4VSamplerConfigurator8.hh"
#include "G4ParallelTransport.hh"
#include "G4ProcessPlacer8.hh"

class G4ParallelWorld;

class G4ParallelTransportConfigurator : public G4VSamplerConfigurator8
{

public:  // with description

  G4ParallelTransportConfigurator(const G4String &particlename,
                                        G4ParallelWorld &pworld);
  virtual ~G4ParallelTransportConfigurator();
  virtual void Configure(G4VSamplerConfigurator8 *preConf);
  virtual const G4VTrackTerminator *GetTrackTerminator() const;
  
private:

  G4ParallelTransportConfigurator(const G4ParallelTransportConfigurator &);
  G4ParallelTransportConfigurator &
  operator=(const G4ParallelTransportConfigurator &);
  G4ProcessPlacer8 fPlacer;
  G4ParallelWorld &fPWorld;
  G4ParallelTransport *fParallelTransport;
};

#endif
