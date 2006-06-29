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
// $Id: AnaEx01AnalysisManager.hh,v 1.9 2006-06-29 16:33:15 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#ifndef AnaEx01AnalysisManager_h
#define AnaEx01AnalysisManager_h 1

#ifdef G4ANALYSIS_USE

class G4Run;
class G4Event;
class G4Step;

namespace AIDA {
  class IAnalysisFactory;
  class ITree;
  class IHistogram1D;
  class ITuple;
}

class AnaEx01AnalysisManager {
public:
  AnaEx01AnalysisManager(AIDA::IAnalysisFactory*);
  virtual ~AnaEx01AnalysisManager();
public:
  virtual void BeginOfRun(const G4Run*); 
  virtual void EndOfRun(const G4Run*); 
  virtual void BeginOfEvent(const G4Event*); 
  virtual void EndOfEvent(const G4Event*); 
  virtual void Step(const G4Step*);
private:
  int fCalorimeterCollID;                
  AIDA::IAnalysisFactory* fAIDA;
  AIDA::ITree* fTree;
  AIDA::IHistogram1D* fEAbs;
  AIDA::IHistogram1D* fLAbs;
  AIDA::IHistogram1D* fEGap;
  AIDA::IHistogram1D* fLGap;
  AIDA::ITuple* fTuple;
};

#else

class AnaEx01AnalysisManager;

#endif

#endif
