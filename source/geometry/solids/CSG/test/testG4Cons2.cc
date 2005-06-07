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
// $Id: testG4Cons2.cc,v 1.16 2005-06-07 08:00:43 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Simple test of G4Cons
// Basic checks on each function + awkward cases for tracking / geom algorithms

#include <assert.h>
#include <cmath>
#include "G4ios.hh"

#include "globals.hh"
#include "geomdefs.hh"

#include "ApproxEqual.hh"

#include "G4ThreeVector.hh"
#include "G4Cons.hh"
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"
#include "G4VoxelLimits.hh"

#define	DELTA 0.0001

// Returns false if actual is within wanted+/- DELTA
//         true if error
G4bool OutRange(G4double actual,G4double wanted)
{
    G4bool rng = false ;
    if (actual < wanted-DELTA || actual > wanted + DELTA ) rng = true ;
    return rng ;
}
G4bool OutRange(G4ThreeVector actual,G4ThreeVector wanted)
{
    G4bool rng = false ;
    if (OutRange(actual.x(),wanted.x())
	||OutRange(actual.y(),wanted.y())
	||OutRange(actual.z(),wanted.z())  ) rng = true ;
    return rng ;
}
///////////////////////////////////////////////////////////////////
//
// Dave's auxiliary function

const G4String OutputInside(const EInside a)
{
        switch(a) 
        {
                case kInside:  return "Inside"; 
                case kOutside: return "Outside";
                case kSurface: return "Surface";
        }
        return "????";
}

