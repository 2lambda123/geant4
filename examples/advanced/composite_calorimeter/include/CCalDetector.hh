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
///////////////////////////////////////////////////////////////////////////////
// File: CCalDetector.hh
// Description: CCalDetector will provide the basic functionallity of a 
//              detector factory. It has a construct method that describes 
//              the construction sequence of the detector. It has a Name, 
//              a file associated with it, and a collection of other 
//              CCalDetectors that are supposed to be inside this one.
///////////////////////////////////////////////////////////////////////////////
#ifndef CCalDetector_h
#define CCalDetector_h 1

#include <iostream>
#include <vector>
#include "globals.hh"

//Forward declartion for the CCalDetectorTable typedef
class CCalDetector;

//A table to hold a list of pointers to CMS Detectors
typedef std::vector<CCalDetector*> CCalDetectorTable;

////////////////////
//At last the class
class CCalDetector {

  friend std::ostream& operator<<(std::ostream&, const CCalDetector&);

public:
  ////////////////////////////////////////////////////////////////
  //Constructor with a name and a filename.
  CCalDetector(const G4String &name);
  ////////////////////////////////////////////////////////////////
  //Destructor
  virtual ~CCalDetector();


  ////////////////////////////////////////////////////////////////
  // Construction related methods

  //This starts the detector construction
  void constructHierarchy() { construct();}
  void construct();


  ////////////////////////////////////////////////////////////////
  //A method that allows to add a new detector inside this one.
  void addDetector(CCalDetector*);
  

  ////////////////////////////////////////////////////////////////
  //Other get methods
  G4String Name() const {return detectorName;}
  G4String baseFileName() const {return fileName;}
  G4String File() const {return fileName+".geom";}
  CCalDetector* getDaughter(int i) const {return theDetectorsInside[i];}
  int getNDaughters() const {return theDetectorsInside.size();}
  

  ////////////////////////////////////////////////////////////////
  //Local operators
  //Equality only checks name !!!
  G4bool operator==(const CCalDetector& left) const {
    return (detectorName==left.detectorName);
  }
  G4bool operator!=(const CCalDetector& left) const {
    return (detectorName!=left.detectorName);
  }


protected:

  ////////////////////////////////////////////////////////////////
  //Pure Virtual methods  

  //Should read a file and store the information inside the concrete 
  //CCalDetector
  virtual int readFile() = 0;
  //Construct the daughters by calling the apropiate constructors
  virtual void constructDaughters() = 0;

  ////////////////////////////////////////////////////////////////
  //Building related methods  

  //Builds the detector from a file
  int buildFromFile();

protected:

  ////////////////////////////////////////////////////////////////
  //Data Members

  G4String detectorName;                //Detector name
  G4String fileName;                    //File name from it will be read
  static G4String pathName;             //Path in which to look for files
  
  CCalDetectorTable theDetectorsInside; //A collection of CCalDetectors inside

  int constructFlag;                    //True if this detector is to be built
};

#endif



