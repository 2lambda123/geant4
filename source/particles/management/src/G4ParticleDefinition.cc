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
// $Id: G4ParticleDefinition.cc,v 1.18 2003-06-11 07:20:06 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
//	History: first implementation, based on object model of
//	2nd December 1995, G.Cosmo
//      ---------------- G4ParticleDefinition -----------------
//      first implementation by Makoto Asai, 29 January 1996
//      revised by G.Cosmo, 29 February 1996
//      revised by H.Kurashige, 19 April 1996
//      Code uses operators (+=, *=, ++, -> etc.) correctly, P. Urban, 26/6/96
//      revised by H.Kurashige, 4 July 1996
//      revised by H.Kurashige, 16 Feb 1997
//      revised by H.Kurashige, 10 Nov 1997
//      remove new/delete G4ProcessManager   by H.Kurashige  06 June 1998 
//      added  Resonance flag and ApplyCuts flag  H.Kurashige 27  June 1998
//      modify FillQuarkContents() for quarks/diquarks H.Kurashige 30 June 1998
//      modify encoding rule H.Kurashige 23 Oct. 98
//      modify FillQuarkContents() for deltas      25 Nov.,98 H.Kurashige
//
//      modify FillQuarkContents() to use G4PDGCodeChecker 17 Aug. 99 H.Kurashige
// --------------------------------------------------------------


#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4PDGCodeChecker.hh"

G4ParticleDefinition::G4ParticleDefinition(
		     const G4String&     aName,  
		     G4double            mass,
		     G4double            width,
                     G4double            charge,   
		     G4int               iSpin,
                     G4int               iParity,    
		     G4int               iConjugation,
                     G4int               iIsospin,   
		     G4int               iIsospin3, 
		     G4int               gParity,
		     const G4String&     pType,
                     G4int               lepton,      
		     G4int               baryon,
		     G4int               encoding,
		     G4bool              stable,
		     G4double            lifetime,
		     G4DecayTable        *decaytable,
		     G4bool              shortlived)
		 : theParticleName(aName), 
		   thePDGMass(mass),
		   thePDGWidth(width),
		   thePDGCharge(charge),
		   thePDGiSpin(iSpin),
		   thePDGSpin(iSpin*0.5),
		   thePDGiParity(iParity), 
		   thePDGiConjugation(iConjugation),
		   thePDGiGParity(gParity),
		   thePDGiIsospin(iIsospin),
		   thePDGiIsospin3(iIsospin3),
		   thePDGIsospin(iIsospin*0.5),
		   thePDGIsospin3(iIsospin3*0.5),
		   theLeptonNumber(lepton),
		   theBaryonNumber(baryon),
		   theParticleType(pType), 
		   theParticleSubType(""), 
		   thePDGEncoding(encoding),
		   theAntiPDGEncoding(-1*encoding),
		   fShortLivedFlag(shortlived),
		   thePDGStable(stable), 
		   thePDGLifeTime(lifetime), 
                   theDecayTable(decaytable),
		   theProcessManager(0),
                   verboseLevel(1),
  		   fApplyCutsFlag(false)
{
   // check name and register this particle into ParticleTable
   theParticleTable = G4ParticleTable::GetParticleTable();
   theParticleTable->Insert(this);

   // check quark contents
#ifdef G4VERBOSE
   if (this->FillQuarkContents() != thePDGEncoding) {
     if (verboseLevel>0) {
       // cerr bnot G4cerr is used intentionally  
       // because G4ParticleDefinition constructor may be called 
       // before G4cerr object is instantiated !!
       G4cerr << "Particle " << aName << " has a strange PDGEncoding " <<G4endl;
     }
   }
#endif
}

G4ParticleDefinition::G4ParticleDefinition(const G4ParticleDefinition &)
{
  G4Exception("You call Copy Constructor of G4ParticleDefinition ");
}

G4ParticleDefinition::G4ParticleDefinition()
{
  G4Exception("You call Default Constructor of G4ParticleDefinition ");
}


G4ParticleDefinition::~G4ParticleDefinition() 
{
  if (theDecayTable!= 0) delete theDecayTable;
}


const G4ParticleDefinition & G4ParticleDefinition::operator=(const G4ParticleDefinition &right)
{
  if (this != &right)  {
  } 
  return *this;
}