int main(void)
{
	G4double dist, vol, volCheck;

	G4ThreeVector   pzero(0,0,0);
	
	G4ThreeVector   pplx(120,0,0),pply(0,120,0),pplz(0,0,120);
	
	G4ThreeVector   pmix(-120,0,0),pmiy(0,-120,0),pmiz(0,0,-120);
	
	G4ThreeVector   ponmiz(0,75,-50),ponplz(0,75,50);
	
	G4ThreeVector	ponr1(std::sqrt(50*50/2.0),std::sqrt(50*50/2.0),0),
	                ponr2(std::sqrt(100*100/2.0),std::sqrt(100*100/2.0),0),
	        	ponphi1(60*std::cos(pi/6),-60*std::sin(pi/6),0),
		        ponphi2(60*std::cos(pi/6),60*std::sin(pi/6),0),
	                ponr2b(150,0,0);
	
	G4ThreeVector pnearplz(45,45,45),pnearmiz(45,45,-45);
	G4ThreeVector pydx(60,150,0),pbigx(500,0,0);

	G4ThreeVector proot1(0,125,-1000),proot2(0,75,-1000);
	
	G4ThreeVector pparr1(0,25,-150);   // Test case parallel to both rs of c8
	G4ThreeVector pparr2(0,75,-50),pparr3(0,125,50);
	G4ThreeVector vparr(0,1./std::sqrt(5.),2./std::sqrt(5.)); 

	G4ThreeVector vnphi1(-std::sin(pi/6),-std::cos(pi/6),0),
	              vnphi2(-std::sin(pi/6),std::cos(pi/6),0);

  G4ThreeVector vx(1,0,0),vy(0,1,0),vz(0,0,1),
	        vmx(-1,0,0),vmy(0,-1,0),vmz(0,0,-1),
		vxy(1./std::sqrt(2.),1./std::sqrt(2.),0),
	        vxmy(1./std::sqrt(2.),-1./std::sqrt(2.),0),
	        vmxmy(-1./std::sqrt(2.),-1./std::sqrt(2.),0),
	        vmxy(-1./std::sqrt(2.),1./std::sqrt(2.),0),
                vx2mz(1.0/std::sqrt(5.0),0,-2.0/std::sqrt(5.0)),
	        vxmz(1./std::sqrt(2.),0,-1./std::sqrt(2.));
	
	G4RotationMatrix r90X,r90Y,r90Z,r180X,r45X,r30Y;
	
  G4Cons c1("Hollow Full Tube",50,100,50,100,50,0,twopi),
         cn1("cn1",45.,50.,45.,50.,50,halfpi,halfpi),
         cn2("cn1",45.,50.,45.,50.,50,halfpi,3*halfpi),
	 c2("Hollow Full Cone",50,100,50,200,50,-1,twopi),
	 c3("Hollow Cut Tube",50,100,50,100,50,-pi/6,pi/3),
	 c4("Hollow Cut Cone",50,100,50,200,50,-pi/6,pi/3),
	 c5("Hollow Cut Cone",25,50,75,150,50,0,3*halfpi),
 	 c6("Solid Full Tube",0,150,0,150,50,0,twopi),
	 c7("Thin Tube",95,100,95,100,50,0,twopi),
	 c8a("Solid Full Cone2",0,100,0,150,50,0,twopi),
	 c8b("Hollow Full Cone2",50,100,100,150,50,0,twopi),
	 c8c("Hollow Full Cone2inv",100,150,50,100,50,0,twopi),
	 c9("Excotic Cone",50,60,
	    0,           // 1.0e-7,   500*kRadTolerance,
                           10,50,0,twopi), 
	 cms("cms cone",0.0,70.0,0.0,157.8,2949.0,0.0,6.2831853071796);

  G4Cons cms2("RearAirCone",401.0,1450.0,
                            1020.0,1450.0,175.0,0.0,6.2831853071796) ;



  G4Cons   ctest10( "aCone", 2*cm, 6*cm, 8*cm, 14*cm,
                           10*cm, 10*deg, 300*deg ); 

  G4ThreeVector pct10(60,0,0);
  G4ThreeVector pct10mx(-50,0,0);
  G4ThreeVector pct10phi1(60*std::cos(10.*degree),60*std::sin(10*degree),0);
  G4ThreeVector pct10phi2(60*std::cos(50.*degree),-60*std::sin(50*degree),0);

  G4ThreeVector pct10e1(-691-500,174,      404 );

  G4ThreeVector pct10e2( 400-500, 20.9,     5.89 );

  G4ThreeVector pct10e3( 456-500, 13,    -14.7 );

  G4ThreeVector pct10e4( 537-500, 1.67,    -44.1 );
  // point P is outside
  G4ThreeVector pct10e5(537, 1.67,    -44.1);

  G4ThreeVector pct10e6(1e+03, -63.5,     -213 );

  G4double a1,a2,a3,am;

  a1=pct10e2.x()-pct10e1.x();
  a2=pct10e2.y()-pct10e1.y();
  a3=pct10e2.z()-pct10e1.z();
  am=std::sqrt(a1*a1+a2*a2+a3*a3);
  G4ThreeVector  d1(a1/am,a2/am,a3/am);
  G4cout<<d1.x()<<"\t"<<d1.y()<<"\t"<<d1.z()<<G4endl;

  a1=pct10e3.x()-pct10e2.x();
  a2=pct10e3.y()-pct10e2.y();
  a3=pct10e3.z()-pct10e2.z();
  am=std::sqrt(a1*a1+a2*a2+a3*a3);
  G4ThreeVector  d2(a1/am,a2/am,a3/am);
  G4cout<<d2.x()<<"\t"<<d2.y()<<"\t"<<d2.z()<<G4endl;

  a1=pct10e4.x()-pct10e3.x();
  a2=pct10e4.y()-pct10e3.y();
  a3=pct10e4.z()-pct10e3.z();
  am=std::sqrt(a1*a1+a2*a2+a3*a3);
  G4ThreeVector  d3(a1/am,a2/am,a3/am);
  G4cout<<d3.x()<<"\t"<<d3.y()<<"\t"<<d3.z()<<G4endl;


  // 19.01.04 modified test10 info:

  G4ThreeVector  pt10s1(  6.454731216775542,
			-90.42080754048007,
                        100.                 );

  G4ThreeVector  pt10s2( 22.65282328600368,
                        -69.34877585931267, 
                         76.51600623610082 );

  G4ThreeVector  pt10s3( 51.28206938732319,
			-32.10510677306267,
                         35.00932544708616 );

  G4ThreeVector    vt10d( 0.4567090876640433 , 
                          0.5941309830320264, 
                         -0.6621368319663807 );


  G4ThreeVector norm,*pNorm;
  G4bool *pgoodNorm,goodNorm,calcNorm=true;
	
  pNorm=&norm;
  pgoodNorm=&goodNorm;

  r90X.rotateX(halfpi);
  r90Y.rotateY(halfpi);
  r90Z.rotateZ(halfpi);
  r45X.rotateX(pi/4);
  r30Y.rotateY(pi/6);

  //	G4cout << "G4Cons:"<< c4.GetName()
  //   << " ID=" << c4.GetIdentifier() << "\n";

  // check cubic volume

  vol = c1.GetCubicVolume();
  volCheck = 2*pi*50*(100*100-50*50);
  assert(ApproxEqual(vol,volCheck));

  vol = c6.GetCubicVolume();
  volCheck = 2*pi*50*(150*150);
  assert(ApproxEqual(vol,volCheck));

  EInside in;
  G4cout.precision(16) ;
  G4cout << "Testing G4Cons::Inside...\n";

  in = ctest10.Inside(pct10e1);
  G4cout << "ctest10.Inside(pct10e1) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10e2);
  G4cout << "ctest10.Inside(pct10e2) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10e3);
  G4cout << "ctest10.Inside(pct10e3) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10e4);
  G4cout << "ctest10.Inside(pct10e4) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10e5);
  G4cout << "ctest10.Inside(pct10e5) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10e6);
  G4cout << "ctest10.Inside(pct10e6) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pct10mx);
  G4cout << "ctest10.Inside(pct10mx) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pt10s1);
  G4cout << "ctest10.Inside(pt10s1) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pt10s2);
  G4cout << "ctest10.Inside(pt10s2) = " <<OutputInside(in)<< G4endl;

  in = ctest10.Inside(pt10s3);
  G4cout << "ctest10.Inside(pt10s3) = " <<OutputInside(in)<< G4endl;

	if (c1.Inside(pzero)!=kOutside)
		G4cout << "Error A" << G4endl;
	if (c6.Inside(pzero)!=kInside)
		G4cout << "Error A2" << G4endl;
	if (c1.Inside(pplx)!=kOutside)
	    G4cout << "Error B1" << G4endl;
	if (c2.Inside(pplx)!=kInside)
	    G4cout << "Error B2" << G4endl;
	if (c3.Inside(pplx)!=kOutside)
	    G4cout << "Error B3" << G4endl;
	if (c4.Inside(pplx)!=kInside)
	    G4cout << "Error B4" << G4endl;
	if (c1.Inside(ponmiz)!=kSurface)
	    G4cout << "Error C" << G4endl;
	if (c1.Inside(ponplz)!=kSurface)
	    G4cout << "Error D" << G4endl;
	if (c1.Inside(ponr1)!=kSurface)
	    G4cout << "Error E" << G4endl;
	if (c1.Inside(ponr2)!=kSurface)
	    G4cout << "Error F" << G4endl;
	if (c3.Inside(ponphi1)!=kSurface)
	    G4cout << "Error G" << G4endl;
	if (c3.Inside(ponphi2)!=kSurface)
	    G4cout << "Error H" << G4endl;

	if (c5.Inside(G4ThreeVector(70,1,0))!=kInside)
	    G4cout << "Error I" << G4endl;
	if (c5.Inside(G4ThreeVector(50,-50,0))!=kOutside)
	    G4cout << "Error I2" << G4endl;
	if (c5.Inside(G4ThreeVector(70,0,0))!=kSurface)
	    G4cout << "Error I3" << G4endl;
// on tolerant r, inside z, within phi
	if (c5.Inside(G4ThreeVector(100,0,0))!=kSurface)
	    G4cout << "Error I4" << G4endl;
	if (c3.Inside(G4ThreeVector(100,0,0))!=kSurface)
	    G4cout << "Error I5" << G4endl;
// on tolerant r, tolerant z, within phi
	if (c5.Inside(G4ThreeVector(100,0,50))!=kSurface)
	    G4cout << "Error I4" << G4endl;
	if (c3.Inside(G4ThreeVector(100,0,50))!=kSurface)
	    G4cout << "Error I5" << G4endl;
	

	G4cout << "Testing G4Cons::SurfaceNormal...\n";

    G4ThreeVector normal;
    G4double p2=1./std::sqrt(2.),p3=1./std::sqrt(3.);

    normal=cn1.SurfaceNormal(G4ThreeVector(0.,50.,0.));
    assert(ApproxEqual(normal,G4ThreeVector(p2,p2,0.)));
    normal=cn1.SurfaceNormal(G4ThreeVector(0.,45.,0.));
    assert(ApproxEqual(normal,G4ThreeVector(p2,-p2,0.)));
    normal=cn1.SurfaceNormal(G4ThreeVector(0.,45.,50.));
    assert(ApproxEqual(normal,G4ThreeVector(p3,-p3,p3)));
    normal=cn1.SurfaceNormal(G4ThreeVector(0.,45.,-50.));
    assert(ApproxEqual(normal,G4ThreeVector(p3,-p3,-p3)));
    normal=cn1.SurfaceNormal(G4ThreeVector(-50.,0.,-50.));
    assert(ApproxEqual(normal,G4ThreeVector(-p3,-p3,-p3)));
    normal=cn1.SurfaceNormal(G4ThreeVector(-50.,0.,0.));
    assert(ApproxEqual(normal,G4ThreeVector(-p2,-p2,0.)));
    normal=cn2.SurfaceNormal(G4ThreeVector(50.,0.,0.));
    assert(ApproxEqual(normal,G4ThreeVector(p2,p2,0.)));
    normal=c6.SurfaceNormal(G4ThreeVector(0.,0.,50.));
    assert(ApproxEqual(normal,G4ThreeVector(0.,0.,1.)));





	norm=c1.SurfaceNormal(ponplz);
	if (OutRange(norm,G4ThreeVector(0,0,1)))
	    G4cout << "Error A " << norm << G4endl;
	norm=c1.SurfaceNormal(ponmiz);
	if (OutRange(norm,G4ThreeVector(0,0,-1)))
	    G4cout << "Error B " << norm << G4endl;
	norm=c1.SurfaceNormal(ponr1);
	if (OutRange(norm,G4ThreeVector(-1.0/std::sqrt(2.0),-1.0/std::sqrt(2.0),0)))
	    G4cout << "Error C " << norm << G4endl;
	norm=c1.SurfaceNormal(ponr2);
	if (OutRange(norm,G4ThreeVector(1.0/std::sqrt(2.0),1.0/std::sqrt(2.0),0)))
	    G4cout << "Error D " << norm << G4endl;
	norm=c3.SurfaceNormal(ponphi1);
	if (OutRange(norm,vnphi1))
	    G4cout << "Error E " << norm << G4endl;
	norm=c3.SurfaceNormal(ponphi2);
	if (OutRange(norm,vnphi2))
	    G4cout << "Error F " << norm << G4endl;
	norm=c4.SurfaceNormal(ponr2b);
	if (OutRange(norm,vxmz))
	    G4cout << "Error G " << norm << G4endl;

	norm=c5.SurfaceNormal(G4ThreeVector(51,0,-50));
	if (OutRange(norm,G4ThreeVector(0.,-p2,-p2)))
	    G4cout << "Errot H " << norm << G4endl;

  G4cout << "Testing G4Cons::DistanceToOut...\n";


	dist=c4.DistanceToOut(ponphi1);
	if (OutRange(dist,0))
		G4cout << "Error A " << dist << G4endl;

	dist=c1.DistanceToOut(ponphi1);
	if (OutRange(dist,10))
		G4cout << "Error B " << dist << G4endl;

	dist=c1.DistanceToOut(pnearplz);
	if (OutRange(dist,5))
		G4cout << "Error C " << dist << G4endl;
	dist=c1.DistanceToOut(pnearmiz);
	if (OutRange(dist,5))
		G4cout << "Error D " << dist << G4endl;

	dist=c1.DistanceToOut(ponr1);
	if (OutRange(dist,0))
	    G4cout << "Error E " << dist << G4endl;
	dist=c1.DistanceToOut(ponr2);
	if (OutRange(dist,0))
	    G4cout << "Error F " << dist << G4endl;

	dist=c6.DistanceToOut(pzero);
	if (OutRange(dist,50))
	    G4cout << "Error G " << dist << G4endl;

	dist=c5.DistanceToOut(G4ThreeVector(0,-70,0));
	if (OutRange(dist,0))
	    G4cout << "Error H " << dist << G4endl;
	
       	G4cout << "Testing G4Cons::DistanceToOut...\n";
	dist=c4.DistanceToOut(pplx,vx,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,30)||OutRange(*pNorm,vxmz)||!*pgoodNorm)
	    G4cout << "Error Rmax1 " << dist << G4endl;

	dist=c2.DistanceToOut(pplx,vx,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,30)||OutRange(*pNorm,vxmz)||!*pgoodNorm)
	    G4cout << "Error Rmax2 " << dist << G4endl;

	dist=c4.DistanceToOut(pplx,vmx,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,70)||*pgoodNorm)
	    G4cout << "Error Rmin1 " << dist << G4endl;


	dist=c2.DistanceToOut(pplx,vmx,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,70)||*pgoodNorm)
	    G4cout << "Error Rmin2 " << dist << G4endl;

	dist=c3.DistanceToOut(ponphi1,vmy,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    OutRange(*pNorm,vnphi1)||
	    !*pgoodNorm)
	    G4cout << "Error PhiS 1" << dist << G4endl;
	dist=c3.DistanceToOut(ponphi1,vy,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,2*60*std::sin(pi/6))||
	    OutRange(*pNorm,vnphi2)||
	    !*pgoodNorm)
	    G4cout << "Error PhiS 2" << dist << G4endl;

	dist=c3.DistanceToOut(ponphi2,vy,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    OutRange(*pNorm,vnphi2)||
	    !*pgoodNorm)
	    G4cout << "Error PhiE 1" << dist << G4endl;
	dist=c3.DistanceToOut(ponphi2,vmy,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,2*60*std::sin(pi/6))||
	    OutRange(*pNorm,vnphi1)||
	    !*pgoodNorm)
	    G4cout << "Error PhiS 2" << dist << G4endl;


	dist=c6.DistanceToOut(ponplz,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100)||
	    OutRange(*pNorm,vmz)||
	    !*pgoodNorm)
	    G4cout << "Error Top Z 1" << dist << G4endl;
	dist=c6.DistanceToOut(ponplz,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    OutRange(*pNorm,vz)||
	    !*pgoodNorm)
	    G4cout << "Error Top Z 2" << dist << G4endl;

	dist=c6.DistanceToOut(ponmiz,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100)||
	    OutRange(*pNorm,vz)||
	    !*pgoodNorm)
	    G4cout << "Error Lower Z 1" << dist << G4endl;
	dist=c6.DistanceToOut(ponmiz,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    OutRange(*pNorm,vmz)||
	    !*pgoodNorm)
	    G4cout << "Error Lower Z 2" << dist << G4endl;

