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
// File name:     RadmonAnalysisSensitiveDetectorTypeLayout.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonAnalysisSensitiveDetectorTypeLayout.hh,v 1.2 2006-06-28 13:43:53 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Internal class to manage sensitive detector types
//

#ifndef   RADMONANALYSISSENSITIVEDETECTORTYPELAYOUT_HH
 #define  RADMONANALYSISSENSITIVEDETECTORTYPELAYOUT_HH

 // Include files
 #include "RadmonDataAnalysisLayout.hh"
 #include "RadmonTLabelledCollection.hh"
 #include "G4String.hh"
 #include "globals.hh"
 
 class RadmonAnalysisSensitiveDetectorTypeLayout
 {
  public:
   inline                                       RadmonAnalysisSensitiveDetectorTypeLayout();
                                                RadmonAnalysisSensitiveDetectorTypeLayout(const RadmonAnalysisSensitiveDetectorTypeLayout & copy);
   inline                                      ~RadmonAnalysisSensitiveDetectorTypeLayout();
   
   RadmonAnalysisSensitiveDetectorTypeLayout &  operator=(const RadmonAnalysisSensitiveDetectorTypeLayout & copy);

   inline const G4String &                      GetLabel(void) const;
   inline void                                  SetLabel(const G4String & label);

   G4int                                        GetNDataAnalyses(void) const;
   G4bool                                       Empty(void) const;

   const RadmonDataAnalysisLayout &             GetDataAnalysis(G4int index) const;
   RadmonDataAnalysisLayout &                   GetDataAnalysis(G4int index);

   G4bool                                       ExistsDataAnalysisByLabel(const G4String & dataAnalysisLabel) const;
   G4int                                        MultiplicityDataAnalysisByLabel(const G4String & dataAnalysisLabel) const;

   const RadmonDataAnalysisLayout &             FindDataAnalysisByLabel(const G4String & dataAnalysisLabel, G4int count = 0) const;
   RadmonDataAnalysisLayout &                   FindDataAnalysisByLabel(const G4String & dataAnalysisLabel, G4int count = 0);

   RadmonDataAnalysisLayout &                   AppendDataAnalysis(void);
   RadmonDataAnalysisLayout &                   PrependDataAnalysis(void);

   void                                         RemoveDataAnalysisByLabel(const G4String & dataAnalysisLabel, G4int count = 0);
   void                                         RemoveDataAnalysesByLabel(const G4String & dataAnalysisLabel);
   void                                         RemoveDataAnalysis(G4int index);
   void                                         RemoveDataAnalysesByRange(G4int first, G4int last);
   void                                         RemoveAllDataAnalyses(void);

   void                                         DumpLayout(std::ostream & out, const G4String & indent=G4String()) const;

  private:
  // Private attributes
   G4String                                     sensitiveDetectorLabel;
   RadmonTLabelledCollection<RadmonDataAnalysisLayout> dataAnalysesCollection;
 };

 // Inline implementations
 #include "RadmonAnalysisSensitiveDetectorTypeLayout.icc"
#endif /* RADMONANALYSISSENSITIVEDETECTORTYPELAYOUT_HH */