G4int G4ParticleDefinition::operator==(const G4ParticleDefinition &right) const
{
  return (this->theParticleName == right.theParticleName);
}

G4int G4ParticleDefinition::operator!=(const G4ParticleDefinition &right) const
{
  return (this->theParticleName != right.theParticleName);
}



G4int G4ParticleDefinition::FillQuarkContents()
      //  calculate quark and anti-quark contents
      //  return value is PDG encoding for this particle.
      //  It means error if the return value is differnt from
      //  this->thePDGEncoding.
{
  G4int flavor;
  for (flavor= 0; flavor<NumberOfQuarkFlavor; flavor++){
    theQuarkContent[flavor]     = 0;
    theAntiQuarkContent[flavor] = 0;
  }

  G4PDGCodeChecker checker;

  G4int temp = checker.CheckPDGCode(thePDGEncoding, theParticleType);

  if ( temp != 0) {
    for (flavor= 0; flavor<NumberOfQuarkFlavor; flavor++){
      theQuarkContent[flavor]     = checker.GetQuarkContent(flavor);
      theAntiQuarkContent[flavor] = checker.GetAntiQuarkContent(flavor);
    }
    if ((theParticleType == "meson")||(theParticleType == "baryon")) {
      // check charge
      if (!checker.CheckCharge(thePDGCharge) ){
	temp = 0;
#ifdef G4VERBOSE
	if (verboseLevel>1) {
	  G4cout << " illegal charge (" << thePDGCharge/eplus;
	  G4cout << " PDG code=" << thePDGEncoding <<G4endl;
	}
#endif
      }
      // check spin 
      if (checker.GetSpin() != thePDGiSpin) {
	temp=0;
#ifdef G4VERBOSE
	if (verboseLevel>1) {
	  G4cout << " illegal SPIN (" << thePDGiSpin << "/2";
	  G4cout << " PDG code=" << thePDGEncoding <<G4endl;
	}
#endif
      }
    }
  }
  return temp;
}

void G4ParticleDefinition::DumpTable() const
{
  G4cout << G4endl;
  G4cout << "--- G4ParticleDefinition ---" << G4endl;
  G4cout << " Particle Name : " << theParticleName << G4endl;
  G4cout << " PDG particle code : " << thePDGEncoding;
  G4cout << " [PDG anti-particle code: " << this->GetAntiPDGEncoding() << "]"<< G4endl;
  G4cout << " Mass [GeV/c2] : " << thePDGMass/GeV ;
  G4cout << "     Width : " << thePDGWidth/GeV << G4endl;
  G4cout << " Lifetime [nsec] : " << thePDGLifeTime/ns << G4endl;
  G4cout << " Charge [e]: " << thePDGCharge/eplus << G4endl;
  G4cout << " Spin : " << thePDGiSpin << "/2" << G4endl;
  G4cout << " Parity : " << thePDGiParity << G4endl;
  G4cout << " Charge conjugation : " << thePDGiConjugation << G4endl;
  G4cout << " Isospin : (I,Iz): (" << thePDGiIsospin <<"/2";
  G4cout << " , " << thePDGiIsospin3 << "/2 ) " << G4endl;
  G4cout << " GParity : " << thePDGiGParity << G4endl;
  G4cout << " Quark contents     (d,u,s,c,b,t) : " << theQuarkContent[0];
  G4cout << ", " << theQuarkContent[1];
  G4cout << ", " << theQuarkContent[2];
  G4cout << ", " << theQuarkContent[3];
  G4cout << ", " << theQuarkContent[4];
  G4cout << ", " << theQuarkContent[5] << G4endl;
  G4cout << " AntiQuark contents               : " << theAntiQuarkContent[0];
  G4cout << ", " << theAntiQuarkContent[1];
  G4cout << ", " << theAntiQuarkContent[2];
  G4cout << ", " << theAntiQuarkContent[3];
  G4cout << ", " << theAntiQuarkContent[4];
  G4cout << ", " << theAntiQuarkContent[5] << G4endl;
  G4cout << " Lepton number : " << theLeptonNumber;
  G4cout << " Baryon number : " << theBaryonNumber << G4endl;
  G4cout << " Particle type : " << theParticleType ;
  G4cout << " [" << theParticleSubType << "]" << G4endl;

  if ( fShortLivedFlag ){
    G4cout << " ShortLived : ON" << G4endl;
  }

  if ( thePDGStable ){
    G4cout << " Stable : stable" << G4endl;
  } else {
    if( theDecayTable != 0 ){
      theDecayTable->DumpInfo();
    } else {
      G4cout << "Decay Table is not defined !!" <<G4endl;
    }
  }

}

