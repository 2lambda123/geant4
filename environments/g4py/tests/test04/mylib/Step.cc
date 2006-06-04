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
// $Id: Step.cc,v 1.3 2006-06-04 21:35:59 kmura Exp $
// ====================================================================
//   Step.cc
//
//                                         2005 Q
// ====================================================================
#include "Step.hh"
#include <iostream>

// ====================================================================
//
// class description
//
// ====================================================================

////////////
Step::Step()
  : x(0)
////////////
{
  std::cout << "Step is created. @" 
	    << this << std::endl;
}


/////////////
Step::~Step()
/////////////
{
  std::cout << "Step is deleted. @" 
	    << this << std::endl;
}