// Test case for rmax root bug
	dist=c7.DistanceToOut(ponr2,vmx,calcNorm,pgoodNorm,pNorm);
	if (OutRange(dist,100/std::sqrt(2.)-std::sqrt(95*95-100*100/2.))||*pgoodNorm)
	    G4cout << "Error rmax root bug" << dist << G4endl;

// Parallel radii test cases
	dist=c8a.DistanceToOut(pparr2,vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100.*std::sqrt(5.)/2.)||
                     !*pgoodNorm||
                     OutRange(*pNorm,vz))
	    G4cout << "Error solid parr2a " <<dist << G4endl;
	dist=c8a.DistanceToOut(pparr2,-vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error solid parr2b " <<dist << G4endl;

	dist=c8a.DistanceToOut(pparr2,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vz))
	    G4cout << "Error solid parr2c " <<dist << G4endl;
	dist=c8a.DistanceToOut(pparr2,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error solid parr2d " <<dist << G4endl;

	dist=c8a.DistanceToOut(pparr3,vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vz))
	    G4cout << "Error solid parr3a " <<dist << G4endl;
	dist=c8a.DistanceToOut(pparr3,-vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100*std::sqrt(5.)/2.)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error solid parr3b " <<dist << G4endl;
	dist=c8a.DistanceToOut(pparr3,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vz))
	    G4cout << "Error solid parr3c " <<dist << G4endl;

	dist=c8a.DistanceToOut(pparr3,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,50)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,2./std::sqrt(5.0),-1./std::sqrt(5.0))))
	    G4cout << "Error solid parr3d " <<dist << G4endl;


	dist=c8b.DistanceToOut(pparr2,vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100*std::sqrt(5.)/2.)||
                     !*pgoodNorm||
                     OutRange(*pNorm,vz))
	    G4cout << "Error hollow parr2a " <<dist << G4endl;
	dist=c8b.DistanceToOut(pparr2,-vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error hollow parr2b " <<dist << G4endl;

	dist=c8b.DistanceToOut(pparr2,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,50)||*pgoodNorm)
	    G4cout << "Error hollow parr2c " <<dist << G4endl;
	dist=c8b.DistanceToOut(pparr2,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error hollow parr2d " <<dist << G4endl;


	dist=c8b.DistanceToOut(pparr3,vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vz))
	    G4cout << "Error hollow parr3a " <<dist << G4endl;
	dist=c8b.DistanceToOut(pparr3,-vparr,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,100.*std::sqrt(5.)/2.)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vmz))
	    G4cout << "Error hollow parr3b " <<dist << G4endl;
	dist=c8b.DistanceToOut(pparr3,vz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,0)||
	    !*pgoodNorm||
	    OutRange(*pNorm,vz))
	    G4cout << "Error hollow parr3c " <<dist << G4endl;

	dist=c8b.DistanceToOut(pparr3,vmz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,50)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,2./std::sqrt(5.),-1.0/std::sqrt(5.))))
	    G4cout << "Error hollow parr3d " <<dist << G4endl;

	dist=c9.DistanceToOut(G4ThreeVector(1e3*kRadTolerance,0,50),
                              vx2mz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,111.8033988)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,0,-1.0)))
