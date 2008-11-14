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
// $Id: G4VoxelNavigation.cc,v 1.9 2008-11-14 18:26:35 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4VoxelNavigation Implementation
//
// Author: P.Kent, 1996
//
// --------------------------------------------------------------------

#include "G4VoxelNavigation.hh"
#include "G4GeometryTolerance.hh"

// ********************************************************************
// Constructor
// ********************************************************************
//
G4VoxelNavigation::G4VoxelNavigation()
  : fVoxelDepth(-1),
    fVoxelAxisStack(kNavigatorVoxelStackMax,kXAxis),
    fVoxelNoSlicesStack(kNavigatorVoxelStackMax,0),
    fVoxelSliceWidthStack(kNavigatorVoxelStackMax,0.),
    fVoxelNodeNoStack(kNavigatorVoxelStackMax,0),
    fVoxelHeaderStack(kNavigatorVoxelStackMax,(G4SmartVoxelHeader*)0),
    fVoxelNode(0),
    fCheck(false),
    fVerbose(0)
{
  kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
}

// ********************************************************************
// Destructor
// ********************************************************************
//
G4VoxelNavigation::~G4VoxelNavigation()
{
#ifdef G4DEBUG_NAVIGATION
  G4cout << "G4VoxelNavigation::~G4VoxelNavigation() called." << G4endl;
#endif
}

