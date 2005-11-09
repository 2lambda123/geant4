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
// $Id: G4FlatSurface.cc,v 1.9 2005-11-09 15:04:28 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class source file
//
//
// G4FlatSurface.cc
//
// Author: 
//   01-Aug-2002 - Kotoyo Hoshina (hoshina@hepburn.s.chiba-u.ac.jp)
//
// History:
//   13-Nov-2003 - O.Link (Oliver.Link@cern.ch), Integration in Geant4
//                 from original version in Jupiter-2.5.02 application.
// --------------------------------------------------------------------

#include "G4FlatSurface.hh"

//=====================================================================
//* constructors ------------------------------------------------------

G4FlatSurface::G4FlatSurface(const G4String         &name,
                             const G4RotationMatrix &rot,
                             const G4ThreeVector    &tlate,
                             const G4ThreeVector    &n,
                             const EAxis             axis0 ,
                             const EAxis             axis1 ,
                                   G4double          axis0min,
                                   G4double          axis1min,
                                   G4double          axis0max,
                                   G4double          axis1max )
  : G4VSurface(name, rot, tlate, 0, axis0, axis1,
               axis0min, axis1min, axis0max, axis1max)
{   
   if (axis0 == kPhi && axis1 == kRho) {
      G4Exception("G4FlatSurface::G4FlatSurface()", "InvalidSetup",
                  FatalException, "Should swap axis0 and axis1!");
   }
   
   G4ThreeVector normal = rot.inverse()*n;
   fCurrentNormal.normal = normal.unit();   // in local coordinate system
   fIsValidNorm = true;

   SetCorners();
   SetBoundaries();
}



G4FlatSurface::G4FlatSurface( const G4String        &name,
                                    G4double         EndInnerRadius[2],
                                    G4double         EndOuterRadius[2],
                                    G4double         DPhi,
                                    G4double         EndPhi[2],
                                    G4double         EndZ[2], 
                                    G4int            handedness ) 
  : G4VSurface(name)
{
   fHandedness = handedness;   // +z = +ve, -z = -ve
   fAxis[0]    = kRho;         // in local coordinate system
   fAxis[1]    = kPhi;
   G4int i     = (handedness < 0 ? 0 : 1);
   fAxisMin[0] = EndInnerRadius[i];  // Inner-hype radius at z=0
   fAxisMax[0] = EndOuterRadius[i];  // Outer-hype radius at z=0
   fAxisMin[1] = -0.5*DPhi;
   fAxisMax[1] = -fAxisMin[1];
   fCurrentNormal.normal.set(0, 0, (fHandedness < 0 ? -1 : 1)); 
         // Unit vector, in local coordinate system
   fRot.rotateZ(EndPhi[i]);
   fTrans.set(0, 0, EndZ[i]);
   fIsValidNorm = true;

   SetCorners();
   SetBoundaries();
}


//=====================================================================
//* Fake default constructor ------------------------------------------

G4FlatSurface::G4FlatSurface( __void__& a )
  : G4VSurface(a)
{
}


//=====================================================================
//* destructor --------------------------------------------------------

G4FlatSurface::~G4FlatSurface()
{
}

//=====================================================================
//* GetNormal ---------------------------------------------------------

G4ThreeVector G4FlatSurface::GetNormal(const G4ThreeVector & /* xx */ , 
                                             G4bool isGlobal)
{
   if (isGlobal) {
      return ComputeGlobalDirection(fCurrentNormal.normal);
   } else {
      return fCurrentNormal.normal;
   }
}

//=====================================================================
//* DistanceToSurface(p, v) -------------------------------------------

