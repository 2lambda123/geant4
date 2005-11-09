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
// $Id: G4EllipticalCone.cc,v 1.7 2005-11-09 15:04:28 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Implementation of G4EllipticalCone class
//
// This code implements an Elliptical Cone given explicitly by the
// equation:
//   x^2/a^2 + y^2/b^2 = (z-h)^2
// and specified by the parameters (a,b,h) and a cut parallel to the
// xy plane above z = 0.
//
// Author: Dionysios Anninos
//
// --------------------------------------------------------------------

#include "globals.hh"

#include "G4EllipticalCone.hh"

#include "G4ClippablePolygon.hh"
#include "G4SolidExtentList.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"

#include "meshdefs.hh"

#include "Randomize.hh"

#include "G4VGraphicsScene.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4NURBSbox.hh"
#include "G4VisExtent.hh"

using namespace CLHEP;

//////////////////////////////////////////////////////////////////////
//
// Constructor - check parameters
//
G4EllipticalCone::G4EllipticalCone(const G4String& pName,
                                         G4double  pxSemiAxis,
                                         G4double  pySemiAxis,
                                         G4double  pzMax,
                                         G4double  pzTopCut)
  : G4VSolid(pName), fpPolyhedron(0), fCubicVolume(0.)
{

  // Check Semi-Axis
  //
  if ( (pxSemiAxis > 0.) && (pySemiAxis > 0.) && (pzMax > 0.) )
  {
     SetSemiAxis( pxSemiAxis, pySemiAxis, pzMax );
  }
  else
  {
     G4cerr << "ERROR - G4EllipticalCone::G4EllipticalCone(): "
            << GetName() << G4endl
            << "        Invalid semi-axis or height!" << G4endl;
     G4Exception("G4EllipticalCone::G4EllipticalCone()", "InvalidSetup",
                 FatalException, "Invalid semi-axis or height.");
  }

  if ( pzTopCut > 0 )
  {
     SetZCut(pzTopCut);
  }
  else
  {
     G4cerr << "ERROR - G4EllipticalCone::G4EllipticalCone(): "
            << GetName() << G4endl
            << "        Invalid z-coordinate for cutting plane !" << G4endl;
     G4Exception("G4EllipticalCone::G4EllipticalCone()", "InvalidSetup",
                 FatalException, "Invalid z-coordinate for cutting plane.");
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// Fake default constructor - sets only member data and allocates memory
//                            for usage restricted to object persistency.
//
G4EllipticalCone::G4EllipticalCone( __void__& a )
  : G4VSolid(a), fpPolyhedron(0), fCubicVolume(0.)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Destructor
//
G4EllipticalCone::~G4EllipticalCone()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Calculate extent under transform and specified limit
//
G4bool
G4EllipticalCone::CalculateExtent( const EAxis axis,
                                   const G4VoxelLimits &voxelLimit,
                                   const G4AffineTransform &transform,
                                         G4double &min, G4double &max ) const
{
  G4SolidExtentList  extentList( axis, voxelLimit );
  
  //
  // We are going to divide up our elliptical face into small pieces
  //
  
  //
  // Choose phi size of our segment(s) based on constants as
  // defined in meshdefs.hh
  //
  G4int numPhi = kMaxMeshSections;
  G4double sigPhi = twopi/numPhi;
  
  //
  // We have to be careful to keep our segments completely outside
  // of the elliptical surface. To do so we imagine we have
  // a simple (unit radius) circular cross section (as in G4Tubs) 
  // and then "stretch" the dimensions as necessary to fit the ellipse.
  //
  G4double rFudge = 1.0/std::cos(0.5*sigPhi);
  G4double dxFudgeBot = xSemiAxis*2.*zheight*rFudge,
           dyFudgeBot = ySemiAxis*2.*zheight*rFudge;
  G4double dxFudgeTop = xSemiAxis*(zheight-zTopCut)*rFudge,
           dyFudgeTop = ySemiAxis*(zheight-zTopCut)*rFudge;
  
  //
  // As we work around the elliptical surface, we build
  // a "phi" segment on the way, and keep track of two
  // additional polygons for the two ends.
  //
  G4ClippablePolygon endPoly1, endPoly2, phiPoly;
  
  G4double phi = 0, 
           cosPhi = std::cos(phi),
           sinPhi = std::sin(phi);
  G4ThreeVector v0( dxFudgeTop*cosPhi, dyFudgeTop*sinPhi, +zTopCut ),
                v1( dxFudgeBot*cosPhi, dyFudgeBot*sinPhi, -zTopCut ),
                w0, w1;
  transform.ApplyPointTransform( v0 );
  transform.ApplyPointTransform( v1 );
  do
  {
    phi += sigPhi;
    if (numPhi == 1) phi = 0;  // Try to avoid roundoff
    cosPhi = std::cos(phi), 
    sinPhi = std::sin(phi);
    
    w0 = G4ThreeVector( dxFudgeTop*cosPhi, dyFudgeTop*sinPhi, +zTopCut );
    w1 = G4ThreeVector( dxFudgeBot*cosPhi, dyFudgeBot*sinPhi, -zTopCut );
    transform.ApplyPointTransform( w0 );
    transform.ApplyPointTransform( w1 );
    
    //
    // Add a point to our z ends
    //
    endPoly1.AddVertexInOrder( v0 );
    endPoly2.AddVertexInOrder( v1 );
    
    //
    // Build phi polygon
    //
    phiPoly.ClearAllVertices();
    
    phiPoly.AddVertexInOrder( v0 );
    phiPoly.AddVertexInOrder( v1 );
    phiPoly.AddVertexInOrder( w1 );
    phiPoly.AddVertexInOrder( w0 );
    
    if (phiPoly.PartialClip( voxelLimit, axis ))
    {
      //
      // Get unit normal
      //
      phiPoly.SetNormal( (v1-v0).cross(w0-v0).unit() );
      
      extentList.AddSurface( phiPoly );
    }

    //
    // Next vertex
    //    
    v0 = w0;
    v1 = w1;
  } while( --numPhi > 0 );

  //
  // Process the end pieces
  //
  if (endPoly1.PartialClip( voxelLimit, axis ))
  {
    static const G4ThreeVector normal(0,0,+1);
    endPoly1.SetNormal( transform.TransformAxis(normal) );
    extentList.AddSurface( endPoly1 );
  }
  
  if (endPoly2.PartialClip( voxelLimit, axis ))
  {
    static const G4ThreeVector normal(0,0,-1);
    endPoly2.SetNormal( transform.TransformAxis(normal) );
    extentList.AddSurface( endPoly2 );
  }
  
  //
  // Return min/max value
  //
  return extentList.GetExtent( min, max );
}

////////////////////////////////////////////////////////////////////////
//
// Return whether point inside/outside/on surface
// Split into radius, phi, theta checks
// Each check modifies `in', or returns as approprate
//
EInside G4EllipticalCone::Inside(const G4ThreeVector& p) const
{
  G4double rad2oo,  // outside surface outer tolerance
           rad2oi;  // outside surface inner tolerance
  
  EInside in;

  // check this side of z cut first, because that's fast
  //
  if ( (p.z() < -zTopCut - 0.5*kCarTolerance)
    || (p.z() > zTopCut + 0.5*kCarTolerance ) )
  {
    return in = kOutside; 
  }

  rad2oo= sqr(p.x()/( xSemiAxis + 0.5*kRadTolerance ))
        + sqr(p.y()/( ySemiAxis + 0.5*kRadTolerance ));

  if ( rad2oo > sqr( zheight-p.z() ) )
  {
    return in = kOutside; 
  }

  rad2oi= sqr( p.x()*(1.0 + 0.5*kRadTolerance/(xSemiAxis*xSemiAxis)) )
        + sqr( p.y()*(1.0 + 0.5*kRadTolerance/(ySemiAxis*ySemiAxis)) );
     
  if (rad2oi < sqr( zheight-p.z() ) )
  {
    in = ( ( p.z() < -zTopCut + 0.5*kRadTolerance )
        || ( p.z() >  zTopCut - 0.5*kRadTolerance ) ) ? kSurface : kInside;
  }
  else 
  {
    in = kSurface;
  }

  return in;
}

/////////////////////////////////////////////////////////////////////////
//
// Return unit normal of surface closest to p not protected against p=0
//
G4ThreeVector G4EllipticalCone::SurfaceNormal( const G4ThreeVector& p) const
{

  G4double rx = sqr(p.x()/xSemiAxis), 
           ry = sqr(p.y()/ySemiAxis);

  G4double rad = std::sqrt(rx + ry); 

  G4ThreeVector norm;

  if( (p.z() < -zTopCut) && ((rx+ry) < sqr(zTopCut + zheight)) )
  {
    return G4ThreeVector( 0., 0., -1. ); 
  }

  if( (p.z() > (zheight > zTopCut ? zheight : zTopCut)) &&
      ((rx+ry) < sqr(zheight-zTopCut)) )
  {
    return G4ThreeVector( 0., 0., 1. );
  }

  if( p.z() > rad + 2.*zTopCut - zheight ) 
  {
    if ( p.z() > zTopCut )
    {
      if( p.x() == 0. ) 
      {
        norm = G4ThreeVector( 0., p.y() < 0. ? -1. : 1., 1. ); 
        return norm /= norm.mag();
      } 
      if( p.y() == 0. )
      {
        norm = G4ThreeVector( p.x() < 0. ? -1. : 1., 0., 1. ); 
        return norm /= norm.mag();
      } 
      
      G4double m =  std::fabs(p.x()/p.y());
      G4double c2 = sqr(zheight-zTopCut)/(1./sqr(xSemiAxis)+sqr(m/ySemiAxis));
      G4double x  = std::sqrt(c2);
      G4double y  = m*x;
        
      x /= sqr(xSemiAxis);
      y /= sqr(ySemiAxis);
      
      norm = G4ThreeVector( p.x() < 0. ? -x : x, 
                            p.y() < 0. ? -y : y,
                            zheight - zTopCut );
      norm /= norm.mag();
      norm += G4ThreeVector( 0., 0., 1. );
      return norm /= norm.mag();      
    }
    
    return G4ThreeVector( 0., 0., 1. );    
  }
  
  if( p.z() < rad - 2.*zTopCut - zheight )
  {
    if( p.x() == 0. ) 
    {
      norm = G4ThreeVector( 0., p.y() < 0. ? -1. : 1., -1. ); 
      return norm /= norm.mag();
    } 
    if( p.y() == 0. )
    {
      norm = G4ThreeVector( p.x() < 0. ? -1. : 1., 0., -1. ); 
      return norm /= norm.mag();
    } 
    
    G4double m =  std::fabs(p.x()/p.y());
    G4double c2 = sqr(zheight+zTopCut)/(1./sqr(xSemiAxis)+sqr(m/ySemiAxis));
    G4double x  = std::sqrt(c2);
    G4double y  = m*x;
    
    x /= sqr(xSemiAxis);
    y /= sqr(ySemiAxis);
    
    norm = G4ThreeVector( p.x() < 0. ? -x : x, 
                          p.y() < 0. ? -y : y,
                          zheight - zTopCut );
    norm /= norm.mag();
    norm += G4ThreeVector( 0., 0., -1. );
    return norm /= norm.mag();      
  }
    
  norm  = G4ThreeVector(p.x()/sqr(xSemiAxis), p.y()/sqr(ySemiAxis), rad);
   
  G4double m = std::tan(pi/8.);
  G4double c = -zTopCut - m*(zTopCut + zheight);

  if( p.z() < -m*rad + c )
    return G4ThreeVector (0.,0.,-1.);

  return norm /= norm.mag();
}

//////////////////////////////////////////////////////////////////////////
//
// Calculate distance to shape from outside, along normalised vector
// return kInfinity if no intersection, or intersection distance <= tolerance
//
G4double G4EllipticalCone::DistanceToIn( const G4ThreeVector& p,
                                         const G4ThreeVector& v  ) const
{
  G4double distMin = kInfinity;
  
  // check to see if Z plane is relevant
  //
  if (p.z() < -zTopCut - 0.5*kCarTolerance)
  {
    if (v.z() <= 0.0)
      return distMin; 

    G4double lambda = (-zTopCut - p.z())/v.z();
    
    if ( sqr((lambda*v.x()+p.x())/xSemiAxis) + 
         sqr((lambda*v.y()+p.y())/ySemiAxis) <=
         sqr(zTopCut + zheight + 0.5*kRadTolerance) ) 
    { 
      return distMin = std::fabs(lambda);    
    }
  }

  if (p.z() > zTopCut+0.5*kCarTolerance) 
  {
    if (v.z() >= 0.0)
      { return distMin; }

    G4double lambda  = (zTopCut - p.z()) / v.z();

    if ( sqr((lambda*v.x() + p.x())/xSemiAxis) + 
         sqr((lambda*v.y() + p.y())/ySemiAxis) <=
         sqr(zheight - zTopCut + 0.5*kRadTolerance) )
      {
        return distMin = std::fabs(lambda);
      }
  }
  
  if (p.z() > zTopCut - 0.5*kCarTolerance)
  {
    if (v.z() > 0.) 
      return kInfinity;
    
    return distMin = 0.;
  }
  
  if (p.z() < -zTopCut + 0.5*kCarTolerance)
  {
    if (v.z() < 0.)
      return distMin = kInfinity;
    
    return distMin = 0.;
  }
  
  // if we are here then it either intersects or grazes the curved surface 
  // or it does not intersect at all
  //
  G4double A = sqr(v.x()/xSemiAxis) + sqr(v.y()/ySemiAxis) - sqr(v.z());
  G4double B = 2*(v.x()*p.x()/sqr(xSemiAxis) + 
                  v.y()*p.y()/sqr(ySemiAxis) + v.z()*(zheight-p.z()));
  G4double C = sqr(p.x()/xSemiAxis) + sqr(p.y()/ySemiAxis) - 
               sqr(zheight - p.z());
 
  G4double discr = B*B - 4.*A*C;
   
  // if the discriminant is negative it never hits the curved object
  //
  if ( discr < -0.5*kCarTolerance )
    { return distMin; }
  
  //case below is when it hits or grazes the surface
  //
  if ( (discr >= - 0.5*kCarTolerance ) && (discr < 0.5*kCarTolerance ) )
  {
    return distMin = std::fabs(-B/(2.*A)); 
  }
  
  G4double plus  = (-B+std::sqrt(discr))/(2.*A);
  G4double minus = (-B-std::sqrt(discr))/(2.*A);
  G4double lambda   = std::fabs(plus) < std::fabs(minus) ? plus : minus;
 
  return std::fabs(lambda);
}

//////////////////////////////////////////////////////////////////////////
//
// Calculate distance (<= actual) to closest surface of shape from outside
// Return 0 if point inside
//
G4double G4EllipticalCone::DistanceToIn(const G4ThreeVector& p) const
{
  G4double distR, distR2, distZ, maxDim;
  G4double distRad;  

  // check if the point lies either below z=-zTopCut in bottom elliptical region
  // or on top within cut elliptical region
  //
  if( (p.z() < -zTopCut) && (sqr(p.x()/xSemiAxis) + sqr(p.y()/ySemiAxis)
                           < sqr(zTopCut + zheight + 0.5*kCarTolerance )) )
  {  
    return distZ = std::fabs(zTopCut - p.z());
  } 
  
  if( (p.z() > zTopCut) && (sqr(p.x()/xSemiAxis)+sqr(p.y()/ySemiAxis)
                          < sqr(zheight - zTopCut + kCarTolerance/2.0 )) )
  {
    return distZ = std::fabs(p.z() - zTopCut);
  } 
  
  // below we use the following approximation: we take the largest of the
  // axes and find the shortest distance to the circular (cut) cone of that
  // radius.  
  //
  maxDim = xSemiAxis >= ySemiAxis ? xSemiAxis:ySemiAxis;
  distRad = std::sqrt(p.x()*p.x()+p.y()*p.y());

  if( p.z() > maxDim*distRad + zTopCut*(1.+maxDim)-sqr(maxDim)*zheight )
  {
    distR2 = sqr(p.z() - zTopCut) + sqr(distRad - maxDim*(zheight - zTopCut));
    return std::sqrt( distR2 );
  } 

  if( distRad > maxDim*( zheight - p.z() ) )
  {
    if( p.z() > maxDim*distRad - (zTopCut*(1.+maxDim)+sqr(maxDim)*zheight) )
    {
      G4double zVal = (p.z()-maxDim*(distRad-maxDim*zheight))/(1.+sqr(maxDim));
      G4double rVal = maxDim*(zheight - zVal);
      return distR  = std::sqrt(sqr(p.z() - zVal) + sqr(distRad - rVal));
    }
  }

  if( distRad <= maxDim*(zheight - p.z()) )
  {
    distR2 = sqr(distRad - maxDim*(zheight + zTopCut)) + sqr(p.z() + zTopCut);
    return std::sqrt( distR2 );    
  }   
  
  return distR = 0;
}

/////////////////////////////////////////////////////////////////////////
//
// Calculate distance to surface of shape from `inside',
// allowing for tolerance
//
G4double G4EllipticalCone::DistanceToOut(const G4ThreeVector& p,
                                         const G4ThreeVector& v,
                                         const G4bool calcNorm,
                                               G4bool *validNorm,
                                               G4ThreeVector *n  ) const
{
  G4double distMin, lambda;
  enum surface_e {kPlaneSurf, kCurvedSurf, kNoSurf} surface;
  
  distMin = kInfinity;
  surface = kNoSurf;

  if (v.z() < 0.0)
  {
    lambda = (-p.z() - zTopCut)/v.z();
    if ( (sqr((p.x() + lambda*v.x())/xSemiAxis) + 
          sqr((p.y() + lambda*v.y())/ySemiAxis)) < 
          sqr(zheight + zTopCut + 0.5*kCarTolerance) )
    {
      distMin = std::fabs(lambda);
      if (!calcNorm) { return distMin; }
    } 
    distMin = std::fabs(lambda);
    surface = kPlaneSurf;
  }

  if (v.z() > 0.0)
  {
    lambda = (zTopCut - p.z()) / v.z();
    if ( (sqr((p.x() + lambda*v.x())/xSemiAxis)
        + sqr((p.y() + lambda*v.y())/ySemiAxis) )
       < (sqr(zheight - zTopCut + 0.5*kCarTolerance)) )
    {
      distMin = std::fabs(lambda);
      if (!calcNorm) { return distMin; }
    }
    distMin = std::fabs(lambda);
    surface = kPlaneSurf;
  }
  
  // if we are here then it either intersects or grazes the 
  // curved surface...
  //
  G4double A = sqr(v.x()/xSemiAxis) + sqr(v.y()/ySemiAxis) - sqr(v.z());
  G4double B = 2.*(v.x()*p.x()/sqr(xSemiAxis) +  
                   v.y()*p.y()/sqr(ySemiAxis) + v.z()*(zheight-p.z()));
  G4double C = sqr(p.x()/xSemiAxis) + sqr(p.y()/ySemiAxis)
             - sqr(zheight - p.z());
 
  G4double discr = B*B - 4.*A*C;
  
  if ( discr >= - 0.5*kCarTolerance && discr < 0.5*kCarTolerance )
  { 
    if(!calcNorm) { return distMin = std::fabs(-B/(2.*A)); }
  }

  else if ( discr > 0.5*kCarTolerance )
  {
    G4double plus  = (-B+std::sqrt(discr))/(2.*A);
    G4double minus = (-B-std::sqrt(discr))/(2.*A);

    lambda   = std::fabs(plus) < std::fabs(minus) ? plus:minus;
    distMin  = std::fabs(lambda);
    surface  = kCurvedSurf;
  }

  // set normal if requested
  //
  if (calcNorm)
  {
    if (surface == kNoSurf)
    {
      *validNorm = false;
    }
    else
    {
      *validNorm = true;
      switch (surface)
      {
        case kPlaneSurf:
        {
          *n = G4ThreeVector(0.,0.,(v.z() > 0.0 ? 1. : -1.));
        }
        break;

        case kCurvedSurf:
        {
          G4ThreeVector pexit = p + distMin*v;
          G4ThreeVector truenorm(pexit.x()/(xSemiAxis*xSemiAxis),
                                 pexit.y()/(ySemiAxis*ySemiAxis),
                                 pexit.z() - zheight);
          truenorm /= truenorm.mag();
          *n= truenorm;
        } 
        break;

        default:
          G4cout.precision(16);
          G4cout << G4endl;
          DumpInfo();
          G4cout << "Position:"  << G4endl << G4endl;
          G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl;
          G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl;
          G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl;
          G4cout << "Direction:" << G4endl << G4endl;
          G4cout << "v.x() = "   << v.x() << G4endl;
          G4cout << "v.y() = "   << v.y() << G4endl;
          G4cout << "v.z() = "   << v.z() << G4endl << G4endl;
          G4cout << "Proposed distance :" << G4endl << G4endl;
          G4cout << "distMin = "    << distMin/mm << " mm" << G4endl << G4endl;
          G4Exception("G4EllipticalCone::DistanceToOut(p,v,..)",
                      "Notification", JustWarning,
                      "Undefined side for valid surface normal to solid.");
          break;
      }
    }
  }
  return distMin;
}

/////////////////////////////////////////////////////////////////////////
//
// Calculate distance (<=actual) to closest surface of shape from inside
//
G4double G4EllipticalCone::DistanceToOut(const G4ThreeVector& p) const
{
  G4double rad, distR, distZ, distMin=0.;
  G4double minAxis = xSemiAxis < ySemiAxis ? xSemiAxis : ySemiAxis;

#ifdef G4SPECSDEBUG
  if( Inside(p) == kOutside )
  {
     G4cout.precision(16) ;
     G4cout << G4endl ;
     DumpInfo();
     G4cout << "Position:"  << G4endl << G4endl ;
     G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl ;
     G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl ;
     G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl ;
     G4Exception("G4Ellipsoid::DistanceToOut(p)", "Notification", JustWarning, 
                 "Point p is outside !?" );
  }
#endif
    
  // since we have made the above warning, below we are working assuming p
  // is inside check how close it is to the circular cone with radius equal
  // to the smaller of the axes
  //
  if( sqr(p.x()/minAxis)+sqr(p.y()/minAxis) < sqr(zheight - p.z()) )
  {
    rad     = std::sqrt(sqr(p.x()) + sqr(p.y()));
    distZ   = p.z() + (distMin*(rad-distMin*zheight)-p.z())/(1+sqr(distMin));
    distR   = rad-(distMin*(zheight-distZ ));
    distMin = std::sqrt(sqr(distR) + sqr(distZ));
    distMin = distMin < std::fabs(p.z() + zTopCut)
            ? distMin : std::fabs(p.z() + zTopCut);
    distMin = distMin < std::fabs(zTopCut - p.z())
            ? distMin : std::fabs(zTopCut - p.z());
  }
  
  return distMin;
}

//////////////////////////////////////////////////////////////////////////
//
// GetEntityType
//
G4GeometryType G4EllipticalCone::GetEntityType() const
{
  return G4String("G4EllipticalCone");
}

//////////////////////////////////////////////////////////////////////////
//
// Stream object contents to an output stream
//
std::ostream& G4EllipticalCone::StreamInfo( std::ostream& os ) const
{
  os << "-----------------------------------------------------------\n"
     << "    *** Dump for solid - " << GetName() << " ***\n"
     << "    ===================================================\n"
     << " Solid type: G4EllipticalCone\n"
     << " Parameters: \n"

     << "    semi-axis x: " << xSemiAxis/mm << " mm \n"
     << "    semi-axis y: " << ySemiAxis/mm << " mm \n"
     << "    height    z: " << zheight/mm << " mm \n"
     << "    half length in  z: " << zTopCut/mm << " mm \n"
     << "-----------------------------------------------------------\n";

  return os;
}

/////////////////////////////////////////////////////////////////////////
//
// GetPointOnSurface
//
// returns quasi-uniformly distributed point on surface of elliptical cone
//
G4ThreeVector G4EllipticalCone::GetPointOnSurface() const
{

  G4double phi, sinphi, cosphi, aOne, aTwo, aThree,
           chose, zRand, rRand1, rRand2;
  
  G4double rOne = std::sqrt(sqr(xSemiAxis)
                + sqr(ySemiAxis))*(zheight - zTopCut);
  G4double rTwo = std::sqrt(sqr(xSemiAxis)
                + sqr(ySemiAxis))*(zheight + zTopCut);
  
  aOne   = pi*(rOne + rTwo)*std::sqrt(sqr(rOne - rTwo)+sqr(2.*zTopCut));
  aTwo   = pi*xSemiAxis*ySemiAxis*sqr(zheight+zTopCut);
  aThree = pi*xSemiAxis*ySemiAxis*sqr(zheight-zTopCut);  

  phi = RandFlat::shoot(0.,twopi);
  cosphi = std::cos(phi);
  sinphi = std::sin(phi);
  
  if(zTopCut >= zheight) aThree = 0.;

  chose = RandFlat::shoot(0.,aOne+aTwo+aThree);
  if((chose>=0.) && (chose<aOne))
  {
    zRand = RandFlat::shoot(-zTopCut,zTopCut);
    return G4ThreeVector(xSemiAxis*(zheight-zRand)*cosphi,
                         ySemiAxis*(zheight-zRand)*sinphi,zRand);    
  }
  else if((chose>=aOne) && (chose<aOne+aTwo))
  {
    rRand1 = RandFlat::shoot(0.,1.);
    rRand2 = RandFlat::shoot(0.,std::sqrt(1.-sqr(rRand1)));
    return G4ThreeVector(rRand1*xSemiAxis*(zheight+zTopCut),
                         rRand2*ySemiAxis*(zheight+zTopCut), -zTopCut);
  }
  // else
  //
  rRand1 = RandFlat::shoot(0.,1.);
  rRand2 = RandFlat::shoot(0.,std::sqrt(1.-sqr(rRand1)));
  return G4ThreeVector(rRand1*xSemiAxis*(zheight-zTopCut),
                       rRand2*ySemiAxis*(zheight-zTopCut), zTopCut);
}

//
// Methods for visualisation
//

void G4EllipticalCone::DescribeYourselfTo (G4VGraphicsScene& scene) const
{
  scene.AddSolid(*this);
}

G4VisExtent G4EllipticalCone::GetExtent() const
{
  // Define the sides of the box into which the solid instance would fit.
  //
  G4double maxDim;
  maxDim = xSemiAxis > ySemiAxis ? xSemiAxis : ySemiAxis;
  maxDim = maxDim > zTopCut ? maxDim : zTopCut;
  
  return G4VisExtent (-maxDim, maxDim,
                      -maxDim, maxDim,
                      -maxDim, maxDim);
}

G4NURBS* G4EllipticalCone::CreateNURBS () const
{
  // Box for now!!!
  //
  return new G4NURBSbox(xSemiAxis, ySemiAxis,zheight);
}

G4Polyhedron* G4EllipticalCone::CreatePolyhedron () const
{
  return new G4PolyhedronEllipticalCone(xSemiAxis, ySemiAxis, zheight, zTopCut);
}

G4Polyhedron* G4EllipticalCone::GetPolyhedron () const
{
  if ( (!fpPolyhedron)
    || (fpPolyhedron->GetNumberOfRotationStepsAtTimeOfCreation() !=
        fpPolyhedron->GetNumberOfRotationSteps()) )
    {
      delete fpPolyhedron;
      fpPolyhedron = CreatePolyhedron();
    }
  return fpPolyhedron;
}
