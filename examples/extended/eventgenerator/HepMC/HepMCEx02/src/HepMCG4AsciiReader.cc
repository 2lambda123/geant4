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

// ====================================================================
//
//   HepMCG4AsciiReader.cc
//   $Id: HepMCG4AsciiReader.cc,v 1.3 2003-06-16 16:48:40 gunter Exp $
//
// ====================================================================
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4AsciiReaderMessenger.hh"

#include <iostream>
#include <fstream>

////////////////////////////////////////
HepMCG4AsciiReader::HepMCG4AsciiReader()
  :  filename("xxx.dat"), verbose(0)
////////////////////////////////////////
{
  asciiInput= new HepMC::IO_Ascii(filename.c_str(), std::ios::in);

  messenger= new HepMCG4AsciiReaderMessenger(this);
}

/////////////////////////////////////////
HepMCG4AsciiReader::~HepMCG4AsciiReader()
/////////////////////////////////////////
{
  delete asciiInput;
  delete messenger;
}

/////////////////////////////////////
void HepMCG4AsciiReader::Initialize()
/////////////////////////////////////
{
  delete asciiInput;

  asciiInput= new HepMC::IO_Ascii(filename.c_str(), std::ios::in);
  asciiInput-> print();
}

/////////////////////////////////////////////////////////
HepMC::GenEvent* HepMCG4AsciiReader::GenerateHepMCEvent()
/////////////////////////////////////////////////////////
{
  HepMC::GenEvent* evt= asciiInput-> read_next_event();
  if(!evt) return 0; // no more event

  if(verbose>0) evt-> print();
    
  return evt;
}

