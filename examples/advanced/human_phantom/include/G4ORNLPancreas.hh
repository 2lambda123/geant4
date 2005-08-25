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

#ifndef G4ORNLPancreas_h
#define G4ORNLPancreas_h 1

#include "G4VPhysicalVolume.hh"

#include "Saxana/SAXProcessor.h"
#include "Saxana/ProcessingConfigurator.h"

class G4VPhysicalVolume;
class G4LogicalVolume;

class G4ORNLPancreas
{
public:

  G4ORNLPancreas();
  ~G4ORNLPancreas();
  G4VPhysicalVolume* ConstructPancreas(G4VPhysicalVolume*, G4String, G4bool);
 
private:
    SAXProcessor sxp;
    ProcessingConfigurator config;

};
#endif
