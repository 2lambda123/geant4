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
// $Id: MedLinacDecorator.cc,v 1.1 2004-05-14 18:25:40 mpiergen Exp $
//
//
// Code developed by: M. Piergentili

#include "MedLinacVGeometryComponent.hh"
#include "MedLinacDecorator.hh"

MedLinacDecorator::MedLinacDecorator(MedLinacVGeometryComponent* geoComponent)
  : component(geoComponent)
{
}

MedLinacDecorator::~MedLinacDecorator()
{;}

void MedLinacDecorator::ConstructComponent(G4VPhysicalVolume*,G4VPhysicalVolume*)
{;}

void MedLinacDecorator::DestroyComponent()
{;}