G4int G4FlatSurface::DistanceToSurface(const G4ThreeVector &gp,
                                       const G4ThreeVector &gv,
                                             G4ThreeVector  gxx[],
                                             G4double       distance[],
                                             G4int          areacode[],
                                             G4bool         isvalid[],
                                             EValidate      validate) 
{
   fCurStatWithV.ResetfDone(validate, &gp, &gv);
   
   if (fCurStatWithV.IsDone()) {
      G4int i;
      for (i=0; i<fCurStatWithV.GetNXX(); i++) {
         gxx[i] = fCurStatWithV.GetXX(i);
         distance[i] = fCurStatWithV.GetDistance(i);
         areacode[i] = fCurStatWithV.GetAreacode(i);
         isvalid[i]  = fCurStatWithV.IsValid(i);
      }
      return fCurStatWithV.GetNXX();
   } else {
      // initialize
      G4int i;
      for (i=0; i<2; i++) {
         distance[i] = kInfinity;
         areacode[i] = sOutside;
         isvalid[i]  = false;
         gxx[i].set(kInfinity, kInfinity, kInfinity);
      }
   }

   G4ThreeVector p = ComputeLocalPoint(gp);
   G4ThreeVector v = ComputeLocalDirection(gv);

   //
   // special case!
   // if p is on surface, distance = 0. 
   //

   if (std::fabs(p.z()) == 0.) {   // if p is on the plane
      distance[0] = 0;
      G4ThreeVector xx = p;
      gxx[0] = ComputeGlobalPoint(xx);
      
      if (validate == kValidateWithTol) {
         areacode[0] = GetAreaCode(xx);
         if (!IsOutside(areacode[0])) {
            isvalid[0] = true;
         }
      } else if (validate == kValidateWithoutTol) {
         areacode[0] = GetAreaCode(xx, false);
         if (IsInside(areacode[0])) {
            isvalid[0] = true;
         }
      } else { // kDontValidate
         areacode[0] = sInside;
         isvalid[0] = true;
      }

      return 1;
   }
   //
   // special case end
   //
   
   if (v.z() == 0) { 

      fCurStatWithV.SetCurrentStatus(0, gxx[0], distance[0], areacode[0], 
                                     isvalid[0], 0, validate, &gp, &gv);
      return 0;
   }
   
   distance[0] = - (p.z() / v.z());
      
   G4ThreeVector xx = p + distance[0]*v;
   gxx[0] = ComputeGlobalPoint(xx);

   if (validate == kValidateWithTol) {
      areacode[0] = GetAreaCode(xx);
      if (!IsOutside(areacode[0])) {
         if (distance[0] >= 0) isvalid[0] = true;
      }
   } else if (validate == kValidateWithoutTol) {
      areacode[0] = GetAreaCode(xx, false);
      if (IsInside(areacode[0])) {
         if (distance[0] >= 0) isvalid[0] = true;
      }
   } else { // kDontValidate
      areacode[0] = sInside;
         if (distance[0] >= 0) isvalid[0] = true;
   }

   fCurStatWithV.SetCurrentStatus(0, gxx[0], distance[0], areacode[0],
                                  isvalid[0], 1, validate, &gp, &gv);

#ifdef G4SPECSDEBUG
   G4cerr << "ERROR - G4FlatSurface::DistanceToSurface(p,v)" << G4endl;
   G4cerr << "        Name        : " << GetName() << G4endl;
   G4cerr << "        xx          : " << xx << G4endl;
   G4cerr << "        gxx[0]      : " << gxx[0] << G4endl;
   G4cerr << "        dist[0]     : " << distance[0] << G4endl;
   G4cerr << "        areacode[0] : " << areacode[0] << G4endl;
   G4cerr << "        isvalid[0]  : " << isvalid[0]  << G4endl;
#endif
   return 1;
}

//=====================================================================
//* DistanceToSurface(p) ----------------------------------------------

G4int G4FlatSurface::DistanceToSurface(const G4ThreeVector &gp,
                                             G4ThreeVector  gxx[],
                                             G4double       distance[],
                                             G4int          areacode[])
{
   // Calculate distance to plane in local coordinate,
   // then return distance and global intersection points.
   //  

   fCurStat.ResetfDone(kDontValidate, &gp);

   if (fCurStat.IsDone()) {
      G4int i;
      for (i=0; i<fCurStat.GetNXX(); i++) {
         gxx[i] = fCurStat.GetXX(i);
         distance[i] = fCurStat.GetDistance(i);
         areacode[i] = fCurStat.GetAreacode(i);
      }
      return fCurStat.GetNXX();
   } else {
      // initialize
      G4int i;
      for (i=0; i<2; i++) {
         distance[i] = kInfinity;
         areacode[i] = sOutside;
         gxx[i].set(kInfinity, kInfinity, kInfinity);
      }
   }
   
   G4ThreeVector p = ComputeLocalPoint(gp);
   G4ThreeVector xx;

   // The plane is placed on origin with making its normal 
   // parallel to z-axis. 
   if (std::fabs(p.z()) <= 0.5 * kCarTolerance) {   // if p is on the plane, return 1
      distance[0] = 0;
      xx = p;
   } else {
      distance[0] = std::fabs(p.z());
      xx.set(p.x(), p.y(), 0);  
   }

   gxx[0] = ComputeGlobalPoint(xx);
   areacode[0] = sInside;
   G4bool isvalid = true;
   fCurStat.SetCurrentStatus(0, gxx[0], distance[0], areacode[0],
                             isvalid, 1, kDontValidate, &gp);
   return 1;

}

