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
// $Id: G4Tubs.cc,v 1.46 2005-03-18 15:39:11 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4Tubs
//
// History:
//
//
// 16.03.05 V.Grichine: SurfaceNormal(p) with edges/corners for boolean
// 20.07.01 V.Grichine: bug fixed in Inside(p)
// 20.02.01 V.Grichine: bug fixed in Inside(p) and CalculateExtent was 
//                      simplified base on G4Box::CalculateExtent
// 07.12.00 V.Grichine: phi-section algorithm was changed in Inside(p)
// 28.11.00 V.Grichine: bug fixed in Inside(p)
// 31.10.00 V.Grichine: assign sr, sphi in Distance ToOut(p,v,...)
// 08.08.00 V.Grichine: more stable roots of 2-equation in DistanceToOut(p,v,..)
// 02.08.00 V.Grichine: point is outside check in Distance ToOut(p)
// 17.05.00 V.Grichine: bugs (#76,#91) fixed in Distance ToOut(p,v,...)
// 31.03.00 V.Grichine: bug fixed in Inside(p)
// 19.11.99 V.Grichine: side = kNull in DistanceToOut(p,v,...)
// 13.10.99 V.Grichine: bugs fixed in DistanceToIn(p,v) 
// 28.05.99 V.Grichine: bugs fixed in DistanceToOut(p,v,...)
// 25.05.99 V.Grichine: bugs fixed in DistanceToIn(p,v) 
// 23.03.99 V.Grichine: bug fixed in DistanceToIn(p,v) 
// 09.10.98 V.Grichine: modifications in DistanceToOut(p,v,...)
// 18.06.98 V.Grichine: n-normalisation in DistanceToOut(p,v)
// 
// 1994-95  P.Kent:     implementation
// 


#include "G4Tubs.hh"

#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"

#include "G4VPVParameterisation.hh"

#include "meshdefs.hh"

#include "G4VGraphicsScene.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4NURBStube.hh"
#include "G4NURBScylinder.hh"
#include "G4NURBStubesector.hh"

/////////////////////////////////////////////////////////////////////////
//
// Constructor - check parameters, convert angles so 0<sphi+dpshi<=2_PI
//             - note if pdphi>2PI then reset to 2PI

