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
// $Id: Tst01ParticleGun.hh,v 1.2 2001-07-11 10:02:28 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Tst01ParticleGun_h
#define Tst01ParticleGun_h 1


#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4PrimaryParticle.hh"
#include "g4std/vector"

class G4Event;
class G4DecayProducts;

class Tst01ParticleGunMessenger;

class Tst01ParticleGun : public G4ParticleGun
{
  public:
    Tst01ParticleGun();
    Tst01ParticleGun(G4int numberofparticles);
    Tst01ParticleGun(G4ParticleDefinition * particleDef, 
                   G4int numberofparticles = 1);

  public:
    virtual ~Tst01ParticleGun();
    Tst01ParticleGun(const Tst01ParticleGun &right);

    const Tst01ParticleGun & operator=(const Tst01ParticleGun &right);

  public:
    void GeneratePrimaryVertex(G4Event* evt);
    void SetDecayProperTime(G4double t);
    void AddDaughter(const G4PrimaryParticle *daughter);

  public:
    G4double GetDecayProperTime() const;

  protected:  
    virtual              void SetInitialValues();
    G4double             particle_decay_time;

    G4std::vector<const G4PrimaryParticle*>  decay_products;  

  private:
    Tst01ParticleGunMessenger* theMessenger;
};

 
inline
 void Tst01ParticleGun::SetDecayProperTime(G4double t)
{
  particle_decay_time = t;
}

inline
 G4double  Tst01ParticleGun::GetDecayProperTime() const
{
  return particle_decay_time;
}

inline 
 void Tst01ParticleGun::AddDaughter(const G4PrimaryParticle *daughter)
{
    decay_products.push_back(daughter);
}

#endif







