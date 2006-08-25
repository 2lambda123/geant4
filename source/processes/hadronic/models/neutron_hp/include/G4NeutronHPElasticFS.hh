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
// $Id: G4NeutronHPElasticFS.hh,v 1.10 2006-08-25 22:54:22 tkoi Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 25-08-06 New Final State type (refFlag==3 , Legendre (Low Energy) + Probability (High Energy) )
//          is added by T. KOI
//
#ifndef G4NeutronHPElasticFS_h
#define G4NeutronHPElasticFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4NeutronHPFinalState.hh"
#include "G4NeutronHPLegendreStore.hh"
#include "G4NeutronHPPartial.hh"
#include "G4NeutronHPFastLegendre.hh"
#include "G4NeutronHPInterpolator.hh"
#include "G4NeutronHPNames.hh"

class G4NeutronHPElasticFS : public G4NeutronHPFinalState
{
  public:
  
  G4NeutronHPElasticFS()
  {
    hasXsec = false; 
    theCoefficients = NULL;
    theProbArray = NULL;
  }
  ~G4NeutronHPElasticFS()
  {
    if(theCoefficients!=NULL) delete theCoefficients;
    if(theProbArray!=NULL) delete theProbArray;
  }
  void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType);
  G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
  G4NeutronHPFinalState * New() 
  {
   G4NeutronHPElasticFS * theNew = new G4NeutronHPElasticFS;
   return theNew;
  }
  
  private:
  G4int repFlag;    // Legendre coeff(1), or probability array(2), or isotropic(0).
                    // add 3: for Legendre (Low Energy) + Probability (High Energy)
  G4double tE_of_repFlag3; // transition energy in case of  repFlag 3:
  G4double targetMass; // in neutronmass units.
  G4int frameFlag;  // CMS or Lab system.
  
  G4NeutronHPLegendreStore * theCoefficients; // the legendre coefficients
  G4NeutronHPPartial * theProbArray; // the probability array p,costh for energy
  G4NeutronHPInterpolator theInt; // interpolation
  
  G4NeutronHPFastLegendre theLegend; // fast look-up for leg-integrals
};
#endif
