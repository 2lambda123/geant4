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
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: NTSTBabarEvtReadGenerator.hh,v 1.3 2005-11-09 14:50:48 gcosmo Exp $
//
// Description:
//	Class NTSTBabarEvtReadGenerator
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bill Lockman
//
// Copyright Information:
//	Copyright (C) 2000         SCIPP, U.C. Santa Cruz
//
//------------------------------------------------------------------------

#ifndef NTSTBabarEvtReadGenerator_hh
#define NTSTBabarEvtReadGenerator_hh 1

#include <fstream>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"

class G4Event;

class NTSTBabarEvtReadGenerator:public G4VPrimaryGenerator
{
public:
  NTSTBabarEvtReadGenerator(const char* evfile);
  ~NTSTBabarEvtReadGenerator();
  
  void GeneratePrimaryVertex(G4Event* evt);
  
private:
  G4String fileName;
  std::ifstream inputFile;
};

#endif



