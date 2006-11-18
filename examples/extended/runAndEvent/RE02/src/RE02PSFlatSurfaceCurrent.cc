//
// ********************************************************************
// * Disclaimer                                                       *
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
// $Id: RE02PSFlatSurfaceCurrent.cc,v 1.1 2006-11-18 01:37:24 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// RE02PSFlatSurfaceCurrent
#include "RE02PSFlatSurfaceCurrent.hh"

///////////////////////////////////////////////////////////////////////////////
//
//  The developement and research on this code is supported by Core Research
// for Evolutional Science and Technology (CREST) Program of Japan Science
// and Technology Corporation (JST). You may acknowlege to JST and authers
// whenever you publish your results using this code, as well as the Geant4
// collaboration. Both JST and authers remain to have a right to refuse to use
// this code for any case.
//   Tsukasa Aso    ( aso@toyama-cmt.ac.jp   )
//   Akinori Kimura ( AKimura@ashitech.ac.jp )
//
///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell charge.
//   The Cell Charge is defined by  a sum of charge inside the cell
//  which calculates the deposited charge in the cell.
//   
//    
//
//
//
// Created: 2006-06-20  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////

RE02PSFlatSurfaceCurrent::RE02PSFlatSurfaceCurrent(G4String name, G4int direction,
					     G4int nx, G4int ny, G4int nz)
  :G4PSFlatSurfaceCurrent(name,direction),fNx(nx),fNy(ny),fNz(nz)
{;}

RE02PSFlatSurfaceCurrent::~RE02PSFlatSurfaceCurrent()
{;}

G4int RE02PSFlatSurfaceCurrent::GetIndex(G4Step* aStep)
{
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int ix = touchable->GetReplicaNumber(1);
  G4int iy = touchable->GetReplicaNumber(2);
  G4int iz = touchable->GetReplicaNumber(0);
  
  return iy*fNx*fNy+ix*fNz+iz;
}
