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
// $Id: G4ProductionCuts.hh,v 1.9 2003-05-22 16:05:37 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//
// Class Description
//  This class is 
//
// ------------------------------------------------------------
//   First Implementation          17 Sep. 2002  H.Kurahige
// ------------------------------------------------------------

#ifndef G4ProductionCuts_h 
#define G4ProductionCuts_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "g4std/vector"
#include "G4ParticleDefinition.hh"

enum G4ProductionCutsIndex
{
  idxG4GammaCut =0,
  idxG4ElectronCut,
  idxG4PositronCut,

  NumberOfG4CutIndex
};

class G4ProductionCuts  
{
  public: // with description
  //  constructor 
  G4ProductionCuts();

  //  copy constructor 
  G4ProductionCuts(const G4ProductionCuts &right);

  G4ProductionCuts & operator=(const G4ProductionCuts &right);

  public: 
  //  destructor 
  virtual ~G4ProductionCuts();

  // equal opperators
  G4int operator==(const G4ProductionCuts &right) const;
  G4int operator!=(const G4ProductionCuts &right) const;

  public: // with description
  // Set Cuts methods
  void              SetProductionCut(G4double cut, G4int index = -1);
  void              SetProductionCut(G4double cut, G4ParticleDefinition* ptcl);
  void              SetProductionCut(G4double cut, const G4String& pName);
  // Set the productionCut in range with an index to particle type
  // if index is omitted, the value is applied to all particles

  G4double          GetProductionCut(G4int index) const;
  // Get the productionCut in range with an index to particle type

  G4double          GetProductionCut(const G4String& name) const;
  // Get the productionCut in range with a name of particle type
  
  void              SetProductionCuts(G4std::vector<G4double>&);
  // Set the vector of production cuts in range for all particles

  const G4std::vector<G4double>&   GetProductionCuts() const;
  // Get the vector of production cuts in range for all particles

  G4bool           IsModified() const;
  // return true if any cut value has been modified 
  // after last calculation of PhysicsTable          

  void             PhysicsTableUpdated();
  // inform end of calculation of PhysicsTable  to ProductionCut 
 
  public:
  static G4int GetIndex(const G4String& name);
  static G4int GetIndex(const G4ParticleDefinition* ptcl);

  protected:
  G4std::vector<G4double>         fRangeCuts;
  G4bool                          isModified;

  private:
  static const G4ParticleDefinition* gammaDef;
  static const G4ParticleDefinition* electDef;
  static const G4ParticleDefinition* positDef;
};


inline
G4int  G4ProductionCuts::GetIndex(const G4String& name)
{
  G4int index;
  if       ( name == "gamma" )        { index =  0; }
  else  if ( name == "e-" )           { index =  1; }
  else  if ( name == "e+" )           { index =  2; }
  else                                { index = -1; }

  return index;
}

inline
G4int  G4ProductionCuts::GetIndex(const G4ParticleDefinition* ptcl)
{ 
  if(!ptcl) return -1;
  if(gammaDef==0 && ptcl->GetParticleName()=="gamma") { gammaDef = ptcl; }
  if(electDef==0 && ptcl->GetParticleName()=="e-") { electDef = ptcl; }
  if(positDef==0 && ptcl->GetParticleName()=="e+") { positDef = ptcl; }
  G4int index;
  if(ptcl==gammaDef)      { index = 0; }
  else if(ptcl==electDef) { index = 1; }
  else if(ptcl==positDef) { index = 2; }
  else                    { index = -1; }

  return index;
}

inline
void  G4ProductionCuts::SetProductionCut(G4double cut, G4int index)
{
  if (index<0) {
    for(G4int i = 0; i < NumberOfG4CutIndex; i++) {
      fRangeCuts[i] = cut;
    }
    isModified = true;

  } else if (index < NumberOfG4CutIndex) {
    fRangeCuts[index] = cut;
    isModified = true;
  }     
}

inline 
void  G4ProductionCuts::SetProductionCut(G4double cut, G4ParticleDefinition* ptcl)
{
  G4int idx = -1;
  if(ptcl) idx = GetIndex(ptcl);
  if(idx>=0) SetProductionCut(cut,idx);
}

inline
void  G4ProductionCuts::SetProductionCut(G4double cut, const G4String& pName)
{
  G4int idx = GetIndex(pName);
  if(idx>=0) SetProductionCut(cut,idx);
}

inline
G4double  G4ProductionCuts::GetProductionCut(G4int index) const
{
  G4double cut=-1.0;
  if ( (index>=0) && (index<NumberOfG4CutIndex) ) {
    cut = fRangeCuts[index]; 
  }
  return cut;
}

inline
G4double  G4ProductionCuts::GetProductionCut(const G4String& name) const
{
  return GetProductionCut(GetIndex(name)); 
}

inline
void  G4ProductionCuts::SetProductionCuts(G4std::vector<G4double>& cut)
{
  for(G4int i = 0; (i<NumberOfG4CutIndex); i++) {
    fRangeCuts[i] = cut[i];
  }
  isModified = true;
}

inline
const G4std::vector<G4double>&   G4ProductionCuts::GetProductionCuts() const
{
  return fRangeCuts;
}

inline
G4bool  G4ProductionCuts::IsModified() const
{
  return isModified;
}

inline
void   G4ProductionCuts::PhysicsTableUpdated()
{
  isModified = false;
}








#endif











