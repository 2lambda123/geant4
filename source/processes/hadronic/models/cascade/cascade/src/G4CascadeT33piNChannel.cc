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
// $Id: G4CascadeT33piNChannel.cc,v 1.3 2010-08-04 05:28:24 mkelsey Exp $
// GEANT4 tag: $Name: not supported by cvs2svn $
//
// 20100612  M. Kelsey -- BUG FIX:  Swapped eight-body channel final-states
// 20100804  M. Kelsey -- Add name string to ctor

#include "G4CascadePiPlusPChannel.hh"
#include "G4CascadePiMinusNChannel.hh"
#include "G4InuclParticleNames.hh"
using namespace G4InuclParticleNames;

namespace {
  // pi+ p : Outgoing particle types of a given multiplicity

  static const G4int pipP2bfs[2][2] =
   {{pip,pro}, {kpl,sp}};

  static const G4int pipP3bfs[7][3] =
   {{pip,pro,pi0}, {pip,neu,pip}, {pi0,sp,kpl}, {pip,sp,k0}, 
    {pip,s0,kpl},   {pip,lam,kpl},  {kpl,pro,k0b}};

  static const G4int pipP4bfs[15][4] =
   {{pip,pro,pip,pim},{pip,pro,pi0,pi0},{pip,neu,pip,pi0},
    {pip,sp,kpl,pim},  {pi0,sp,kpl,pi0},  {pip,sp,k0,pi0},
    {pip,s0,k0,pip},  {pip,s0,kpl,pi0},  {pip,lam,kpl,pi0},
    {pip,lam,k0,pip}, {pip,sm,kpl,pip},  {pip,pro,kpl,kmi},
    {pip,pro,k0,k0b}, {pi0,pro,kpl,k0b}, {pip,neu,kpl,k0b}};

  static const G4int pipP5bfs[24][5] =
   {{pip,pro,pip,pim,pi0}, {pip,pro,pi0,pi0,pi0}, {pip,neu,pip,pip,pim},
    {pip,neu,pip,pi0,pi0}, {pip,sp,kpl,pim,pi0},   {pi0,sp,kpl,pi0,pi0},
    {pip,sp,k0,pip,pim},   {pip,sp,k0,pi0,pi0},   {pip,lam,k0,pip,pi0},
    {pip,lam,kpl,pip,pim},  {pip,lam,kpl,pi0,pi0},  {pip,s0,kpl,pip,pim},
    {pip,s0,kpl,pi0,pi0},   {pip,s0,k0,pip,pi0},   {pip,sm,kpl,pip,pi0},
    {pip,sm,k0,pip,pip},   {pip,pro,pim,kpl,k0b},  {pip,pro,pip,k0,kmi},
    {pip,pro,pi0,kpl,kmi},   {pip,pro,pi0,k0,k0b},  {pi0,pro,pi0,kpl,k0b},
    {pip,neu,pip,kpl,kmi},   {pip,neu,pip,k0,k0b},  {pip,neu,pi0,kpl,k0b}};

  static const G4int pipP6bfs[5][6] =
   {{pip,pro,pip,pip,pim,pim}, {pip,pro,pip,pim,pi0,pi0},
    {pip,pro,pi0,pi0,pi0,pi0}, {pip,neu,pip,pi0,pi0,pi0},
    {pip,neu,pip,pip,pim,pi0}};

  static const G4int pipP7bfs[6][7] =
   {{pip,pro,pip,pip,pim,pim,pi0}, {pip,pro,pip,pim,pi0,pi0,pi0},
    {pip,pro,pi0,pi0,pi0,pi0,pi0}, {pip,neu,pip,pip,pip,pim,pim},
    {pip,neu,pip,pip,pim,pi0,pi0}, {pip,neu,pip,pi0,pi0,pi0,pi0}};