void G4ParticleDefinition::SetApplyCutsFlag(G4bool flg)
{
  if(theParticleName=="gamma"
  || theParticleName=="e-"
  || theParticleName=="e+")
  { fApplyCutsFlag = flg; }
  else
  {
    G4cerr
     << "G4ParticleDefinition::SetApplyCutsFlag() for " << theParticleName
     << G4endl;
    G4cerr
     << "becomes obsolete. Production threshold is applied only for "
     << "gamma, e- and e+." << G4endl;
  }
}

// Following methods are moved from G4ParticleWithCuts class
// for keeping backward compatibility. These methods are obsolete
// and will be completely removed away in near future.

#include "G4MaterialCutsCouple.hh"
#include "G4ProductionCuts.hh"
#include "G4ProductionCutsTable.hh"

void G4ParticleDefinition::SetCuts(G4double aCut)
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::SetCuts." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4ProductionCuts* defaultCuts
   = G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts();
  defaultCuts->SetProductionCut(aCut,this);
}

void G4ParticleDefinition::SetRangeCut(G4double aCut, const G4Material*)
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::SetRangeCut." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4ProductionCuts* defaultCuts
   = G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts();
  defaultCuts->SetProductionCut(aCut,this);
}

void G4ParticleDefinition::SetRangeCutVector(G4std::vector<G4double>& vec)
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::SetRangeCutVector." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4ProductionCuts* defaultCuts
   = G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts();
  defaultCuts->SetProductionCut(vec[0],this);
}

G4double* G4ParticleDefinition::GetLengthCuts() const
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::GetLengthCuts." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4int idx = G4ProductionCuts::GetIndex(this);
  if(idx<0) return NULL;
  return G4ProductionCutsTable::GetProductionCutsTable()->GetRangeCutsDoubleVector(idx);
}

G4double  G4ParticleDefinition::GetRangeThreshold(const G4Material* aMat) const
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::GetLengthThreshold." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4int idx = G4ProductionCuts::GetIndex(this);
  if(idx<0) return -1.;
  G4ProductionCuts* defaultCuts
   = G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts();
  const G4MaterialCutsCouple* materialCutsCouple
   = G4ProductionCutsTable::GetProductionCutsTable()->GetMaterialCutsCouple(aMat,defaultCuts);
  if(materialCutsCouple==NULL) return -1.;
  G4double* vec 
   = G4ProductionCutsTable::GetProductionCutsTable()->GetRangeCutsDoubleVector(idx);
  return vec[materialCutsCouple->GetIndex()];
}

G4double* G4ParticleDefinition::GetEnergyCuts() const
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::GetEnergyCuts." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4int idx = G4ProductionCuts::GetIndex(this);
  if(idx<0) return NULL;
  return G4ProductionCutsTable::GetProductionCutsTable()->GetEnergyCutsDoubleVector(idx);
}

G4double G4ParticleDefinition::GetEnergyThreshold(const G4Material* aMat) const
{
  G4cerr << "Warning : you invoked G4ParticleDefinition::GetEnergyThreshold." << G4endl;
  G4cerr << " This method is obsolete and will be removed soon." << G4endl;
  G4cerr << " Use G4ProductionCuts class." << G4endl;

  G4int idx = G4ProductionCuts::GetIndex(this);
  if(idx<0) return -1.;
  G4ProductionCuts* defaultCuts
   = G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts();
  const G4MaterialCutsCouple* materialCutsCouple
   = G4ProductionCutsTable::GetProductionCutsTable()->GetMaterialCutsCouple(aMat,defaultCuts);
  if(materialCutsCouple==NULL) return -1.;
  G4double* vec 
   = G4ProductionCutsTable::GetProductionCutsTable()->GetEnergyCutsDoubleVector(idx);
  return vec[materialCutsCouple->GetIndex()];
}

#include "G4VRangeToEnergyConverter.hh"

void G4ParticleDefinition::SetEnergyRange(G4double lowedge, G4double highedge)
{
  G4VRangeToEnergyConverter::SetEnergyRange(lowedge, highedge);
}












