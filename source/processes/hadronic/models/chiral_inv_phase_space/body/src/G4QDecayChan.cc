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
//
// $Id: G4QDecayChan.cc,v 1.26 2006-06-29 20:06:55 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QDecayChan ----------------
//             by Mikhail Kossov, Sept 1999.
//      class for Decay Channels of Hadrons in CHIPS Model
// -------------------------------------------------------------------
 
//#define debug
//#define pdebug

#include "G4QDecayChanVector.hh"
#include <algorithm>

G4QDecayChan::G4QDecayChan():aDecayChanLimit(0.),theMinMass(0.)
{}

G4QDecayChan::G4QDecayChan(G4double pLev, G4int PDG1, G4int PDG2, G4int PDG3):
  aDecayChanLimit(pLev)
{
  G4QPDGCode* firstPDG = new G4QPDGCode(PDG1);
  theMinMass =firstPDG->GetMass();
  aVecOfSecHadrons.push_back(firstPDG);
  G4QPDGCode* secondPDG = new G4QPDGCode(PDG2);
  theMinMass+=secondPDG->GetMass();
  aVecOfSecHadrons.push_back(secondPDG);
  if(PDG3) 
  {
    G4QPDGCode* thirdPDG = new G4QPDGCode(PDG3);
    theMinMass+=thirdPDG->GetMass();
    aVecOfSecHadrons.push_back(thirdPDG);
  }
#ifdef debug
  cout<<"G4QDecayChan is defined with pL="<<pLev<<",1="<<PDG1<<",2="<<PDG2<<",3="<<PDG3
      <<",m1="<<firstPDG->GetMass()<<",m2="<<secondPDG->GetMass()<<",minM="<<theMinMass<<endl;
#endif
}

G4QDecayChan::G4QDecayChan(const G4QDecayChan& right)
{
  aDecayChanLimit     = right.aDecayChanLimit;
  theMinMass          = right.theMinMass;
  //aVecOfSecHadrons (Vector)
  G4int nSH           = right.aVecOfSecHadrons.size();
  if(nSH) for(G4int ih=0; ih<nSH; ih++)
  {
    G4QPDGCode* curPC = new G4QPDGCode(right.aVecOfSecHadrons[ih]);
    aVecOfSecHadrons.push_back(curPC);
  }
}

G4QDecayChan::G4QDecayChan(G4QDecayChan* right)
{
  aDecayChanLimit     = right->aDecayChanLimit;
  theMinMass          = right->theMinMass;
  //aVecOfSecHadrons (Vector)
  G4int nSH           = right->aVecOfSecHadrons.size();
  if(nSH) for(G4int ih=0; ih<nSH; ih++)
  {
    G4QPDGCode* curPC = new G4QPDGCode(right->aVecOfSecHadrons[ih]);
    aVecOfSecHadrons.push_back(curPC);
  }
}

G4QDecayChan::~G4QDecayChan() 
{
  G4int nSH=aVecOfSecHadrons.size();
  //G4cout<<"G4QDecayChan::Destructor: Before nSH="<<nSH<<G4endl; // TMP
  if(nSH)std::for_each(aVecOfSecHadrons.begin(), aVecOfSecHadrons.end(), DeleteQPDGCode());
  //G4cout<<"G4QDecayChan::Destructor: After"<<G4endl; // TMP
  aVecOfSecHadrons.clear();
}

// Assignment operator
const G4QDecayChan& G4QDecayChan::operator=(const G4QDecayChan& right)
{
  aDecayChanLimit     = right.aDecayChanLimit;
  theMinMass          = right.theMinMass;
  //aVecOfSecHadrons (Vector)
  G4int nSH           = right.aVecOfSecHadrons.size();
  if(nSH) for(G4int ih=0; ih<nSH; ih++)
  {
    G4QPDGCode* curPC = new G4QPDGCode(right.aVecOfSecHadrons[ih]);
    aVecOfSecHadrons.push_back(curPC);
  }

  return *this;
}

// Standard output for QDecayChan
std::ostream& operator<<(std::ostream& lhs, G4QDecayChan& rhs)
//       =========================================
{
  lhs << "[L=" << rhs.GetDecayChanLimit(); 
  G4QPDGCodeVector VSH = rhs.GetVecOfSecHadrons();
  G4int n = VSH.size();
  lhs << ", N=" << n << ": ";
  for (int i=0; i<n; i++)
  {
    if(!i) lhs << ":";
    else   lhs << ",";
    lhs << VSH[i]->GetPDGCode();
  }
  lhs << "]";
  return lhs;
}





