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
// $Id: G4PVDivision.cc,v 1.9 2003-11-18 12:15:42 arce Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4PVDivision Implementation file
//
// 26.05.03 - P.Arce Initial version
// ********************************************************************

#include "G4PVDivision.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4ParameterisationBox.hh"
#include "G4ParameterisationTubs.hh"
#include "G4ParameterisationCons.hh"
#include "G4ParameterisationTrd.hh"
#include "G4ParameterisationPara.hh"
#include "G4ParameterisationPolycone.hh"
#include "G4ParameterisationPolyhedra.hh"

//--------------------------------------------------------------------------
G4PVDivision::G4PVDivision(const G4String& pName,
                                 G4LogicalVolume* pLogical,
                                 G4LogicalVolume* pMotherLogical,
                           const EAxis pAxis,
                           const G4int nDivs,
                           const G4double width,
                           const G4double offset )
  : G4VPhysicalVolume(0,G4ThreeVector(),pName,pLogical,0),
    fcopyNo(-1)
{
  if (pMotherLogical) pMotherLogical->AddDaughter(this);
  SetParameterisation( pMotherLogical, pAxis, nDivs,
                       width, offset, DivNDIVandWIDTH );
  CheckAndSetParameters (pAxis, nDivs, width, offset, DivNDIVandWIDTH, pMotherLogical );
}

//--------------------------------------------------------------------------
G4PVDivision::G4PVDivision(const G4String& pName,
                                 G4LogicalVolume* pLogical,
                                 G4LogicalVolume* pMotherLogical,
                           const EAxis pAxis,
                           const G4int nDivs,
                           const G4double offset )
  : G4VPhysicalVolume(0,G4ThreeVector(),pName,pLogical,0),
    fcopyNo(-1)
{
  if (pMotherLogical) pMotherLogical->AddDaughter(this);
  SetParameterisation( pMotherLogical, pAxis, nDivs, 0., offset, DivNDIV );
  CheckAndSetParameters (pAxis, nDivs, 0., offset, DivNDIV, pMotherLogical );
}

//--------------------------------------------------------------------------
G4PVDivision::G4PVDivision(const G4String& pName,
                                 G4LogicalVolume* pLogical,
                                 G4LogicalVolume* pMotherLogical,
                           const EAxis pAxis,
                           const G4double width,
                           const G4double offset )
  : G4VPhysicalVolume(0,G4ThreeVector(),pName,pLogical,0),
    fcopyNo(-1)
{
  if (pMotherLogical) pMotherLogical->AddDaughter(this);
  SetParameterisation( pMotherLogical, pAxis, 0, width, offset, DivWIDTH );
  CheckAndSetParameters (pAxis, 0, width, offset, DivWIDTH, pMotherLogical );
}

//--------------------------------------------------------------------------
void G4PVDivision::
CheckAndSetParameters( const EAxis pAxis,
		       const G4int nDivs,
		       const G4double width,
		       const G4double offset, 
		       DivisionType divType,
		       const G4LogicalVolume* pMotherLogical )
{
  if( divType == DivWIDTH )
  {
    fnReplicas = fparam->GetNoDiv();
  }
  else
  {
    fnReplicas = nDivs;
  }
  if (fnReplicas < 1 )
  {
    G4Exception("G4PVDivision::G4PVDivision()", "IllegalConstruct",
                FatalException, "Illegal number of replicas!");
  }

  if( divType != DivNDIV)
  {
    fwidth = fparam->GetWidth();
  }
  else
  {
    fwidth = width;
  }
  if( fwidth < 0 )
  {
    G4Exception("G4PVDivision::G4PVDivision()", "IllegalConstruct",
                FatalException, "Width must be positive!");
  }
  
  foffset=offset;
  
  //!!!!! axis has to be x/y/z in G4VoxelLimits::GetMinExtent
  //
  if( pAxis == kRho || pAxis == kRadial3D || pAxis == kPhi )
  {
    faxis = kZAxis;
  }
  else
  {
    faxis = pAxis;
  }
  
  // Create rotation matrix: for phi axis it will be changed
  // in G4VPVParameterisation::ComputeTransformation, for others
  // it will stay the unity
  //
  G4RotationMatrix *pRMat = new G4RotationMatrix();
  SetRotation(pRMat);
  
  switch (faxis)
  {
    case kPhi:
      break;
    case kRho:
    case kXAxis:
    case kYAxis:
    case kZAxis:
      break;
    default:
      G4Exception("G4PVDivision::G4PVDivision()", "IllegalConstruct",
                  FatalException, "Unknown axis of replication.");
      break;
  }


  //----- Check that mother solid is of the same type than daughter solid (otherwise, the corresponding Parameterisation::ComputeDimension() will not be called)
  G4cout << " GetMotherLogical " << GetMotherLogical() << G4endl;
  G4cout << " GetLogicalVolume " << GetLogicalVolume() << G4endl;
  G4String msolType = pMotherLogical->GetSolid()->GetEntityType();
  G4String dsolType = GetLogicalVolume()->GetSolid()->GetEntityType();
  if( msolType != dsolType )
  {
    G4cerr << "ERROR - G4PVDivision::CheckMotherSolid()"
           << G4endl
           << "        Incorrect solid type in call to G4PVDivision of volume "
           << GetName() << G4endl
           << "        It is: " << msolType
           << ", while it should be: " << dsolType << "." << G4endl;
    G4Exception("G4VDivisionParameterisation::CheckMotherSolid()",
                "IllegalConstruct", FatalException,
                "Incorrect solid type for division !");
  }


}

//--------------------------------------------------------------------------
G4PVDivision::~G4PVDivision()
{
  delete GetRotation();
}

//--------------------------------------------------------------------------
G4bool G4PVDivision::IsParameterised() const
{ 
  return true;
}

