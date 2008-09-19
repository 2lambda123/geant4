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
// $Id: G4NeutronCoulombBarrier.cc,v 1.5 2008-09-19 13:32:54 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)


#include "G4NeutronCoulombBarrier.hh"

G4NeutronCoulombBarrier::G4NeutronCoulombBarrier(const G4NeutronCoulombBarrier & ) : G4CoulombBarrier()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4NeutronCoulombBarrier::copy_constructor meant to not be accessable.");
}


const G4NeutronCoulombBarrier & G4NeutronCoulombBarrier::operator=(const G4NeutronCoulombBarrier & )
{
    throw G4HadronicException(__FILE__, __LINE__, "G4NeutronCoulombBarrier::operator= meant to not be accessable.");
    return *this;
}

G4bool G4NeutronCoulombBarrier::operator==(const G4NeutronCoulombBarrier & ) const 
{
    return false;
}

G4bool G4NeutronCoulombBarrier::operator!=(const G4NeutronCoulombBarrier & ) const 
{
    return true;
}