G4cout<<"Error:c9.Out((1e3*kRadTolerance,0,50),vx2mz,...) = " <<dist << G4endl;

	dist=c9.DistanceToOut(G4ThreeVector(5,0,50),
                              vx2mz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,111.8033988)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,0,-1.0)))
	    G4cout << "Error:c9.Out((5,0,50),vx2mz,...) = " <<dist << G4endl;

	dist=c9.DistanceToOut(G4ThreeVector(10,0,50),
                              vx2mz,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,111.8033988)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,0,-1.0)))
	    G4cout << "Error:c9.Out((10,0,50),vx2mz,...) = " <<dist << G4endl;

	dist=cms.DistanceToOut(
        G4ThreeVector(0.28628920024909,-0.43438111004815,-2949.0),
        G4ThreeVector(6.0886686196674e-05,-9.2382200635766e-05,0.99999999387917),
        calcNorm,pgoodNorm,pNorm);
	if (OutRange(dist,5898.0))
	G4cout << "Error:cms.DistToOut() =  " <<dist << G4endl;

	dist=cms.DistanceToOut(
        G4ThreeVector(0.28628920024909,-0.43438111004815,
                     -2949.0 + kCarTolerance*0.25),
        G4ThreeVector(6.0886686196674e-05,-9.2382200635766e-05,0.99999999387917),
        calcNorm,pgoodNorm,pNorm);
	if (OutRange(dist,5898.0))
	G4cout << "Error:cms.DistToOut(+) =  " <<dist << G4endl;

	dist=cms.DistanceToOut(G4ThreeVector(0.28628920024909,
                                            -0.43438111004815,
                                            -2949.0 - kCarTolerance*0.25),
                               G4ThreeVector(6.0886686196674e-05,
                                            -9.2382200635766e-05,
                                             0.99999999387917),
        calcNorm,pgoodNorm,pNorm);
	if (OutRange(dist,5898.0))
	G4cout << "Error:cms.DistToOut(-) =  " <<dist << G4endl;

	dist=cms2.DistanceToOut(G4ThreeVector(-344.13684353113,
		                               258.98049377272,
                                              -158.20772167926),
                                G4ThreeVector(-0.30372024336672,
                                              -0.5581146924652,
                                               0.77218003329776),
                                calcNorm,pgoodNorm,pNorm);
	if (OutRange(dist,0.))
 G4cout<<"cms2.DistanceToOut(G4ThreeVector(-344.13684 ... = "<<dist<<G4endl;

	dist=ctest10.DistanceToOut(pct10e2,
                              d1,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,111.8033988)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,0,-1.0)))
       G4cout << "ctest10.DistanceToOut(pct10e2,d1,...) = " <<dist << G4endl;
	dist=ctest10.DistanceToOut(pct10e3,
                              d1,calcNorm,pgoodNorm,pNorm);
	*pNorm=pNorm->unit();
	if (OutRange(dist,111.8033988)||
	    !*pgoodNorm||
	    OutRange(*pNorm,G4ThreeVector(0,0,-1.0)))
       G4cout << "ctest10.DistanceToOut(pct10e3,d1,...) = " <<dist << G4endl;

	/////////////////////////////////////////////
	//

	G4cout << "Testing G4Cons::DistanceToIn(p) ...\n";


	dist=c1.DistanceToIn(pzero);
	if (OutRange(dist,50))
	  G4cout << "Error A " << dist << G4endl;

	dist=c1.DistanceToIn(pplx);
	if (OutRange(dist,20))
	  G4cout << "Error B " << dist << G4endl;

	dist=c1.DistanceToIn(pply);
	if (OutRange(dist,20))
	  G4cout << "Error C " << dist << G4endl;

	dist=c4.DistanceToIn(pply);
	if (OutRange(dist,120*std::sin(pi/3)))
	  G4cout << "Error D " << dist << G4endl;

	dist=c4.DistanceToIn(pmiy);
	if (OutRange(dist,120*std::sin(pi/3)))
	  G4cout << "Error D " << dist << G4endl;

	dist=c1.DistanceToIn(pplz);
	if (OutRange(dist,70))
	    G4cout << "Error E " << dist << G4endl;
