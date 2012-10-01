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
/// \file persistency/P03/include/ExTGRCRegionCutsMgr.hh
/// \brief Definition of the ExTGRCRegionCutsMgr class
//
// $Id: ExTGRCRegionCutsMgr.hh,v 1.3 2010-11-05 08:52:34 gcosmo Exp $

#ifndef ExTGRCRegionCutsMgr_h
#define ExTGRCRegionCutsMgr_h

#include <vector>
#include "globals.hh"

class ExTGRCRegionData;

/// Region cuts manager
///
/// Changes:     creation   May 2007
/// \author      P. Arce

class ExTGRCRegionCutsMgr 
{ 
  public:

   ~ExTGRCRegionCutsMgr();

    static ExTGRCRegionCutsMgr* GetInstance();
      // Get the only instance 

    void AddRegionData( const std::vector<G4String>& rd );

    void AddRegionCuts( const std::vector<G4String>& rc );
  
    std::vector<ExTGRCRegionData*> FindRegionData( const G4String& name);

    void BuildRegions();
    void BuildProductionCuts();

  private:

    ExTGRCRegionCutsMgr();

  private:

    static ExTGRCRegionCutsMgr* fInstance;
    std::vector<ExTGRCRegionData*> fRegionDatae;
};

#endif
