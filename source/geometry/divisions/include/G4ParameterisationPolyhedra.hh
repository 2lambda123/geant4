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
// $Id: G4ParameterisationPolyhedra.hh,v 1.5 2003-11-04 17:03:03 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// classes G4ParameterisationPolyhedraRho,
//         G4ParameterisationPolyhedraPhi,
//         G4ParameterisationPolyhedraZ
//
// Class description:
//
// These classes represent the parameterised positioning equivalent to 
// dividing a G4Polyhedra along one of each axis Rho, Phi, Z.

// History:
// 09.05.01 - P.Arce First version
//---------------------------------------------------------------------
#ifndef G4ParameterisationPolyhedra_H
#define G4ParameterisationPolyhedra_H 1

#include "G4VDivisionParameterisation.hh"
#include "G4Polyhedra.hh"

class G4VPhysicalVolume;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Sphere;
class G4Orb;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;

//---------------------------------------------------------------------
// Class G4ParameterisationPolyhedraRho
//---------------------------------------------------------------------

class G4ParameterisationPolyhedraRho : public G4VDivisionParameterisation
{ 
  public:  // with description

    G4ParameterisationPolyhedraRho( EAxis axis, G4int nCopies,
                                    G4double offset, G4double step,
                                    G4VSolid* motherSolid,
                                    DivisionType divType );
    virtual ~G4ParameterisationPolyhedraRho();

    virtual void ComputeTransformation( const G4int copyNo,
                                        G4VPhysicalVolume* physVol ) const;
    void ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                            const G4VPhysicalVolume* physVol ) const;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
};

//---------------------------------------------------------------------
// Class G4ParameterisationPolyhedraPhi
//---------------------------------------------------------------------

class G4ParameterisationPolyhedraPhi : public G4VDivisionParameterisation
{ 
  public:  // with description

    G4ParameterisationPolyhedraPhi( EAxis axis, G4int nCopies,
                                   G4double offset, G4double step,
                                   G4VSolid* motherSolid,
                                   DivisionType divType );
    virtual ~G4ParameterisationPolyhedraPhi();

    virtual void ComputeTransformation( const G4int copyNo,
                                        G4VPhysicalVolume* physVol ) const;
    void ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                            const G4VPhysicalVolume* physVol ) const;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
};

//---------------------------------------------------------------------
// Class G4ParameterisationPolyhedraZ
//---------------------------------------------------------------------

class G4ParameterisationPolyhedraZ : public G4VDivisionParameterisation
{ 
  public:  // with description

    G4ParameterisationPolyhedraZ( EAxis axis, G4int nCopies,
                                  G4double offset, G4double step,
                                  G4VSolid* motherSolid,
                                  DivisionType divType );
    virtual ~G4ParameterisationPolyhedraZ();

    virtual void ComputeTransformation( const G4int copyNo,
                                        G4VPhysicalVolume* physVol ) const;
    void ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                            const G4VPhysicalVolume* physVol ) const;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
};

#endif
