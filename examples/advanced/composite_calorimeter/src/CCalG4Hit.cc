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
///////////////////////////////////////////////////////////////////////////////
// File: CCalG4Hit.cc
// Description: G4 Hit class for Calorimeters (Ecal, Hcal, ...)
///////////////////////////////////////////////////////////////////////////////

#include "CCalG4Hit.hh"
#include "g4std/iostream"


CCalG4Hit::CCalG4Hit() : CCalHit(), 
  elem(0.0), hadr(0.0) 
{}


CCalG4Hit::~CCalG4Hit() {}


CCalG4Hit::CCalG4Hit(const CCalG4Hit &right) : 
  G4VHit(right), CCalHit(right),
  elem(right.elem), hadr(right.hadr) 
{}


const CCalG4Hit& CCalG4Hit::operator=(const CCalG4Hit &right) {  
  CCalHit::operator=(right);
  elem  = right.elem;
  hadr  = right.hadr;
  return *this;
}


double CCalG4Hit::getEM() const      { return elem; }
void   CCalG4Hit::setEM (double e)   { elem = e; }
      
double CCalG4Hit::getHadr() const    { return hadr; }
void   CCalG4Hit::setHadr (double e) { hadr = e; }
      
void CCalG4Hit::addEnergyDeposit(const CCalG4Hit& aHit) {
  addEnergyDeposit( aHit.getEM(), aHit.getHadr() );
}

void CCalG4Hit::addEnergyDeposit(double em, double hd) {
  elem  += em; 
  hadr += hd;
  CCalHit::addEnergyDeposit(em+hd);
}


void CCalG4Hit::Print() {
  G4cout << (*this);
}


G4std::ostream& operator<< (G4std::ostream& os, const CCalG4Hit& hit) {
  os << static_cast<CCalHit>(hit);
  os << " Data specific of this CCalG4Hit are:" << G4endl
     << " \t EnergyDeposit of EM particles = " << hit.getEM() 
     << " (MeV)" << G4endl
     << " \t EnergyDeposit of HD particles = " << hit.getHadr() 
     << " (MeV)" << G4endl;
  return os;
}