  static const G4int pipP8bfs[7][8] =
   {{pip,pro,pip,pip,pip,pim,pim,pim}, {pip,pro,pip,pip,pim,pim,pi0,pi0},
    {pip,pro,pip,pim,pi0,pi0,pi0,pi0}, {pip,pro,pi0,pi0,pi0,pi0,pi0,pi0},
    {pip,neu,pip,pip,pip,pim,pim,pi0}, {pip,neu,pip,pip,pim,pi0,pi0,pi0},
    {pip,neu,pip,pi0,pi0,pi0,pi0,pi0}};

  static const G4int pipP9bfs[8][9] =
   {{pip,pro,pip,pip,pip,pim,pim,pim,pi0}, {pip,pro,pip,pip,pim,pim,pi0,pi0,pi0},
    {pip,pro,pip,pim,pi0,pi0,pi0,pi0,pi0}, {pip,pro,pi0,pi0,pi0,pi0,pi0,pi0,pi0},
    {pip,neu,pip,pip,pip,pip,pim,pim,pim}, {pip,neu,pip,pip,pip,pim,pim,pi0,pi0},
    {pip,neu,pip,pip,pim,pi0,pi0,pi0,pi0}, {pip,neu,pip,pi0,pi0,pi0,pi0,pi0,pi0}};
}

namespace {
  // pi- n : Outgoing particle types of a given multiplicity

  static const G4int pimN2bfs[2][2] =
   {{pim,neu}, {k0,sm}};

  static const G4int pimN3bfs[7][3] =
   {{pim,neu,pi0}, {pim,pro,pim}, {pi0,sm,k0}, {pim,sm,kpl},
    {pim,s0,k0},   {pim,lam,k0},  {k0,neu,kmi}};

  static const G4int pimN4bfs[15][4] =
   {{pim,neu,pip,pim},{pim,neu,pi0,pi0},{pim,pro,pim,pi0},
    {pim,sm,k0,pip},  {pi0,sm,k0,pi0},  {pim,sm,kpl,pi0},
    {pim,s0,kpl,pim},  {pim,s0,k0,pi0},  {pim,lam,k0,pi0},
    {pim,lam,kpl,pim}, {pim,sp,k0,pim},  {pim,neu,k0,k0b},
    {pim,neu,kpl,kmi},  {pi0,neu,k0,kmi},  {pim,pro,k0,kmi}};

  static const G4int pimN5bfs[24][5] =
   {{pim,neu,pip,pim,pi0}, {pim,neu,pi0,pi0,pi0}, {pim,pro,pip,pim,pim},
    {pim,pro,pim,pi0,pi0}, {pim,sm,k0,pip,pi0},   {pi0,sm,k0,pi0,pi0},
    {pim,sm,kpl,pip,pim},   {pim,sm,kpl,pi0,pi0},   {pim,lam,kpl,pim,pi0},
    {pim,lam,k0,pip,pim},  {pim,lam,k0,pi0,pi0},  {pim,s0,k0,pip,pim},
    {pim,s0,k0,pi0,pi0},   {pim,s0,kpl,pim,pi0},   {pim,sp,k0,pim,pi0},
    {pim,sp,kpl,pim,pim},   {pim,neu,pip,k0,kmi},   {pim,neu,pim,kpl,k0b},
    {pim,neu,pi0,k0,k0b},  {pim,neu,pi0,kpl,kmi},   {pi0,neu,pi0,k0,kmi},
    {pim,pro,pim,k0,k0b},  {pim,pro,pim,kpl,kmi},   {pim,pro,pi0,k0,kmi}};

  static const G4int pimN6bfs[5][6] =
   {{pim,neu,pip,pip,pim,pim}, {pim,neu,pip,pim,pi0,pi0},
    {pim,neu,pi0,pi0,pi0,pi0}, {pim,pro,pim,pi0,pi0,pi0},
    {pim,pro,pip,pim,pim,pi0}};

