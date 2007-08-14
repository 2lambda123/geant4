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
// $Id: G4PSEnergyDeposit3D.hh,v 1.1 2007-08-14 16:51:10 taso Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4PSEnergyDeposit3D_h
#define G4PSEnergyDeposit3D_h 1

#include "G4PSEnergyDeposit.hh"
///////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for 3D scoring energy deposit.
//
// Created: 2007-08-14  Tsukasa ASO
///////////////////////////////////////////////////////////////////////////////


class G4PSEnergyDeposit3D : public G4PSEnergyDeposit
{
   public: // with description
    G4PSEnergyDeposit3D(G4String name,
			G4int nx,G4int ny, G4int nz,
			G4int depX=2, G4int depY=1, G4int depZ=0);
    virtual ~G4PSEnergyDeposit3D();

  protected: // with description
    virtual G4int GetIndex(G4Step*);

  private:
    G4int fNx, fNy, fNz;
    G4int fDepthX, fDepthY, fDepthZ;
};
#endif

