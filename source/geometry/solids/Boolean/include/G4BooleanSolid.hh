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
// $Id: G4BooleanSolid.hh,v 1.10 2004-09-22 09:27:47 gcosmo Exp $
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
// 10.09.98 V.Grichine, created
//
// --------------------------------------------------------------------
#ifndef G4BOOLEANSOLID_HH
#define G4BOOLEANSOLID_HH

#include "G4DisplacedSolid.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

class G4VSolid;

class G4BooleanSolid : public G4VSolid
{
  public:  // with description
 
    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB   );

    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB,
                          G4RotationMatrix* rotMatrix,
                    const G4ThreeVector& transVector    );

    G4BooleanSolid( const G4String& pName,
                          G4VSolid* pSolidA ,
                          G4VSolid* pSolidB , 
                    const G4Transform3D& transform   );

    virtual ~G4BooleanSolid();

    virtual const G4VSolid* GetConstituentSolid(G4int no) const;
    virtual       G4VSolid* GetConstituentSolid(G4int no);
      // If Solid is made up from a Boolean operation of two solids,
      // return the corresponding solid (for no=0 and 1).
      // If the solid is not a "Boolean", return 0.

    virtual G4GeometryType  GetEntityType() const;
    inline G4double GetCubicVolume();

    std::ostream& StreamInfo(std::ostream& os) const;

    inline G4int GetCubVolStatistics() const;
    inline G4double GetCubVolEpsilon() const;
    inline void SetCubVolStatistics(G4int st);
    inline void SetCubVolEpsilon(G4double ep);

  protected:
  
    G4VSolid* fPtrSolidA;
    G4VSolid* fPtrSolidB;

  private:

    G4BooleanSolid(const G4BooleanSolid&);
    G4BooleanSolid& operator=(const G4BooleanSolid&);
      // Private copy constructor and assignment operator.

  private:

    G4int    fCubVolStatistics;
    G4double fCubVolEpsilon;
    G4double fCubicVolume;

    G4bool  createdDisplacedSolid;
      // If & only if this object created it, it must delete it

} ;

#include "G4BooleanSolid.icc"

#endif
