//
// File name:     RadmonLayoutEntityWithAttributes.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonLayoutEntityWithAttributes.hh,v 1.2 2005-10-25 16:40:56 capra Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Provides attributes to other detector classes
//

#ifndef   RADMONLAYOUTENTITYWITHATTRIBUTES_HH
 #define  RADMONLAYOUTENTITYWITHATTRIBUTES_HH
 
 // Include files
 #include "G4String.hh"
 #include "globals.hh"
 #include "G4ThreeVector.hh"

 #include <vector>
 #include <utility>
 
 class RadmonLayoutEntityWithAttributes
 {
  public:
   G4int                                        GetNAttributes(void) const;
   const G4String &                             GetAttributeName(G4int index) const;

   G4String                                     GetAttribute(const G4String & attributeName, const G4String & defaultValue = G4String("")) const;
   G4bool                                       ExistsAttribute(const G4String & attributeName) const;
   void                                         SetAttribute(const G4String & attributeName, const G4String & value);
   void                                         ClearAttribute(const G4String & attributeName);
   void                                         ClearAllAttributes(void);

   G4double                                     GetAttributeAsDouble(const G4String & attributeName, double defaultValue) const;
   G4double                                     GetAttributeAsMeasure(const G4String & attributeName, const char * category, double defaultValue) const;
   G4int                                        GetAttributeAsInteger(const G4String & attributeName, G4int defaultValue) const;
   G4ThreeVector                                GetAttributeAsThreeVector(const G4String & attributeName, const G4ThreeVector & defaultValue) const;
   G4ThreeVector                                GetAttributeAsThreeVectorWithMeasure(const G4String & attributeName, const char * category, const G4ThreeVector & defaultValue) const;
   G4ThreeVector                                GetAttributeAsDirection(const G4String & attributeName, const G4ThreeVector & defaultValue) const;
   
  protected:
   inline                                       RadmonLayoutEntityWithAttributes();
   inline                                       RadmonLayoutEntityWithAttributes(const RadmonLayoutEntityWithAttributes & copy);
   inline                                      ~RadmonLayoutEntityWithAttributes();

   void                                         DumpAttributesLayout(std::ostream & out, const G4String & indent=G4String()) const;

   inline RadmonLayoutEntityWithAttributes &    operator=(const RadmonLayoutEntityWithAttributes & copy);

  private:
  // Private methods
   void                                         CopyFrom(const RadmonLayoutEntityWithAttributes & copy);
   
  // Private attributes
   typedef std::pair<G4String, G4String>        AttributeItem;
   typedef std::vector<AttributeItem>           AttributesVector;
   AttributesVector                             attributesVector;
 };
 
 // Inline implementations
 #include "RadmonLayoutEntityWithAttributes.icc"
#endif /* RADMONLAYOUTENTITYWITHATTRIBUTES_HH */