//=====================================================================
//* GetAreaCode -------------------------------------------------------

G4int G4FlatSurface::GetAreaCode(const G4ThreeVector &xx, 
                                       G4bool withTol)
{

   static const G4double rtol = 0.5*kRadTolerance;
   
   G4int areacode = sInside;

   if (fAxis[0] == kRho && fAxis[1] == kPhi) {
      G4int rhoaxis = 0;
     // G4int phiaxis = 0;

      G4ThreeVector dphimin;   // direction of phi-minimum boundary
      G4ThreeVector dphimax;   // direction of phi-maximum boundary
      dphimin = GetCorner(sC0Max1Min);
      dphimax = GetCorner(sC0Max1Max);   
      
      if (withTol) {

         G4bool isoutside = false;

         // test boundary of rho-axis

         if (xx.getRho() <= fAxisMin[rhoaxis] + rtol) {

            areacode |= (sAxis0 & (sAxisRho | sAxisMin)) | sBoundary; // rho-min
            if (xx.getRho() < fAxisMin[rhoaxis] - rtol) isoutside = true; 
            
         } else if (xx.getRho() >= fAxisMax[rhoaxis] - rtol) {

            areacode |= (sAxis0 & (sAxisRho | sAxisMax)) | sBoundary; // rho-max
            if (xx.getRho() > fAxisMax[rhoaxis] + rtol) isoutside = true; 

         }         
         
         // test boundary of phi-axis

         if (AmIOnLeftSide(xx, dphimin) >= 0) {           // xx is on dphimin

            areacode |= (sAxis1 & (sAxisPhi | sAxisMin)); 
            if   (areacode & sBoundary) areacode |= sCorner;  // xx is on the corner.
            else                        areacode |= sBoundary;

            if (AmIOnLeftSide(xx, dphimin) > 0) isoutside = true; 

         } else if (AmIOnLeftSide(xx, dphimax) <= 0) {    // xx is on dphimax

            areacode |= (sAxis1 & (sAxisPhi | sAxisMax)); 
            if   (areacode & sBoundary) areacode |= sCorner;  // xx is on the corner.
            else                        areacode |= sBoundary;

            if (AmIOnLeftSide(xx, dphimax) < 0) isoutside = true; 

         }
         
         // if isoutside = true, clear inside bit.
         // if not on boundary, add axis information. 

         if (isoutside) {
            G4int tmpareacode = areacode & (~sInside);
            areacode = tmpareacode;
         } else if ((areacode & sBoundary) != sBoundary) {
            areacode |= (sAxis0 & sAxisRho) | (sAxis1 & sAxisPhi);
         }

      } else {

         // out of boundary of rho-axis

         if (xx.getRho() < fAxisMin[rhoaxis]) {
            areacode |= (sAxis0 & (sAxisRho | sAxisMin)) | sBoundary;
         } else if (xx.getRho() > fAxisMax[rhoaxis]) {
            areacode |= (sAxis0 & (sAxisRho | sAxisMax)) | sBoundary;
         }
         
         // out of boundary of phi-axis

         if (AmIOnLeftSide(xx, dphimin, false) >= 0) {       // xx is leftside or
            areacode |= (sAxis1 & (sAxisPhi | sAxisMin)) ;   // boundary of dphimin
            if   (areacode & sBoundary) areacode |= sCorner; // xx is on the corner.
            else                        areacode |= sBoundary;

         } else if (AmIOnLeftSide(xx, dphimax, false) <= 0) { // xx is rightside or
            areacode |= (sAxis1 & (sAxisPhi | sAxisMax)) ;    // boundary of dphimax
            if   (areacode & sBoundary) areacode |= sCorner;  // xx is on the corner.
            else                        areacode |= sBoundary;
           
         }

         if ((areacode & sBoundary) != sBoundary) {
            areacode |= (sAxis0 & sAxisRho) | (sAxis1 & sAxisPhi);
         }

      }
      return areacode;
   } else {

      G4cerr << "ERROR - G4FlatSurface::GetAreaCode()" << G4endl
             << "        fAxis[0] = " << fAxis[0] << G4endl
             << "        fAxis[1] = " << fAxis[1] << G4endl;
      G4Exception("G4FlatSurface::GetAreaCode()", "NotImplemented",
                  FatalException, "Feature NOT implemented !");
   }
   return areacode;
}
                                      
                                           
//=====================================================================
//* SetCorners --------------------------------------------------------

