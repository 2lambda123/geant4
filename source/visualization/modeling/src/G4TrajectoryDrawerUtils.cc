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
// $Id: G4TrajectoryDrawerUtils.cc,v 1.12 2009-02-24 12:00:56 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Jane Tinslay, John Allison, Joseph Perl November 2005
//
#include "G4TrajectoryDrawerUtils.hh"
#include "G4Colour.hh"
#include "G4Polyline.hh"
#include "G4Polymarker.hh"
#include "G4VTrajectory.hh"
#include "G4VTrajectoryPoint.hh"
#include "G4VisAttributes.hh"
#include "G4VisTrajContext.hh"
#include "G4VVisManager.hh"
#include "G4UIcommand.hh"
#include "G4AttValue.hh"

namespace G4TrajectoryDrawerUtils {


  void GetPoints(const G4VTrajectory& traj, G4Polyline& trajectoryLine,
		 G4Polymarker& auxiliaryPoints, G4Polymarker& stepPoints) 
  {
    for (G4int i=0; i<traj.GetPointEntries(); i++) {
      G4VTrajectoryPoint* aTrajectoryPoint = traj.GetPoint(i);
      
      const std::vector<G4ThreeVector>* auxiliaries
	= aTrajectoryPoint->GetAuxiliaryPoints();
      
      if (0 != auxiliaries) {
	for (size_t iAux=0; iAux<auxiliaries->size(); ++iAux) {
	  const G4ThreeVector pos((*auxiliaries)[iAux]);
	  trajectoryLine.push_back(pos);
	  auxiliaryPoints.push_back(pos);
	}
      }
      const G4ThreeVector pos(aTrajectoryPoint->GetPosition());
      trajectoryLine.push_back(pos);
      stepPoints.push_back(pos);
    }    
  }

  void DrawLineAndPoints(const G4VTrajectory& traj, const G4int& i_mode, const G4Colour& colour, const G4bool& visible) {
    // If i_mode>=0, draws a trajectory as a polyline (default is blue for
    // positive, red for negative, green for neutral) and, if i_mode!=0,
    // adds markers - yellow circles for step points and magenta squares
    // for auxiliary points, if any - whose screen size in pixels is     
    // given by std::abs(i_mode)/1000.  E.g: i_mode = 5000 gives easily 
    // visible markers.
    
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (0 == pVVisManager) return;
    
    const G4double markerSize = std::abs(i_mode)/1000;
    G4bool lineRequired (i_mode >= 0);
    G4bool markersRequired (markerSize > 0.);   
    
    // Return if don't need to do anything
    if (!lineRequired && !markersRequired) return;
    
    // Get points to draw
    G4Polyline trajectoryLine;
    G4Polymarker stepPoints;
    G4Polymarker auxiliaryPoints;
    
    GetPoints(traj, trajectoryLine, auxiliaryPoints, stepPoints);
    
    if (lineRequired) {
      G4VisAttributes trajectoryLineAttribs(colour);
      trajectoryLineAttribs.SetVisibility(visible);
      trajectoryLine.SetVisAttributes(&trajectoryLineAttribs);
      pVVisManager->Draw(trajectoryLine);
    }
    
    if (markersRequired) {
      auxiliaryPoints.SetMarkerType(G4Polymarker::squares);
      auxiliaryPoints.SetScreenSize(markerSize);
      auxiliaryPoints.SetFillStyle(G4VMarker::filled);
      G4VisAttributes auxiliaryPointsAttribs(G4Colour(0.,1.,1.));  // Magenta
      auxiliaryPointsAttribs.SetVisibility(visible);
      auxiliaryPoints.SetVisAttributes(&auxiliaryPointsAttribs);
      pVVisManager->Draw(auxiliaryPoints);
      
      stepPoints.SetMarkerType(G4Polymarker::circles);
      stepPoints.SetScreenSize(markerSize);
      stepPoints.SetFillStyle(G4VMarker::filled);  
      G4VisAttributes stepPointsAttribs(G4Colour(1.,1.,0.));  // Yellow.
      stepPoints.SetVisAttributes(&stepPointsAttribs);
      pVVisManager->Draw(stepPoints);
    }
    
  }
  
