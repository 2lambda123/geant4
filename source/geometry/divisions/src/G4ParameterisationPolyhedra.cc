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
// $Id: G4ParameterisationPolyhedra.cc,v 1.5 2003-11-18 12:15:43 arce Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4ParameterisationPolyhedra Implementation file
//
// 14.10.03 - P.Arce Initial version
//---------------------------------------------------------------------

#include "G4ParameterisationPolyhedra.hh"

#include <iomanip>
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Polyhedra.hh"

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraRho::
G4ParameterisationPolyhedraRho( EAxis axis, G4int nDiv,
                               G4double width, G4double offset,
                               G4VSolid* msolid, DivisionType divType )
  :  G4VDivisionParameterisation( axis, nDiv, width, offset, divType, msolid )
{
  CheckParametersValidity();
  SetType( "DivisionPolyhedraRho" );

  G4Polyhedra* msol = (G4Polyhedra*)(msolid);
  G4PolyhedraHistorical* original_pars = msol->GetOriginalParameters();

  if( divType == DivWIDTH )
  {
    fnDiv = CalculateNDiv( original_pars->Rmax[0]
                         - original_pars->Rmin[0], width, offset );
  }
  else if( divType == DivNDIV )
  {
    G4cout  << " original_pars " << original_pars << G4endl;
    G4cout  << " original_pars rmax size " << original_pars->Rmax << " "
            <<  original_pars->Rmin << G4endl;

    fwidth = CalculateWidth( original_pars->Rmax[0]
                           - original_pars->Rmin[0], nDiv, offset );
  }

  if( verbose >= 1 )
  {
    G4cout << " G4ParameterisationPolyhedraRho - # divisions " << fnDiv
           << " = " << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }
}

//------------------------------------------------------------------------
G4ParameterisationPolyhedraRho::~G4ParameterisationPolyhedraRho()
{
}

//---------------------------------------------------------------------
void G4ParameterisationPolyhedraRho::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH ){
    G4cerr << "!!!WARNING:  Solid " << msol->GetName() << " G4ParameterisationPolyhedraRho::CheckParametersValidity. Division along R will be done with a width different for each solid section, WIDTH will not be used " << G4endl;
  }
  if( foffset != 0. ) {
    G4cerr << "!!!WARNING:  Solid " << msol->GetName() << " G4ParameterisationPolyhedraZ::CheckParametersValidity. Division along  R will be done with a width different for each solid section, OFFSET will not be used " << G4endl;
  }

}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraRho::GetMaxParameter() const{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* original_pars = msol->GetOriginalParameters();
  return original_pars->Rmax[0] - original_pars->Rmin[0];
}


//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraRho::
ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol ) const
{
  //----- translation 
  G4ThreeVector origin(0.,0.,0.);

  //----- set translation 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix: unit
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraRho - copyNo: " << copyNo << G4endl
           << " foffset: " << foffset/deg
           << " - fwidth: " << fwidth/deg << G4endl;
  }
  ChangeRotMatrix( physVol );

  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraRho "
           << copyNo << G4endl
           << " Position: " << origin
           << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraRho::
ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                   const G4VPhysicalVolume* ) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );
  G4int nZplanes = origparamMother->Num_z_planes;

  G4double width = 0.;
  for( G4int ii = 0; ii < nZplanes; ii++ )
  {
    width = CalculateWidth( origparamMother->Rmax[ii]
                          - origparamMother->Rmin[ii], fnDiv, foffset );
    origparam.Rmin[ii] = origparamMother->Rmin[ii]+foffset+width*copyNo;
    origparam.Rmax[ii] = origparamMother->Rmin[ii]+foffset+width*(copyNo+1);
  }

  phedra.SetOriginalParameters(&origparam); // copy values & transfer pointers
  phedra.Reset();                           // reset to new solid parameters

  if( verbose >= -2 )
  {
    G4cout << "G4ParameterisationPolyhedraRho::ComputeDimensions()" << G4endl
           << "-- Parametrised phedra copy-number: " << copyNo << G4endl;
    phedra.DumpInfo();
  }
}

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraPhi::
G4ParameterisationPolyhedraPhi( EAxis axis, G4int nDiv,
                               G4double width, G4double offset,
                               G4VSolid* msolid, DivisionType divType )
  :  G4VDivisionParameterisation( axis, nDiv, width, offset, divType, msolid )
{ 
  CheckParametersValidity();
  SetType( "DivisionPolyhedraPhi" );

  G4Polyhedra* msol = (G4Polyhedra*)(msolid);
  G4double deltaPhi = msol->GetEndPhi() - msol->GetStartPhi();

  if( divType == DivWIDTH )
  {
    fnDiv = CalculateNDiv( deltaPhi, width, offset );
  }
  else if( divType == DivNDIV )
  {
    fwidth = CalculateWidth( deltaPhi, nDiv, offset );
  }

  if( verbose >= 1 )
  {
    G4cout << " G4ParameterisationPolyhedraPhi - # divisions " << fnDiv
           << " = " << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }
}

