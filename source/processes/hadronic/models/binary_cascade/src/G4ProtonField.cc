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
// -------------------------------------------------------------------
//      GEANT 4 class implementation file 
//
//      CERN, Geneva, Switzerland
//
//      File name:     G4ProtonField.cc
//
//      Author:        Alessandro Brunengo (Alessandro.Brunengo@ge.infn.it)
// 
//      Creation date: 5 June 2000
// -------------------------------------------------------------------
#include "G4ProtonField.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4VNuclearDensity.hh"
#include "G4FermiMomentum.hh"
#include "G4V3DNucleus.hh"

G4ProtonField::G4ProtonField(G4V3DNucleus * aNucleus) : 
   G4VNuclearField(aNucleus), theDensity(theNucleus->GetNuclearDensity())
{ 
  theA = theNucleus->GetMassNumber();
  theZ = theNucleus->GetCharge();
  theBarrier = GetBarrier();
  theRadius = 2.*theNucleus->GetOuterRadius();
  theFermi.Init(theA, theZ);
  G4double aR=0;
  while(aR<theRadius)
  {
    G4ThreeVector aPosition(0,0,aR);
    G4double density = GetDensity(aPosition);
    G4double fermiMom = GetFermiMomentum(density);
    theFermiMomBuffer.push_back(fermiMom);
    aR+=0.3*fermi;
  }
  {
  G4ThreeVector aPosition(0,0,theRadius);
  G4double density = GetDensity(aPosition);
  G4double fermiMom = GetFermiMomentum(density);
  theFermiMomBuffer.push_back(fermiMom);  
  }
  {
  G4ThreeVector aPosition(0,0,theRadius+0.001*fermi);
  theFermiMomBuffer.push_back(0);  
  }
  {
  G4ThreeVector aPosition(0,0,1.*m);
  theFermiMomBuffer.push_back(0);  
  }
}


G4ProtonField::~G4ProtonField()
{ }


const G4ProtonField & G4ProtonField::operator=(const G4ProtonField & right)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ProtonField::operator= meant not to be accessible");
  return *this;
}


G4int G4ProtonField::operator==(const G4ProtonField & right) const
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ProtonField::operator== meant not to be accessible");
  return 0;
}


G4int G4ProtonField::operator!=(const G4ProtonField & right) const
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ProtonField::operator!= meant not to be accessible");
  return 1;
}


G4double G4ProtonField::GetField(const G4ThreeVector & aPosition)
{
//G4cout << " Fermi Potential " << (fermiMom*fermiMom)/(2*proton_mass_c2) <<G4endl;
  G4double x = aPosition.mag();
  G4int index = static_cast<G4int>(x/(0.3*fermi) );
  if(index+2>theFermiMomBuffer.size()) return theFermiMomBuffer.back();
  G4double y1 = theFermiMomBuffer[index];
  G4double y2 = theFermiMomBuffer[index+1];
  G4double x1 = (0.3*fermi)*index;
  G4double x2 = (0.3*fermi)*(index+1);
  G4double fermiMom = y1 + (x-x1)*(y2-y1)/(x2-x1);
  G4double y = -1*(fermiMom*fermiMom)/(2*proton_mass_c2)+theBarrier;
//  G4cout <<" Protonfield test "<<index<<" "<< x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x<<" "<<y<<" "<<theBarrier<<G4endl;
  return y;
}



G4double G4ProtonField::GetBarrier()
{
  G4double coulombBarrier = (1.44/1.14) * MeV * theZ / (1.0 + pow(theA,1./3.));
//GF   G4double bindingEnergy = G4NucleiPropertiesTable::GetBindingEnergy(Z, A);
  G4double bindingEnergy =0;
/*
 *   G4cout << " coulombBarrier/bindingEnergy : " 
 *   	 << coulombBarrier << " /" << bindingEnergy << G4endl;
 */
  return bindingEnergy/theA+coulombBarrier;
}


