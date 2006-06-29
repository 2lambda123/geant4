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
// File name:     RadmonApplicationGeneratorSetup.cc
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonApplicationGeneratorSetup.cc,v 1.3 2006-06-29 16:08:39 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//

// Include files
#include "RadmonApplicationGeneratorSetup.hh"
#include "RadmonApplicationOptions.hh"

#include "RadmonGeneratorFixedPosition.hh"
#include "RadmonGeneratorFixedDirection.hh"
#include "RadmonGeneratorFixedEnergy.hh"
#include "RadmonGeneratorFixedParticle.hh"
#include "RadmonGeneratorUniformSphere.hh"
#include "RadmonGeneratorUniformPlane.hh"


#include "RadmonGeneratorsWithLabelFactory.hh"


#define DECLARE_GENERATOR_CONSTRUCTOR(name)     constructor=new name();                                                                  \
                                                if (constructor==0)                                                                      \
                                                {                                                                                        \
                                                 G4cerr << currentOptions.ApplicationName() << ": Cannot allocate " #name "." << G4endl; \
                                                 return false;                                                                           \
                                                }                                                                                        \
                                                factory->AppendGenerator(constructor)

G4bool                                          RadmonApplicationGeneratorSetup :: CreateGenerators(RadmonGeneratorsWithLabelFactory * factory)
{
 RadmonVGeneratorWithLabel * constructor;

 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorFixedPosition);
 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorFixedDirection);
 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorFixedEnergy);
 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorFixedParticle);
 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorUniformSphere);
 DECLARE_GENERATOR_CONSTRUCTOR(RadmonGeneratorUniformPlane);
 
 return true;
}
