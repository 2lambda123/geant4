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
// $Id: G4EvaporationGEMFactory.hh,v 1.3 2005-06-28 11:09:37 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara


#ifndef G4EvaporationGEMFactory_hh
#define G4EvaporationGEMFactory_hh


#include "G4VEvaporationFactory.hh"

class G4EvaporationGEMFactory : public G4VEvaporationFactory
{
public:
  G4EvaporationGEMFactory() {};
  virtual ~G4EvaporationGEMFactory() {};

private:
  G4EvaporationGEMFactory(const G4EvaporationGEMFactory & ) : G4VEvaporationFactory() {};
  const G4EvaporationGEMFactory & operator=(const G4EvaporationGEMFactory & val);
  G4bool operator==(const G4EvaporationGEMFactory & val) const;
  G4bool operator!=(const G4EvaporationGEMFactory & val) const;

private:
  std::vector<G4VEvaporationChannel*> * CreateChannel();


};

#include "G4Pair.hh"
#include "G4VEvaporationChannel.hh"
#include "G4PhotonEvaporation.hh"

template<class U>
void gem_push_one_new
(std::vector<G4VEvaporationChannel*> * list)
{
  list->push_back(new typename U::first);
  gem_push_one_new<typename U::rest>(list);
}
// partial specialization not supported yet...
template<> 
void gem_push_one_new<G4Pair<G4PhotonEvaporation, G4Terminator> >
(std::vector<G4VEvaporationChannel*> * list)
{
  list->push_back(new G4PhotonEvaporation);
}

#endif
