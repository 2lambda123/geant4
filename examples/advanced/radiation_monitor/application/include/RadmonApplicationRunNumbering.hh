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
// File name:     RadmonApplicationRunNumbering.hh
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonApplicationRunNumbering.hh,v 1.3 2006-06-29 16:08:25 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Dumps run id
//

#ifndef   RADMONAPPLICATIONRUNNUMBERING_HH
 #define  RADMONAPPLICATIONRUNNUMBERING_HH
 
 // Include files
 #include "RadmonRunActionObserver.hh"
 #include "globals.hh"
 
 class RadmonApplicationRunNumbering : public RadmonRunActionObserver
 {
  public:
   inline                                       RadmonApplicationRunNumbering();
   inline virtual                              ~RadmonApplicationRunNumbering();
   
   inline void                                  Enable(void);
   inline void                                  Disable(void);
   
   virtual void                                 OnBeginOfRun(const G4Run * run);
   inline virtual void                          OnEndOfRun(const G4Run * run);
   
  private:
                                                RadmonApplicationRunNumbering(const RadmonApplicationRunNumbering & copy);
   RadmonApplicationRunNumbering &              operator=(const RadmonApplicationRunNumbering & copy);
   
   G4bool                                       enable;
 };
 
 // Inline implementations
 #include "RadmonApplicationRunNumbering.icc"
#endif /* RADMONAPPLICATIONRUNNUMBERING_HH */
