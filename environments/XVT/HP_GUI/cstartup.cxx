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
// $Id: cstartup.cxx,v 1.3 2001-07-11 09:56:51 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
/*********************************************************************
 *
    Copyright (c) 1996

    File Name:        CStartup.cxx

    Implements main()
 *
 *********************************************************************/

#include "XVTPwr.h"
#include "AppDef.h"

#include GUI_i
#include G4XvtGUI_i

#ifdef DSP_RELEASE
#include FactoryCommands_i
#endif

#ifndef PAFACTORY_QBASE_NAME
#define PAFACTORY_QBASE_NAME "GUI"
#endif

int main( int argc, char** argv )
{
	// Instantiate an application object, then invoke its Go()
	// method to begin life as a GUI application.  On some platforms
	// Go() will never return control to main(), so do not place
	// any meaningful code after Go().
	//
	// Add any GUI initialization code to the application class
	// constructor, rather than doing it here.

	G4XvtGUI theApplication;

	theApplication.Go(
		argc,
		argv,
		PAFACTORY_APP_MENU,
		PAFACTORY_APP_ABOUT_BOX,
		PAFACTORY_QBASE_NAME,
		PAFACTORY_APP_NAME,
		PAFACTORY_APP_TITLE );

	// In case Go() returns and/or to avoid compiler warnings

	return( 0 );
}

