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
// G4UIcmdPargInteger.cc
//
// Implementation of an integer argument for G4UIcmdWithPargs
//

#include "G4UIcmdPargInteger.hh"

#include <strstream>

//
// Fetch argument value from input stream
//
std::istream &G4UIcmdPargInteger::FetchValue( std::istream &istr )
{
	G4int newValue;
	istr >> newValue;
	
	if (istr) value = newValue;
	
	return istr;
}


//
// ConvertToString
//
G4String G4UIcmdPargInteger::ConvertToString()
{
	char buff[20];
	std::ostrstream os(buff,20);
	
	os << value << '\0';
	
	G4String answer(buff);
	return answer;
}
