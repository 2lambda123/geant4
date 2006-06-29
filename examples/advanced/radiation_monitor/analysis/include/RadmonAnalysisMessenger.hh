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
// File name:     RadmonDetectorMessenger.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonAnalysisMessenger.hh,v 1.3 2006-06-29 16:06:47 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   UI commands for managing a layout
//

#ifndef   RADMONANALYSISMESSENGER_HH
 #define  RADMONANALYSISMESSENGER_HH

 // Include files
 #include "RadmonMessenger.hh"
 #include "G4String.hh"

 // Forward declarations
 class RadmonVAnalysisLayout;
 
 class RadmonAnalysisMessenger : public RadmonMessenger
 {
  public:
                                                RadmonAnalysisMessenger(RadmonVAnalysisLayout * layout);
                                               ~RadmonAnalysisMessenger();

   virtual G4String                             GetCurrentValue(G4UIcommand * command);
   virtual void                                 SetNewValue(G4UIcommand * command, G4String newValue);

  private:
  // Hidden constructors and operators
                                                RadmonAnalysisMessenger();
                                                RadmonAnalysisMessenger(const RadmonAnalysisMessenger & copy);
   RadmonAnalysisMessenger &                    operator=(const RadmonAnalysisMessenger & copy);

  // Private Attributes
   RadmonVAnalysisLayout *                      analysisLayout;

  // Commands
   RADMON_DECLARE_COMMAND(SetOutputFileName);
   RADMON_DECLARE_COMMAND(SetOutputFileFormat);
  
   RADMON_DECLARE_COMMAND(CreateSensitiveDetector);
   RADMON_DECLARE_COMMAND(SetSensitiveDetectorType);
   RADMON_DECLARE_COMMAND(RemoveSensitiveDetector);
   
   RADMON_DECLARE_COMMAND(CreateSensitiveDetectorType);
   RADMON_DECLARE_COMMAND(RemoveSensitiveDetectorType);

   RADMON_DECLARE_COMMAND(AppendDataAnalysisToSensitiveDetectorType);
   RADMON_DECLARE_COMMAND(SetDataAnalysisType);
   RADMON_DECLARE_COMMAND(RemoveDataAnalysis);

   RADMON_DECLARE_COMMAND(SetDataAnalysisAttribute);
   RADMON_DECLARE_COMMAND(ClearDataAnalysisAttribute);

   RADMON_DECLARE_COMMAND(DumpLayout);
   RADMON_DECLARE_COMMAND(Load);
   RADMON_DECLARE_COMMAND(Save);
 };
#endif /* RADMONANALYSISMESSENGER_HH */
