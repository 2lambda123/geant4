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
// $Id: G4QParentCluster.cc,v 1.15 2003-06-16 17:04:27 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QParentCluster ----------------
//             by Mikhail Kossov, Sept 1999.
//      class fora Parent nuclear cluster in the CHIPS Model
// ----------------------------------------------------------

//#define debug
//#define pdebug

#include "G4QParentClusterVector.hh"

G4QParentCluster::G4QParentCluster(G4int PDGCode): thePDGCode(PDGCode), theProbability(0.){}

G4QParentCluster::G4QParentCluster(G4int PDGCode, G4double prob): 
  thePDGCode(PDGCode), theProbability(prob){}

G4QParentCluster::G4QParentCluster(const G4QParentCluster& rhs)
{
  thePDGCode          = rhs.thePDGCode;
  theProbability      = rhs.theProbability;
  nQPart2             = rhs.nQPart2;
  transQC             = rhs.transQC;
  lowLimit            = rhs.lowLimit;
  highLimit           = rhs.highLimit;
  theEnvBoundedMass   = rhs.theEnvBoundedMass;
  theEnvBindingEnergy = rhs.theEnvBindingEnergy;
  theNucBoundedMass   = rhs.theNucBoundedMass;
  theNucBindingEnergy = rhs.theNucBindingEnergy;
}

G4QParentCluster::G4QParentCluster(G4QParentCluster* rhs)
{
  thePDGCode          = rhs->thePDGCode;
  theProbability      = rhs->theProbability;
  nQPart2             = rhs->nQPart2;
  transQC             = rhs->transQC;
  lowLimit            = rhs->lowLimit;
  highLimit           = rhs->highLimit;
  theEnvBoundedMass   = rhs->theEnvBoundedMass;
  theEnvBindingEnergy = rhs->theEnvBindingEnergy;
  theNucBoundedMass   = rhs->theNucBoundedMass;
  theNucBindingEnergy = rhs->theNucBindingEnergy;
}

const G4QParentCluster& G4QParentCluster::operator=(const G4QParentCluster& rhs)
{
  thePDGCode          = rhs.thePDGCode;
  theProbability      = rhs.theProbability;
  nQPart2             = rhs.nQPart2;
  transQC             = rhs.transQC;
  lowLimit            = rhs.lowLimit;
  highLimit           = rhs.highLimit;
  theEnvBoundedMass   = rhs.theEnvBoundedMass;
  theEnvBindingEnergy = rhs.theEnvBindingEnergy;
  theNucBoundedMass   = rhs.theNucBoundedMass;
  theNucBindingEnergy = rhs.theNucBindingEnergy;

  return *this;
}

G4QParentCluster::~G4QParentCluster() {}

// Standard output for G4QParentCluster
std::ostream& operator<<(std::ostream& lhs, G4QParentCluster& rhs)
{//      ===============================================
  lhs << "[ParClPDG=" << rhs.GetPDGCode() << ", probab=" << rhs.GetProbability() << "]";
  return lhs;
}

// Standard output for const G4QParentCluster
std::ostream& operator<<(std::ostream& lhs, const G4QParentCluster& rhs)
{//      ===============================================
  lhs << "[ParClPDG=" << rhs.GetPDGCode() << ", probab=" << rhs.GetProbability() << "]";
  return lhs;
}