  static void GetTimes(const G4VTrajectory& traj,
		       std::vector<G4double>& trajectoryLineTimes,
		       std::vector<G4double>& auxiliaryPointTimes,
		       std::vector<G4double>& stepPointTimes)
  {
    // It is important check that the sizes of times vectors produced
    // by this function matches those of points vectors from
    // GetPoints.  If not, assume that the time information is
    // invalid.

    G4ThreeVector lastTrajectoryPointPosition;
    for (G4int i=0; i<traj.GetPointEntries(); i++) {
      G4VTrajectoryPoint* aTrajectoryPoint = traj.GetPoint(i);

      // Pre- and Post-Point times from the trajectory point...
      G4double trajectoryPointPreTime = -DBL_MAX;
      G4double trajectoryPointPostTime = DBL_MAX;
      std::vector<G4AttValue>* trajectoryPointAttValues =
        aTrajectoryPoint->CreateAttValues();
      if (!trajectoryPointAttValues) {
	G4cout << "G4TrajectoryDrawerUtils::GetTimes: no att values."
	       << G4endl;
	return;
      } else {
	G4bool foundPreTime = false, foundPostTime = false;
        for (std::vector<G4AttValue>::iterator i =
               trajectoryPointAttValues->begin();
             i != trajectoryPointAttValues->end(); ++i) {
          if (i->GetName() == "PreT") {
            trajectoryPointPreTime =
              G4UIcommand::ConvertToDimensionedDouble(i->GetValue());
	    foundPreTime = true;
          }
          if (i->GetName() == "PostT") {
            trajectoryPointPostTime =
	      G4UIcommand::ConvertToDimensionedDouble(i->GetValue());
	    foundPostTime = true;
          }
        }
	if (!foundPreTime || !foundPostTime) {
	  static G4bool warnedTimesNotFound = false;
	  if (!warnedTimesNotFound) {
	    G4cout <<
	      "WARNING: G4TrajectoryDrawerUtils::GetTimes: times not found."
		   << G4endl;
	    warnedTimesNotFound = true;
	  }
	  return;
	}
      }
      
      const G4ThreeVector& trajectoryPointPosition =
	aTrajectoryPoint->GetPosition();

      const std::vector<G4ThreeVector>* auxiliaries
	= aTrajectoryPoint->GetAuxiliaryPoints();
      
      if (0 != auxiliaries) {
	for (size_t iAux=0; iAux<auxiliaries->size(); ++iAux) {
	  // Interpolate time for auxiliary points...
	  const G4ThreeVector pos((*auxiliaries)[iAux]);
	  G4double s1 = (pos - lastTrajectoryPointPosition).mag();
	  G4double s2 = (trajectoryPointPosition - pos).mag();
	  G4double t = trajectoryPointPreTime +
	    (trajectoryPointPostTime - trajectoryPointPreTime) *
	    (s1 / (s1 + s2));
	  trajectoryLineTimes.push_back(t);
	  auxiliaryPointTimes.push_back(t);
	}
      }

      trajectoryLineTimes.push_back(trajectoryPointPostTime);
      stepPointTimes.push_back(trajectoryPointPostTime);

      lastTrajectoryPointPosition = trajectoryPointPosition;
    }    
  }

  static void SliceLine(G4double timeIncrement,
			G4Polyline& trajectoryLine,
			std::vector<G4double>& trajectoryLineTimes)
  {
    // Assumes valid arguments from GetPoints and GetTimes.

    G4Polyline newTrajectoryLine;
    std::vector<G4double> newTrajectoryLineTimes;

    newTrajectoryLine.push_back(trajectoryLine[0]);
    newTrajectoryLineTimes.push_back(trajectoryLineTimes[0]);
    size_t lineSize = trajectoryLine.size();
    if (lineSize > 1) {
      for (size_t i = 1; i < trajectoryLine.size(); ++i) {
	G4double deltaT = trajectoryLineTimes[i] - trajectoryLineTimes[i - 1];
	if (deltaT > 0.) {
	  G4double practicalTimeIncrement = 
	    std::max(timeIncrement, deltaT / 100.);
	  for (G4double t =
		 (int(trajectoryLineTimes[i - 1]/practicalTimeIncrement) + 1) *
		 practicalTimeIncrement;
	       t <= trajectoryLineTimes[i];
	       t += practicalTimeIncrement) {
	    G4ThreeVector pos = trajectoryLine[i - 1] +
	      (trajectoryLine[i] - trajectoryLine[i - 1]) *
	      ((t - trajectoryLineTimes[i - 1]) / deltaT);
	    newTrajectoryLine.push_back(pos);
	    newTrajectoryLineTimes.push_back(t);
	  }
	}
	newTrajectoryLine.push_back(trajectoryLine[i]);
	newTrajectoryLineTimes.push_back(trajectoryLineTimes[i]);
      }
    }

    trajectoryLine = newTrajectoryLine;
    trajectoryLineTimes = newTrajectoryLineTimes;
  }