  static const G4int pimN7bfs[6][7] =
   {{pim,neu,pip,pip,pim,pim,pi0}, {pim,neu,pip,pim,pi0,pi0,pi0},
    {pim,neu,pi0,pi0,pi0,pi0,pi0}, {pim,pro,pip,pip,pim,pim,pim},
    {pim,pro,pip,pim,pim,pi0,pi0}, {pim,pro,pim,pi0,pi0,pi0,pi0}};

  static const G4int pimN8bfs[7][8] =
   {{pim,neu,pip,pip,pip,pim,pim,pim}, {pim,neu,pip,pip,pim,pim,pi0,pi0},
    {pim,neu,pip,pim,pi0,pi0,pi0,pi0}, {pim,neu,pi0,pi0,pi0,pi0,pi0,pi0},
    {pim,pro,pip,pip,pim,pim,pim,pi0}, {pim,pro,pip,pim,pim,pi0,pi0,pi0},
    {pim,pro,pim,pi0,pi0,pi0,pi0,pi0}};

  static const G4int pimN9bfs[8][9] =
   {{pim,neu,pip,pip,pip,pim,pim,pim,pi0}, {pim,neu,pip,pip,pim,pim,pi0,pi0,pi0},
    {pim,neu,pip,pim,pi0,pi0,pi0,pi0,pi0}, {pim,neu,pi0,pi0,pi0,pi0,pi0,pi0,pi0},
    {pim,pro,pip,pip,pip,pim,pim,pim,pim}, {pim,pro,pip,pip,pim,pim,pim,pi0,pi0},
    {pim,pro,pip,pim,pim,pi0,pi0,pi0,pi0}, {pim,pro,pim,pi0,pi0,pi0,pi0,pi0,pi0}};
}

namespace {
  // Total pi+ p cross sections as a function of kinetic energy
  // New cs after 9-body tuning (27 July 09)
  static const G4double pipPtotXSec[30] = 
   {  0.0,   1.2,   2.5,   3.8,   5.0,  7.0,   9.0,  15.0, 30.0,  64.0,
    130.0, 190.0, 130.0,  56.0,  28.0, 17.14, 19.28, 27.4, 40.05, 32.52,
     30.46, 29.0,  27.26, 25.84, 25.5, 24.5,  24.0,  23.5, 23.0,  23.0};

  // pi+ p cross sections as functions of kinetic energy and multiplicity
  static const G4double pipPCrossSections[74][30] = {
  //
  // multiplicity 2 (2 channels)
  //
  //  p pi+ (n pi-)
   {  0.0,   1.2,   2.5,  3.8,  5.0,  7.00, 9.00, 15.0,  30.0,  64.0,
    130.0, 190.0, 130.0, 55.7, 27.2, 13.95, 8.38, 12.98, 18.53, 11.81,
      9.4,   7.7,   6.8,  5.9,  5.3,  4.8,  4.2,   4.0,   3.80,  3.6},

  //  S+ K+ (S- K0)
   {  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.16, 0.62, 0.34,
      0.25, 0.2, 0.1, 0.05, 0.04, 0.02, 0.02, 0.01, 0.01, 0.01},
  //
  // multiplicity 3 (7 channels)
  //
  //  p pi+ pi0 (n pi- pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,   0.0,  0.0,
      0.0, 0.0, 0.0,  0.2,  0.6, 2.39, 8.67, 9.99, 12.36, 6.66,
      4.4, 2.6, 1.95, 1.15, 0.8, 0.6,  0.41, 0.31,  0.24, 0.15},

  //  n pi+ pi+ (p pi- pi-)
   {  0.0, 0.0, 0.0, 0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0, 0.1, 0.20, 0.6, 1.48, 2.30, 3.71, 3.22,
      2.7, 2.1, 1.5, 1.0, 0.74, 0.5, 0.38, 0.27, 0.20, 0.15},