//------------------------------------------------------------------------
G4ParameterisationPolyhedraPhi::~G4ParameterisationPolyhedraPhi()
{
}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraPhi::GetMaxParameter() const{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  return msol->GetEndPhi() - msol->GetStartPhi();
}


//---------------------------------------------------------------------
void G4ParameterisationPolyhedraPhi::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH ){
    G4cerr << " Solid " << msol->GetName() << " G4ParameterisationPolyhedraPhi::CheckParametersValidity. Division along PHI will be done splitting in the defined numSide, WIDTH will not be used " << G4endl;
  }
  if( foffset != 0. ) {
    G4cerr << " Solid " << msol->GetName() << " G4ParameterisationPolyhedraPhi::CheckParametersValidity. Division along PHI will be done splitting in the defined numSide, OFFSET will not be used " << G4endl;
  }

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();

  if( origparamMother->numSide != fnDiv ) { 
    char msgcha[10];
    gcvt( origparamMother->numSide, 10, msgcha );
    char msgcha2[10];
    gcvt( fnDiv, 10, msgcha2 );
    G4String msgstr = G4String("Division along PHI will be done splitting in the defined numSide, that is the number of division will be ") + G4String(msgcha) + G4String(" instead of ") + G4String(msgcha2); 
    G4Exception("G4ParameterisationPolyhedraPHI::CheckParametersValidity()",
                "IllegalConstruct", FatalException,
                msgstr);
  }
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraPhi::
ComputeTransformation( const G4int copyNo, G4VPhysicalVolume *physVol ) const
{
  //----- translation 
  G4ThreeVector origin(0.,0.,0.); 
  //----- set translation 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix (so that all volumes point to the centre)
  G4double posi = copyNo*fwidth;
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraPhi - position: " << posi/deg
           << G4endl
           << " copyNo: " << copyNo
           << " - fwidth: " << fwidth/deg << G4endl;
  }
  ChangeRotMatrix( physVol, -posi );

  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraPhi " << copyNo
           << G4endl
           << " Position: " << origin << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
}

//--------------------------------------------------------------------------
void
G4ParameterisationPolyhedraPhi::
ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                   const G4VPhysicalVolume* ) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );

  origparam.numSide = 1;
  origparam.Start_angle = origparamMother->Start_angle;
  origparam.Opening_angle = fwidth;

  phedra.SetOriginalParameters(&origparam);  // copy values & transfer pointers
  phedra.Reset();                            // reset to new solid parameters

  if( verbose >= 2 )
  {
    G4cout << "G4ParameterisationPolyhedraPhi::ComputeDimensions()" << G4endl
           << "-- Parametrised phedra copy-number: " << copyNo << G4endl;
    phedra.DumpInfo();
  }
}

//--------------------------------------------------------------------------
G4ParameterisationPolyhedraZ::
G4ParameterisationPolyhedraZ( EAxis axis, G4int nDiv,
                             G4double width, G4double offset,
                             G4VSolid* msolid, DivisionType divType )
  :  G4VDivisionParameterisation( axis, nDiv, width, offset, divType, msolid )
{ 
  CheckParametersValidity();
  SetType( "DivisionPolyhedraZ" );

  G4Polyhedra* msol = (G4Polyhedra*)(msolid);
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  
  if( divType == DivWIDTH )
    {
    fnDiv =
      CalculateNDiv( origparamMother->Z_values[origparamMother->Num_z_planes-1]
		     - origparamMother->Z_values[0] , width, offset );
  }
  else if( divType == DivNDIV )
    {
    fwidth =
      CalculateNDiv( origparamMother->Z_values[origparamMother->Num_z_planes-1]
		     - origparamMother->Z_values[0] , nDiv, offset );
  }
  
  if( verbose >= 1 )
    {
    G4cout << " G4ParameterisationPolyhedraZ - # divisions " << fnDiv << " = "
           << nDiv << G4endl
           << " Offset " << foffset << " = " << offset << G4endl
           << " Width " << fwidth << " = " << width << G4endl;
  }

}

