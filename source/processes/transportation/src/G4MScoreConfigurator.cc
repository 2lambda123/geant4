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
// $Id: G4MScoreConfigurator.cc,v 1.8 2005-11-21 21:46:53 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4MScoreConfigurator
//
// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------

#include "G4MScoreConfigurator.hh"
#include "G4VScorer.hh"
#include "G4VTrackTerminator.hh"
#include "G4MScoreProcess.hh"

G4MScoreConfigurator::
G4MScoreConfigurator(const G4String &particlename,
                           G4VScorer &scorer) 
  : fPlacer(particlename),
    fScorer(scorer),
    fMScoreProcess(0)
{
}

G4MScoreConfigurator::~G4MScoreConfigurator()
{
  if (fMScoreProcess)
  {
    fPlacer.RemoveProcess(fMScoreProcess);
    delete fMScoreProcess;
  }
}
  
const G4VTrackTerminator *
G4MScoreConfigurator::GetTrackTerminator() const
{
  return fMScoreProcess;
}

void G4MScoreConfigurator::Configure(G4VSamplerConfigurator *)
{
  fMScoreProcess = new G4MScoreProcess(fScorer);
  if (!fMScoreProcess)
  {
    G4Exception("G4MScoreConfigurator::Configure()",
                "FatalError", FatalException,
                "Failed allocation of G4MScoreProcess !");
  }
  fPlacer.AddProcessAsSecondDoIt(fMScoreProcess);
}
