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
// $Id: TiaraTally.hh,v 1.5 2006-06-29 15:44:18 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
//
// Class TiaraTally
//

#ifndef TiaraTally_hh
#define TiaraTally_hh TiaraTally_hh
#include "TiaraMeasure.hh"
#include <vector>
#include <map>


typedef std::map<G4double, std::pair<G4double, TiaraMeasure > > TiaraTallyData;

class TiaraTally{
public:
  TiaraTally();
  ~TiaraTally();
	     
  void setBinEdges(const std::vector<double>  & binEdges);
  void fill(G4double x, G4double w);
  void EndOfEventAction();
  TiaraMeasure measure(G4int i);
  std::vector<double>  binEdges();
  G4int size();

private:
  std::vector<double>  fBinEdges;
  G4double fLowestEdge;
  TiaraTallyData fMapHighEdgeToMeasure;
  
  

};

#endif