//---------------------------------------------------------------------
G4ParameterisationPolyhedraZ::~G4ParameterisationPolyhedraZ()
{
}

//------------------------------------------------------------------------
G4double G4ParameterisationPolyhedraZ::GetMaxParameter() const
{
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  return origparamMother->Z_values[origparamMother->Num_z_planes-1] - origparamMother->Z_values[0];
}


//---------------------------------------------------------------------
void G4ParameterisationPolyhedraZ::CheckParametersValidity()
{
  G4VDivisionParameterisation::CheckParametersValidity();

  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  if( fDivisionType == DivNDIVandWIDTH || fDivisionType == DivWIDTH ){
    G4cerr << "!!!WARNING:  Solid " << msol->GetName() << " G4ParameterisationPolyhedraZ::CheckParametersValidity. Division along Z will be done splitting in the defined z_planes, WIDTH will not be used " << G4endl;
  }

  if( foffset != 0. ) {
    G4cerr << "!!!WARNING:  Solid " << msol->GetName() << " G4ParameterisationPolyhedraZ::CheckParametersValidity. Division along Z will be done splitting in the defined z_planes, OFFSET will not be used " << G4endl;
  }

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();

  if( origparamMother->Num_z_planes-1 != fnDiv ) { 
    char msgcha[10];
    gcvt( origparamMother->Num_z_planes-1, 10, msgcha );
    char msgcha2[10];
    gcvt( fnDiv, 10, msgcha2 );
    G4String msgstr = G4String("Division along Z will be done splitting in the defined z_planes, that is the number of division will be ") + G4String(msgcha) + G4String(" instead of ") + G4String(msgcha2); 
    G4Exception("G4ParameterisationPolyhedraZ::CheckParametersValidity()",
                "IllegalConstruct", FatalException,
                msgstr);
  }

}

//---------------------------------------------------------------------
void
G4ParameterisationPolyhedraZ::
ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4Polyhedra* msol = (G4Polyhedra*)(GetMotherSolid());

  //----- set translation: along Z axis
  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4double posi = (origparamMother->Z_values[copyNo]
		   + origparamMother->Z_values[copyNo+1])/2;
  G4ThreeVector origin(0.,0.,posi); 
  physVol->SetTranslation( origin );

  //----- calculate rotation matrix: unit
  if( verbose >= 2 )
  {
    G4cout << " G4ParameterisationPolyhedraZ - position: " << posi << G4endl
           << " copyNo: " << copyNo << " - foffset: " << foffset/deg
           << " - fwidth: " << fwidth/deg << G4endl;
  }
  ChangeRotMatrix( physVol );

  if( verbose >= 2 )
  {
    G4cout << std::setprecision(8) << " G4ParameterisationPolyhedraZ "
           << copyNo << G4endl
           << " Position: " << origin << " - Width: " << fwidth
           << " - Axis: " << faxis  << G4endl;
  }
}

//---------------------------------------------------------------------
void
G4ParameterisationPolyhedraZ::
ComputeDimensions( G4Polyhedra& phedra, const G4int copyNo,
                   const G4VPhysicalVolume* ) const
{
  // only for mother number of planes = 2!!
  //
  G4Polyhedra* msol = (G4Polyhedra*)(fmotherSolid);

  G4PolyhedraHistorical* origparamMother = msol->GetOriginalParameters();
  G4PolyhedraHistorical origparam( *origparamMother );

  G4double posi = (origparamMother->Z_values[copyNo]
		   + origparamMother->Z_values[copyNo+1])/2;

  origparam.Num_z_planes = 2;
  origparam.Z_values[0] = origparamMother->Z_values[copyNo] - posi;
  origparam.Z_values[1] = origparamMother->Z_values[copyNo+1] - posi;
  origparam.Rmin[0] = origparamMother->Rmin[copyNo];
  origparam.Rmin[1] = origparamMother->Rmin[copyNo+1];
  origparam.Rmax[0] = origparamMother->Rmax[copyNo];
  origparam.Rmax[1] = origparamMother->Rmax[copyNo+1];

  phedra.SetOriginalParameters(&origparam);  // copy values & transfer pointers
  phedra.Reset();                            // reset to new solid parameters

  if( verbose >= 2 )
  {
    G4cout << "G4ParameterisationPolyhedraZ::ComputeDimensions()" << G4endl
           << "-- Parametrised phedra copy-number: " << copyNo << G4endl;
    phedra.DumpInfo();
  }
}