G4Tubs::G4Tubs( const G4String &pName,
                      G4double pRMin, G4double pRMax,
                      G4double pDz,
                      G4double pSPhi, G4double pDPhi )
  : G4CSGSolid(pName)
{

  if (pDz>0) // Check z-len
  {
    fDz = pDz ;
  }
  else
  {
    G4cerr << "ERROR - G4Tubs()::G4Tubs(): " << GetName() << G4endl
           << "        Negative Z half-length ! - "
           << pDz << G4endl;
    G4Exception("G4Tubs::G4Tubs()", "InvalidSetup", FatalException,
                "Invalid Z half-length");
  }
  if ( pRMin < pRMax && pRMin >= 0 ) // Check radii
  {
    fRMin = pRMin ; 
    fRMax = pRMax ;
  }
  else
  {
    G4cerr << "ERROR - G4Tubs()::G4Tubs(): " << GetName() << G4endl
           << "        Invalid values for radii !" << G4endl
           << "        pRMin = " << pRMin << ", pRMax = " << pRMax << G4endl;
    G4Exception("G4Tubs::G4Tubs()", "InvalidSetup", FatalException,
                "Invalid radii.");
  }
  if ( pDPhi >= twopi ) // Check angles
  {
    fDPhi=twopi;
  }
  else
  {
    if ( pDPhi > 0 )
    {
      fDPhi = pDPhi;
    }
    else
    {
      G4cerr << "ERROR - G4Tubs()::G4Tubs(): " << GetName() << G4endl
             << "        Negative delta-Phi ! - "
             << pDPhi << G4endl;
      G4Exception("G4Tubs::G4Tubs()", "InvalidSetup", FatalException,
                  "Invalid dphi.");
    }
  }
  
  // Ensure fSphi in 0-2PI or -2PI-0 range if shape crosses 0

  fSPhi = pSPhi;

  if ( fSPhi < 0 )
  {
    fSPhi = twopi - std::fmod(std::fabs(fSPhi),twopi) ;
  }
  else
  {
    fSPhi = std::fmod(fSPhi,twopi) ;
  }
  if (fSPhi + fDPhi > twopi )
  {
    fSPhi -= twopi ;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Destructor

G4Tubs::~G4Tubs()
{
}

/////////////////////////////////////////////////////////////////////////
//
// Dispatch to parameterisation for replication mechanism dimension
// computation & modification.

void G4Tubs::ComputeDimensions(       G4VPVParameterisation* p,
                                const G4int n,
                                const G4VPhysicalVolume* pRep )
{
  p->ComputeDimensions(*this,n,pRep) ;
}

////////////////////////////////////////////////////////////////////////
//
// Calculate extent under transform and specified limit

G4bool G4Tubs::CalculateExtent( const EAxis              pAxis,
                                const G4VoxelLimits&     pVoxelLimit,
                                const G4AffineTransform& pTransform,
                                      G4double&          pMin, 
                                      G4double&          pMax    ) const
{

  if ( !pTransform.IsRotated() && fDPhi == twopi && fRMin == 0 )
  {
    // Special case handling for unrotated solid tubes
    // Compute x/y/z mins and maxs fro bounding box respecting limits,
    // with early returns if outside limits. Then switch() on pAxis,
    // and compute exact x and y limit for x/y case
      
    G4double xoffset, xMin, xMax ;
    G4double yoffset, yMin, yMax ;
    G4double zoffset, zMin, zMax ;

    G4double diff1, diff2, maxDiff, newMin, newMax ;
    G4double xoff1, xoff2, yoff1, yoff2 ;

    xoffset = pTransform.NetTranslation().x() ;
    xMin = xoffset - fRMax ;
    xMax = xoffset + fRMax ;

    if (pVoxelLimit.IsXLimited())
    {
      if ( (xMin > pVoxelLimit.GetMaxXExtent())
        || (xMax < pVoxelLimit.GetMinXExtent()) )
      {
        return false;
      }
      else
      {
        if ( xMin < pVoxelLimit.GetMinXExtent() )
        {
          xMin = pVoxelLimit.GetMinXExtent() ;
        }
        if (xMax > pVoxelLimit.GetMaxXExtent() )
        {
          xMax = pVoxelLimit.GetMaxXExtent() ;
        }
      }
    }
    yoffset = pTransform.NetTranslation().y() ;
    yMin    = yoffset - fRMax ;
    yMax    = yoffset + fRMax ;

    if ( pVoxelLimit.IsYLimited() )
    {
      if ( (yMin > pVoxelLimit.GetMaxYExtent())
        || (yMax < pVoxelLimit.GetMinYExtent()) )
      {
        return false ;
      }
      else
      {
        if ( yMin < pVoxelLimit.GetMinYExtent() )
        {
          yMin = pVoxelLimit.GetMinYExtent() ;
        }
        if ( yMax > pVoxelLimit.GetMaxYExtent() )
        {
          yMax=pVoxelLimit.GetMaxYExtent();
        }
      }
    }
    zoffset = pTransform.NetTranslation().z() ;
    zMin    = zoffset - fDz ;
    zMax    = zoffset + fDz ;

    if ( pVoxelLimit.IsZLimited() )
    {
      if ( (zMin > pVoxelLimit.GetMaxZExtent())
        || (zMax < pVoxelLimit.GetMinZExtent()) )
      {
        return false ;
      }
      else
      {
        if ( zMin < pVoxelLimit.GetMinZExtent() )
        {
          zMin = pVoxelLimit.GetMinZExtent() ;
        }
        if ( zMax > pVoxelLimit.GetMaxZExtent() )
        {
          zMax = pVoxelLimit.GetMaxZExtent();
        }
      }
    }
    switch ( pAxis )  // Known to cut cylinder
    {
      case kXAxis :
      {
        yoff1 = yoffset - yMin ;
        yoff2 = yMax    - yoffset ;

        if ( yoff1 >= 0 && yoff2 >= 0 ) // Y limits cross max/min x => no change
        {
          pMin = xMin ;
          pMax = xMax ;
        }
        else
        {
          // Y limits don't cross max/min x => compute max delta x,
          // hence new mins/maxs

          diff1   = std::sqrt(fRMax*fRMax - yoff1*yoff1);
          diff2   = std::sqrt(fRMax*fRMax - yoff2*yoff2);
          maxDiff = (diff1 > diff2) ? diff1:diff2;
          newMin  = xoffset - maxDiff;
          newMax  = xoffset + maxDiff;
          pMin    = (newMin < xMin) ? xMin : newMin;
          pMax    = (newMax > xMax) ? xMax : newMax;
        }    
        break;
      }
      case kYAxis :
      {
        xoff1 = xoffset - xMin ;
        xoff2 = xMax - xoffset ;

        if ( xoff1 >= 0 && xoff2 >= 0 ) // X limits cross max/min y => no change
        {
          pMin = yMin ;
          pMax = yMax ;
        }
        else
        {
          // X limits don't cross max/min y => compute max delta y,
          // hence new mins/maxs

          diff1   = std::sqrt(fRMax*fRMax - xoff1*xoff1) ;
          diff2   = std::sqrt(fRMax*fRMax - xoff2*xoff2) ;
          maxDiff = (diff1 > diff2) ? diff1 : diff2 ;
          newMin  = yoffset - maxDiff ;
          newMax  = yoffset + maxDiff ;
          pMin    = (newMin < yMin) ? yMin : newMin ;
          pMax     =(newMax > yMax) ? yMax : newMax ;
        }
        break ;
      }
      case kZAxis:
      {
        pMin = zMin ;
        pMax = zMax ;
        break ;
      }
      default:
        break;
    }
    pMin -= kCarTolerance ;
    pMax += kCarTolerance ;
    return true;    
  }
  else // Calculate rotated vertex coordinates
  {
    G4int i, noEntries, noBetweenSections4 ;
    G4bool existsAfterClip = false ;
    G4ThreeVectorList* vertices = CreateRotatedVertices(pTransform) ;
    
    pMin = +kInfinity ;
    pMax = -kInfinity ;

    noEntries = vertices->size() ;
    noBetweenSections4 = noEntries - 4 ;
    /*
    G4cout << "vertices = " << noEntries << "\t"
           << "v-4 = " << noBetweenSections4 << G4endl;
    G4cout << G4endl;
    for(i = 0 ; i < noEntries ; i++ )
    {
      G4cout << i << "\t" << "v.x = " << ((*vertices)[i]).x() << "\t"
                          << "v.y = " << ((*vertices)[i]).y() << "\t"
                          << "v.z = " << ((*vertices)[i]).z() << "\t" << G4endl;
    }      
    G4cout << G4endl;
    G4cout << "ClipCrossSection" << G4endl;
    */
    for (i = 0 ; i < noEntries ; i += 4 )
    {
      // G4cout << "section = " << i << G4endl;
      ClipCrossSection(vertices,i,pVoxelLimit,pAxis,pMin,pMax) ;
    }
    // G4cout << "ClipBetweenSections" << G4endl;
    for (i = 0 ; i < noBetweenSections4 ; i += 4 )
    {
      // G4cout << "between sections = " << i << G4endl;
      ClipBetweenSections(vertices,i,pVoxelLimit,pAxis,pMin,pMax) ;
    }
    if (pMin != kInfinity || pMax != -kInfinity )
    {
      existsAfterClip = true ;
      pMin -= kCarTolerance ; // Add 2*tolerance to avoid precision troubles
      pMax += kCarTolerance ;
    }
    else
    {
      // Check for case where completely enveloping clipping volume
      // If point inside then we are confident that the solid completely
      // envelopes the clipping volume. Hence set min/max extents according
      // to clipping volume extents along the specified axis.

      G4ThreeVector clipCentre(
             (pVoxelLimit.GetMinXExtent()+pVoxelLimit.GetMaxXExtent())*0.5,
             (pVoxelLimit.GetMinYExtent()+pVoxelLimit.GetMaxYExtent())*0.5,
             (pVoxelLimit.GetMinZExtent()+pVoxelLimit.GetMaxZExtent())*0.5 ) ;
        
      if ( Inside(pTransform.Inverse().TransformPoint(clipCentre)) != kOutside )
      {
        existsAfterClip = true ;
        pMin            = pVoxelLimit.GetMinExtent(pAxis) ;
        pMax            = pVoxelLimit.GetMaxExtent(pAxis) ;
      }
    }
    delete vertices;
    return existsAfterClip;
  }
}


///////////////////////////////////////////////////////////////////////////
//
// Return whether point inside/outside/on surface

EInside G4Tubs::Inside( const G4ThreeVector& p ) const
{
  G4double r2,pPhi,tolRMin,tolRMax;
  EInside in = kOutside ;

  if (std::fabs(p.z()) <= fDz - kCarTolerance*0.5)
  {
    r2 = p.x()*p.x() + p.y()*p.y() ;

    if (fRMin) tolRMin = fRMin + kRadTolerance*0.5 ;
    else       tolRMin = 0 ;

    tolRMax = fRMax - kRadTolerance*0.5 ;
      
    if (r2 >= tolRMin*tolRMin && r2 <= tolRMax*tolRMax)
    {
      //  if ( fDPhi == twopi || r2 == 0 )  in = kInside ;
      if ( fDPhi == twopi )  in = kInside ;
      else
      {
        // Try inner tolerant phi boundaries (=>inside)
        // if not inside, try outer tolerant phi boundaries

        pPhi = std::atan2(p.y(),p.x()) ;

        if ( pPhi < -kAngTolerance*0.5 ) pPhi += twopi ; // 0<=pPhi<2pi

        if ( fSPhi >= 0 )
        {
          if ( (std::abs(pPhi) < kAngTolerance*0.5)
            && (std::abs(fSPhi + fDPhi - twopi) < kAngTolerance*0.5) )
          { 
            pPhi += twopi ; // 0 <= pPhi < 2pi
          }
          if ( (pPhi >= fSPhi + kAngTolerance*0.5)
            && (pPhi <= fSPhi + fDPhi - kAngTolerance*0.5) )
          {
            in = kInside ;
          }
          else if ( (pPhi >= fSPhi - kAngTolerance*0.5)
                 && (pPhi <= fSPhi + fDPhi + kAngTolerance*0.5) )
          {
            in = kSurface ;
          }
        }
        else  // fSPhi < 0
        {
          if ( (pPhi <= fSPhi + twopi - kAngTolerance*0.5)
            && (pPhi >= fSPhi + fDPhi  + kAngTolerance*0.5) ) ;
          else if ( (pPhi <= fSPhi + twopi + kAngTolerance*0.5)
                 && (pPhi >= fSPhi + fDPhi  - kAngTolerance*0.5) )
          {
            in = kSurface ;
          }
          else
          {
            in = kInside ;
          }
        }                    
      }
    }
    else  // Try generous boundaries
    {
      tolRMin = fRMin - kRadTolerance*0.5 ;
      tolRMax = fRMax + kRadTolerance*0.5 ;

      if ( tolRMin < 0 ) tolRMin = 0 ;

      if ( (r2 >= tolRMin*tolRMin) && (r2 <= tolRMax*tolRMax) )
      {
        if ( fDPhi == twopi || r2 == 0 ) // Continuous in phi or on z-axis
        {
          in = kSurface ;
        }
        else // Try outer tolerant phi boundaries only
        {
          pPhi = std::atan2(p.y(),p.x()) ;

          if ( pPhi < -kAngTolerance*0.5 ) pPhi += twopi ; // 0<=pPhi<2pi
          if ( fSPhi >= 0 )
          {
            if ( (std::abs(pPhi) < kAngTolerance*0.5)
              && (std::abs(fSPhi + fDPhi - twopi) < kAngTolerance*0.5) )
            { 
              pPhi += twopi ; // 0 <= pPhi < 2pi
            }
            if ( (pPhi >= fSPhi - kAngTolerance*0.5)
              && (pPhi <= fSPhi + fDPhi + kAngTolerance*0.5) )
            {
              in = kSurface ;
            }
          }
          else  // fSPhi < 0
          {
            if ( (pPhi <= fSPhi + twopi - kAngTolerance*0.5)
              && (pPhi >= fSPhi + fDPhi  + kAngTolerance*0.5) )  ;
            else
            {
              in = kSurface ;
            }
          }
        }
      }
    }
  }
  else if (std::fabs(p.z()) <= fDz + kCarTolerance*0.5)
  {                                          // Check within tolerant r limits
    r2      = p.x()*p.x() + p.y()*p.y() ;
    tolRMin = fRMin - kRadTolerance*0.5 ;
    tolRMax = fRMax + kRadTolerance*0.5 ;

    if ( tolRMin < 0 ) tolRMin = 0 ;

    if ( (r2 >= tolRMin*tolRMin) && (r2 <= tolRMax*tolRMax) )
    {
      if (fDPhi == twopi || r2 == 0 ) // Continuous in phi or on z-axis
      {
        in = kSurface ;
      }
      else // Try outer tolerant phi boundaries
      {
        pPhi = std::atan2(p.y(),p.x()) ;

        if ( pPhi < -kAngTolerance*0.5 ) pPhi += twopi ;   // 0<=pPhi<2pi
        if ( fSPhi >= 0 )
        {
          if ( (std::abs(pPhi) < kAngTolerance*0.5)
            && (std::abs(fSPhi + fDPhi - twopi) < kAngTolerance*0.5) )
          { 
            pPhi += twopi ; // 0 <= pPhi < 2pi
          }
          if ( (pPhi >= fSPhi - kAngTolerance*0.5)
            && (pPhi <= fSPhi + fDPhi + kAngTolerance*0.5) )
          {
            in = kSurface;
          }
        }
        else  // fSPhi < 0
        {
          if ( (pPhi <= fSPhi + twopi - kAngTolerance*0.5)
            && (pPhi >= fSPhi + fDPhi  + kAngTolerance*0.5) )  ;
          else
          {
            in = kSurface ;
          }
        }      
      }
    }
  }
  return in ;
}

///////////////////////////////////////////////////////////////////////////
//
// Return unit normal of surface closest to p
// - note if point on z axis, ignore phi divided sides
// - unsafe if point close to z axis a rmin=0 - no explicit checks

G4ThreeVector G4Tubs::SurfaceNormal( const G4ThreeVector& p ) const
{
  ENorm side;
  G4ThreeVector norm;
  G4double rho, phi;
  G4double distZ, distRMin, distRMax, distSPhi, distEPhi, distMin;

  rho = std::sqrt(p.x()*p.x() + p.y()*p.y());

  distRMin = std::fabs(rho - fRMin);
  distRMax = std::fabs(rho - fRMax);
  distZ    = std::fabs(std::fabs(p.z()) - fDz);

  if (fDPhi < twopi)   //  &&  rho ) // Protected against (0,0,z) 
  {
    phi = std::atan2(p.y(),p.x());

    if ( phi < 0 )    phi += twopi;

    if ( fSPhi < 0 )  distSPhi = std::fabs(phi - (fSPhi + twopi))*rho;    
    else              distSPhi = std::fabs(phi - fSPhi)*rho;
    
                      distEPhi = std::fabs(phi - fSPhi - fDPhi)*rho;
  }

#ifndef G4NEW_SURF_NORMAL

  if (distRMin < distRMax) // First minimum
  {
    if ( distZ < distRMin )
    {
       distMin = distZ;
       side    = kNZ;
    }
    else
    {
      distMin = distRMin;
      side    = kNRMin;
    }
  }
  else
  {
    if ( distZ < distRMax )
    {
      distMin = distZ;
      side    = kNZ;
    }
    else
    {
      distMin = distRMax;
      side    = kNRMax;
    }
  }   
  if (fDPhi < twopi  &&  rho ) // Protected against (0,0,z) 
  {                                      
    if (distSPhi < distEPhi) // Find new minimum
    {
      if ( distSPhi < distMin )  side = kNSPhi;      
    }
    else
    {
      if ( distEPhi < distMin )  side = kNEPhi ;      
    }
  }    
  switch ( side )
  {
    case kNRMin : // Inner radius
    {                      
      norm = G4ThreeVector(-p.x()/rho,-p.y()/rho,0);
      break;
    }
    case kNRMax : // Outer radius
    {                  
      norm = G4ThreeVector(p.x()/rho,p.y()/rho,0);
      break ;
    }
    case kNZ : //    + or - dz
    {                              
      if ( p.z() > 0 ) norm = G4ThreeVector(0,0,1); 
      else             norm = G4ThreeVector(0,0,-1); 
      break ;
    }
    case kNSPhi:
    {
      norm = G4ThreeVector(std::sin(fSPhi),-std::cos(fSPhi),0);
      break ;
    }
    case kNEPhi:
    {
      norm = G4ThreeVector(-std::sin(fSPhi+fDPhi),std::cos(fSPhi+fDPhi),0);
      break;
    }
    default:
    {
      DumpInfo();
      G4Exception("G4Tubs::SurfaceNormal()", "Notification", JustWarning,
                  "Undefined side for valid surface normal to solid.");
      break ;
    }    
  } 
               
#else  // algorithm supporting edges/corners

  G4double delta    = 0.5*kCarTolerance;
  G4ThreeVector nZ  = G4ThreeVector(0,0, 1.0);
  G4ThreeVector nmZ = G4ThreeVector(0,0,-1.0);
  G4ThreeVector nR, nr, nPs, nPe;

  if ( rho > delta ) 
  { 
                 nR  = G4ThreeVector(p.x()/rho,p.y()/rho,0);
    if ( fRMin ) nr  = G4ThreeVector(-p.x()/rho,-p.y()/rho,0);
  }
  if( fDPhi < twopi )   
  {
    nPs = G4ThreeVector(std::sin(fSPhi),-std::cos(fSPhi),0);
    nPe = G4ThreeVector(-std::sin(fSPhi+fDPhi),std::cos(fSPhi+fDPhi),0);
  }

  // Z -> (R -> Phi)-> (r -> Phi)

  if( distZ <= delta ) // on Z/mZ surfaces
  {
    if( p.z() >= 0.)  // on Z surface
    {
      if( distRMax <= delta )
      {
        if( fDPhi < twopi )
	{
          if (distSPhi <= delta)    norm = ( nZ + nR + nPs ).unit();	  
          else
	  {
            if (distEPhi <= delta)  norm = ( nZ + nR + nPe ).unit();
	    else                    norm = ( nZ + nR ).unit(); 
	  }
	}
        else                        norm = ( nZ + nR ).unit(); 
      }
      else  // out of RMax
      {
        if( fRMin )
	{
          if( distRMin <= delta )
          {
            if( fDPhi < twopi )
	    {
              if (distSPhi <= delta)    norm = ( nZ + nr + nPs ).unit();	  
              else
	      {
                if (distEPhi <= delta)  norm = ( nZ + nr + nPe ).unit();
	        else                    norm = ( nZ + nr ).unit(); 
	      }
	    }
            else                        norm = ( nZ + nr ).unit(); 
          }
          else
          {
            if( fDPhi < twopi )
	    {
              if (distSPhi <= delta)    norm = ( nZ + nPs ).unit();	  
              else
	      {
                if (distEPhi <= delta)  norm = ( nZ + nPe ).unit();
	        else                    norm = nZ; 
	      }
	    }
            else                        norm = nZ ;
	  }
	}
        else
	{
          if( fDPhi < twopi )
	  {
            if (distSPhi <= delta)    
	    {
              if( distEPhi <= delta ) norm = ( nZ + nPs + nPe).unit();
              else                    norm = ( nZ + nPs ).unit();
	    }	  
            else
	    {
              if (distEPhi <= delta)  norm = ( nZ + nPe ).unit();
	      else                    norm = nZ; 
	    }
	  }
          else                        norm = nZ;
	} 
      }
    }
    else  // on mZ surface
    {
      if( distRMax <= delta )
      {
        if( fDPhi < twopi )
	{
          if (distSPhi <= delta)    norm = ( nmZ + nR + nPs ).unit();	  
          else
	  {
            if (distEPhi <= delta)  norm = ( nmZ + nR + nPe ).unit();
	    else                    norm = ( nmZ + nR ).unit(); 
	  }
	}
        else                        norm = ( nmZ + nR ).unit(); 
      }
      else  // out of RMax
      {
        if( fRMin )
	{
          if( distRMin <= delta )
          {
            if( fDPhi < twopi )
	    {
              if (distSPhi <= delta)    norm = ( nmZ + nr + nPs ).unit();	  
              else
	      {
                if (distEPhi <= delta)  norm = ( nmZ + nr + nPe ).unit();
	        else                    norm = ( nmZ + nr ).unit(); 
	      }
	    }
            else                        norm = ( nmZ + nr ).unit(); 
          }
          else
          {
            if( fDPhi < twopi )
	    {
              if (distSPhi <= delta)    norm = ( nmZ + nPs ).unit();	  
              else
	      {
                if (distEPhi <= delta)  norm = ( nmZ + nPe ).unit();
	        else                    norm = nmZ; 
	      }
	    }
            else                        norm = nmZ ;
	  }
	}
        else
	{
          if( fDPhi < twopi )
	  {
            if (distSPhi <= delta)    
	      {
                if (distEPhi <= delta)  norm = ( nmZ + nPs + nPe ).unit();
		else                    norm = ( nmZ + nPs ).unit();
	      }	  
            else
	    {
              if (distEPhi <= delta)  norm = ( nmZ + nPe ).unit();
	      else                    norm = nmZ; 
	    }
	  }
          else                        norm = nmZ;
	} 
      }
    }
  }
  else  // out of Z/mZ surfaces, try R/r and SPhi/EPhi only
  {
    if( distRMax <= delta )
    {
      if( fDPhi < twopi )
      {
        if (distSPhi <= delta)    norm = ( nR + nPs ).unit();	  
        else
	{
          if (distEPhi <= delta)  norm = ( nR + nPe ).unit();
	  else                    norm = nR; 
	}
      }
      else                        norm = nR; 
    }
    else  // out of RMax
    {
      if( fRMin )
      {
        if( distRMin <= delta )
        {
          if( fDPhi < twopi )
	  {
            if (distSPhi <= delta)    norm = ( nr + nPs ).unit();	  
            else
	    {
              if (distEPhi <= delta)  norm = ( nr + nPe ).unit();
	      else                    norm = nr; 
	    }
	  }
          else                        norm = nr; 
        }
        else  // only Phi, if any
        {
          if( fDPhi < twopi )
	  {
            if (distSPhi <= delta)    norm = nPs;	  
            else
	    {
              if (distEPhi <= delta)  norm = nPe; 
              else
	      {
                G4Exception("G4Tubs::SurfaceNormal(p)", "Notification", JustWarning, 
                            "Point p is not on surface !?" ); 
	      }
	    }
	  }
          else
	  {
          G4Exception("G4Tubs::SurfaceNormal(p)", "Notification", JustWarning, 
                      "Point p is not on surface !?" ); 
	  }
	}
      }
      else
      {
        if( fDPhi < twopi )
	{
          if (distSPhi <= delta)    
	  {
            if (distEPhi <= delta)  norm = ( nPs + nPe ).unit(); 
            else                    norm = nPs;
	  }	  
          else
	  {
            if (distEPhi <= delta)  norm = nPe; 
            else                    // is not on surface !?
            {
              G4Exception("G4Tubs::SurfaceNormal(p)", "Notification", JustWarning, 
                          "Point p is not on surface !?" ); 
            }
	  }
	}
        else
	{
          G4Exception("G4Tubs::SurfaceNormal(p)", "Notification", JustWarning, 
                      "Point p is not on surface !?" ); 
	}
      } 
    }
  }

#endif

  return norm;
}

////////////////////////////////////////////////////////////////////
//
//
// Calculate distance to shape from outside, along normalised vector
// - return kInfinity if no intersection, or intersection distance <= tolerance
//
// - Compute the intersection with the z planes 
//        - if at valid r, phi, return
//
// -> If point is outer outer radius, compute intersection with rmax
//        - if at valid phi,z return
//
// -> Compute intersection with inner radius, taking largest +ve root
//        - if valid (in z,phi), save intersction
//
//    -> If phi segmented, compute intersections with phi half planes
//        - return smallest of valid phi intersections and
//          inner radius intersection
//
// NOTE:
// - Precalculations for phi trigonometry are Done `just in time'
// - `if valid' implies tolerant checking of intersection points

G4double G4Tubs::DistanceToIn( const G4ThreeVector& p,
                               const G4ThreeVector& v  ) const
{
  G4double snxt = kInfinity ;  // snxt = default return value

  // Precalculated trig for phi intersections - used by r,z intersections to
  //                                            check validity

  G4bool seg ;        // true if segmented

  G4double hDPhi, hDPhiOT, hDPhiIT, cosHDPhiOT=0., cosHDPhiIT=0. ;
          // half dphi + outer tolerance

  G4double cPhi, sinCPhi=0., cosCPhi=0. ;  // central phi

  G4double tolORMin2, tolIRMax2 ;  // `generous' radii squared

  G4double tolORMax2, tolIRMin2, tolODz, tolIDz ;

  // Intersection point variables
  //
  G4double Dist, s, xi, yi, zi, rho2, inum, iden, cosPsi ; 

  G4double t1, t2, t3, b, c, d ;   // Quadratic solver variables 

  G4double Comp ;
  G4double cosSPhi, sinSPhi ;    // Trig for phi start intersect

  G4double ePhi, cosEPhi, sinEPhi ;  // for phi end intersect

  // Set phi divided flag and precalcs

  if ( fDPhi < twopi )
  {
    seg        = true ;
    hDPhi      = 0.5*fDPhi ;    // half delta phi
    cPhi       = fSPhi + hDPhi ; 
    hDPhiOT    = hDPhi + 0.5*kAngTolerance ;  // outers tol' half delta phi 
    hDPhiIT    = hDPhi - 0.5*kAngTolerance ;
    sinCPhi    = std::sin(cPhi) ;
    cosCPhi    = std::cos(cPhi) ;
    cosHDPhiOT = std::cos(hDPhiOT) ;
    cosHDPhiIT = std::cos(hDPhiIT) ;
  }
  else
  {
    seg = false  ;
  }

  // Calculate tolerant rmin and rmax

  if (fRMin > kRadTolerance)
  {
    tolORMin2 = (fRMin - 0.5*kRadTolerance)*(fRMin - 0.5*kRadTolerance) ;
    tolIRMin2 = (fRMin + 0.5*kRadTolerance)*(fRMin + 0.5*kRadTolerance) ;
  }
  else
  {
    tolORMin2 = 0.0 ;
    tolIRMin2 = 0.0 ;
  }
  tolORMax2 = (fRMax + 0.5*kRadTolerance)*(fRMax + 0.5*kRadTolerance) ;
  tolIRMax2 = (fRMax - 0.5*kRadTolerance)*(fRMax - 0.5*kRadTolerance) ;

  // Intersection with Z surfaces

  tolIDz = fDz - kCarTolerance*0.5 ;
  tolODz = fDz + kCarTolerance*0.5 ;

  if (std::fabs(p.z()) >= tolIDz)
  {
    if ( p.z()*v.z() < 0 )    // at +Z going in -Z or visa versa
    {
      s = (std::fabs(p.z()) - fDz)/std::fabs(v.z()) ;     // Z intersect distance

      if(s < 0.0) s = 0.0 ;

      xi   = p.x() + s*v.x() ;                // Intersection coords
      yi   = p.y() + s*v.y() ;
      rho2 = xi*xi + yi*yi ;

      // Check validity of intersection

      if (tolIRMin2 <= rho2 && rho2 <= tolIRMax2)
      {
        if (seg && rho2)
        {
          // Psi = angle made with central (average) phi of shape
          //
          inum   = xi*cosCPhi + yi*sinCPhi ;
          iden   = std::sqrt(rho2) ;
          cosPsi = inum/iden ;
          if (cosPsi >= cosHDPhiIT) return s ;
        }
        else return s ;
      }
    }
    else
    {
      if ( snxt<kCarTolerance*0.5 ) snxt=0 ;
      return snxt ;  // On/outside extent, and heading away
                     // -> cannot intersect
    }
  }

  // -> Can not intersect z surfaces
  //
  // Intersection with rmax (possible return) and rmin (must also check phi)
  //
  // Intersection point (xi,yi,zi) on line x=p.x+t*v.x etc.
  //
  // Intersects with x^2+y^2=R^2
  //
  // Hence (v.x^2+v.y^2)t^2+ 2t(p.x*v.x+p.y*v.y)+p.x^2+p.y^2-R^2=0
  //            t1                t2                t3

  t1 = 1.0 - v.z()*v.z() ;
  t2 = p.x()*v.x() + p.y()*v.y() ;
  t3 = p.x()*p.x() + p.y()*p.y() ;

  if ( t1 > 0 )        // Check not || to z axis
  {
    b = t2/t1 ;
    c = t3 - fRMax*fRMax ;
    if (t3 >= tolORMax2 && t2<0)   // This also handles the tangent case
    {
      // Try outer cylinder intersection
      //          c=(t3-fRMax*fRMax)/t1;

      c /= t1 ;
      d = b*b - c ;

      if (d >= 0)  // If real root
      {
        s = -b - std::sqrt(d) ;
        if (s >= 0)  // If 'forwards'
        {
          // Check z intersection
          //
          zi = p.z() + s*v.z() ;
          if (std::fabs(zi)<=tolODz)
          {
            // Z ok. Check phi intersection if reqd
            //
            if (!seg)
            {
              return s ;
            }
            else
            {
              xi     = p.x() + s*v.x() ;
              yi     = p.y() + s*v.y() ;
              cosPsi = (xi*cosCPhi + yi*sinCPhi)/fRMax ;
              if (cosPsi >= cosHDPhiIT) return s ;
            }
          }  //  end if std::fabs(zi)
        }    //  end if (s>=0)
      }      //  end if (d>=0)
    }        //  end if (r>=fRMax)
    else 
    {
      // Inside outer radius :
      // check not inside, and heading through tubs (-> 0 to in)

      if (t3 > tolIRMin2 && t2 < 0 && std::fabs(p.z()) <= tolIDz)
      {
        // Inside both radii, delta r -ve, inside z extent

        if (seg)
        {
          inum   = p.x()*cosCPhi + p.y()*sinCPhi ;
          iden   = std::sqrt(t3) ;
          cosPsi = inum/iden ;
          if (cosPsi >= cosHDPhiIT) return 0.0 ;
        }
        else
        {
          return 0.0 ;
        }
      }
    }      
    if ( fRMin )    // Try inner cylinder intersection
    {
      c = (t3 - fRMin*fRMin)/t1 ;
      d = b*b - c ;
      if ( d >= 0.0 )  // If real root
      {
        // Always want 2nd root - we are outside and know rmax Hit was bad
        // - If on surface of rmin also need farthest root

        s = -b + std::sqrt(d) ;
        if (s >= -0.5*kCarTolerance)  // check forwards
        {
          // Check z intersection
          //
          if(s < 0.0) s = 0.0 ;
          zi = p.z() + s*v.z() ;
          if (std::fabs(zi) <= tolODz)
          {
            // Z ok. Check phi
            //
            if ( !seg )
            {
              return s ; 
            }
            else
            {
              xi     = p.x() + s*v.x() ;
              yi     = p.y() + s*v.y() ;
              cosPsi = (xi*cosCPhi + yi*sinCPhi)/fRMin ;
              if (cosPsi >= cosHDPhiIT)
              {
                // Good inner radius isect
                // - but earlier phi isect still possible

                snxt = s ;
              }
            }
          }        //    end if std::fabs(zi)
        }          //    end if (s>=0)
      }            //    end if (d>=0)
    }              //    end if (fRMin)
  }

  // Phi segment intersection
  //
  // o Tolerant of points inside phi planes by up to kCarTolerance*0.5
  //
  // o NOTE: Large duplication of code between sphi & ephi checks
  //         -> only diffs: sphi -> ephi, Comp -> -Comp and half-plane
  //            intersection check <=0 -> >=0
  //         -> use some form of loop Construct ?
  //
  if ( seg )
  {
    // First phi surface (`S'tarting phi)

    sinSPhi = std::sin(fSPhi) ;
    cosSPhi = std::cos(fSPhi) ;
    Comp    = v.x()*sinSPhi - v.y()*cosSPhi ;
                    
    if ( Comp < 0 )  // Component in outwards normal dirn
    {
      Dist = (p.y()*cosSPhi - p.x()*sinSPhi) ;

      if ( Dist < kCarTolerance*0.5 )
      {
        s = Dist/Comp ;

        if (s < snxt)
        {
          if ( s < 0 ) s = 0.0 ;
          zi = p.z() + s*v.z() ;
          if ( std::fabs(zi) <= tolODz )
          {
            xi   = p.x() + s*v.x() ;
            yi   = p.y() + s*v.y() ;
            rho2 = xi*xi + yi*yi ;

            if ( ( (rho2 >= tolIRMin2) && (rho2 <= tolIRMax2) )
              || ( (rho2 >  tolORMin2) && (rho2 <  tolIRMin2)
                && ( v.y()*cosSPhi - v.x()*sinSPhi >  0 )
                && ( v.x()*cosSPhi + v.y()*sinSPhi >= 0 )     )
              || ( (rho2 > tolIRMax2) && (rho2 < tolORMax2)
                && (v.y()*cosSPhi - v.x()*sinSPhi > 0)
                && (v.x()*cosSPhi + v.y()*sinSPhi < 0) )    )
            {
              // z and r intersections good
              // - check intersecting with correct half-plane
              //
              if ((yi*cosCPhi-xi*sinCPhi) <= 0) snxt = s ;
            }    
          }
        }
      }    
    }
      
    // Second phi surface (`E'nding phi)

    ePhi    = fSPhi + fDPhi ;
    sinEPhi = std::sin(ePhi) ;
    cosEPhi = std::cos(ePhi) ;
    Comp    = -(v.x()*sinEPhi - v.y()*cosEPhi) ;
        
    if (Comp < 0 )  // Component in outwards normal dirn
    {
      Dist = -(p.y()*cosEPhi - p.x()*sinEPhi) ;

      if ( Dist < kCarTolerance*0.5 )
      {
        s = Dist/Comp ;

        if (s < snxt)
        {
          if ( s < 0 ) s = 0 ;
          zi = p.z() + s*v.z() ;
          if ( std::fabs(zi) <= tolODz )
          {
            xi   = p.x() + s*v.x() ;
            yi   = p.y() + s*v.y() ;
            rho2 = xi*xi + yi*yi ;
            if ( ( (rho2 >= tolIRMin2) && (rho2 <= tolIRMax2) )
                || ( (rho2 > tolORMin2)  && (rho2 < tolIRMin2)
                  && (v.x()*sinEPhi - v.y()*cosEPhi >  0)
                  && (v.x()*cosEPhi + v.y()*sinEPhi >= 0) )
                || ( (rho2 > tolIRMax2) && (rho2 < tolORMax2)
                  && (v.x()*sinEPhi - v.y()*cosEPhi > 0)
                  && (v.x()*cosEPhi + v.y()*sinEPhi < 0) ) )
            {
              // z and r intersections good
              // - check intersecting with correct half-plane
              //
              if ( (yi*cosCPhi-xi*sinCPhi) >= 0 ) snxt = s ;
            }    
          }
        }
      }
    }         //  Comp < 0
  }           //  seg != 0
  if ( snxt<kCarTolerance*0.5 ) snxt=0 ;
  return snxt ;
}
 
//////////////////////////////////////////////////////////////////
//
// Calculate distance to shape from outside, along normalised vector
// - return kInfinity if no intersection, or intersection distance <= tolerance
//
// - Compute the intersection with the z planes 
//        - if at valid r, phi, return
//
// -> If point is outer outer radius, compute intersection with rmax
//        - if at valid phi,z return
//
// -> Compute intersection with inner radius, taking largest +ve root
//        - if valid (in z,phi), save intersction
//
//    -> If phi segmented, compute intersections with phi half planes
//        - return smallest of valid phi intersections and
//          inner radius intersection
//
// NOTE:
// - Precalculations for phi trigonometry are Done `just in time'
// - `if valid' implies tolerant checking of intersection points
//   Calculate distance (<= actual) to closest surface of shape from outside
// - Calculate distance to z, radial planes
// - Only to phi planes if outside phi extent
// - Return 0 if point inside

G4double G4Tubs::DistanceToIn( const G4ThreeVector& p ) const
{
  G4double safe=0.0, rho, safe1, safe2, safe3 ;
  G4double phiC, cosPhiC, sinPhiC, safePhi, ePhi, cosPsi ;

  rho   = std::sqrt(p.x()*p.x() + p.y()*p.y()) ;
  safe1 = fRMin - rho ;
  safe2 = rho - fRMax ;
  safe3 = std::fabs(p.z()) - fDz ;

  if ( safe1 > safe2 ) safe = safe1 ;
  else                 safe = safe2 ;
  if ( safe3 > safe )  safe = safe3 ;

  if (fDPhi < twopi && rho)
  {
    phiC    = fSPhi + fDPhi*0.5 ;
    cosPhiC = std::cos(phiC) ;
    sinPhiC = std::sin(phiC) ;

    // Psi=angle from central phi to point
    //
    cosPsi = (p.x()*cosPhiC + p.y()*sinPhiC)/rho ;

    if ( cosPsi < std::cos(fDPhi*0.5) )
    {
      // Point lies outside phi range

      if ( (p.y()*cosPhiC - p.x()*sinPhiC) <= 0 )
      {
        safePhi = std::fabs(p.x()*std::sin(fSPhi) - p.y()*std::cos(fSPhi)) ;
      }
      else
      {
        ePhi    = fSPhi + fDPhi ;
        safePhi = std::fabs(p.x()*std::sin(ePhi) - p.y()*std::cos(ePhi)) ;
      }
      if ( safePhi > safe ) safe = safePhi ;
    }
  }
  if ( safe < 0 ) safe = 0 ;
  return safe ;
}

//////////////////////////////////////////////////////////////////////////////
//
// Calculate distance to surface of shape from `inside', allowing for tolerance
// - Only Calc rmax intersection if no valid rmin intersection

G4double G4Tubs::DistanceToOut( const G4ThreeVector& p,
                                const G4ThreeVector& v,
                                const G4bool calcNorm,
                                      G4bool *validNorm,
                                      G4ThreeVector *n    ) const
{
  ESide side = kNull , sider = kNull, sidephi = kNull ;
  G4double snxt, sr = kInfinity, sphi = kInfinity, pdist ;
  G4double deltaR, t1, t2, t3, b, c, d2, roMin2 ;

  // Vars for phi intersection:

  G4double sinSPhi, cosSPhi, ePhi, sinEPhi, cosEPhi ;
  G4double cPhi, sinCPhi, cosCPhi ;
  G4double pDistS, compS, pDistE, compE, sphi2, xi, yi, vphi, roi2 ;

  // Z plane intersection

  if (v.z() > 0 )
  {
    pdist = fDz - p.z() ;
    if ( pdist > kCarTolerance*0.5 )
    {
      snxt = pdist/v.z() ;
      side = kPZ ;
    }
    else
    {
      if (calcNorm)
      {
        *n         = G4ThreeVector(0,0,1) ;
        *validNorm = true ;
      }
      return snxt = 0 ;
    }
  }
  else if ( v.z() < 0 )
  {
    pdist = fDz + p.z() ;

    if ( pdist > kCarTolerance*0.5 )
    {
      snxt = -pdist/v.z() ;
      side = kMZ ;
    }
    else
    {
      if (calcNorm)
      {
        *n         = G4ThreeVector(0,0,-1) ;
        *validNorm = true ;
      }
      return snxt = 0.0 ;
    }
  }
  else
  {
    snxt = kInfinity ;    // Travel perpendicular to z axis
    side = kNull;
  }

  // Radial Intersections
  //
  // Find intersction with cylinders at rmax/rmin
  // Intersection point (xi,yi,zi) on line x=p.x+t*v.x etc.
  //
  // Intersects with x^2+y^2=R^2
  //
  // Hence (v.x^2+v.y^2)t^2+ 2t(p.x*v.x+p.y*v.y)+p.x^2+p.y^2-R^2=0
  //
  //            t1                t2                    t3

  t1   = 1.0 - v.z()*v.z() ;      // since v normalised
  t2   = p.x()*v.x() + p.y()*v.y() ;
  t3   = p.x()*p.x() + p.y()*p.y() ;

  if ( snxt > 10*(fDz+fRMax) )  roi2 = 2*fRMax*fRMax;
  else  roi2 = snxt*snxt*t1 + 2*snxt*t2 + t3 ; // radius^2 on +-fDz

  if ( t1 > 0 ) // Check not parallel
  {
    // Calculate sr, r exit distance
     
    if ( (t2 >= 0.0) && (roi2 > fRMax*(fRMax + kRadTolerance)) )
    {
      // Delta r not negative => leaving via rmax

      deltaR = t3 - fRMax*fRMax ;

      // NOTE: Should use rho-fRMax<-kRadTolerance*0.5
      // - avoid sqrt for efficiency

      if ( deltaR < -kRadTolerance*fRMax )
      {
        b     = t2/t1 ;
        c     = deltaR/t1 ;
        sr    = -b + std::sqrt(b*b - c);
        sider = kRMax ;
      }
      else
      {
        // On tolerant boundary & heading outwards (or perpendicular to)
        // outer radial surface -> leaving immediately

        if ( calcNorm ) 
        {
          // if ( p.x() || p.y() )
          // {
          //  *n=G4ThreeVector(p.x(),p.y(),0);
          // }
          // else
          // {
          //  *n=v;
          // }
          *n         = G4ThreeVector(p.x()/fRMax,p.y()/fRMax,0) ;
          *validNorm = true ;
        }
        return snxt = 0 ; // Leaving by rmax immediately
      }
    }             
    else if ( t2 < 0. ) // i.e.  t2 < 0; Possible rmin intersection
    {
      roMin2 = t3 - t2*t2/t1 ; // min ro2 of the plane of movement 

      if ( fRMin && (roMin2 < fRMin*(fRMin - kRadTolerance)) )
      {
        deltaR = t3 - fRMin*fRMin ;
        b      = t2/t1 ;
        c      = deltaR/t1 ;
        d2     = b*b - c ;

        if ( d2 >= 0 )   // Leaving via rmin
        {
          // NOTE: SHould use rho-rmin>kRadTolerance*0.5
          // - avoid sqrt for efficiency

          if (deltaR > kRadTolerance*fRMin)
          {
            sr    = -b-std::sqrt(d2) ;
            sider = kRMin ;
          }
          else
          {
            if ( calcNorm ) *validNorm = false ; // Concave side
            return           snxt      = 0.0 ;
          }
        }
        else    // No rmin intersect -> must be rmax intersect
        {
          deltaR = t3 - fRMax*fRMax ;
          c      = deltaR/t1 ;
          sr     = -b + std::sqrt(b*b - c) ;
          sider  = kRMax ;
        }
      }
      else if ( roi2 > fRMax*(fRMax + kRadTolerance) )
           // No rmin intersect -> must be rmax intersect
      {
        deltaR = t3 - fRMax*fRMax ;
        b      = t2/t1 ;
        c      = deltaR/t1;
        sr     = -b + std::sqrt(b*b - c) ;
        sider  = kRMax ;
      }
    }
    
    // Phi Intersection

    if ( fDPhi < twopi )
    {
      sinSPhi = std::sin(fSPhi) ;
      cosSPhi = std::cos(fSPhi) ;
      ePhi    = fSPhi + fDPhi ;
      sinEPhi = std::sin(ePhi) ;
      cosEPhi = std::cos(ePhi) ;
      cPhi    = fSPhi + fDPhi*0.5 ;
      sinCPhi = std::sin(cPhi) ;
      cosCPhi = std::cos(cPhi) ;

      if ( p.x() || p.y() )  // Check if on z axis (rho not needed later)
      {
        // pDist -ve when inside

        pDistS = p.x()*sinSPhi - p.y()*cosSPhi ;
        pDistE = -p.x()*sinEPhi + p.y()*cosEPhi ;

        // Comp -ve when in direction of outwards normal

        compS   = -sinSPhi*v.x() + cosSPhi*v.y() ;
        compE   =  sinEPhi*v.x() - cosEPhi*v.y() ;
        sidephi = kNull;

        //       if ( pDistS <= 0 && pDistE <= 0 )

        if( ( (fDPhi <= pi) && ( (pDistS <= 0.5*kCarTolerance)
                              && (pDistE <= 0.5*kCarTolerance) ) )
         || ( (fDPhi >  pi) && !((pDistS >  0.5*kCarTolerance)
                              && (pDistE >  0.5*kCarTolerance) ) )  )
        {
          // Inside both phi *full* planes
          if ( compS < 0 )
          {
            sphi = pDistS/compS ;
            if (sphi >= -0.5*kCarTolerance)
            {
              xi = p.x() + sphi*v.x() ;
              yi = p.y() + sphi*v.y() ;

              // Check intersecting with correct half-plane
              // (if not -> no intersect)
              //
              if ((yi*cosCPhi-xi*sinCPhi)>=0)
              {
                sphi = kInfinity ;
              }
              else
              {
                sidephi = kSPhi ;
                if ( pDistS > -kCarTolerance*0.5 )
                {
                  sphi = 0.0 ; // Leave by sphi immediately
                }    
              }       
            }
            else
            {
              sphi = kInfinity ;
            }
          }
          else
          {
            sphi = kInfinity ;
          }

          if ( compE < 0 )
          {
            sphi2 = pDistE/compE ;

            // Only check further if < starting phi intersection
            //
            if ( (sphi2 > -0.5*kCarTolerance) && (sphi2 < sphi) )
            {
              xi = p.x() + sphi2*v.x() ;
              yi = p.y() + sphi2*v.y() ;

              // Check intersecting with correct half-plane 

              if ( (yi*cosCPhi-xi*sinCPhi) >= 0)
              {
                // Leaving via ending phi

                sidephi = kEPhi ;
                if ( pDistE <= -kCarTolerance*0.5 ) sphi = sphi2 ;
                else                                sphi = 0.0 ;
              }
            }
          }
        }
        else
        {
          sphi = kInfinity ;
        }
      }
      else
      {
        // On z axis + travel not || to z axis -> if phi of vector direction
        // within phi of shape, Step limited by rmax, else Step =0

        vphi = std::atan2(v.y(),v.x()) ;
        if ( (fSPhi < vphi) && (vphi < fSPhi + fDPhi) )
        {
          sphi = kInfinity ;
        }
        else
        {
          sidephi = kSPhi ; // arbitrary 
          sphi    = 0.0 ;
        }
      }
      if (sphi < snxt)  // Order intersecttions
      {
        snxt = sphi ;
        side = sidephi ;
      }
    }
    if (sr < snxt)  // Order intersections
    {
      snxt = sr ;
      side = sider ;
    }
  }
  if (calcNorm)
  {
    switch(side)
    {
      case kRMax:
        // Note: returned vector not normalised
        // (divide by fRMax for unit vector)
        //
        xi = p.x() + snxt*v.x() ;
        yi = p.y() + snxt*v.y() ;
        *n = G4ThreeVector(xi/fRMax,yi/fRMax,0) ;
        *validNorm = true ;
        break ;

      case kRMin:
        *validNorm = false ;  // Rmin is inconvex
        break ;

      case kSPhi:
        if ( fDPhi <= pi )
        {
          *n         = G4ThreeVector(std::sin(fSPhi),-std::cos(fSPhi),0) ;
          *validNorm = true ;
        }
        else
        {
          *validNorm = false ;
        }
        break ;

      case kEPhi:
        if (fDPhi <= pi)
        {
          *n         = G4ThreeVector(-std::sin(fSPhi+fDPhi),std::cos(fSPhi+fDPhi),0) ;
          *validNorm = true ;
        }
        else
        {
          *validNorm = false ;
        }
        break ;

      case kPZ:
        *n=G4ThreeVector(0,0,1) ;
        *validNorm=true ;
        break ;

      case kMZ:
        *n         = G4ThreeVector(0,0,-1) ;
        *validNorm = true ;
        break ;

      default:
        G4cout.precision(16) ;
        G4cout << G4endl ;
        DumpInfo();
        G4cout << "Position:"  << G4endl << G4endl ;
        G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl ;
        G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl ;
        G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl ;
        G4cout << "Direction:" << G4endl << G4endl ;
        G4cout << "v.x() = "   << v.x() << G4endl ;
        G4cout << "v.y() = "   << v.y() << G4endl ;
        G4cout << "v.z() = "   << v.z() << G4endl << G4endl ;
        G4cout << "Proposed distance :" << G4endl << G4endl ;
        G4cout << "snxt = "    << snxt/mm << " mm" << G4endl << G4endl ;
        G4Exception("G4Tubs::DistanceToOut(p,v,..)","Notification",JustWarning,
                    "Undefined side for valid surface normal to solid.");
        break ;
    }
  }
  if ( snxt<kCarTolerance*0.5 ) snxt=0 ;
  return snxt ;
}

//////////////////////////////////////////////////////////////////////////
//
// Calculate distance (<=actual) to closest surface of shape from inside

G4double G4Tubs::DistanceToOut( const G4ThreeVector& p ) const
{
  G4double safe=0.0, rho, safeR1, safeR2, safeZ ;
  G4double safePhi, phiC, cosPhiC, sinPhiC, ePhi ;
  rho = std::sqrt(p.x()*p.x() + p.y()*p.y()) ;

#ifdef G4CSGDEBUG
  if( Inside(p) == kOutside )
  {
    G4cout.precision(16) ;
    G4cout << G4endl ;
    DumpInfo();
    G4cout << "Position:"  << G4endl << G4endl ;
    G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl ;
    G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl ;
    G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl ;
    G4Exception("G4Tubs::DistanceToOut(p)", "Notification", JustWarning, 
                 "Point p is outside !?");
  }
#endif

  if ( fRMin )
  {
    safeR1 = rho   - fRMin ;
    safeR2 = fRMax - rho ;
 
    if ( safeR1 < safeR2 ) safe = safeR1 ;
    else                   safe = safeR2 ;
  }
  else
  {
    safe = fRMax - rho ;
  }
  safeZ = fDz - std::fabs(p.z()) ;

  if ( safeZ < safe ) safe = safeZ ;

  // Check if phi divided, Calc distances closest phi plane
  //
  if ( fDPhi < twopi )
  {
    // Above/below central phi of Tubs?

    phiC    = fSPhi + fDPhi*0.5 ;
    cosPhiC = std::cos(phiC) ;
    sinPhiC = std::sin(phiC) ;

    if ( (p.y()*cosPhiC - p.x()*sinPhiC) <= 0 )
    {
      safePhi = -(p.x()*std::sin(fSPhi) - p.y()*std::cos(fSPhi)) ;
    }
    else
    {
      ePhi    = fSPhi + fDPhi ;
      safePhi = (p.x()*std::sin(ePhi) - p.y()*std::cos(ePhi)) ;
    }
    if (safePhi < safe) safe = safePhi ;
  }
  if ( safe < 0 ) safe = 0 ;

  return safe ;  
}

/////////////////////////////////////////////////////////////////////////
//
// Create a List containing the transformed vertices
// Ordering [0-3] -fDz cross section
//          [4-7] +fDz cross section such that [0] is below [4],
//                                             [1] below [5] etc.
// Note:
//  Caller has deletion resposibility
//  Potential improvement: For last slice, use actual ending angle
//                         to avoid rounding error problems.

G4ThreeVectorList*
G4Tubs::CreateRotatedVertices( const G4AffineTransform& pTransform ) const
{
  G4ThreeVectorList* vertices ;
  G4ThreeVector vertex0, vertex1, vertex2, vertex3 ;
  G4double meshAngle, meshRMax, crossAngle,
           cosCrossAngle, sinCrossAngle, sAngle;
  G4double rMaxX, rMaxY, rMinX, rMinY, meshRMin ;
  G4int crossSection, noCrossSections;

  // Compute no of cross-sections necessary to mesh tube
  //
  noCrossSections = G4int(fDPhi/kMeshAngleDefault) + 1 ;

  if ( noCrossSections < kMinMeshSections )
  {
    noCrossSections = kMinMeshSections ;
  }
  else if (noCrossSections>kMaxMeshSections)
  {
    noCrossSections = kMaxMeshSections ;
  }
  // noCrossSections = 4 ;

  meshAngle = fDPhi/(noCrossSections - 1) ;
  // meshAngle = fDPhi/(noCrossSections) ;

  meshRMax  = (fRMax+100*kCarTolerance)/std::cos(meshAngle*0.5) ;
  meshRMin = fRMin - 100*kCarTolerance ; 
 
  // If complete in phi, set start angle such that mesh will be at fRMax
  // on the x axis. Will give better extent calculations when not rotated.

  if (fDPhi == pi*2.0 && fSPhi == 0 ) sAngle = -meshAngle*0.5 ;
  else                                  sAngle =  fSPhi ;
    
  vertices = new G4ThreeVectorList();
  vertices->reserve(noCrossSections*4);
    
  if ( vertices )
  {
    for (crossSection = 0 ; crossSection < noCrossSections ; crossSection++ )
    {
      // Compute coordinates of cross section at section crossSection

      crossAngle    = sAngle + crossSection*meshAngle ;
      cosCrossAngle = std::cos(crossAngle) ;
      sinCrossAngle = std::sin(crossAngle) ;

      rMaxX = meshRMax*cosCrossAngle ;
      rMaxY = meshRMax*sinCrossAngle ;

      if(meshRMin <= 0.0)
      {
        rMinX = 0.0 ;
        rMinY = 0.0 ;
      }
      else
      {
        rMinX = meshRMin*cosCrossAngle ;
        rMinY = meshRMin*sinCrossAngle ;
      }
      vertex0 = G4ThreeVector(rMinX,rMinY,-fDz) ;
      vertex1 = G4ThreeVector(rMaxX,rMaxY,-fDz) ;
      vertex2 = G4ThreeVector(rMaxX,rMaxY,+fDz) ;
      vertex3 = G4ThreeVector(rMinX,rMinY,+fDz) ;

      vertices->push_back(pTransform.TransformPoint(vertex0)) ;
      vertices->push_back(pTransform.TransformPoint(vertex1)) ;
      vertices->push_back(pTransform.TransformPoint(vertex2)) ;
      vertices->push_back(pTransform.TransformPoint(vertex3)) ;
    }
  }
  else
  {
    DumpInfo();
    G4Exception("G4Tubs::CreateRotatedVertices()",
                "FatalError", FatalException,
                "Error in allocation of vertices. Out of memory !");
  }
  return vertices ;
}

//////////////////////////////////////////////////////////////////////////
//
// Stream object contents to an output stream

G4GeometryType G4Tubs::GetEntityType() const
{
  return G4String("G4Tubs");
}

//////////////////////////////////////////////////////////////////////////
//
// Stream object contents to an output stream

std::ostream& G4Tubs::StreamInfo( std::ostream& os ) const
{
  os << "-----------------------------------------------------------\n"
     << "    *** Dump for solid - " << GetName() << " ***\n"
     << "    ===================================================\n"
     << " Solid type: G4Tubs\n"
     << " Parameters: \n"
     << "    inner radius : " << fRMin/mm << " mm \n"
     << "    outer radius : " << fRMax/mm << " mm \n"
     << "    half length Z: " << fDz/mm << " mm \n"
     << "    starting phi : " << fSPhi/degree << " degrees \n"
     << "    delta phi    : " << fDPhi/degree << " degrees \n"
     << "-----------------------------------------------------------\n";

  return os;
}

///////////////////////////////////////////////////////////////////////////
//
// Methods for visualisation


void G4Tubs::DescribeYourselfTo ( G4VGraphicsScene& scene ) const 
{
  scene.AddSolid (*this) ;
}

G4Polyhedron* G4Tubs::CreatePolyhedron () const 
{
  return new G4PolyhedronTubs (fRMin, fRMax, fDz, fSPhi, fDPhi) ;
}

G4NURBS* G4Tubs::CreateNURBS () const 
{
  G4NURBS* pNURBS ;
  if (fRMin != 0) 
  {
    if (fDPhi >= twopi) 
    {
      pNURBS = new G4NURBStube (fRMin,fRMax,fDz) ;
    }
    else 
    {
      pNURBS = new G4NURBStubesector (fRMin,fRMax,fDz,fSPhi,fSPhi+fDPhi) ;
    }
  }
  else 
  {
    if (fDPhi >= twopi) 
    {
      pNURBS = new G4NURBScylinder (fRMax,fDz) ;
    }
    else 
    {
      const G4double epsilon = 1.e-4 ; // Cylinder sector not yet available!
      pNURBS = new G4NURBStubesector (epsilon,fRMax,fDz,fSPhi,fSPhi+fDPhi) ;
    }
  }
  return pNURBS ;
}

//
//
/////////////////////////////////// End of G4Tubs.cc ////////////////////////