// ********************************************************************
// ComputeStep
// ********************************************************************
//
G4double
G4VoxelNavigation::ComputeStep( const G4ThreeVector& localPoint,
                                const G4ThreeVector& localDirection,
                                const G4double currentProposedStepLength,
                                      G4double& newSafety,
                                      G4NavigationHistory& history,
                                      G4bool& validExitNormal,
                                      G4ThreeVector& exitNormal,
                                      G4bool& exiting,
                                      G4bool& entering,
                                      G4VPhysicalVolume *(*pBlockedPhysical),
                                      G4int& blockedReplicaNo )
{
  G4VPhysicalVolume *motherPhysical, *samplePhysical, *blockedExitedVol=0;
  G4LogicalVolume *motherLogical;
  G4VSolid *motherSolid;
  G4ThreeVector sampleDirection;
  G4double ourStep=currentProposedStepLength, motherSafety, ourSafety;
  G4int localNoDaughters, sampleNo;

  G4bool initialNode, noStep;
  G4SmartVoxelNode *curVoxelNode;
  G4int curNoVolumes, contentNo;
  G4double voxelSafety;

  motherPhysical = history.GetTopVolume();
  motherLogical = motherPhysical->GetLogicalVolume();
  motherSolid = motherLogical->GetSolid();

  //
  // Compute mother safety
  //

  motherSafety = motherSolid->DistanceToOut(localPoint);
  ourSafety = motherSafety;                 // Working isotropic safety
  
#ifdef G4VERBOSE
  if ( fCheck )
  {
    if(fVerbose == 1 )
    {
      G4cout << "*** G4VoxelNavigation::ComputeStep(): ***" << G4endl
             << "    Invoked DistanceToOut(p) for mother solid: "
             << motherSolid->GetName()
             << ". Solid replied: " << motherSafety << G4endl
             << "    For local point p: " << localPoint
             << ", to be considered as 'mother safety'." << G4endl;
    }
    if( motherSafety < 0.0 )
    {
      G4cout << "ERROR - G4VoxelNavigation::ComputeStep()" << G4endl
             << "        Current solid " << motherSolid->GetName()
             << " gave negative safety: " << motherSafety << G4endl
             << "        for the current (local) point " << localPoint
             << G4endl;
      motherSolid->DumpInfo();
      G4Exception("G4VoxelNavigation::ComputeStep()",
                  "NegativeSafetyMotherVol", FatalException,
                  "Negative Safety In Voxel Navigation !" ); 
    }
    if( motherSolid->Inside(localPoint)==kOutside )
    { 
      G4cout << "WARNING - G4VoxelNavigation::ComputeStep()" << G4endl
             << "          Point " << localPoint
             << " is outside current volume " << motherPhysical->GetName()
             << G4endl;
      G4double  estDistToSolid= motherSolid->DistanceToIn(localPoint); 
      G4cout << "          Estimated isotropic distance to solid (distToIn)= " 
             << estDistToSolid << G4endl;
      if( estDistToSolid > 100.0 * kCarTolerance )
      {
        motherSolid->DumpInfo();
        G4Exception("G4VoxelNavigation::ComputeStep()",
                    "FarOutsideCurrentVolume", FatalException,
                    "Point is far outside Current Volume !"); 
      }
      else
        G4Exception("G4VoxelNavigation::ComputeStep()", "OutsideCurrentVolume", 
                    JustWarning, "Point is a little outside Current Volume."); 
    }
  }
#endif

  //
  // Compute daughter safeties & intersections
  //

  // Exiting normal optimisation
  //
  if ( exiting && validExitNormal )
  {
    if ( localDirection.dot(exitNormal)>=kMinExitingNormalCosine )
    {
      // Block exited daughter volume
      //
      blockedExitedVol = *pBlockedPhysical;
      ourSafety = 0;
    }
  }
  exiting = false;
  entering = false;

  localNoDaughters = motherLogical->GetNoDaughters();

  fBList.Enlarge(localNoDaughters);
  fBList.Reset();

  initialNode = true;
  noStep = true;

  while (noStep)
  {
    curVoxelNode = fVoxelNode;
    curNoVolumes = curVoxelNode->GetNoContained();
    for (contentNo=curNoVolumes-1; contentNo>=0; contentNo--)
    {
      sampleNo = curVoxelNode->GetVolume(contentNo);
      if ( !fBList.IsBlocked(sampleNo) )
      {
        fBList.BlockVolume(sampleNo);
        samplePhysical = motherLogical->GetDaughter(sampleNo);
        if ( samplePhysical!=blockedExitedVol )
        {
          G4AffineTransform sampleTf(samplePhysical->GetRotation(),
                                     samplePhysical->GetTranslation());
          sampleTf.Invert();
          const G4ThreeVector samplePoint =
                     sampleTf.TransformPoint(localPoint);
          const G4VSolid *sampleSolid     =
                     samplePhysical->GetLogicalVolume()->GetSolid();
          const G4double sampleSafety     =
                     sampleSolid->DistanceToIn(samplePoint);
#ifdef G4VERBOSE
          if(( fCheck ) && ( fVerbose == 1 ))
          {
            G4cout << "*** G4VoxelNavigation::ComputeStep(): ***" << G4endl
                   << "    Invoked DistanceToIn(p) for daughter solid: "
                   << sampleSolid->GetName()
                   << ". Solid replied: " << sampleSafety << G4endl
                   << "    For local point p: " << samplePoint
                   << ", to be considered as 'daughter safety'." << G4endl;
          }
#endif
          if ( sampleSafety<ourSafety )
          {
            ourSafety = sampleSafety;
          }
          if ( sampleSafety<=ourStep )
          {
            sampleDirection = sampleTf.TransformAxis(localDirection);
            G4double sampleStep =
                     sampleSolid->DistanceToIn(samplePoint, sampleDirection);
#ifdef G4VERBOSE
            if(( fCheck ) && ( fVerbose == 1 ))
            {
              G4cout << "*** G4VoxelNavigation::ComputeStep(): ***" << G4endl
                     << "    Invoked DistanceToIn(p,v) for daughter solid: "
                     << sampleSolid->GetName()
                     << ". Solid replied: " << sampleStep << G4endl
                     << "    For local point p: " << samplePoint << G4endl
                     << "    Direction v: " << sampleDirection
                     << ", to be considered as 'daughter step'." << G4endl;
            }
#endif
            if ( sampleStep<=ourStep )
            {
              ourStep = sampleStep;
              entering = true;
              exiting = false;
              *pBlockedPhysical = samplePhysical;
              blockedReplicaNo = -1;
#ifdef G4VERBOSE
              // Check to see that the resulting point is indeed in/on volume.
              // This check could eventually be made only for successful
              // candidate.

              if ( ( fCheck ) && ( sampleStep < kInfinity ) )
              {
                G4ThreeVector intersectionPoint;
                intersectionPoint= samplePoint + sampleStep * sampleDirection;
                EInside insideIntPt= sampleSolid->Inside(intersectionPoint); 
                G4String solidResponse = "-kInside-";
                if (insideIntPt == kOutside)
                  { solidResponse = "-kOutside-"; }
                else if (insideIntPt == kSurface)
                  { solidResponse = "-kSurface-"; }
                if( fVerbose == 1 )
                {
                  G4cout << "*** G4VoxelNavigation::ComputeStep(): ***"<<G4endl
                         << "    Invoked Inside() for solid: "
                         << sampleSolid->GetName()
                         << ". Solid replied: " << solidResponse << G4endl
                         << "    For point p: " << intersectionPoint
                         << ", considered as 'intersection' point." << G4endl;
                }
                G4double safetyIn= -1, safetyOut= -1;  //  Set to invalid values
                G4double newDistIn= -1,  newDistOut= -1;
                if( insideIntPt != kInside )
                {
                  safetyIn= sampleSolid->DistanceToIn(intersectionPoint);
                  newDistIn= sampleSolid->DistanceToIn(intersectionPoint,
                                                       sampleDirection);
                }
                if( insideIntPt != kOutside )
                {
                  safetyOut= sampleSolid->DistanceToOut(intersectionPoint);
                  newDistOut= sampleSolid->DistanceToOut(intersectionPoint,
                                                         sampleDirection);
                }
                if( insideIntPt != kSurface )
                {
                  G4int oldcoutPrec = G4cout.precision(16); 
                  G4cout << "WARNING - G4VoxelNavigation::ComputeStep()"
                         << G4endl
                         << "          Inaccurate solid DistanceToIn"
                         << " for solid " << sampleSolid->GetName() << G4endl;
                  G4cout << "          Solid gave DistanceToIn = "
                         << sampleStep << " yet returns " << solidResponse
                         << " for this point !" << G4endl; 
                  G4cout << "          Point = " << intersectionPoint << G4endl;
                  G4cout << "          Safety values: " << G4endl;
                  if ( insideIntPt != kInside )
                  {
                    G4cout << "          DistanceToIn(p)  = " << safetyIn
                           << G4endl;
                  }
                  if ( insideIntPt != kOutside )
                  {
                    G4cout << "          DistanceToOut(p) = " << safetyOut
                           << G4endl;
                  }
                  G4Exception("G4VoxelNavigation::ComputeStep()", 
                              "InaccurateDistanceToIn", JustWarning,
                              "Conflicting response from Solid.");
                  G4cout.precision(oldcoutPrec);
                }
                else
                {  
                  // If it is on the surface, *ensure* that either DistanceToIn
                  // or DistanceToOut returns a finite value ( >= Tolerance).
                  //
                  if( std::max( newDistIn, newDistOut ) <= kCarTolerance )
                  { 
                    G4cout << "ERROR - G4VoxelNavigation::ComputeStep()"
                       << G4endl
                       << "  Identified point for which the solid " 
                       << sampleSolid->GetName() << G4endl
                       << "  has MAJOR problem:  " << G4endl
                       << "  --> Both DistanceToIn(p,v) and DistanceToOut(p,v) "
                       << "return Zero, an equivalent value or negative value."
                       << G4endl; 
                    G4cout << "    Solid: " << sampleSolid << G4endl;
                    G4cout << "    Point p= " << intersectionPoint << G4endl;
                    G4cout << "    Direction v= " << sampleDirection << G4endl;
                    G4cout << "    DistanceToIn(p,v)     = " << newDistIn
                           << G4endl;
                    G4cout << "    DistanceToOut(p,v,..) = " << newDistOut
                           << G4endl;
                    G4cout << "    Safety values: " << G4endl;
                    G4cout << "      DistanceToIn(p)  = " << safetyIn
                           << G4endl;
                    G4cout << "      DistanceToOut(p) = " << safetyOut
                           << G4endl;
                    G4Exception("G4VoxelNavigation::ComputeStep()", 
                              "DistanceToInAndOutAreZero", FatalException,
                              "Zero from both Solid DistanceIn and Out(p,v).");
                  }
                }
              }
#endif
            }
          }
        }
      }
    }
    if (initialNode)
    {
      initialNode = false;
      voxelSafety = ComputeVoxelSafety(localPoint);
      if ( voxelSafety<ourSafety )
      {
        ourSafety = voxelSafety;
      }
      if ( currentProposedStepLength<ourSafety )
      {
        // Guaranteed physics limited
        //      
        noStep = false;
        entering = false;
        exiting = false;
        *pBlockedPhysical = 0;
        ourStep = kInfinity;
      }
      else
      {
        //
        // Compute mother intersection if required
        //
        if ( motherSafety<=ourStep )
        {
          G4double motherStep =
              motherSolid->DistanceToOut(localPoint,
                                         localDirection,
                                         true, &validExitNormal, &exitNormal);
#ifdef G4VERBOSE
          if ( fCheck )
          {
            if(fVerbose == 1)
            {
              G4cout << "*** G4VoxelNavigation::ComputeStep(): ***" << G4endl
                     << "    Invoked DistanceToOut(p,v,...) for mother solid: "
                     << motherSolid->GetName()
                     << ". Solid replied: " << motherStep << G4endl
                     << "    For local point p: " << localPoint << G4endl
                     << "    Direction v: " << localDirection
                     << ", to be considered as 'mother step'." << G4endl;
            }
            if( ( motherStep < 0.0 ) || ( motherStep >= kInfinity) )
            {
              G4int oldPrOut= G4cout.precision(16); 
              G4int oldPrErr= G4cerr.precision(16);
              G4cerr << "ERROR - G4VoxelNavigation::ComputeStep()" << G4endl
                     << "        Problem in Navigation"  << G4endl
                     << "        Point (local coordinates): "
                     << localPoint << G4endl
                     << "        Local Direction: " << localDirection << G4endl
                     << "        Solid: " << motherSolid->GetName() << G4endl; 
              motherSolid->DumpInfo();
              G4Exception("G4VoxelNavigation::ComputeStep()",
                          "PointOutsideCurrentVolume", FatalException,
                          "Current point is outside the current solid !");
              G4cout.precision(oldPrOut);
              G4cerr.precision(oldPrErr);
            }
          }
#endif
          if ( motherStep<=ourStep )
          {
            ourStep = motherStep;
            exiting = true;
            entering = false;
            if ( validExitNormal )
            {
              const G4RotationMatrix *rot = motherPhysical->GetRotation();
              if (rot)
              {
                exitNormal *= rot->inverse();
              }
            }  
          }
          else
          {
            validExitNormal = false;
          }
        }
      }
      newSafety = ourSafety;
    }
    if (noStep)
    {
      noStep = LocateNextVoxel(localPoint, localDirection, ourStep);
    }
  }  // end -while (noStep)- loop

  return ourStep;
}

