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
// $Id: G4HepRepViewer.hh,v 1.1 2001-08-24 23:27:26 perl Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  5th April 2001
// A base class for a dummy viewer to dump geometry hierarchy.

#ifndef G4HepRepVIEWER_HH
#define G4HepRepVIEWER_HH

#include "G4VViewer.hh"

#include "JHepRep.hh"
#include "JHepRepFactory.hh"

class G4HepRepViewer: public G4VViewer {
public:
  G4HepRepViewer(G4VSceneHandler&,const G4String& name);
  virtual ~G4HepRepViewer();
  void SetView();
  void ClearView();
  void DrawView();
  void ShowView();

private:
  JHepRep *heprep;
  JHepRepFactory *factory;
};

#endif