void G4FlatSurface::SetCorners()
{
   // Set Corner points in local coodinate.
   
   if (fAxis[0] == kRho && fAxis[1] == kPhi) {
       
      G4int rhoaxis = 0;  // kRho
      G4int phiaxis = 1;  // kPhi
      
      G4double x, y, z;
      // corner of Axis0min and Axis1min
         x = fAxisMin[rhoaxis]*std::cos(fAxisMin[phiaxis]);
         y = fAxisMin[rhoaxis]*std::sin(fAxisMin[phiaxis]);
         z = 0;
         SetCorner(sC0Min1Min, x, y, z);
      // corner of Axis0max and Axis1min
         x = fAxisMax[rhoaxis]*std::cos(fAxisMin[phiaxis]);
         y = fAxisMax[rhoaxis]*std::sin(fAxisMin[phiaxis]);
         z = 0;
         SetCorner(sC0Max1Min, x, y, z);
      // corner of Axis0max and Axis1max
         x = fAxisMax[rhoaxis]*std::cos(fAxisMax[phiaxis]);
         y = fAxisMax[rhoaxis]*std::sin(fAxisMax[phiaxis]);
         z = 0;
         SetCorner(sC0Max1Max, x, y, z);
      // corner of Axis0min and Axis1max
         x = fAxisMin[rhoaxis]*std::cos(fAxisMax[phiaxis]);
         y = fAxisMin[rhoaxis]*std::sin(fAxisMax[phiaxis]);
         z = 0;
         SetCorner(sC0Min1Max, x, y, z);
       
   } else {
      G4cerr << "ERROR - G4FlatSurface::SetCorners()" << G4endl
             << "        fAxis[0] = " << fAxis[0] << G4endl
             << "        fAxis[1] = " << fAxis[1] << G4endl;
      G4Exception("G4FlatSurface::SetCorners()", "NotImplemented",
                  FatalException, "Feature NOT implemented !");
   }
}

//=====================================================================
//* SetBoundaries() ---------------------------------------------------

void G4FlatSurface::SetBoundaries()
{
   // Set direction-unit vector of phi-boundary-lines in local coodinate.
   // Don't call the function twice.
   
   if (fAxis[0] == kRho && fAxis[1] == kPhi) {
   
      G4ThreeVector direction;
      // sAxis0 & sAxisMin
      direction = GetCorner(sC0Min1Max) - GetCorner(sC0Min1Min);
      direction = direction.unit();
      SetBoundary(sAxis0 & (sAxisPhi | sAxisMin), direction,
                  GetCorner(sC0Min1Min), sAxisPhi);
                  
      // sAxis0 & sAxisMax
      direction = GetCorner(sC0Max1Max) - GetCorner(sC0Max1Min);
      direction = direction.unit();
      SetBoundary(sAxis0 & (sAxisPhi | sAxisMax), direction,
                  GetCorner(sC0Max1Min), sAxisPhi);

      // sAxis1 & sAxisMin
      direction = GetCorner(sC0Max1Min) - GetCorner(sC0Min1Min);
      direction = direction.unit();
      SetBoundary(sAxis1 & (sAxisRho | sAxisMin), direction,
                  GetCorner(sC0Min1Min), sAxisRho);
      
      // sAxis1 & sAxisMax
      direction = GetCorner(sC0Max1Max) - GetCorner(sC0Min1Max);
      direction = direction.unit();
      SetBoundary(sAxis1 & (sAxisRho | sAxisMax), direction,
                  GetCorner(sC0Min1Max), sAxisPhi);
   } else {
      G4cerr << "ERROR - G4FlatSurface::SetBoundaries()" << G4endl
             << "        fAxis[0] = " << fAxis[0] << G4endl
             << "        fAxis[1] = " << fAxis[1] << G4endl;
      G4Exception("G4FlatSurface::SetBoundaries()", "NotImplemented",
                  FatalException, "Feature NOT implemented !");
   }
}
