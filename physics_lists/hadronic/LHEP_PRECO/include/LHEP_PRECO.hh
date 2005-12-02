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
// $Id: LHEP_PRECO.hh,v 1.2 2005-12-02 17:30:34 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:  LHEP_PRECO
//
// Author: 2002 J.P. Wellisch
//
// Modified:
//
//----------------------------------------------------------------------------
//
#ifndef TLHEP_PRECO_h
#define TLHEP_PRECO_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "CompileTimeConstraints.hh"

template<class T>
class TLHEP_PRECO: public T
{
public:
  TLHEP_PRECO();
  virtual ~TLHEP_PRECO();
  
public:
  // SetCuts() 
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};
#include "LHEP_PRECO.icc"
typedef TLHEP_PRECO<G4VModularPhysicsList> LHEP_PRECO;

// 2002 by J.P. Wellisch

#endif



