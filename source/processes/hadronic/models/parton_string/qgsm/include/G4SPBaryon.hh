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
#ifndef G4SPBaryon_h
#define G4SPBaryon_h

#include "globals.hh"

#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4SigmaPlus.hh"
#include "G4SigmaZero.hh"
#include "G4SigmaMinus.hh"
#include "G4XiMinus.hh"
#include "G4XiZero.hh"
#include "G4Lambda.hh"
#include "G4OmegaMinus.hh"

#include "G4AntiProton.hh"
#include "G4AntiNeutron.hh"
#include "G4AntiSigmaPlus.hh"
#include "G4AntiSigmaZero.hh"
#include "G4AntiSigmaMinus.hh"
#include "G4AntiXiMinus.hh"
#include "G4AntiXiZero.hh"
#include "G4AntiLambda.hh"
#include "G4AntiOmegaMinus.hh"

#include "G4ParticleDefinition.hh"
#include "G4SPPartonInfo.hh"
#include <vector>
#include "globals.hh"

class G4SPBaryon
{
  public:
    G4SPBaryon(G4Proton * aProton);
    G4SPBaryon(G4Neutron * aNeutron);
    G4SPBaryon(G4Lambda * aLambda);
    G4SPBaryon(G4SigmaPlus * aSigmaPlus);
    G4SPBaryon(G4SigmaZero * aSigmaZero);
    G4SPBaryon(G4SigmaMinus * aSigmaMinus);
    G4SPBaryon(G4XiMinus * aXiMinus);
    G4SPBaryon(G4XiZero * aXiZero);
    G4SPBaryon(G4OmegaMinus * anOmegaMinus);
    
    G4SPBaryon(G4AntiProton * aAntiProton);
    G4SPBaryon(G4AntiNeutron * aAntiNeutron);
    G4SPBaryon(G4AntiLambda * aAntiLambda);
    G4SPBaryon(G4AntiSigmaPlus * aAntiSigmaPlus);
    G4SPBaryon(G4AntiSigmaZero * aAntiSigmaZero);
    G4SPBaryon(G4AntiSigmaMinus * aAntiSigmaMinus);
    G4SPBaryon(G4AntiXiMinus * aAntiXiMinus);
    G4SPBaryon(G4AntiXiZero * aAntiXiZero);
    G4SPBaryon(G4AntiOmegaMinus * anAntiOmegaMinus);
    
    G4SPBaryon(G4ParticleDefinition * aDefinition);
    
    ~G4SPBaryon()
    {
      for(unsigned int i=0;i<thePartonInfo.size(); i++) delete thePartonInfo[i];
    }
    
    G4bool operator == ( const G4SPBaryon & aBaryon) const
    {return this == &aBaryon; }
    
    G4ParticleDefinition * GetDefinition() {return theDefinition;}
    void SampleQuarkAndDiquark(G4int & quark, G4int & diQuark) const;
    void FindDiquark(G4int quark, G4int & diQuark) const;
    G4int FindQuark(G4int diQuark) const;
    G4double GetProbability(G4int diQuark) const;
    G4int MatchDiQuarkAndGetQuark(const G4SPBaryon & aBaryon, G4int & aDiQuark) const;
        
  private:
  
  G4ParticleDefinition * theDefinition;
  std::vector<G4SPPartonInfo *> thePartonInfo;
};

#endif
