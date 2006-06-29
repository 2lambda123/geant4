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
// $Id: testG4AtomicShells.cc,v 1.5 2006-06-29 19:13:28 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Test for access to atomic subshell data table, which is the field of
// G4AtomicShells class
//
// 09-11-98 adaptation, MMA
// 28-04-98 implementation of the first version, V. Grichine


#include "G4ios.hh"
#include <iomanip>

#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4AtomicShells.hh"
#include "G4Element.hh"


int main()
{
   G4cout.precision(3);
   G4UnitDefinition::BuildUnitsTable();
//
//direct access to G4AtomicShells
//

   G4int Z, NbOfShells, shell;
   G4double BindingEnergy;

   for (Z=1; Z<=100; Z++)
      {
        G4cout << "\n Atomic Number: " << Z << G4endl; 
        NbOfShells = G4AtomicShells::GetNumberOfShells(Z);
        for (shell=0; shell<NbOfShells; shell++)
           { 
            BindingEnergy = G4AtomicShells::GetBindingEnergy(Z,shell);            
            G4cout << std::setw(12) << G4BestUnit(BindingEnergy, "Energy");
            if ((shell+1)%5 == 0) G4cout << G4endl;
           }
      }      

//
// access via G4Element
//
   G4Element* van = new G4Element("Vanadium","V" ,23.0 ,50.9415*g/mole);
   
   G4cout << "\n Element: " << van->GetName() << G4endl; 
   NbOfShells = van->GetNbOfAtomicShells();
   for (shell=0; shell<NbOfShells; shell++)
      { 
        BindingEnergy = van->GetAtomicShell(shell);            
        G4cout << std::setw(12) << G4BestUnit(BindingEnergy, "Energy");
        if ((shell+1)%5 == 0) G4cout << G4endl;
      }
//
//      
   G4Element xe("Xenon","Xe",54.0,131.29*g/mole);
   G4cout << "\n Element: " << xe.GetName() << G4endl; 
   NbOfShells = xe.GetNbOfAtomicShells();
   for (shell=0; shell<NbOfShells; shell++)
      { 
        BindingEnergy = xe.GetAtomicShell(shell);            
        G4cout << std::setw(12) << G4BestUnit(BindingEnergy, "Energy");
        if ((shell+1)%5 == 0) G4cout << G4endl;
      }
//
//                  
   G4Element* fm  = new G4Element("Fermium" ,"Fm",100.0,257.083*g/mole);
   G4cout << "\n Element: " << fm->GetName() << G4endl; 
   NbOfShells = fm->GetNbOfAtomicShells();
   for (shell=0; shell<NbOfShells; shell++)
      { 
        BindingEnergy = fm->GetAtomicShell(shell);            
        G4cout << std::setw(12) << G4BestUnit(BindingEnergy, "Energy");
        if ((shell+1)%5 == 0) G4cout << G4endl;
      }   
  
   return EXIT_SUCCESS;
}
