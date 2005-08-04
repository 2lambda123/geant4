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
// * Vanderbilt University Free Electron Laser Center                 *
// * Vanderbilt University, Nashville, TN, USA                        *
// * Development supported by:                                        *
// * United States MFEL program  under grant FA9550-04-1-0045         *
// * and NASA under contract number NNG04CT05P.                       *
// * Written by Marcus H. Mendenhall and Robert A. Weller.            *
// *                                                                  *
// * Contributed to the Geant4 Core, January, 2005.                   *
// *                                                                  *
// ********************************************************************
//
//
// $Id: G4Tet.hh,v 1.3 2005-08-04 09:18:11 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// --------------------------------------------------------------------
// GEANT 4 class header file
//
// G4Tet
//
// Class description:
//
//   A G4Tet is a tetrahedrasolid.
//

// History:
// -------
// 03.09.2004 - M.H.Mendenhall & R.A.Weller (Vanderbilt University, USA)
// 10.02.2005 - D.Anninos (CERN) - Added GetPointOnSurface() method.
// --------------------------------------------------------------------
#ifndef G4TET_HH
#define G4TET_HH

#include "G4VSolid.hh"

class G4Tet : public G4VSolid
{

  public:  // with description

    G4Tet(const G4String& pName, 
                G4ThreeVector anchor,
                G4ThreeVector p2,
                G4ThreeVector p3,
                G4ThreeVector p4, 
                G4bool *degeneracyFlag=0);

    virtual ~G4Tet();

    void ComputeDimensions(G4VPVParameterisation* p,
                           const G4int n,
                           const G4VPhysicalVolume* pRep);

    G4bool CalculateExtent(const EAxis pAxis,
                           const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform,
                                 G4double& pmin, G4double& pmax) const;
    // Methods for solid

    EInside Inside(const G4ThreeVector& p) const;

    G4ThreeVector SurfaceNormal( const G4ThreeVector& p) const;

    G4double DistanceToIn(const G4ThreeVector& p, const G4ThreeVector& v) const;

    G4double DistanceToIn(const G4ThreeVector& p) const;

    G4double DistanceToOut(const G4ThreeVector& p, const G4ThreeVector& v,
                           const G4bool calcNorm=false,
                                 G4bool *validNorm=0, G4ThreeVector *n=0) const;

    G4double DistanceToOut(const G4ThreeVector& p) const;

    G4GeometryType GetEntityType() const;

    std::ostream& StreamInfo(std::ostream& os) const;

    G4ThreeVector GetPointOnSurface() const;

    // Functions for visualization

    void          DescribeYourselfTo (G4VGraphicsScene& scene) const;
    G4VisExtent   GetExtent          () const;
    G4Polyhedron* CreatePolyhedron   () const;
    G4NURBS*      CreateNURBS        () const;

  public:   // without description

    const char* CVSHeaderVers()
      { return "$Id: G4Tet.hh,v 1.3 2005-08-04 09:18:11 gcosmo Exp $"; }
    const char* CVSFileVers()
      { return CVSVers; }
    void PrintWarnings(G4bool flag)
      { warningFlag=flag; }
    static G4bool CheckDegeneracy(G4ThreeVector anchor,
                                  G4ThreeVector p2,
                                  G4ThreeVector p3,
                                  G4ThreeVector p4);

  protected:  // with description

    G4ThreeVectorList*
    CreateRotatedVertices(const G4AffineTransform& pTransform) const;
      // Create the List of transformed vertices in the format required
      // for G4VSolid:: ClipCrossSection and ClipBetweenSections.

  private:

    G4ThreeVector GetPointOnFace(G4ThreeVector p1, G4ThreeVector p2, 
                                 G4ThreeVector p3, G4double& area) const;
    static const char CVSVers[];

  private:

    G4ThreeVector fAnchor, fP2, fP3, fP4, fMiddle;
    G4ThreeVector fNormal123, fNormal142, fNormal134, fNormal234;

    G4bool warningFlag;

    G4double fCdotN123, fCdotN142, fCdotN134, fCdotN234;
    G4double fXMin, fXMax, fYMin, fYMax, fZMin, fZMax;
    G4double fDx, fDy, fDz, fTol, fMaxSize;
};

#endif