// ********************************************************************
// ComputeVoxelSafety
//
// Computes safety from specified point to voxel boundaries
// using already located point
// o collected boundaries for most derived level
// o adjacent boundaries for previous levels
// ********************************************************************
//
G4double
G4VoxelNavigation::ComputeVoxelSafety(const G4ThreeVector& localPoint) const
{
  G4SmartVoxelHeader *curHeader;
  G4double voxelSafety, curNodeWidth;
  G4double curNodeOffset, minCurCommonDelta, maxCurCommonDelta;
  G4int minCurNodeNoDelta, maxCurNodeNoDelta;
  G4int localVoxelDepth, curNodeNo;
  EAxis curHeaderAxis;

  localVoxelDepth = fVoxelDepth;

  curHeader = fVoxelHeaderStack[localVoxelDepth];
  curHeaderAxis = fVoxelAxisStack[localVoxelDepth];
  curNodeNo = fVoxelNodeNoStack[localVoxelDepth];
  curNodeWidth = fVoxelSliceWidthStack[localVoxelDepth];
  
  // Compute linear intersection distance to boundaries of max/min
  // to collected nodes at current level
  //
  curNodeOffset = curNodeNo*curNodeWidth;
  maxCurNodeNoDelta = fVoxelNode->GetMaxEquivalentSliceNo()-curNodeNo;
  minCurNodeNoDelta = curNodeNo-fVoxelNode->GetMinEquivalentSliceNo();
  minCurCommonDelta = localPoint(curHeaderAxis)
                      - curHeader->GetMinExtent() - curNodeOffset;
  maxCurCommonDelta = curNodeWidth-minCurCommonDelta;

  if ( minCurNodeNoDelta<maxCurNodeNoDelta )
  {
    voxelSafety = minCurNodeNoDelta*curNodeWidth;
    voxelSafety += minCurCommonDelta;
  }
  else if (maxCurNodeNoDelta < minCurNodeNoDelta)
       {
         voxelSafety = maxCurNodeNoDelta*curNodeWidth;
         voxelSafety += maxCurCommonDelta;
        }
        else    // (maxCurNodeNoDelta == minCurNodeNoDelta)
        {
          voxelSafety = minCurNodeNoDelta*curNodeWidth;
          voxelSafety += std::min(minCurCommonDelta,maxCurCommonDelta);
        }

  // Compute isotropic safety to boundaries of previous levels
  // [NOT to collected boundaries]
  //
  while ( (localVoxelDepth>0) && (voxelSafety>0) )
  {
    localVoxelDepth--;
    curHeader = fVoxelHeaderStack[localVoxelDepth];
    curHeaderAxis = fVoxelAxisStack[localVoxelDepth];
    curNodeNo = fVoxelNodeNoStack[localVoxelDepth];
    curNodeWidth = fVoxelSliceWidthStack[localVoxelDepth];
    curNodeOffset = curNodeNo*curNodeWidth;
    minCurCommonDelta = localPoint(curHeaderAxis)
                        - curHeader->GetMinExtent() - curNodeOffset;
    maxCurCommonDelta = curNodeWidth-minCurCommonDelta;
    
    if ( minCurCommonDelta<voxelSafety )
    {
      voxelSafety = minCurCommonDelta;
    }
    if ( maxCurCommonDelta<voxelSafety )
    {
      voxelSafety = maxCurCommonDelta;
    }
  }
  if ( voxelSafety<0 )
  {
    voxelSafety = 0;
  }

  return voxelSafety;
}

