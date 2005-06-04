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
// $Id: G4HEKaonZeroShortInelastic.hh,v 1.12 2005-06-04 13:32:52 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// G4 Gheisha High Energy model class -- header file
// H. Fesefeldt, RWTH Aachen 23-October-1996
// Last modified: 10-December-1996

// A prototype of the Gheisha High Energy collision model.

#ifndef G4HEKaonZeroShortInelastic_h
#define G4HEKaonZeroShortInelastic_h 1

// Class description:
// High energy parameterized model for K0S inelastic scattering.  This
// class is responsible for producing the final state of the interaction and
// is typically valid for incident K0S energies above 20 GeV.  This
// physics may be invoked by registering an instance of the class with
// G4KaonZeroSInelasticProcess in the user's physics list.
//
// This class is derived from G4HEInelastic which in turn is derived from
// G4HadronicInteraction.

// Class Description - End

#include "G4HEKaonZeroInelastic.hh"
#include "G4HEAntiKaonZeroInelastic.hh"

class G4HEKaonZeroShortInelastic : public G4HEInelastic  
{
 public:  // with description
        G4HEKaonZeroShortInelastic() 
           {
              theMinEnergy =  20*GeV;
              theMaxEnergy = 10*TeV;
              MAXPART      = 2048;
              verboseLevel = 0; 
           }

        ~G4HEKaonZeroShortInelastic(){ };
         
        G4int vecLength;

        void SetMaxNumberOfSecondaries(G4int maxnumber)
             { MAXPART = maxnumber;};
        void SetVerboseLevel(G4int verbose)
             { verboseLevel = verbose;};
        G4int GetNumberOfSecondaries()
             { return vecLength;};           

        G4HadFinalState * ApplyYourself( const G4HadProjectile &aTrack, G4Nucleus &targetNucleus );

};
#endif                     
                                         

