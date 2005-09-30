//
// File name:     RadmonTDetectorVolumesWithGroundDecorator.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonTDetectorVolumesWithGroundDecorator.hh,v 1.2 2005-09-30 08:31:23 capra Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Decorates with a ground plane
//                 - Constructor
//                 - Destructor
//                 - RadmonDetectorLayerVolumesList * GenerateVolumesList(void)
//

#ifndef   RADMONTDETECTORVOLUMESWITHGROUNDDECORATOR_HH
 #define  RADMONTDETECTORVOLUMESWITHGROUNDDECORATOR_HH

 // Forward declaration
 class RadmonVDetectorLabelledEntityConstructor;
 class RadmonDetectorLayerVolumesList;
 class G4Box;
 class G4VisAttributes;

 template <class LayerVolumesComponent>
 class RadmonTDetectorVolumesWithGroundDecorator
 {
  public:
                                                RadmonTDetectorVolumesWithGroundDecorator(const RadmonVDetectorLabelledEntityConstructor * constructor);
                                               ~RadmonTDetectorVolumesWithGroundDecorator();

   RadmonDetectorLayerVolumesList *             GenerateVolumesList(void);

  private:
  // Hidden constructors and operators
                                                RadmonTDetectorVolumesWithGroundDecorator();
                                                RadmonTDetectorVolumesWithGroundDecorator(const  RadmonTDetectorVolumesWithGroundDecorator & copy);
   RadmonTDetectorVolumesWithGroundDecorator &  operator=(const  RadmonTDetectorVolumesWithGroundDecorator & copy);

  // Private attributes
   const RadmonVDetectorLabelledEntityConstructor * owner;
   LayerVolumesComponent                        component;
   G4Box *                                      box;
   G4VisAttributes *                            visAttributes;
 };
 
 // Inline implementations
 #include "RadmonTDetectorVolumesWithGroundDecorator.icc"
#endif /* RADMONTDETECTORVOLUMESWITHGROUNDDECORATOR_HH */
