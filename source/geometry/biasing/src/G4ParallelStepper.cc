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
// $Id: G4ParallelStepper.cc,v 1.11 2003-10-22 13:52:10 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// G4ParallelStepper.cc
//
// ----------------------------------------------------------------------

#include "G4ParallelStepper.hh"
#include "G4VPhysicalVolume.hh"

G4ParallelStepper::G4ParallelStepper() :
  G4VParallelStepper(),
  fPStep(0)
{}

G4ParallelStepper::~G4ParallelStepper()
{
  if (fPStep) {
    delete fPStep;
  }
}

G4ParallelStepper::G4ParallelStepper(const G4ParallelStepper &rhs)
  : G4VParallelStepper(), fPStep(new G4GeometryCellStep(rhs.GetPStep()))
{
  if (!fPStep) {
    Error("G4ParallelStepper:: new failed to create a G4GeometryCellStep!");
  }
}

G4ParallelStepper &G4ParallelStepper::operator=(const G4ParallelStepper &rhs)
{
  if (this != &rhs) {
    fPStep = new G4GeometryCellStep(rhs.GetPStep());
    if (!fPStep) {
      Error("operator=: new failed to create a G4GeometryCellStep!");
    }
  }
  return *this;
}

G4GeometryCellStep G4ParallelStepper::GetPStep() const {
  G4GeometryCellStep p = *fPStep;
  return p;
}


void G4ParallelStepper::Init(const G4GeometryCell &agCell)
{
  if (!fPStep) {
    fPStep = new G4GeometryCellStep(agCell, agCell);
      if (!fPStep) {
	Error("Init new failed to create a G4GeometryCellStep!");
      }

  }
  else {
    fPStep->SetPreGeometryCell(agCell);
    fPStep->SetPostGeometryCell(agCell);
    fPStep->SetCrossBoundary(false);
  }
}

void G4ParallelStepper::Update(const G4GeometryCell &agCell)
{
  if (!fPStep) {
    Error("fPStep == 0, Init not called?");
  }
  fPStep->SetPreGeometryCell(fPStep->GetPostGeometryCell());
  fPStep->SetPostGeometryCell(agCell);
  fPStep->SetCrossBoundary(true);
}

void G4ParallelStepper::UnSetCrossBoundary()
{
  if (!fPStep) {
    Error("fPStep == 0, Init not called?");
  }
  fPStep->SetCrossBoundary(false);
}

void G4ParallelStepper::Error(const G4String &m)
{
  G4cout << "ERROR: in G4ParallelStepper: " << m << G4endl;
  G4Exception("G4ParallelStepper::Error()",
              "FatalException", FatalException, m);
}
