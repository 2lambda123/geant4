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
// $Id: G4DAWNFILE.cc,v 1.8 2001-07-11 10:08:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Satoshi TANAKA
// DAWNFILE factory.

//=================//
#ifdef G4VIS_BUILD_DAWNFILE_DRIVER
//=================//

//#define DEBUG_FR_SYSTEM


#include "G4DAWNFILE.hh"

#define __G_ANSI_C__

//#include "G4VisFeaturesOfDAWNFILE.hh"
#include "G4FRFeatures.hh" 
#include "G4VSceneHandler.hh"
#include "G4DAWNFILESceneHandler.hh"
#include "G4DAWNFILEViewer.hh"
#include "G4FRConst.hh"


	//----- G4DAWNFILE, constructor
G4DAWNFILE::G4DAWNFILE ():
  G4VGraphicsSystem ("DAWNFILE",
		     "DAWNFILE",
		     FR_DAWNFILE_FEATURES,
		     G4VGraphicsSystem::threeD)
{}

	//----- G4DAWNFILE, destructor
G4DAWNFILE::~G4DAWNFILE () 
{}


	//-----  G4DAWNFILE::CreateSceneHandler (const G4String& name) 
G4VSceneHandler* G4DAWNFILE::CreateSceneHandler (const G4String& name) 
{
	G4VSceneHandler* p = new G4DAWNFILESceneHandler (*this, name);

	G4cout	<< G4DAWNFILESceneHandler::GetSceneCount ()
		<< ' ' << fName << " scene handlers extanct." << G4endl;

	return p;
}

	//-----  G4DAWNFILE::CreateViewer ()
G4VViewer* G4DAWNFILE::CreateViewer (G4VSceneHandler& scene, const G4String& name) 
{
       	G4VViewer* pView = 
	  new G4DAWNFILEViewer ((G4DAWNFILESceneHandler&) scene, name);
	return pView;
}


#endif // G4VIS_BUILD_DAWNFILE_DRIVER
