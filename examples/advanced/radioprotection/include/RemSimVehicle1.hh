#ifndef RemSimVehicle1_h
#define RemSimVehicle1_h 1

class RemSimVGeometryComponent;
class G4VPhysicalVolume;
class G4Box;
class G4LogicalVolume;
class G4Material;
class RemSimMaterial;
class G4VisAttributes;
class RemSimVehicle1: public RemSimVGeometryComponent
{
public:
  RemSimVehicle1();
  ~RemSimVehicle1();
  void ConstructComponent(G4VPhysicalVolume*);
  void DestroyComponent(); 

private:
  RemSimMaterial* pMaterial;
  G4Box* layer;
  G4LogicalVolume* layer1Log;
  G4VPhysicalVolume* layer1Phys;
  G4LogicalVolume* layer2Log;
  G4VPhysicalVolume* layer2Phys;
  G4VisAttributes* layer1VisAtt;
  G4VisAttributes* layer2VisAtt; 
};
#endif
