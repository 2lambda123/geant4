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
// $Id: G4LundStringFragmentation.hh,v 1.2 2005-06-04 13:47:01 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $ Maxim Komogorov
//
// -----------------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, Maxim Komogorov, 10-Jul-1998
// -----------------------------------------------------------------------------

#ifndef G4LundStringFragmentation_h
#define G4LundStringFragmentation_h 1

#include "G4VLongitudinalStringDecay.hh"

//**************************************************************************************************************

class G4LundStringFragmentation: public G4VLongitudinalStringDecay
    {
public:
    G4LundStringFragmentation();
//    G4LundStringFragmentation(G4double sigmaPt);
    G4LundStringFragmentation(const G4LundStringFragmentation &right);
   virtual ~G4LundStringFragmentation();

public:
    const G4LundStringFragmentation & operator=(const G4LundStringFragmentation &right);
    int operator==(const G4LundStringFragmentation &right) const;
    int operator!=(const G4LundStringFragmentation &right) const;


private:
   virtual G4double GetLightConeZ(G4double zmin, G4double zmax, G4int PartonEncoding,  G4ParticleDefinition* pHadron, G4double Px, G4double Py);      
};

//**************************************************************************************************************
// Class G4LundStringFragmentation 
#endif


