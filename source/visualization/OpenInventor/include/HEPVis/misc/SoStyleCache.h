//  27/03/2004 : G.Barrand : creation.

#ifndef HEPVis_SoStyleCache_h
#define HEPVis_SoStyleCache_h

#include <Inventor/nodes/SoGroup.h>

//#include <HEPVis/SbStyle.h>

class SoMaterial;
class SoDrawStyle;
class SoLightModel;
class SoResetTransform;
class SbColor;
 
#define SoStyleCache Geant4_SoStyleCache

class SoStyleCache : public SoGroup {
public:
  SoStyleCache();
protected:
  ~SoStyleCache();
public:
  SoMaterial* getMaterial(const SbColor&,float = 0); 
  SoMaterial* getMaterial(float,float,float,float = 0); 
  //SoDrawStyle* getLineStyle(SbLineStyle,float = 0); 
  SoDrawStyle* getLineStyle(unsigned short = 0xFFFF,float = 0); 
  SoLightModel* getLightModelPhong(); 
  SoLightModel* getLightModelBaseColor(); 
  SoResetTransform* getResetTransform();
private:
  SoGroup* fMaterials;
  SoGroup* fLineStyles;
  SoGroup* fLightModels;
  SoResetTransform* fResetTransform;
};

#endif