  static void DrawWithoutTime(const G4VisTrajContext& myContext,
			      G4Polyline& trajectoryLine,
			      G4Polymarker& auxiliaryPoints,
			      G4Polymarker& stepPoints)
  {
    // Draw without time slice information

    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (0 == pVVisManager) return;
    
    if (myContext.GetDrawLine()) {
      G4VisAttributes trajectoryLineAttribs(myContext.GetLineColour());
      trajectoryLineAttribs.SetVisibility(myContext.GetLineVisible());
      trajectoryLine.SetVisAttributes(&trajectoryLineAttribs);

      pVVisManager->Draw(trajectoryLine);
    }
  
    if (myContext.GetDrawAuxPts() && (auxiliaryPoints.size() > 0)) {
      auxiliaryPoints.SetMarkerType(myContext.GetAuxPtsType());
      auxiliaryPoints.SetSize(myContext.GetAuxPtsSizeType(), myContext.GetAuxPtsSize());
      auxiliaryPoints.SetFillStyle(myContext.GetAuxPtsFillStyle());

      G4VisAttributes auxiliaryPointsAttribs(myContext.GetAuxPtsColour());  
      auxiliaryPointsAttribs.SetVisibility(myContext.GetAuxPtsVisible());
      auxiliaryPoints.SetVisAttributes(&auxiliaryPointsAttribs);

      pVVisManager->Draw(auxiliaryPoints);
    }
  
    if (myContext.GetDrawStepPts() && (stepPoints.size() > 0)) {
      stepPoints.SetMarkerType(myContext.GetStepPtsType());
      stepPoints.SetSize(myContext.GetStepPtsSizeType(), myContext.GetStepPtsSize());
      stepPoints.SetFillStyle(myContext.GetStepPtsFillStyle());

      G4VisAttributes stepPointsAttribs(myContext.GetStepPtsColour()); 
      stepPointsAttribs.SetVisibility(myContext.GetStepPtsVisible());
      stepPoints.SetVisAttributes(&stepPointsAttribs);

      pVVisManager->Draw(stepPoints);
    }
  }

