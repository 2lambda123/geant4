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
// $Id: G4DistributionGenerator.cc,v 1.5 2001-07-11 10:08:08 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4DistributionGenerator
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 8 May 1998
//
//      Modifications: 
// -------------------------------------------------------------------

#include "globals.hh"
#include "G4DistributionGenerator.hh"
#include "G4ios.hh"
#include <assert.h>

// Constructor

G4DistributionGenerator::G4DistributionGenerator(G4RWTValOrderedVector<G4double>& x,
						 G4RWTValOrderedVector<G4double>& values)
  
{
  _x = x;

  // Check boundaries: must be size(x) = size(values) + 1
  if (x.entries() != (values.entries() + 1))
    { G4cout << " Inconsistent parameters in G4DistributionGenerator "
	   << G4endl;
    }
  assert (x.entries() == (values.entries() + 1));

  G4double tot = 0.;
  int i;
  for (i=0; i<values.entries(); i++) { tot += values[i]; }
  assert (tot > 0.);
  
  _cumProb.insert(0.);    
  //  _cumProb.insert(values[0] / tot);
  G4double sum = 0.;
  for (i=0; i<values.entries(); i++) 
    { 
      sum += values[i];
      _cumProb.insert(sum / tot); }

  // Debugging
  /*
  for (i=0; i<values.entries(); i++)
    { G4cout << values[i] << "  " ; }
  G4cout << "  Integral = " << tot << G4endl;
  for (i=0; i<_cumProb.entries(); i++)
    { 
      G4cout << "Variable " << _x[i]  
	   << " --- cumProb = " << _cumProb[i] << G4endl;
    }
  */
  // End of debugging

}

// Destructor

G4DistributionGenerator::~G4DistributionGenerator()
{
}


G4double G4DistributionGenerator::Generate(G4double ranflat)
{
  G4double xRandom = _x[0];

  G4int bin = _cumProb.entries() - 1;
  int i;
  for (i=1; i<_cumProb.entries(); i++)
    {
      if (ranflat >= _cumProb[i-1] && ranflat < _cumProb[i])
	{
	  bin = i - 1;
	}
    }

  if (bin >= 0 && bin < (_cumProb.entries()-1) && bin < (_x.entries()-1)) 
    {
      G4double coeff = (ranflat - _cumProb[bin]) *  (_x[bin+1] - _x[bin]) / 
                   (_cumProb[bin+1] - _cumProb[bin]);
      xRandom = _x[bin] + coeff;

      // Deugging
      /*
      G4cout << "Random = " << ranflat << " Generated " << xRandom << G4endl;
      */
      // Endo of Debugging

    }
  else
    { 
      // Debugging
      /*
      G4cout << "Bin " << bin << " "
	   << _cumProb.entries() << " " 
	   << _x.entries()
	   << G4endl;
      */
      // End of debugging
    }

  return xRandom;

}


