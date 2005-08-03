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
// $Id: G4Torus.hh,v 1.20 2005-08-03 16:00:37 danninos Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
// G4Torus
//
// Class description:
//
//   A torus or torus segment with curved sides parallel to the z-axis.
//   The torus has a specified swept radius about which it is centered,
//   and a given minimum and maximum radius. A minimum radius of 0
//   signifies a filled torus.
//   The torus segment is specified by starting and delta angles for phi,
//   with 0 being the +x axis, PI/2 the +y axis. A delta angle of 2PI
//   signifies a complete, unsegmented torus/cylindr.
//
//   Member functions:
//
//   As inherited from G4CSGSolid+
//
//     G4Torus(const G4String      &pName
//             G4double      pRmin
//             G4double      pRmax
//             G4double      pRtor
//             G4double      pSPhi
//             G4double      pDPhi )
//
//     - Construct a torus with the given name and dimensions.
//       The angles are provided is radians. pRtor >= pRmax
//
//
//   Protected:
//
//     G4ThreeVectorList*
//     CreateRotatedVertices(const G4AffineTransform& pTransform) const
//
//     - Create the List of transformed vertices in the format required
//       for G4VSolid:: ClipCrossSection and ClipBetweenSections.
//   
//   Member Data:
//
//  fRmin  Inside radius
//  fRmax  Outside radius
//  fRtor  swept radius of torus
//
//  fSPhi  The starting phi angle in radians,
//         adjusted such that fSPhi+fDPhi<=2PI, fSPhi>-2PI
//
//  fDPhi  Delta angle of the segment in radians
//
//   You could find very often in G4Torus functions values like 'pt' or
//   'it'. These are the distances from p or i G4ThreeVector points in the
//   plane (Z axis points p or i) to fRtor point in XY plane. This value is
//   similar to rho for G4Tubs and is used for definiton of the point
//   relative to fRmin and fRmax, i.e. for solution of inside/outside
//   problems
 
// History:
// 30.10.96 V.Grichine: first version of G4Torus
// 21.04.98 J.Apostolakis: added SetAllParameters() function
// 26.05.00 V.Grichine: added new SolveBiQuadratic/Cubic() developed
//                      by O.Cremonesi 
// 31.08.00 E.Medernach: added SolveNumeric functions, migrated to
//                       numeric solutions 
// --------------------------------------------------------------------

#ifndef G4Torus_HH
#define G4Torus_HH

#include "G4CSGSolid.hh"

class G4Torus : public G4CSGSolid
{

  public:  // with description

    G4Torus(const G4String &pName,
                  G4double pRmin,
                  G4double pRmax,
                  G4double pRtor,
                  G4double pSPhi,
                  G4double pDPhi);

    virtual ~G4Torus();
    
    // Accessors

    inline G4double GetRmin() const;
    inline G4double GetRmax() const;
    inline G4double GetRtor() const;
    inline G4double GetSPhi() const;
    inline G4double GetDPhi() const;
    inline G4double GetCubicVolume();

    // Methods of solid

    EInside Inside(const G4ThreeVector& p) const;
    G4bool CalculateExtent(const EAxis pAxis,
                           const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform,
                                 G4double& pmin, G4double& pmax) const;
    void ComputeDimensions(      G4VPVParameterisation* p,
                           const G4int n,
                           const G4VPhysicalVolume* pRep);
    G4ThreeVector SurfaceNormal( const G4ThreeVector& p) const;
    G4double DistanceToIn(const G4ThreeVector& p,const G4ThreeVector& v) const;
    G4double DistanceToIn(const G4ThreeVector& p) const;
    G4double DistanceToOut(const G4ThreeVector& p,const G4ThreeVector& v,
                           const G4bool calcNorm=G4bool(false),
                                 G4bool *validNorm=0,G4ThreeVector *n=0) const;
    G4double DistanceToOut(const G4ThreeVector& p) const;

    G4GeometryType GetEntityType() const;

    std::ostream& StreamInfo(std::ostream& os) const;

    G4ThreeVector GetPointOnSurface() const;

    // Visualisation functions

