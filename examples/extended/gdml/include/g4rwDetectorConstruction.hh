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
// -*- C++ -*-
// $Id: g4rwDetectorConstruction.hh,v 1.2 2004-12-15 15:41:27 gunter Exp $
#ifndef g4rwDetectorConstruction_H
#define g4rwDetectorConstruction_H 1

class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"

#include "Saxana/SAXProcessor.h"
#include "Saxana/ProcessingConfigurator.h"

class gogdmlDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    gogdmlDetectorConstruction();
    ~gogdmlDetectorConstruction();

  public:
    G4VPhysicalVolume* Construct();

  private:
    SAXProcessor sxp;
    ProcessingConfigurator config;
    G4VPhysicalVolume* fWorld;
};

#endif