// ********************************************************************
// LocateNextVoxel
//
// Finds the next voxel from the current voxel and point
// in the specified direction
//
// Returns false if all voxels considered
//              [current Step ends inside same voxel or leaves all voxels]
//         true  otherwise
//              [the information on the next voxel is put into the set of
//               fVoxel* variables & "stacks"] 
// ********************************************************************
// 
G4bool
G4VoxelNavigation::LocateNextVoxel(const G4ThreeVector& localPoint,
                                   const G4ThreeVector& localDirection,
                                   const G4double currentStep)
{
  G4SmartVoxelHeader *workHeader=0, *newHeader=0;
  G4SmartVoxelProxy *newProxy=0;
  G4SmartVoxelNode *newVoxelNode=0;
  G4ThreeVector targetPoint, voxelPoint;
  G4double workNodeWidth, workMinExtent, workCoord;
  G4double minVal, maxVal, newDistance=0.;
  G4double newHeaderMin, newHeaderNodeWidth;
  G4int depth=0, newDepth=0, workNodeNo=0, newNodeNo=0, newHeaderNoSlices=0;
  EAxis workHeaderAxis, newHeaderAxis;
  G4bool isNewVoxel=false;
  
  G4double currentDistance = currentStep;

  // Determine if end of Step within current voxel
  //
  for (depth=0; depth<fVoxelDepth; depth++)
  {
    targetPoint = localPoint+localDirection*currentDistance;
    newDistance = currentDistance;
    workHeader = fVoxelHeaderStack[depth];
    workHeaderAxis = fVoxelAxisStack[depth];
    workNodeNo = fVoxelNodeNoStack[depth];
    workNodeWidth = fVoxelSliceWidthStack[depth];
    workMinExtent = workHeader->GetMinExtent();
    workCoord = targetPoint(workHeaderAxis);
    minVal = workMinExtent+workNodeNo*workNodeWidth;

    if ( minVal<=workCoord+kCarTolerance*0.5 )
    {
      maxVal = minVal+workNodeWidth;
      if ( maxVal<=workCoord-kCarTolerance*0.5 )
      {
        // Must consider next voxel
        //
        newNodeNo = workNodeNo+1;
        newHeader = workHeader;
        newDistance = (maxVal-localPoint(workHeaderAxis))
                    / localDirection(workHeaderAxis);
        isNewVoxel = true;
        newDepth = depth;
      }
    }
    else
    {
      newNodeNo = workNodeNo-1;
      newHeader = workHeader;
      newDistance = (minVal-localPoint(workHeaderAxis))
                  / localDirection(workHeaderAxis);
      isNewVoxel = true;
      newDepth = depth;
    }
    currentDistance = newDistance;
  }
  targetPoint = localPoint+localDirection*currentDistance;

  // Check if end of Step within collected boundaries of current voxel
  //
  depth = fVoxelDepth;
  {
    workHeader = fVoxelHeaderStack[depth];
    workHeaderAxis = fVoxelAxisStack[depth];
    workNodeNo = fVoxelNodeNoStack[depth];
    workNodeWidth = fVoxelSliceWidthStack[depth];
    workMinExtent = workHeader->GetMinExtent();
    workCoord = targetPoint(workHeaderAxis);
    minVal = workMinExtent+fVoxelNode->GetMinEquivalentSliceNo()*workNodeWidth;

    if ( minVal<=workCoord+kCarTolerance*0.5 )
    {
      maxVal = workMinExtent+(fVoxelNode->GetMaxEquivalentSliceNo()+1)
                            *workNodeWidth;
      if ( maxVal<=workCoord-kCarTolerance*0.5 )
      {
        newNodeNo = fVoxelNode->GetMaxEquivalentSliceNo()+1;
        newHeader = workHeader;
        newDistance = (maxVal-localPoint(workHeaderAxis))
                    / localDirection(workHeaderAxis);
        isNewVoxel = true;
        newDepth = depth;
      }
    }
    else
    {
      newNodeNo = fVoxelNode->GetMinEquivalentSliceNo()-1;
      newHeader = workHeader;
      newDistance = (minVal-localPoint(workHeaderAxis))
                  / localDirection(workHeaderAxis);
      isNewVoxel = true;
      newDepth = depth;
    }
    currentDistance = newDistance;
  }
  if (isNewVoxel)
  {
    // Compute new voxel & adjust voxel stack
    //
    // newNodeNo=Candidate node no at 
    // newDepth =refinement depth of crossed voxel boundary
    // newHeader=Header for crossed voxel
    // newDistance=distance to crossed voxel boundary (along the track)
    //
    if ( (newNodeNo<0) || (newNodeNo>=newHeader->GetNoSlices()))
    {
      // Leaving mother volume
      //
      isNewVoxel = false;
    }
    else
    {
      // Compute intersection point on the least refined
      // voxel boundary that is hit
      //
      voxelPoint = localPoint+localDirection*newDistance;
      fVoxelNodeNoStack[newDepth] = newNodeNo;
      fVoxelDepth = newDepth;
      newVoxelNode = 0;
      while ( !newVoxelNode )
      {
        newProxy = newHeader->GetSlice(newNodeNo);
        if (newProxy->IsNode())
        {
          newVoxelNode = newProxy->GetNode();
        }
        else
        {
          fVoxelDepth++;
          newHeader = newProxy->GetHeader();
          newHeaderAxis = newHeader->GetAxis();
          newHeaderNoSlices = newHeader->GetNoSlices();
          newHeaderMin = newHeader->GetMinExtent();
          newHeaderNodeWidth = (newHeader->GetMaxExtent()-newHeaderMin)
                             / newHeaderNoSlices;
          newNodeNo = G4int( (voxelPoint(newHeaderAxis)-newHeaderMin)
                             / newHeaderNodeWidth );
          // Rounding protection
          //
          if ( newNodeNo<0 )
          {
            newNodeNo=0;
          }
          else if ( newNodeNo>=newHeaderNoSlices )
               {
                 newNodeNo = newHeaderNoSlices-1;
               }
          // Stack info for stepping
          //
          fVoxelAxisStack[fVoxelDepth] = newHeaderAxis;
          fVoxelNoSlicesStack[fVoxelDepth] = newHeaderNoSlices;
          fVoxelSliceWidthStack[fVoxelDepth] = newHeaderNodeWidth;
          fVoxelNodeNoStack[fVoxelDepth] = newNodeNo;
          fVoxelHeaderStack[fVoxelDepth] = newHeader;
        }
      }
      fVoxelNode = newVoxelNode;
    }
  }
  return isNewVoxel;        
}