  //  S+ K+ pi0 (S- K0 pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.13,
      0.15, 0.12, 0.09, 0.06, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01},

  //  S+ K0 pi+
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.13,
      0.15, 0.12, 0.09, 0.06, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01},

  //  S0 K+ pi+
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.13,
      0.15, 0.12, 0.09, 0.06, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01},

  //  L K+ pi+
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.13,
      0.15, 0.12, 0.09, 0.06, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01},

  //  p K+ K0bar
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.04,
      0.07, 0.05, 0.04, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01, 0.01},
  //
  // multiplicity 4 (15 channels)
  //
  //  p pi+ pi+ pi-
   {  0.0, 0.0, 0.0,  0.0, 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0, 0.0,  0.06, 0.2, 0.78, 2.26, 3.43,
      3.9, 3.3, 2.85, 2.4, 2.16, 1.7,  1.4, 1.20, 1.00, 0.90},

  //  p pi+ pi0 pi0
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.03, 0.08, 0.15, 1.03, 2.16,
      2.2, 1.7, 1.15, 0.55, 0.41, 0.33, 0.24, 0.19, 0.14, 0.09},

  //  n pi+ pi+ pi0
   {  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0, 0.0,  0.0, 0.0,  0.11, 0.45, 0.95, 1.06, 1.51,
      1.65, 1.5, 1.12, 0.7, 0.43, 0.24, 0.11, 0.05, 0.02, 0.01},

  //  S+ K+ pi+ pi-
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.05, 0.13, 0.09, 0.06, 0.03, 0.02, 0.01, 0.01, 0.0},

  //  S+ K+ pi0 pi0
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.05, 0.13, 0.09, 0.06, 0.03, 0.02, 0.01, 0.01, 0.0},

  //  S+ K0 pi+ pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.02, 0.06, 0.07, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.01},

  //  S0 K0 pi+ pi+
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01},

  //  S0 K+ pi+ pi0
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01},

  //  L K+ pi+ pi0
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.02, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01},

  //  L K0 pi+ pi+
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.02, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01},

  //  S- K+ pi+ pi+
   {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
      0.02, 0.16, 0.18, 0.14, 0.1, 0.06, 0.04, 0.03, 0.03, 0.02},

  //  p pi+ K+ K-
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.02, 0.13, 0.11, 0.10, 0.08, 0.07, 0.06, 0.05, 0.05, 0.04},

  //  p pi+ K0 K0bar
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.01, 0.06, 0.15, 0.14, 0.11, 0.09, 0.07, 0.05, 0.05, 0.04},

  //  p pi0 K+ K0bar
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.01, 0.05, 0.09, 0.08, 0.08, 0.07, 0.06, 0.05, 0.05, 0.04},

  //  n pi+ K+ K0bar
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.01, 0.05, 0.07, 0.06, 0.06, 0.05, 0.04, 0.03, 0.03, 0.02},
  //
  // multiplicity 5 (24 channels)
  //
  //  p pi+ pi+ pi- pi0
   { 0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
     0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,  0.05, 0.31, 2.14,
     3.45, 4.4, 3.5, 2.62, 2.20, 1.8, 1.40, 1.10, 0.89, 0.70},

  //  p pi+ pi0 pi0 pi0
   { 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.03, 0.21,
     0.34, 0.55, 0.4, 0.29, 0.23, 0.18, 0.14, 0.11, 0.09, 0.07},

  //  n pi+ pi+ pi+ pi-
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.01, 0.02, 0.05, 0.20,
     0.4, 0.7, 0.99, 0.92, 0.75, 0.58, 0.42, 0.31, 0.24, 0.18},

  //  n pi+ pi+ pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.01, 0.02, 0.05, 0.20,
     0.4, 0.8, 0.99, 0.92, 0.75, 0.58, 0.42, 0.31, 0.24, 0.18},

  //  S+ K+ pi+ pi- pi0
   { 0.0, 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.02, 0.05, 0.1, 0.08, 0.06, 0.05, 0.04, 0.04, 0.03},

  //  S+ K+ pi0 pi0 pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.04, 0.03, 0.03, 0.02, 0.02, 0.01},

  //  S+ K0 pi+ pi+ pi-
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.04, 0.03, 0.02, 0.02, 0.02, 0.01},

  //  S+ K0 pi+ pi0 pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.04, 0.03, 0.02, 0.02, 0.02, 0.01},

  //  L K0 pi+ pi+ pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.04, 0.05, 0.06, 0.05, 0.05, 0.04, 0.04, 0.03},

  //  L K+ pi+ pi+ pi-
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02},

  //  L K+ pi+ pi0 pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02},

  //  S0 K+ pi+ pi+ pi-
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01},

  //  S0 K+ pi+ pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01},

  //  S0 K0 pi+ pi+ pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01},

  //  S- K+ pi+ pi+ pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.02, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  S- K0 pi+ pi+ pi+
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.02, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  p pi+ pi- K+ K0bar
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.02, 0.06, 0.08, 0.07, 0.06, 0.05, 0.05, 0.04},

  //  p pi+ pi+ K0 K-
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.02, 0.04, 0.05, 0.05, 0.04, 0.03, 0.03, 0.03},

  //  p pi+ pi0 K+ K-
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.02, 0.06, 0.06, 0.05, 0.05, 0.04, 0.04, 0.03, 0.03},
 
  //  p pi+ pi0 K0 K0bar
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.04, 0.05, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03},

  //  p pi0 pi0 K+ K0bar
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.03, 0.04, 0.04, 0.03, 0.03, 0.02, 0.02},

  //  n pi+ pi+ K+ K-
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.02, 0.06, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03, 0.02},

  //  n pi+ pi+ K0 K0bar
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.02, 0.01},

  //  n pi+ pi0 K+ K0bar
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.02, 0.01},
  //
  // multiplicity 6 (5 channels)
  //
  //  p pi+ pi+ pi+ pi- pi-
   { 0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.02,
     0.09, 0.27, 0.35, 0.42, 0.5, 0.44, 0.4, 0.32, 0.29, 0.23},

  //  p pi+ pi+ pi- pi0 pi0
   { 0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.02,
     0.09, 0.27, 0.35, 0.42, 0.5, 0.44, 0.4, 0.32, 0.29, 0.23},

  //  p pi+ pi0 pi0 pi0 pi0
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.02, 0.04, 0.06, 0.08, 0.09, 0.08, 0.08, 0.07, 0.06, 0.05},

  //  n pi+ pi+ pi0 pi0 pi0
   { 0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.02,
     0.09, 0.27, 0.35, 0.42, 0.5, 0.44, 0.4, 0.32, 0.29, 0.23},

  //  n pi+ pi+ pi+ pi- pi0
   { 0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.02,
     0.09, 0.27, 0.35, 0.42, 0.5, 0.44, 0.4, 0.32, 0.29, 0.23},
