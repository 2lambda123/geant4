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
// $Id: G4NeutronHPContAngularPar.hh,v 1.5 2001-07-11 10:06:56 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPContAngularPar_h
#define G4NeutronHPContAngularPar_h 1

#include "G4ios.hh"
#include "g4std/fstream"
#include "globals.hh"
#include "G4NeutronHPList.hh"
#include "G4ReactionProduct.hh"
#include "G4NeutronHPInterpolator.hh"
#include "G4InterpolationManager.hh"

class G4NeutronHPContAngularPar
{
  public:
  
  G4NeutronHPContAngularPar()
  {
    theAngular = NULL;
    currentMeanEnergy = -2;
  }
  ~G4NeutronHPContAngularPar()
  {
    if(theAngular!=NULL) delete [] theAngular;
  }
  
  void Init(G4std::ifstream & aDataFile);
  
  G4ReactionProduct * Sample(G4double anEnergy, G4double massCode, G4double mass, 
                             G4int angularRep, G4int interpol);
  
  G4double GetEnergy() { return theEnergy; }
  
  void SetPrimary(G4ReactionProduct * aPrimary)
  {
    thePrimary = aPrimary;
  }
  
  void SetTarget(G4ReactionProduct * aTarget)
  {
    theTarget = aTarget;
  }
  
  void SetTargetCode(G4double aTargetCode) { theTargetCode = aTargetCode; }
  
  void SetInterpolation(G4int theInterpolation)
  {
    theManager.Init(theInterpolation, nEnergies); // one range only
  }

  void Merge(G4double anEnergy, G4InterpolationScheme & aScheme, 
             G4NeutronHPContAngularPar & store1, 
             G4NeutronHPContAngularPar & store2) // hmmmm, this interpolates legendre coefficients. Dangerous @@@
  {
    nDiscreteEnergies = store1.nDiscreteEnergies;
    nAngularParameters = store1.nAngularParameters;
    nEnergies = store1.nEnergies;
    theManager = store1.theManager;
    theEnergy = anEnergy;
    if(theAngular != NULL) delete [] theAngular;
    theAngular = new G4NeutronHPList[nEnergies];
    G4int i, ii;
    G4double value;
    for(i=0; i<nEnergies; i++)
    {
      theAngular[i].SetLabel(store1.theAngular[i].GetLabel());
      for(ii=0; ii<nAngularParameters; ii++)
      {
//        G4cout <<"test "<<i<<" "<<store1.theEnergy<<" "<<store2.theEnergy<<" "
//             << store1.theAngular[i].GetValue(ii)<<" "<<
//             store2.theAngular[i].GetValue(ii)<<G4endl;
        value = theInt.Interpolate(aScheme, anEnergy, 
                                   store1.theEnergy, store2.theEnergy,
                                   store1.theAngular[i].GetValue(ii),
                                   store2.theAngular[i].GetValue(ii));
        theAngular[i].SetValue(ii, value);
      }
    }
  };
  
  G4double MeanEnergyOfThisInteraction()
  {
    G4double result;
    if(currentMeanEnergy<-1)
    {
      return 0;
      // G4Exception("G4NeutronHPContAngularPar: Logical error in Product class");
    }
    else
    {
      result = currentMeanEnergy;
    }
    currentMeanEnergy = -2;
    return result;
  }
  
  private:
  
  // incoming particle
  G4double theEnergy; 
  
  // number of exit channel energies
  G4int nEnergies; 
  // number of discrete exit channels
  G4int nDiscreteEnergies;
  // number of angular paramerers per channel
  G4int nAngularParameters;
  // knows the interpolation between List labels
  G4InterpolationManager theManager; 
  // on per exit-channel energy
  G4NeutronHPList * theAngular; 
  
  private:
  
  G4NeutronHPInterpolator theInt;
  
  G4double theTargetCode;
  G4ReactionProduct * theTarget;
  G4ReactionProduct * thePrimary;
  
  G4double currentMeanEnergy;
};
#endif
