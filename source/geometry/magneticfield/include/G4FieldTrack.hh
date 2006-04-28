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
// $Id: G4FieldTrack.hh,v 1.15 2006-04-28 15:46:11 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4FieldTrack
//
// Class description:
//
// Data structure bringing together a magnetic track's state.
// (position, momentum direction & modulus, energy, spin, ... )
// Uses/abilities:
//  - does not maintain any relationship between its data (eg energy/momentum).
//  - for use in Runge-Kutta solver (in passing it the values right now).

// History
// - First version: Oct 14, 1996  John Apostolakis
// - Modified:      Oct 24, 1996  JA: Added dist_on_curve, deleted constructor
//                  Nov  5, 1998  JA: Added energy, momentum, TOF, spin &
//                                    several constructor, access, set methods
// -------------------------------------------------------------------

#ifndef G4FieldTrack_HH
#define G4FieldTrack_HH

#include "G4ThreeVector.hh"

class  G4ChargeState;

class  G4FieldTrack
{
   public:  // with description

     G4FieldTrack( const G4ThreeVector& pPosition, 
                         G4double       LaboratoryTimeOfFlight,
                   const G4ThreeVector& pMomentumDirection,
                         G4double       kineticEnergy,
                         G4double       restMass_c2,
		         G4double       charge, 
		   const G4ThreeVector& pSpin,
 		         G4double       magnetic_dipole_moment= 0.0,
		         G4double       curve_length= 0.0
		 );
     G4FieldTrack( const G4FieldTrack&   pFieldTrack );
     G4FieldTrack( char );   //  Almost default constructor

     ~G4FieldTrack();
       // End of preferred Constructors / Destructor 

     inline void
     UpdateState( const G4ThreeVector& pPosition, 
                        G4double       LaboratoryTimeOfFlight,
		  const G4ThreeVector& pMomentumDirection,
		        G4double       kineticEnergy); 
        //  Update four-vectors for space/time and momentum/energy
        //    Also resets curve length.
     inline
     void  UpdateFourMomentum( G4double             kineticEnergy, 
			       const G4ThreeVector& momentumDirection ); 
        //  Update momentum (and direction), and kinetic energy 
     void SetChargeAndMoments(G4double charge, 
			      G4double magnetic_dipole_moment= DBL_MAX, // default: do not change
			      G4double electric_dipole_moment= DBL_MAX, 
			      G4double magnetic_charge=DBL_MAX );             

     // Older constructor
     G4FieldTrack( const G4ThreeVector& pPosition, 
                   const G4ThreeVector& pMomentumDirection,
                         G4double       curve_length,
                         G4double       kineticEnergy,
                   const G4double       restMass_c2,
                         G4double       velocity,
                         G4double       LaboratoryTimeOfFlight=0.0,
                         G4double       ProperTimeOfFlight=0.0, 
                   const G4ThreeVector* pSpin=0);
            //  Misses charge !!!

     inline G4FieldTrack& operator = ( const G4FieldTrack & rStVec );
       // Assignment operator

     inline G4ThreeVector  GetMomentum() const;   
     inline G4ThreeVector  GetPosition() const; 
     inline const G4ThreeVector& GetMomentumDir() const;
     inline G4ThreeVector  GetMomentumDirection() const;
     inline G4double       GetCurveLength() const;
       // Distance along curve of point.
     // inline G4double       GetMomentumModulus() const;  // Obsolete
     inline G4ThreeVector  GetSpin()   const;
     inline G4double       GetLabTimeOfFlight() const;
     inline G4double       GetProperTimeOfFlight() const;
       // Accessors.

     inline void SetPosition(G4ThreeVector nPos); 
     inline void SetMomentum(G4ThreeVector nMomDir);
       // Does change mom-dir too.

     inline void SetMomentumDir(G4ThreeVector nMomDir);
       // Does NOT change Momentum or Velocity Vector.

     inline void SetCurveLength(G4double nCurve_s);
       // Distance along curve.
     inline void SetKineticEnergy(G4double nEnergy);
       // Does not modify momentum.
     // inline void SetMomentumModulus(G4double nMomentumMod);  // Obsolete
       // Does not modify energy.
     inline void SetSpin(G4ThreeVector nSpin);
     inline void SetLabTimeOfFlight(G4double nTOF); 
     inline void SetProperTimeOfFlight(G4double nTOF);
       //  Modifiers

   public: // without description

     inline G4FieldTrack& SetCurvePnt(const G4ThreeVector& pPosition, 
                                      const G4ThreeVector& pMomentum,
                                            G4double       s_curve );
     inline void          InitialiseSpin( const G4ThreeVector& Spin );
       //  Used to update / initialise the state

     inline G4double  GetKineticEnergy() const;

     enum { ncompSVEC = 12 };     // Needed and should be used only for RK integration driver
     inline void DumpToArray(G4double valArr[ncompSVEC]) const; 
     inline void LoadFromArray(const G4double valArr[ncompSVEC], G4int noVarsIntegrated); 
     
     friend  std::ostream&
             operator<<( std::ostream& os, const G4FieldTrack& SixVec);

   private:

     G4double  SixVector[6];
     G4double  fDistanceAlongCurve;  // distance along curve of point
     G4double  fKineticEnergy;
     G4double  fRestMass_c2;
     G4double  fLabTimeOfFlight;
     G4double  fProperTimeOfFlight;
     // G4double  fMomentumModulus;  // Unused
     G4ThreeVector fSpin;
     G4ThreeVector fMomentumDir;

     G4ChargeState* fpChargeState;   // Charge & moments
}; 

#include "G4FieldTrack.icc"

#endif  /* End of ifndef G4FieldTrack_HH */

// Rename:
//
// s/distance_along_curve/fDistanceAlongCurve/g;
// s/SixVector/fSixVector/g;
// s/G4SixVector/G4FieldTrack/g;