    void                DescribeYourselfTo (G4VGraphicsScene& scene) const;
    G4Polyhedron*       CreatePolyhedron   () const;
    G4NURBS*            CreateNURBS        () const;

  public:  // without description

    void SetAllParameters(G4double pRmin, G4double pRmax, G4double pRtor,
                          G4double pSPhi, G4double pDPhi);
 
    G4int TorusRoots(      G4double Ri,
                     const G4ThreeVector& p,
                     const G4ThreeVector& v) const ;

  protected:

    G4int SolveBiQuadratic(G4double c[], G4double s[]  ) const ;
    G4int SolveCubic(G4double c[], G4double s[]  ) const ;

    G4int SolveBiQuadraticNew(G4double c[], G4double s[]  ) const ;
    G4int SolveCubicNew(G4double c[], G4double s[], G4double& cd  ) const ;

    G4int SolveQuadratic(G4double c[], G4double s[]  ) const ;

    G4double SolveNumeric(const G4ThreeVector& p,
                          const G4ThreeVector& v,
                                G4bool IsDistanceToIn) const;

    G4double SolveNumericJT(G4double c[]) const;

    G4ThreeVectorList*
    CreateRotatedVertices(const G4AffineTransform& pTransform,
                                G4int& noPolygonVertices) const;

  protected:

    G4double fRmin,fRmax,fRtor,fSPhi,fDPhi;

    // Used by distanceToOut
    enum ESide {kNull,kRMin,kRMax,kSPhi,kEPhi};

    // used by normal
    enum ENorm {kNRMin,kNRMax,kNSPhi,kNEPhi};

  private:

    G4ThreeVector ApproxSurfaceNormal( const G4ThreeVector& p) const;
      // Algorithm for SurfaceNormal() following the original
      // specification for points not on the surface

    inline G4double TorusEquation (G4double x, G4double y, G4double z,
                                   G4double R0, G4double R1) const;
    inline G4double TorusDerivativeX (G4double x, G4double y, G4double z,
                                      G4double R0, G4double R1) const;
    inline G4double TorusDerivativeY (G4double x, G4double y, G4double z,
                                      G4double R0, G4double R1) const;
    inline G4double TorusDerivativeZ (G4double x, G4double y, G4double z,
                                      G4double R0, G4double R1) const;
    inline G4double TorusGradient(G4double dx, G4double dy, G4double dz,
                                  G4double x, G4double y, G4double z,
                                  G4double Rmax, G4double Rmin) const;

    void BVMIntersection (G4double x, G4double y, G4double z,
                          G4double dx, G4double dy, G4double dz,
                          G4double Rmax, G4double Rmin,
                          G4double *NewL, G4int *valid) const;
  
    void SortIntervals (G4double *SortL, G4double *NewL,
                        G4int *valid, G4int *NbIntersection) const;
  
    G4double DistanceToTorus (G4double x, G4double y, G4double z,
                              G4double dx, G4double dy, G4double dz,
                              G4double R0,G4double R1) const;
};


class G4TorusEquation
{
  public:

    G4TorusEquation();
    G4TorusEquation(G4double Rmax, G4double Rmin);
  
    ~G4TorusEquation();

    inline void setRadius (G4double Rmax, G4double Rmin);
    inline void setPosition (G4double x,G4double y,G4double z);
    inline void setPosition (const G4ThreeVector& p);
    inline void setDirection (G4double dirx,G4double diry,G4double dirz);
    inline void setDirection (const G4ThreeVector& v);

  public:

    inline G4double Function (G4double value);
    inline G4double Derivative(G4double value);

  private:

    inline G4double TorusEquation    (G4double x, G4double y, G4double z);
    inline G4double TorusDerivativeX (G4double x, G4double y, G4double z);
    inline G4double TorusDerivativeY (G4double x, G4double y, G4double z);
    inline G4double TorusDerivativeZ (G4double x, G4double y, G4double z);

  private:

    G4double R0;
    G4double R1;

    G4double Px,Py,Pz;
    G4double dx,dy,dz;  
};

#include "G4Torus.icc"

#endif
