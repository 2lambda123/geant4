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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4QParentClusterVector.hh,v 1.13 2003-10-08 14:48:22 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QCandidateVector ----------------
//             by Mikhail Kossov, Sept 1999.
// Type defenition of Parent nuclear cluster Vector in CHIPS model
// ---------------------------------------------------------------

#ifndef G4QParentClusterVector_h
#define G4QParentClusterVector_h 1

#include "G4QParentCluster.hh"
#include <vector>

typedef std::vector<G4QParentCluster *> G4QParentClusterVector;
struct DeleteQParentCluster{ void operator()(G4QParentCluster *aN){delete aN;} };

#endif


