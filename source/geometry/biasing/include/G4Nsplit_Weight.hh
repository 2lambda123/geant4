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
// $Id: G4Nsplit_Weight.hh,v 1.10 2003-06-16 16:51:01 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4Nsplit_Weight
//
// Class description:
// 
// A class (struct) used by importance sampling. It contains the number
// of tracks a mother track should be split into and their weight.
 
// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4Nsplit_Weight_hh
#define G4Nsplit_Weight_hh G4Nsplit_Weight_hh

#include "globals.hh"

class G4Nsplit_Weight
{
  public:

  G4int fN;
    // number of tracks a mother track should be split into
    // including the mother track

  G4double fW;
    // the weight to be given to the tracks
};

std::ostream& operator<<(std::ostream &out, 
			   const G4Nsplit_Weight &nw);


#endif