//--------------------------------------------------------------------------
G4bool G4PVDivision::IsMany() const
{
  return false; 
}

//--------------------------------------------------------------------------
G4int G4PVDivision::GetCopyNo() const
{
  return fcopyNo;
}

//--------------------------------------------------------------------------
void  G4PVDivision::SetCopyNo(G4int newCopyNo)
{
  fcopyNo= newCopyNo;
}

//--------------------------------------------------------------------------
G4bool G4PVDivision::IsReplicated() const
{
  return true;
}

//--------------------------------------------------------------------------
G4VPVParameterisation* G4PVDivision::GetParameterisation() const
{
  return fparam;
}

//--------------------------------------------------------------------------
void G4PVDivision::GetReplicationData(EAxis& axis,
                                      G4int& nDivs,
                                      G4double& width,
                                      G4double& offset,
                                      G4bool& consuming ) const
{
  axis=faxis;
  nDivs=fnReplicas;
  width=fwidth;
  offset=foffset;
  consuming=false;
}


//--------------------------------------------------------------------------
//TODO: this method should check that the child lv is of the correct type,
//      else the ComputeDimensions will never be called
void G4PVDivision::SetParameterisation( G4LogicalVolume* motherLogical,
                                  const EAxis axis,
                                  const G4int nDivs,
                                  const G4double width,
                                  const G4double offset,
                                        DivisionType divType )
{
  // Check that solid is compatible with mother solid and axis of division   
  // CheckSolid( solid, motherSolid );
  // G4cout << " Axis " << axis << G4endl;

  G4VSolid* mSolid = motherLogical->GetSolid();
  G4String mSolidType = mSolid->GetEntityType();

  // Parameterisation type depend of mother solid type and axis of division
  //
  if( mSolidType == "G4Box" )
  {
    switch( axis )
    {
      case kXAxis:
        fparam = new G4ParameterisationBoxX( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kYAxis:
        fparam = new G4ParameterisationBoxY( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationBoxZ( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
        break;
    }
  }
  else if( mSolidType == "G4Tubs" )
  {
    switch( axis )
    {
      case kRho:
        fparam = new G4ParameterisationTubsRho( axis, nDivs, width,
                                                offset, mSolid, divType );
        break;
      case kPhi:
        fparam = new G4ParameterisationTubsPhi( axis, nDivs, width,
                                                offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationTubsZ( axis, nDivs, width,
                                              offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
        break;
    }
  }
  else if( mSolidType == "G4Cons" )
  {
    switch( axis )
    {
      case kRho:
        fparam = new G4ParameterisationConsRho( axis, nDivs, width,
                                                offset, mSolid, divType );
        break;
      case kPhi:
        fparam = new G4ParameterisationConsPhi( axis, nDivs, width,
                                                offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationConsZ( axis, nDivs, width,
                                              offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
        break;
    }
  }
  else if( mSolidType == "G4Trd" )
  { 
    switch( axis )
    {
      case kXAxis:
        fparam = new G4ParameterisationTrdX( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kYAxis:
        fparam = new G4ParameterisationTrdY( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationTrdZ( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
        break;
    }
  }
  else if( mSolidType == "G4Para" )
  { 
    switch( axis )
    {
      case kXAxis:
        fparam = new G4ParameterisationParaX( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kYAxis:
        fparam = new G4ParameterisationParaY( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationParaZ( axis, nDivs, width,
                                             offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
        break;
    }
  }
//  else if( mSolidType == "G4Trap" )
//  {
//  }
  else if( mSolidType == "G4Polycone" )
  {
    switch( axis )
    {
      case kRho:
        fparam = new G4ParameterisationPolyconeRho( axis, nDivs, width,
                                                    offset, mSolid, divType );
        break;
      case kPhi:
        fparam = new G4ParameterisationPolyconePhi( axis, nDivs, width,
                                                    offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationPolyconeZ( axis, nDivs, width,
                                                  offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
      break;
    }
  }
  else if( mSolidType == "G4Polyhedra" )
  {
    switch( axis )
    {
      case kRho:
        fparam = new G4ParameterisationPolyhedraRho( axis, nDivs, width,
                                                    offset, mSolid, divType );
        break;
      case kPhi:
        fparam = new G4ParameterisationPolyhedraPhi( axis, nDivs, width,
                                                    offset, mSolid, divType );
        break;
      case kZAxis:
        fparam = new G4ParameterisationPolyhedraZ( axis, nDivs, width,
                                                  offset, mSolid, divType );
        break;
      default:
        ErrorInAxis( axis, mSolid );
      break;
    }
  }
  else
  {
    G4cerr << "ERROR - G4PVDivision::SetParameterisation()" << G4endl
           << "        Divisions for " << mSolidType
           << " not implemented." << G4endl;
    G4Exception("G4PVDivision::SetParameterisation()", "IllegalConstruct",
                FatalException, "Solid type not supported.");
  }
}

//--------------------------------------------------------------------------
void G4PVDivision::ErrorInAxis( EAxis axis, G4VSolid* solid )
{
  G4String error = "Trying to divide solid " + solid->GetName()
                 + " of type " + solid->GetEntityType() + " along axis ";
  switch( axis )
  {
    case kXAxis:
      error += "X.";
      break;
    case kYAxis:
      error += "Y.";
      break;
    case kZAxis:
      error += "Z.";
      break;
    case kRho:
      error += "Rho.";
      break;
    case kRadial3D:
      error += "Radial3D.";
      break;
    case kPhi:
      error += "Phi.";
      break;
    default:
      break;
  }
  G4Exception("G4PVDivision::ErrorInAxis()", "IllegalConstruct",
              FatalException, error);
}
