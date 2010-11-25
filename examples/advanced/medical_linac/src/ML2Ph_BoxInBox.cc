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
// The code was written by :
//	^Claudio Andenna  claudio.andenna@ispesl.it, claudio.andenna@iss.infn.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//	with the contribute of Alessandro Occhigrossi*
//
// ^INAIL DIPIA - ex ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//


#include "ML2Ph_BoxInBox.hh"

CML2Ph_BoxInBox::CML2Ph_BoxInBox()
{// phantom size
	this->halfSize.set(150.*mm,150.*mm,150.*mm);
// phantom position
	this->centre.set(0.*mm,0.*mm,0.*mm);
}

CML2Ph_BoxInBox::~CML2Ph_BoxInBox(void)
{
}
void CML2Ph_BoxInBox::writeInfo()
{
	std::cout<<"\n\n\tcentre of the inside box: " <<this->centreBoxInside/mm<<" [mm]"<< G4endl;
	std::cout<<"\thalf thickness of the inside box: " <<this->halfBoxInside_Thickness/mm<<" [mm]\n"<< G4endl;
}
bool CML2Ph_BoxInBox::Construct(G4VPhysicalVolume *PVWorld, G4int saving_in_ROG_Voxels_every_events, G4int seed, G4String ROGOutFile, G4bool bSaveROG)
{	
	this->PVWorld=PVWorld;


	G4double A, Z;
	A = 1.01*g/mole;
	G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);

	A = 12.011*g/mole;
	G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);  

	A = 16.00*g/mole;
	G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);

	G4double d= 1.18*g/cm3;
	G4int natoms, ncomponents;
	G4Material* PMMA = new G4Material("Polimetilmetacrilato",d,ncomponents=3);
	PMMA->AddElement(elC, natoms=5);
	PMMA->AddElement(elH, natoms=8);
	PMMA->AddElement(elO, natoms=2);

	d= 0.1*g/cm3;
	G4Material* lightWater = new G4Material("lightWater",d,ncomponents=2);
	lightWater->AddElement(elH, natoms=2);
	lightWater->AddElement(elO, natoms=1);



// BOX INSIDE
	G4Material *boxInSideMaterial;

	boxInSideMaterial=PMMA;
	std::cout <<"boxInSideMaterial name "<<boxInSideMaterial->GetName() <<" density "<< boxInSideMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;

	this->centreBoxInside.set(0,0,-50); // the centre of the inside box
	this->halfBoxInside_Thickness=3.*cm; // the half thickness of the inside box

	G4Box *boxInSide=new G4Box("BoxInSide", this->halfBoxInside_Thickness, this->halfBoxInside_Thickness, this->halfBoxInside_Thickness);
	G4LogicalVolume *boxInSideLV=new G4LogicalVolume(boxInSide, boxInSideMaterial, "boxInSideLV");
	G4VPhysicalVolume *boxInSidePV = 0;
	boxInSidePV = new G4PVPlacement(0, centre+this->centreBoxInside,"BoxInsidePV", boxInSideLV,this->PVWorld,false,0,0);

// layer PMMA
	G4Material *layerMaterial=G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS"); // changable
	G4double halfPMMA_Z_Thickness=0.5*cm;
	G4Box *layer=new G4Box("layer", this->halfSize.getX(), this->halfSize.getY(), halfPMMA_Z_Thickness);
	G4LogicalVolume *layerLV = new G4LogicalVolume(layer, layerMaterial, "layerLV");
	G4VPhysicalVolume *layerPV = 0;
	layerPV = new G4PVPlacement(0, centre+G4ThreeVector(0,0,-this->halfSize.getZ()+halfPMMA_Z_Thickness),"layerPV", layerLV,this->PVWorld,false,0,0);

	std::cout <<"layerMaterial name "<<layerMaterial->GetName() <<" density " << layerMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;