// Check with both rmins=0
	dist=c5.DistanceToIn(pplx);
	if (OutRange(dist,20./std::sqrt(2.)))
	  G4cout << "Error F " << dist << G4endl;

	/////////////////////////////////////////////////////
	//

	G4cout << "Testing G4Cons::DistanceToIn(p,v,...) ...\n";

	dist=c1.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	  G4cout << "Error A " << dist << G4endl;

	dist=c2.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c2.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c3.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c3.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c4.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c4.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c5.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c5.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c6.DistanceToIn(pplz,vmz);
	if (OutRange(dist,70.0))
	G4cout << "Error:c6.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c7.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c7.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c8a.DistanceToIn(pplz,vmz);
	if (OutRange(dist,70.0))
	G4cout << "Error:c8a.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c8b.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c8b.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c8c.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c8c.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c9.DistanceToIn(pplz,vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c9.DistanceToIn(pplz,vmz) = " << dist << G4endl;

	dist=c9.DistanceToIn(G4ThreeVector(0,0,50),vmz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c9.DistanceToIn((0,0,50),vmz) = " << dist << G4endl;

	///////////////

	dist=c1.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error A " << dist << G4endl;

	dist=c2.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c2.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c3.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c3.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c4.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c4.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c5.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c5.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c6.DistanceToIn(pmiz,vz);
	if (OutRange(dist,70.0))
	G4cout << "Error:c6.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c7.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c7.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c8a.DistanceToIn(pmiz,vz);
	if (OutRange(dist,70.0))
	G4cout << "Error:c8a.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c8b.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c8b.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c8c.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c8c.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	dist=c9.DistanceToIn(pmiz,vz);
	if (OutRange(dist,kInfinity))
	G4cout << "Error:c9.DistanceToIn(pmiz,vz) = " << dist << G4endl;

	//////////////

	dist=c1.DistanceToIn(pplx,vmx);
	if (OutRange(dist,20))
	  G4cout << "Error B " << dist << G4endl;
	dist=c1.DistanceToIn(pplz,vx);
	if (OutRange(dist,kInfinity))
	  G4cout << "Error C " << dist << G4endl;
	dist=c4.DistanceToIn(pply,vmy);
	if (OutRange(dist,kInfinity))
	  G4cout << "Error D " << dist << G4endl;

	dist=c1.DistanceToIn(pydx,vmy);
	if (OutRange(dist,70))
	  G4cout << "Error E " << dist << G4endl;
	dist=c3.DistanceToIn(pydx,vmy);
	if (OutRange(dist,150-60*std::tan(pi/6)))
	  G4cout << "Error F " << dist << G4endl;

	dist=c1.DistanceToIn(pplx,vmx);
	if (OutRange(dist,20))
	  G4cout << "Error G " << dist << G4endl;
	dist=c1.DistanceToIn(pplx,vx);
	if (OutRange(dist,kInfinity))
	  G4cout << "Error G2 " << dist << G4endl;

	dist=c4.DistanceToIn(pbigx,vmx);
	if (OutRange(dist,350))
	    G4cout << "Error G3 " << dist << G4endl;

	dist=c4.DistanceToIn(pzero,vx);
	if (OutRange(dist,50))
	  G4cout << "Error H " << dist << G4endl;

	dist=c1.DistanceToIn(ponr2,vx);
	if (OutRange(dist,kInfinity))
	    G4cout << "Error I" << dist << G4endl;
	dist=c1.DistanceToIn(ponr2,vmx);
	if (OutRange(dist,0))
	    G4cout << "Error I2" << dist << G4endl;
	
	dist=c1.DistanceToIn(ponr1,vx);
	if (OutRange(dist,0))
	    G4cout << "Error J" << dist << G4endl;
	dist=c1.DistanceToIn(ponr1,vmx);
	if (OutRange(dist,2.0*std::sqrt(50*50/2.)))
	    G4cout << "Error J2" << dist << G4endl;

	dist=c1.DistanceToIn(ponr2,vmxmy);
	if (OutRange(dist,0))
	    G4cout << "Error K" << dist << G4endl;

// Parallel test case -> parallel to both radii
	dist=c8b.DistanceToIn(pparr1,vparr);
	if (OutRange(dist,100*std::sqrt(5.)/2.))
	    G4cout << "Error parr1 " << dist << G4endl;
	dist=c8b.DistanceToIn(pparr2,-vparr);
	if (OutRange(dist,kInfinity))
	    G4cout << "Error parr2 " << dist << G4endl;
	dist=c8b.DistanceToIn(pparr3,vparr);
	if (OutRange(dist,kInfinity))
	    G4cout << "Error parr3a " << dist << G4endl;
	dist=c8b.DistanceToIn(pparr3,-vparr);
	if (OutRange(dist,0))
	    G4cout << "Error parr3b " << dist << G4endl;

// Check we don't Hit `shadow cone' at `-ve radius' on rmax or rmin
	dist=c8a.DistanceToIn(proot1,vz);
	if (OutRange(dist,1000))
	    G4cout << "Error shadow rmax root problem " << dist << G4endl;

	dist=c8c.DistanceToIn(proot2,vz);
	if (OutRange(dist,1000))
	    G4cout << "Error shadow rmin root problem " << dist << G4endl;

        dist = cms2.DistanceToIn(G4ThreeVector(-344.13684353113,
                                                258.98049377272,
                                               -158.20772167926),
				 G4ThreeVector(-0.30372022869765,
					       -0.55811472925794,
					       0.77218001247454)) ;
	if (OutRange(dist,kInfinity))
	G4cout<<"cms2.DistanceToIn(G4ThreeVector(-344.1 ... = "<<dist<<G4endl;

	dist=ctest10.DistanceToIn(pct10,vx);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10,vx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10,vmx);
	if (OutRange(dist,110))
	    G4cout << "ctest10.DistanceToIn(pct10,vmx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10,vy);
	if (OutRange(dist,10.57961))
	    G4cout << "ctest10.DistanceToIn(pct10,vy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10,vmy);
	if (OutRange(dist,71.5052))
	    G4cout << "ctest10.DistanceToIn(pct10,vmy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10,vz);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10,vz) = " << dist << G4endl;


	dist=ctest10.DistanceToIn(pct10phi1,vx);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi1,vmx);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vmx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi1,vy);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi1,vmy);
	if (OutRange(dist,80.83778))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vmy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi1,vz);
	if (OutRange(dist,33.3333))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vz) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi1,vmz);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10phi1,vmz) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vx);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vmx);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vmx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vy);
	if (OutRange(dist,77.78352))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vmy);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vmy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vz);
	if (OutRange(dist,33.3333))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vz) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10phi2,vmz);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10phi2,vmz) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vx);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vmx);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vmx) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vy);
	if (OutRange(dist,77.78352))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vmy);
	if (OutRange(dist,0))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vmy) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vz);
	if (OutRange(dist,33.3333))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vz) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10mx,vmz);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10mx,vmz) = " << dist << G4endl;



	dist=ctest10.DistanceToIn(pct10e1,d1);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10e1,d1) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pct10e4,d1);
	if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pct10e4,d1) = " << dist << G4endl;

	dist=ctest10.DistanceToIn(pt10s2,vt10d);
	// if (OutRange(dist,kInfinity))
	    G4cout << "ctest10.DistanceToIn(pt10s2,vt10d) = " << dist << G4endl;

	G4cout << "NOT Checking G4Cons::ScopeCar...\n";
	G4cout << "NOT Checking G4Cons::ScopePhi...\n";
	G4cout << "NOT Checking G4Cons::ScopeRad...\n";

	return 0;
}











