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
// $Id: G4Polyhedra.hh,v 1.8 2003-06-17 08:07:20 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
//
// G4Polyhedra.hh
//
// Class description:
//
//   Class implementing a CSG-like type "PGON" Geant 3.21 volume,
//   inherited from class G4VCSGfaceted:
//
//   G4Polyhedra( const G4String& name, 
//                G4double phiStart,         - initial phi starting angle
//                G4double phiTotal,         - total phi angle
//                G4int numSide,             - number sides
//                G4int numZPlanes,          - number of z planes
//                const G4double zPlane[],   - position of z planes
//                const G4double rInner[],   - tangent distance to inner surface
//                const G4double rOuter[]  ) - tangent distance to outer surface
//
//   G4Polyhedra( const G4String& name, 
//                G4double phiStart,    - initial phi starting angle
//                G4double phiTotal,    - total phi angle
//                G4int    numSide,     - number sides
//                G4int    numRZ,       - number corners in r,z space
//                const G4double r[],   - r coordinate of these corners
//                const G4double z[] )  - z coordinate of these corners

// Author: 
//   David C. Williams (davidw@scipp.ucsc.edu)
// --------------------------------------------------------------------

#ifndef G4Polyhedra_hh
#define G4Polyhedra_hh

#include "G4VCSGfaceted.hh"
#include "G4PolyhedraSide.hh"

class G4EnclosingCylinder;
class G4ReduciblePolygon;
class G4PolyhedraHistorical
{
  public:

    G4PolyhedraHistorical();
    ~G4PolyhedraHistorical();
    G4PolyhedraHistorical( const G4PolyhedraHistorical &source );
  
    G4double Start_angle;
    G4double Opening_angle;
    G4int   numSide;
    G4int   Num_z_planes;
    G4double *Z_values;
    G4double *Rmin;
    G4double *Rmax;
};

class G4Polyhedra : public G4VCSGfaceted
{
  public:  // with description

  G4Polyhedra( const G4String& name, 
                     G4double phiStart,    // initial phi starting angle
                     G4double phiTotal,    // total phi angle
                     G4int numSide,        // number sides
                     G4int numZPlanes,     // number of z planes
               const G4double zPlane[],    // position of z planes
               const G4double rInner[],    // tangent distance to inner surface
               const G4double rOuter[]  ); // tangent distance to outer surface

  G4Polyhedra( const G4String& name, 
                     G4double phiStart,    // initial phi starting angle
                     G4double phiTotal,    // total phi angle
                     G4int    numSide,     // number sides
                     G4int    numRZ,       // number corners in r,z space
               const G4double r[],         // r coordinate of these corners
               const G4double z[]       ); // z coordinate of these corners

  virtual ~G4Polyhedra();
  
  G4Polyhedra( const G4Polyhedra &source );
  const G4Polyhedra &operator=( const G4Polyhedra &source );

  // Methods for solid

  EInside Inside( const G4ThreeVector &p ) const;
  G4double DistanceToIn( const G4ThreeVector &p,
                         const G4ThreeVector &v ) const;
  G4double DistanceToIn( const G4ThreeVector &p ) const;
  
  void ComputeDimensions(       G4VPVParameterisation* p,
                          const G4int n,
                          const G4VPhysicalVolume* pRep);

  G4GeometryType  GetEntityType() const;

  std::ostream& StreamInfo( std::ostream& os ) const;

  G4Polyhedron* CreatePolyhedron() const;
  G4NURBS*      CreateNURBS() const;

  // Accessors

  inline G4int GetNumSide()     const;
  inline G4double GetStartPhi() const;
  inline G4double GetEndPhi()   const;
  inline G4bool IsOpen()        const;
  inline G4int GetNumRZCorner() const;
  inline G4PolyhedraSideRZ GetCorner( const G4int index ) const;
  inline G4PolyhedraHistorical* GetOriginalParameters() const;
  
  protected:  // without description

  // Here are our parameters

  G4int   numSide;      // Number of sides
  G4double startPhi;    // Starting phi value (0 < phiStart < 2pi)
  G4double endPhi;      // end phi value (0 < endPhi-phiStart < 2pi)
  G4bool   phiIsOpen;   // true if there is a phi segment
  G4int   numCorner;    // number RZ points
  G4PolyhedraSideRZ *corners;  // our corners
  G4PolyhedraHistorical  *original_parameters;  // original input parameters

  // Our quick test

  G4EnclosingCylinder *enclosingCylinder;

  // Generic initializer, call by all constructors

  void Create( G4double phiStart,           // initial phi starting angle
               G4double phiTotal,           // total phi angle
               G4int    numSide,            // number sides
               G4ReduciblePolygon *rz );    // rz coordinates

  void CopyStuff( const G4Polyhedra &source );
  void DeleteStuff();
};

#include "G4Polyhedra.icc"

#endif