// BOX OUTSIDE 
	G4Material *boxOutSideMaterial=G4NistManager::Instance()->FindOrBuildMaterial("G4_LUNG_ICRP"); // changable
	boxOutSideMaterial=lightWater;
	G4double halfBoxOutSide_Thickness=this->halfSize.getZ()-halfPMMA_Z_Thickness;
	G4Box *boxOutSide=new G4Box("BoxOutSide", this->halfSize.getX(), this->halfSize.getY(), halfBoxOutSide_Thickness);

	// boolean logic subtraction between outside box and inside box

	G4SubtractionSolid* OutMinusInBox = new G4SubtractionSolid("OutMinusInBox",	boxOutSide, boxInSide, 0, centreBoxInside-G4ThreeVector(0,0,5));
	G4LogicalVolume *OutMinusInBoxLV = new G4LogicalVolume(OutMinusInBox, boxOutSideMaterial,"OutMinusInBoxLV",0,0,0);
	G4VPhysicalVolume *OutMinusInBoxPV = 0;
	OutMinusInBoxPV = new G4PVPlacement(0, centre+G4ThreeVector(0,0,-this->halfSize.getZ()+2*halfPMMA_Z_Thickness+halfBoxOutSide_Thickness),
					"OutMinusInBoxPV",OutMinusInBoxLV,this->PVWorld,false,0);

	std::cout <<"boxOutSideMaterial name "<<boxOutSideMaterial->GetName() <<" density "<<boxOutSideMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;
 
	// Region for cuts
	G4Region *regVol= new G4Region("BoxInBoxR");
	G4ProductionCuts* cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.01*mm);
	regVol->SetProductionCuts(cuts);


	OutMinusInBoxLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(OutMinusInBoxLV);
	OutMinusInBoxLV->SetUserLimits(new G4UserLimits(0.01*mm));

	layerLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(layerLV);
	layerLV->SetUserLimits(new G4UserLimits(0.01*mm));

	boxInSideLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(boxInSideLV);
	boxInSideLV->SetUserLimits(new G4UserLimits(0.01*mm));

	// Visibility
	G4VisAttributes* simple_PMMA_VisAttWalls= new G4VisAttributes(G4Colour::Gray());
	G4VisAttributes* simple_InBox_VisAttWalls= new G4VisAttributes(G4Colour::Red());
	G4VisAttributes* simple_OutBox_VisAttWalls= new G4VisAttributes(G4Colour::Blue());

	simple_OutBox_VisAttWalls->SetVisibility(true);
// 	simple_OutBox_VisAttWalls->SetForceWireframe(true);
// 	simple_OutBox_VisAttWalls->SetForceAuxEdgeVisible(true);
	simple_OutBox_VisAttWalls->SetLineWidth(2.);
//	simple_OutBox_VisAttWalls->SetForceSolid(true);

	simple_PMMA_VisAttWalls->SetVisibility(true);
// 	simple_PMMA_VisAttWalls->SetForceSolid(true);

	simple_InBox_VisAttWalls->SetVisibility(true);
// 	simple_InBox_VisAttWalls->SetForceSolid(true);


	OutMinusInBoxLV->SetVisAttributes(simple_OutBox_VisAttWalls);
	boxInSideLV->SetVisAttributes(simple_InBox_VisAttWalls);
	layerLV->SetVisAttributes(simple_PMMA_VisAttWalls);

	// Sensitive detector
	this->sensDet=new CML2SDWithVoxels("BoxInBoxPhantom", saving_in_ROG_Voxels_every_events, seed, ROGOutFile, bSaveROG, centre, this->halfSize, 100, 100, 100);
	G4SDManager *SDManager=G4SDManager::GetSDMpointer();
	SDManager->AddNewDetector(this->sensDet);
	
	// Read Out Geometry
	CML2ReadOutGeometry *ROG = new CML2ReadOutGeometry();
	ROG->setBuildData(this->PVWorld->GetFrameTranslation(), this->halfSize, 100, 100, 100);
	ROG->BuildROGeometry();
	this->sensDet->SetROgeometry(ROG);
	OutMinusInBoxLV->SetSensitiveDetector(this->sensDet);
	layerLV->SetSensitiveDetector(this->sensDet);
	boxInSideLV->SetSensitiveDetector(this->sensDet);

	return true;
}
