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
//
// Original author: Zoltan Torzsok, November 2007
//
// --------------------------------------------------------------------

#include "G4GDMLReadDefine.hh"

G4GDMLMatrix::G4GDMLMatrix(size_t rows0,size_t cols0) { 
   
   rows = rows0;
   cols = cols0;
   m = new G4double[rows*cols];
}

G4GDMLMatrix::~G4GDMLMatrix() {
   
   if (m) delete [] m;
}

void G4GDMLMatrix::set(size_t r,size_t c,G4double a) {
   
   if (r>=rows || c>=cols) G4Exception("G4GDMLMatrix: ERROR! Index out of range!");
   m[cols*r+c] = a;
}

G4double G4GDMLMatrix::get(size_t r,size_t c) const {
   
   if (r>=rows || c>=cols) G4Exception("G4GDMLMatrix: ERROR! Index out of range!");
   return m[cols*r+c];
}

G4RotationMatrix G4GDMLReadDefine::getRotationMatrix(const G4ThreeVector& angles) {

   G4RotationMatrix rot;

   rot.rotateX(angles.x());
   rot.rotateY(angles.y());
   rot.rotateZ(angles.z());

   return rot;
}

void G4GDMLReadDefine::constantRead(const xercesc::DOMElement* const constantElement) {

   G4String name;
   G4double value = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = constantElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = attValue; else
      if (attName=="value") value = eval.Evaluate(attValue);
   }

   eval.defineConstant(name,value);
}

void G4GDMLReadDefine::matrixRead(const xercesc::DOMElement* const matrixElement) {

   G4String name;
   G4int coldim=0;
   G4String values;

   const xercesc::DOMNamedNodeMap* const attributes = matrixElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name  = GenerateName(attValue); else
      if (attName=="coldim") coldim = eval.EvaluateInteger(attValue); else
      if (attName=="values") values = attValue;
   }

   std::stringstream MatrixValueStream(values);
   std::vector<G4double> valueList;

   while (!MatrixValueStream.eof()) {
   
      G4String MatrixValue;
      MatrixValueStream >> MatrixValue;
      valueList.push_back(eval.Evaluate(MatrixValue));
   }

   eval.defineMatrix(name,coldim,valueList);
   G4GDMLMatrix* matPtr = new G4GDMLMatrix(valueList.size()/coldim,coldim);

   for (size_t i=0;i<valueList.size();i++)
      matPtr->set(i/coldim,i%coldim,valueList[i]);

   matrixMap[name] = matPtr;
}

void G4GDMLReadDefine::positionRead(const xercesc::DOMElement* const positionElement) {

   G4String name;
   G4double unit = 1.0;
   G4double x = 0.0;
   G4double y = 0.0;
   G4double z = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = positionElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="unit") unit = eval.Evaluate(attValue); else
      if (attName=="x") x = eval.Evaluate(attValue); else
      if (attName=="y") y = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue);
   }

   positionMap[name] = new G4ThreeVector(x*unit,y*unit,z*unit);
}

void G4GDMLReadDefine::rotationRead(const xercesc::DOMElement* const rotationElement) {

   G4String name;
   G4double unit = 1.0;
   G4double x = 0.0;
   G4double y = 0.0;
   G4double z = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = rotationElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="unit") unit = eval.Evaluate(attValue); else
      if (attName=="x") x = eval.Evaluate(attValue); else
      if (attName=="y") y = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue);
   }

   rotationMap[name] = new G4ThreeVector(x*unit,y*unit,z*unit);
}

void G4GDMLReadDefine::scaleRead(const xercesc::DOMElement* const scaleElement) {

   G4String name;
   G4double x = 1.0;
   G4double y = 1.0;
   G4double z = 1.0;

   const xercesc::DOMNamedNodeMap* const attributes = scaleElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="x") x = eval.Evaluate(attValue); else
      if (attName=="y") y = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue);
   }

   scaleMap[name] = new G4ThreeVector(x,y,z);
}

void G4GDMLReadDefine::variableRead(const xercesc::DOMElement* const variableElement) {

   G4String name;
   G4double value = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = variableElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = attValue; else
      if (attName=="value") value = eval.Evaluate(attValue);
   }

   eval.defineVariable(name,value);
}

void G4GDMLReadDefine::quantityRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double unit = 1.0;
   G4double value = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = attValue; else
      if (attName=="value") value = eval.Evaluate(attValue); else
      if (attName=="unit") unit = eval.Evaluate(attValue);
   }

   quantityMap[name] = value*unit;
}

void G4GDMLReadDefine::defineRead(const xercesc::DOMElement* const defineElement) {

   for (xercesc::DOMNode* iter = defineElement->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="constant") constantRead(child); else
      if (tag=="matrix") matrixRead(child); else
      if (tag=="position") positionRead(child); else
      if (tag=="rotation") rotationRead(child); else
      if (tag=="scale") scaleRead(child); else
      if (tag=="variable") variableRead(child); else
      if (tag=="quantity") quantityRead(child); else
      G4Exception("GDML Reader: ERROR! Unknown tag in define: "+tag);
   }
}

void G4GDMLReadDefine::vectorRead(const xercesc::DOMElement* const vectorElement,G4ThreeVector& vec) {

   G4double unit = 1.0;

   const xercesc::DOMNamedNodeMap* const attributes = vectorElement->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="unit") unit = eval.Evaluate(attValue); else
      if (attName=="x") vec.setX(eval.Evaluate(attValue)); else
      if (attName=="y") vec.setY(eval.Evaluate(attValue)); else
      if (attName=="z") vec.setZ(eval.Evaluate(attValue));
   }

   vec *= unit;
}

G4String G4GDMLReadDefine::refRead(const xercesc::DOMElement* const element) {

   G4String ref;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="ref") ref = attValue;
   }

   return ref;
}

G4double G4GDMLReadDefine::getConstant(const G4String& ref) {

   return eval.getConstant(ref);
}

G4double G4GDMLReadDefine::getVariable(const G4String& ref) {

   return eval.getVariable(ref);
}

G4double G4GDMLReadDefine::getQuantity(const G4String& ref) {

   if (quantityMap.find(ref) == quantityMap.end()) G4Exception("GDML Reader: ERROR! Quantity '"+ref+"' was not found!");

   return quantityMap[ref];
}

G4ThreeVector* G4GDMLReadDefine::getPosition(const G4String& ref) {

   if (positionMap.find(ref) == positionMap.end()) G4Exception("GDML Reader: ERROR! Position '"+ref+"' was not found!");

   return positionMap[ref];
}

G4ThreeVector* G4GDMLReadDefine::getRotation(const G4String& ref) {

   if (rotationMap.find(ref) == rotationMap.end()) G4Exception("GDML Reader: ERROR! Rotation '"+ref+"' was not found!");

   return rotationMap[ref];
}

G4ThreeVector* G4GDMLReadDefine::getScale(const G4String& ref) {

   if (scaleMap.find(ref) == scaleMap.end()) G4Exception("GDML Reader: ERROR! Scale '"+ref+"' was not found!");

   return scaleMap[ref];
}

G4GDMLMatrix* G4GDMLReadDefine::getMatrix(const G4String& ref) {

   if (matrixMap.find(ref) == matrixMap.end()) G4Exception("GDML Reader: ERROR! Matrix '"+ref+"' was not found!");

   return matrixMap[ref];
}
