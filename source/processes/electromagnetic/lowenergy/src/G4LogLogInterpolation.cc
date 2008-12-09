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
// $Id: G4LogLogInterpolation.cc,v 1.13 2008-12-09 22:31:50 sincerti Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//         Nicolas A. Karakatsanis (knicolas@mail.ntua.gr)
// History:
// -----------
// 31 Jul 2001   MGP        Created
// 08 Dec 2008   NAK        Log-Log interpolation math formula streamlined
// -------------------------------------------------------------------

#include "G4LogLogInterpolation.hh"

// Constructor

G4LogLogInterpolation::G4LogLogInterpolation()
{ }

// Destructor

G4LogLogInterpolation::~G4LogLogInterpolation()
{ }

G4VDataSetAlgorithm* G4LogLogInterpolation::Clone() const 
{ return new G4LogLogInterpolation; }


G4double G4LogLogInterpolation::Calculate(G4double x, G4int bin, 
					  const G4DataVector& points, 
					  const G4DataVector& data) const
{
  G4int nBins = data.size() - 1;
  G4double oldresult = 0.;
  G4double value = 0.;
  if (x < points[0])
    {
      value = 0.;
    }
  else if (bin < nBins)
    {
      G4double e1 = points[bin];
      G4double e2 = points[bin+1];
      G4double d1 = data[bin];
      G4double d2 = data[bin+1];
// Check of e1, e2, d1 and d2 values to avoid floating-point errors when estimating the interpolated value below
      if ((d1 > 0.) && (d2 > 0.) && (e1 > 0.) && (e2 > 0.))
        {
// Streamline the Log-Log Interpolation formula in order to reduce the required number of log10() function calls
// Variable oldresult contains the result of old implementation of Log-Log interpolation
         oldresult = (std::log10(d1)*std::log10(e2/x) + std::log10(d2)*std::log10(x/e1)) / std::log10(e2/e1);
         oldresult = std::pow(10.,oldresult);
// Variable value contains the result of new implementation, after streamlining the math operation
         value = std::log10(d1)+(std::log10(d2/d1)/std::log10(e2/e1)*std::log10(x/e1));
         value = std::pow(10.,value);
// Test of the new implementation (value variable) against the old one (oldresult variable)
         G4double diffResult = value - oldresult;
         G4double relativeDiff = 1e-11;
// Comparison of the two values based on a max allowable relative difference
         if ( std::fabs(diffResult) > relativeDiff*std::fabs(oldresult) )
          {
// Abort comparison when at least one of two results is infinite
             if ((!std::isinf(oldresult)) && (!std::isinf(value)))
              {
                G4cout << "G4LogLogInterpolation> Old Interpolated Value is:" << oldresult << G4endl;
                G4cout << "G4LogLogInterpolation> New Interpolated Value is:" << value << G4endl << G4endl;
                G4cerr << "G4LogLogInterpolation> Error in Interpolation:" << G4endl;
                G4cerr << "The difference between new and old interpolated value is:" << diffResult << G4endl << G4endl;
              }
          }
        }
      else value = 0.;
    }
  else
    {
      value = data[nBins];
    }

  return value;
}
