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
// $Id: G4OpenInventorWinViewer.hh,v 1.3 2004-11-08 17:32:52 gbarrand Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Jeff Kallenbach 01 Aug 1996
// OpenInventor viewer - opens window, hard copy, etc.

#ifndef G4OPENINVENTORWINVIEWER_HH
#define G4OPENINVENTORWINVIEWER_HH

#ifdef G4VIS_BUILD_OI_DRIVER

#include "G4VViewer.hh"

#include <windows.h>

class Geant4_SoWinExaminerViewer;
class SoSelection;
class G4OpenInventorSceneHandler;
class G4VInteractorManager;

class G4OpenInventorWinViewer: public G4VViewer {

public:
  G4OpenInventorWinViewer(G4OpenInventorSceneHandler& scene,
		       const G4String& name = "");
  virtual ~G4OpenInventorWinViewer();
  void DrawView();
  void ShowView();
  Geant4_SoWinExaminerViewer* GetInventorViewer() const;
private:
  void ClearView();
  void FinishView();
  void SetView();
  void KernelVisitDecision();
  G4bool CompareForKernelVisit(G4ViewParameters&);
private:
  G4OpenInventorSceneHandler& fG4OpenInventorSceneHandler;
  G4ViewParameters fLastVP;  // Memory for making kernel visit decisions.
  HWND fShell;
  Geant4_SoWinExaminerViewer* fViewer;
  SoSelection* fSelection;
  G4VInteractorManager* fInteractorManager;
};

#endif

#endif
