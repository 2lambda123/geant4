//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// J.L. Chuma, TRIUMF, 31-Oct-1996
// last modified: 19-Dec-1996
// modified by J.L.Chuma, 24-Jul-1997   to include total momentum
// inluded operator *, and some minor modifications.
// modified by H.P.Wellisch to add functionality needed by string models,
// cascade and Nucleus. (Mon Mar 16 1998) 
// M. Kelsey 29-Aug-2011 -- Use G4Allocator model to avoid memory churn.
 
#ifndef G4ReactionProduct_h
#define G4ReactionProduct_h 1

#include "globals.hh"
#include "G4Allocator.hh"
#include "G4DynamicParticle.hh"
#include "G4HadProjectile.hh"
#include "G4HadronicException.hh"

// To support better memory management and reduced fragmentation
class G4ReactionProduct;
#if defined G4HADRONIC_ALLOC_EXPORT
  extern G4DLLEXPORT __thread G4Allocator<G4ReactionProduct> *aRPAllocator_G4MT_TLS_;
#else
  extern G4DLLIMPORT __thread G4Allocator<G4ReactionProduct> *aRPAllocator_G4MT_TLS_;
#endif

class G4ReactionProduct
{
    friend G4ReactionProduct operator+(
     const G4ReactionProduct & p1, const G4ReactionProduct &p2 );
    
    friend G4ReactionProduct operator-(
     const G4ReactionProduct & p1, const G4ReactionProduct &p2 );

    friend G4ReactionProduct operator*(
     const G4double aDouble, const G4ReactionProduct &p2 )
     {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
       G4ReactionProduct result;
       result.SetMomentum(aDouble*p2.GetMomentum());
       result.SetMass(p2.GetMass());
       result.SetTotalEnergy(std::sqrt(result.GetMass()*result.GetMass()+
                                  result.GetMomentum()*result.GetMomentum()));
       return result;
     }

 public:
    G4ReactionProduct();
    
    G4ReactionProduct( G4ParticleDefinition *aParticleDefinition );

    ~G4ReactionProduct() {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  }
    
    G4ReactionProduct( const G4ReactionProduct &right );

    // Override new and delete for use with G4Allocator
    inline void* operator new(size_t) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
      return (void *)aRPAllocator.MallocSingle();
    }
#ifdef __IBMCPP__
    inline void* operator new(size_t, void *p) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
      return p;
    }
#endif
    inline void operator delete(void* aReactionProduct) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
      aRPAllocator.FreeSingle((G4ReactionProduct*)aReactionProduct);
    }

    G4ReactionProduct &operator= ( const G4ReactionProduct &right );
    
    G4ReactionProduct &operator= ( const G4DynamicParticle &right );
    
    G4ReactionProduct &operator= ( const G4HadProjectile &right );

    inline G4bool operator== ( const G4ReactionProduct &right ) const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return ( this == (G4ReactionProduct*) &right ); }
    
    inline G4bool operator!= ( const G4ReactionProduct &right ) const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return ( this != (G4ReactionProduct*) &right ); }
    
    inline G4ParticleDefinition *GetDefinition() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return theParticleDefinition; }
    
    void SetDefinition( G4ParticleDefinition *aParticleDefinition );
   
    void SetDefinitionAndUpdateE( G4ParticleDefinition *aParticleDefinition );
      
    void SetMomentum( const G4double x, const G4double y, const G4double z );
    
    void SetMomentum( const G4double x, const G4double y );
    
    void SetMomentum( const G4double z );

    inline void SetMomentum( const G4ThreeVector &mom )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   momentum = mom; }
    
    inline G4ThreeVector GetMomentum() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return momentum; }
    
    inline G4double GetTotalMomentum() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return std::sqrt(std::abs(kineticEnergy*(totalEnergy+mass))); }
    
    inline G4double GetTotalEnergy() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return totalEnergy; }
    
    inline void SetKineticEnergy( const G4double en )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
      kineticEnergy = en;
      totalEnergy = kineticEnergy + mass;
    }
    
    inline G4double GetKineticEnergy() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return kineticEnergy; }

    inline void SetTotalEnergy( const G4double en )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
      totalEnergy = en;
      kineticEnergy = totalEnergy - mass;
    }
    
    inline void SetMass( const G4double mas )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   mass = mas; }
    
    inline G4double GetMass() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return mass; }
    
    inline void SetTOF( const G4double t )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   timeOfFlight = t; }
    
    inline G4double GetTOF() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return timeOfFlight; }
    
    inline void SetSide( const G4int sid )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   side = sid; }
    
    inline G4int GetSide() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return side; }
    
    inline void SetNewlyAdded( const G4bool f )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   NewlyAdded = f; }
    
    inline G4bool GetNewlyAdded() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return NewlyAdded; }
    
    inline void SetMayBeKilled( const G4bool f )
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   MayBeKilled = f; }
    
    inline G4bool GetMayBeKilled() const
    {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return MayBeKilled; }

    void SetZero();
    
    void Lorentz( const G4ReactionProduct &p1, const G4ReactionProduct &p2 );
    
    G4double Angle( const G4ReactionProduct &p ) const;
    
    inline void SetPositionInNucleus(G4double x, G4double y, G4double z)
     {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
       positionInNucleus.setX(x);
       positionInNucleus.setY(y);
       positionInNucleus.setZ(z);
     }
    
    inline void SetPositionInNucleus( G4ThreeVector & aPosition )
     {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  
       positionInNucleus = aPosition;
     }
    
    inline G4ThreeVector GetPositionInNucleus() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;  return positionInNucleus; }
    inline G4double GetXPositionInNucleus() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return positionInNucleus.x(); }
    inline G4double GetYPositionInNucleus() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return positionInNucleus.y(); }
    inline G4double GetZPositionInNucleus() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return positionInNucleus.z(); }
    
    inline void SetFormationTime(G4double aTime) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   formationTime = aTime; }
    
    inline G4double GetFormationTime() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return formationTime; }
    
    inline void HasInitialStateParton(G4bool aFlag) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   hasInitialStateParton = aFlag; }
    
    inline G4bool HasInitialStateParton() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return hasInitialStateParton; }
 
#ifdef PRECOMPOUND_TEST
     void SetCreatorModel(const G4String& aModel) {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   theCreatorModel = aModel; }
     G4String GetCreatorModel() const {  ;;;   if (!aRPAllocator_G4MT_TLS_) aRPAllocator_G4MT_TLS_ = new G4Allocator<G4ReactionProduct>  ; G4Allocator<G4ReactionProduct> &aRPAllocator = *aRPAllocator_G4MT_TLS_;  ;;;   return theCreatorModel; }
#endif
 
 private:
    
    G4ParticleDefinition *theParticleDefinition;
    
    // for use with string models and cascade.
    G4ThreeVector positionInNucleus;
    G4double formationTime;
    G4bool hasInitialStateParton;
    
    // mass is included here, since pseudo-particles are created with masses different
    // than the standard particle masses, and we are not allowed to create particles
    G4double mass;
    
    G4ThreeVector momentum;
    
    G4double totalEnergy;
    G4double kineticEnergy;
    
    G4double timeOfFlight;
    
    //  side refers to how the particles are distributed in the
    //  forward (+) and backward (-) hemispheres in the center of mass system
    G4int side;

    // NewlyAdded refers to particles added by "nuclear excitation", or as
    //  "black track" particles, or as deuterons, tritons, and alphas
    G4bool NewlyAdded;
    G4bool MayBeKilled;

#ifdef PRECOMPOUND_TEST
    G4String theCreatorModel;
#endif
};
 
#endif
 