//
// multiplicity 7 (6 channels)
//
//  p pi+ pi+ pi+ pi- pi- pi0   (measured R 363)
 {  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.02, 0.25, 0.5, 1.00, 0.97, 0.96, 0.9, 0.84, 0.78, 0.72},

//  p pi+ pi+ pi- pi0 pi0 pi0
 {  0.0,  0.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  0.0, 0.0,
    0.0,  0.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  0.0, 0.0,
    0.02, 0.25, 0.5, 0.95, 1.0, 0.9, 0.8, 0.75, 0.7, 0.65},

//  p pi+ pi0 pi0 pi0 pi0 pi0
 {  0.0, 0.0,  0.0, 0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.0, 0.0,  0.0, 0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.0, 0.04, 0.1, 0.24, 0.3, 0.23, 0.2, 0.18, 0.16, 0.14},

//  n pi+ pi+ pi+ pi+ pi- pi-
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.01, 0.04, 0.08, 0.13, 0.24, 0.32, 0.3, 0.26, 0.24, 0.22},

//  n pi+ pi+ pi+ pi- pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.01, 0.04, 0.08, 0.13, 0.3, 0.25, 0.2, 0.18, 0.15, 0.12},

//  n pi+ pi+ pi0 pi0 pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.01, 0.04, 0.08, 0.13, 0.26, 0.23, 0.2, 0.18, 0.15, 0.12},
//
// multiplicity 8 (7 channels)
//
//  p pi+ pi+ pi+ pi+ pi- pi- pi-  (measured R 386)
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.02, 0.03, 0.05, 0.08, 0.13, 0.17, 0.16, 0.15, 0.15, 0.14},