// ********************************************************************
// ComputeSafety
//
// Calculates the isotropic distance to the nearest boundary from the
// specified point in the local coordinate system. 
// The localpoint utilised must be within the current volume.
// ********************************************************************
//
G4double
G4VoxelNavigation::ComputeSafety(const G4ThreeVector& localPoint,
                                 const G4NavigationHistory& history,
                                 const G4double )
{
  G4VPhysicalVolume *motherPhysical, *samplePhysical;
  G4LogicalVolume *motherLogical;
  G4VSolid *motherSolid;
  G4double motherSafety, ourSafety;
  G4int localNoDaughters, sampleNo;
  G4SmartVoxelNode *curVoxelNode;
  G4int curNoVolumes, contentNo;
  G4double voxelSafety;

  motherPhysical = history.GetTopVolume();
  motherLogical = motherPhysical->GetLogicalVolume();
  motherSolid = motherLogical->GetSolid();

  //
  // Compute mother safety
  //

  motherSafety = motherSolid->DistanceToOut(localPoint);
  ourSafety = motherSafety;                 // Working isotropic safety

#ifdef G4VERBOSE
  if(( fCheck ) && ( fVerbose == 1 ))
  {
    G4cout << "*** G4VoxelNavigation::ComputeSafety(): ***" << G4endl
           << "    Invoked DistanceToOut(p) for mother solid: "
           << motherSolid->GetName()
           << ". Solid replied: " << motherSafety << G4endl
           << "    For local point p: " << localPoint
           << ", to be considered as 'mother safety'." << G4endl;
  }
#endif
  //
  // Compute daughter safeties 
  //

  localNoDaughters = motherLogical->GetNoDaughters();

  //  Look only inside the current Voxel only (in the first version).
  //
  curVoxelNode = fVoxelNode;
  curNoVolumes = curVoxelNode->GetNoContained();

  for ( contentNo=curNoVolumes-1; contentNo>=0; contentNo-- )
  {
    sampleNo = curVoxelNode->GetVolume(contentNo);
    samplePhysical = motherLogical->GetDaughter(sampleNo);

    G4AffineTransform sampleTf(samplePhysical->GetRotation(),
                               samplePhysical->GetTranslation());
    sampleTf.Invert();
    const G4ThreeVector samplePoint =
                          sampleTf.TransformPoint(localPoint);
    const G4VSolid *sampleSolid     =
                          samplePhysical->GetLogicalVolume()->GetSolid();
    G4double sampleSafety = sampleSolid->DistanceToIn(samplePoint);
    if ( sampleSafety<ourSafety )
    {
      ourSafety = sampleSafety;
    }
#ifdef G4VERBOSE
    if(( fCheck ) && ( fVerbose == 1 ))
    {
      G4cout << "*** G4VoxelNavigation::ComputeSafety(): ***" << G4endl
             << "    Invoked DistanceToIn(p) for daughter solid: "
             << sampleSolid->GetName()
             << ". Solid replied: " << sampleSafety << G4endl
             << "    For local point p: " << samplePoint
             << ", to be considered as 'daughter safety'." << G4endl;
    }
#endif
  }
  voxelSafety = ComputeVoxelSafety(localPoint);
  if ( voxelSafety<ourSafety )
  {
    ourSafety = voxelSafety;
  }
  return ourSafety;
}
