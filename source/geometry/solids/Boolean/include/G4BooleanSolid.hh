// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4BooleanSolid.hh,v 1.3 2000-04-27 09:59:36 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4BooleanSolid
//
// Class description:
//
// Abstract base class for solids created by boolean operations
// between other solids.

// History:
//
// 10.09.98 V.Grichine, creation according J. Apostolakis's recommendations.


#ifndef G4BOOLEANSOLID_HH
#define G4BOOLEANSOLID_HH

#include "G4VSolid.hh"
#include "G4DisplacedSolid.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4AffineTransform.hh"

class G4BooleanSolid : public G4VSolid
{
  public:  // with description
 
    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB   ) ;

    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB,
                          G4RotationMatrix* rotMatrix,
                    const G4ThreeVector& transVector    ) ;

    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB , 
                    const G4Transform3D& transform   ) ;

    virtual ~G4BooleanSolid() ;

    virtual const G4VSolid* GetConstituentSolid(G4int no) const;
    virtual       G4VSolid* GetConstituentSolid(G4int no);
      // If Solid is made up from a Boolean operation of two solids,
      //    return the corresponding solid (for no=0 and 1)
      // If the solid is not a "Boolean", return 0.

  protected:
  
    G4VSolid* fPtrSolidA ;
    G4VSolid* fPtrSolidB ;

  private:

    G4bool  createdDisplacedSolid;
      // If & only if this object created it, it must delete it
} ;

#endif