//  p pi+ pi+ pi+ pi- pi- pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.01, 0.03, 0.04, 0.07, 0.13, 0.15, 0.14, 0.12, 0.11, 0.10},

//  p pi+ pi+ pi- pi0 pi0 pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.01, 0.03, 0.05, 0.08, 0.13, 0.17, 0.16, 0.15, 0.15, 0.14},

//  p pi+ pi0 pi0 pi0 pi0 pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
    0.01, 0.02, 0.03, 0.04, 0.06, 0.08, 0.09, 0.08, 0.06, 0.07},

//  n pi+ pi+ pi+ pi+ pi- pi- pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0,  0.0,  0.0,
    0.04, 0.07, 0.12, 0.19, 0.3, 0.42, 0.4, 0.36, 0.36, 0.32},

//  n pi+ pi+ pi+ pi- pi0 pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
    0.04, 0.07, 0.12, 0.19, 0.3, 0.34, 0.32, 0.30, 0.28, 0.26},

//  n pi+ pi+ pi0 pi0 pi0 pi0 pi0
 {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
    0.02, 0.03, 0.05, 0.10, 0.15, 0.21, 0.2, 0.18, 0.18, 0.16},
  //
  // multiplicity 9 (8 channels)
  //
  //  p pi+ pi+ pi+ pi+ pi- pi- pi- pi0  (measured R 383)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.01, 0.02, 0.04, 0.07, 0.12, 0.19, 0.26, 0.36, 0.36, 0.36},

  //  p pi+ pi+ pi+ pi- pi- pi0 pi0 pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.04, 0.08, 0.13, 0.18, 0.24, 0.24, 0.24},

  //  p pi+ pi+ pi- pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.01, 0.02, 0.03, 0.05, 0.07, 0.10, 0.14, 0.14, 0.14},

  //  p pi+ pi0 pi0 pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.02, 0.03, 0.04, 0.06, 0.09, 0.09, 0.09},

  //  n pi+ pi+ pi+ pi+ pi+ pi- pi- pi-   (measured R 446 )
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.02, 0.03, 0.05, 0.08, 0.10, 0.15, 0.15, 0.15},

  //  n pi+ pi+ pi+ pi+ pi- pi- pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.02, 0.03, 0.05, 0.06, 0.10, 0.10, 0.10},

  //  n pi+ pi+ pi+ pi- pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.06},

  //  n pi+ pi+ pi0 pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.01, 0.02, 0.03, 0.04, 0.04, 0.04} };
}

// Initialize both |T Tz> = |3/2 3/2> channels, using pipP cross-section table

G4CascadePiPlusPChannelData::data_t
G4CascadePiPlusPChannelData::data(pipP2bfs, pipP3bfs, pipP4bfs,
				  pipP5bfs, pipP6bfs, pipP7bfs,
				  pipP8bfs, pipP9bfs, pipPCrossSections,
				  pipPtotXSec, "PiPlusP");

G4CascadePiMinusNChannelData::data_t
G4CascadePiMinusNChannelData::data(pimN2bfs, pimN3bfs, pimN4bfs,
				   pimN5bfs, pimN6bfs, pimN7bfs,
				   pimN8bfs, pimN9bfs, pipPCrossSections,
				   pipPtotXSec, "PiMinusN");