  static void DrawWithTime(const G4VisTrajContext& myContext,
			   G4Polyline& trajectoryLine,
			   G4Polymarker& auxiliaryPoints,
			   G4Polymarker& stepPoints,
			   std::vector<G4double>& trajectoryLineTimes,
			   std::vector<G4double>& auxiliaryPointTimes,
			   std::vector<G4double>& stepPointTimes)
  {
    // Draw with time slice information

    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (0 == pVVisManager) return;

    if (myContext.GetDrawLine()) {
      G4VisAttributes trajectoryLineAttribs(myContext.GetLineColour());
      trajectoryLineAttribs.SetVisibility(myContext.GetLineVisible());

      for (size_t i = 1; i < trajectoryLine.size(); ++i ) {
	G4Polyline slice;
	slice.push_back(trajectoryLine[i -1]);
	slice.push_back(trajectoryLine[i]);
	trajectoryLineAttribs.SetStartTime(trajectoryLineTimes[i - 1]);
	trajectoryLineAttribs.SetEndTime(trajectoryLineTimes[i]);
	slice.SetVisAttributes(&trajectoryLineAttribs);
	pVVisManager->Draw(slice);
      }
    }

    if (myContext.GetDrawAuxPts() && (auxiliaryPoints.size() > 0)) {
      G4VisAttributes auxiliaryPointsAttribs(myContext.GetAuxPtsColour());  
      auxiliaryPointsAttribs.SetVisibility(myContext.GetAuxPtsVisible());

      for (size_t i = 0; i < auxiliaryPoints.size(); ++i ) {
	G4Polymarker point;
	point.push_back(auxiliaryPoints[i]);
	point.SetMarkerType(myContext.GetAuxPtsType());
	point.SetSize(myContext.GetAuxPtsSizeType(), myContext.GetAuxPtsSize());
	point.SetFillStyle(myContext.GetAuxPtsFillStyle());
	auxiliaryPointsAttribs.SetStartTime(auxiliaryPointTimes[i]);
	auxiliaryPointsAttribs.SetEndTime(auxiliaryPointTimes[i]);
	point.SetVisAttributes(&auxiliaryPointsAttribs);
	pVVisManager->Draw(point);
      }
    }

    if (myContext.GetDrawStepPts() && (stepPoints.size() > 0)) {
      G4VisAttributes stepPointsAttribs(myContext.GetStepPtsColour()); 
      stepPointsAttribs.SetVisibility(myContext.GetStepPtsVisible());

      for (size_t i = 0; i < stepPoints.size(); ++i ) {
	G4Polymarker point;
	point.push_back(stepPoints[i]);
	point.SetMarkerType(myContext.GetStepPtsType());
	point.SetSize(myContext.GetStepPtsSizeType(), myContext.GetStepPtsSize());
	point.SetFillStyle(myContext.GetStepPtsFillStyle());
	stepPointsAttribs.SetStartTime(stepPointTimes[i]);
	stepPointsAttribs.SetEndTime(stepPointTimes[i]);
	point.SetVisAttributes(&stepPointsAttribs);
	pVVisManager->Draw(point);
      }
    }
  }

  void DrawLineAndPoints(const G4VTrajectory& traj, const G4VisTrajContext& context, const G4int& i_mode) 
  {
    // Extra copy while i_mode is still around
    G4VisTrajContext myContext(context);
    
    if (i_mode != 0) {
      const G4double markerSize = std::abs(i_mode)/1000;
      G4bool lineRequired (i_mode >= 0);
      G4bool markersRequired (markerSize > 0.);        
      
      myContext.SetDrawLine(lineRequired);
      myContext.SetDrawAuxPts(markersRequired);
      myContext.SetDrawStepPts(markersRequired);

      myContext.SetAuxPtsSize(markerSize);
      myContext.SetStepPtsSize(markerSize);

      static G4bool warnedAboutIMode = false;

      if (!warnedAboutIMode) {
	G4cout<<"Trajectory drawing configuration will be based on imode value of "<<i_mode<<G4endl;
	warnedAboutIMode = true;
      }
    }

    // Return if don't need to do anything
    if (!myContext.GetDrawLine() && !myContext.GetDrawAuxPts() && !myContext.GetDrawStepPts()) return;
    
    // Get points to draw
    G4Polyline trajectoryLine;
    G4Polymarker stepPoints;
    G4Polymarker auxiliaryPoints;
    
    GetPoints(traj, trajectoryLine, auxiliaryPoints, stepPoints);
    
    if (myContext.GetTimeSliceInterval()) {

      // Get corresponding track time information, if any
      std::vector<G4double> trajectoryLineTimes;
      std::vector<G4double> stepPointTimes;
      std::vector<G4double> auxiliaryPointTimes;
  
      GetTimes(traj, trajectoryLineTimes, auxiliaryPointTimes, stepPointTimes);

      // Check validity
      if (trajectoryLineTimes.size() != trajectoryLine.size() ||
	  stepPointTimes.size() != stepPoints.size() ||
	  auxiliaryPointTimes.size() != auxiliaryPoints.size()) {

	// Revert to drawing without time information...
	DrawWithoutTime(myContext, trajectoryLine, auxiliaryPoints, stepPoints);
      } else {

	SliceLine(myContext.GetTimeSliceInterval(),
		  trajectoryLine, trajectoryLineTimes);

	DrawWithTime(myContext,
		     trajectoryLine, auxiliaryPoints, stepPoints,
		     trajectoryLineTimes, auxiliaryPointTimes, stepPointTimes);
      }

    } else {

      DrawWithoutTime(myContext, trajectoryLine, auxiliaryPoints, stepPoints);

    }
  }
}
