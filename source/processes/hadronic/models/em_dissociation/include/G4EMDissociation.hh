#ifndef G4EMDissociation_h
#define G4EMDissociation_h
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:		G4EMDissociation.hh
//
// Version:		B.1
// Date:		15/04/04
// Author:		P R Truscott
// Organisation:	QinetiQ Ltd, UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		17191/03/NL/LvH
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 17 October 2003, P R Truscott, QinetiQ Ltd, UK
// Created.
//
// 15 March 2004, P R Truscott, QinetiQ Ltd, UK
// Beta release
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
// * This  code  implementation  is  the  intellectual   property  of *
// * QinetiQ.   Rights  to use  this  software  are  granted  to  the *
// * European Space Agency  under  the  provisions  of  Clause 39  of *
// * the    Standard    Conditions   for  Contract.   ESA    contract *
// * 17191/03/NL/LvH  provides  the  rights to distribute source code *
// * through  the  Geant4 Collaboration Agreement for general  public *
// * use.  Some elements of this source code may be derived from code *
// * developed   by   other   member   institutes   of   the   Geant4 *
// * Collaboration, and the provision of  their code under the Geant4 *
// * Collaboration is acknowledged.                                   *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Class Description
//
//
// Class Description - End
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "G4HadronicInteraction.hh"
#include "G4ExcitationHandler.hh"
#include "G4HadProjectile.hh"
#include "G4Nucleus.hh"
#include "G4EMDissociationCrossSection.hh"
#include "G4EMDissociationSpectrum.hh"
#include "globals.hh"
////////////////////////////////////////////////////////////////////////////////
//
class G4EMDissociation : public G4HadronicInteraction
{
  public:
    G4EMDissociation ();
    G4EMDissociation (G4ExcitationHandler *);
    ~G4EMDissociation ();
    
    const G4EMDissociation &operator=(G4EMDissociation &right);

    virtual G4HadFinalState *ApplyYourself (const G4HadProjectile &,
      G4Nucleus &);

  private:
    void PrintWelcomeMessage ();

  private:
    G4ExcitationHandler          *theExcitationHandler;
    G4bool                       handlerDefinedInternally;
    G4EMDissociationCrossSection *dissociationCrossSection;
    G4EMDissociationSpectrum     *thePhotonSpectrum;
};
////////////////////////////////////////////////////////////////////////////////
//
#endif
