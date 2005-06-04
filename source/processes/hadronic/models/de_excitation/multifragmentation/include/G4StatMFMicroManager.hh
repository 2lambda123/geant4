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
// $Id: G4StatMFMicroManager.hh,v 1.2 2005-06-04 13:27:48 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara


#ifndef G4StatMFMicroManager_h
#define G4StatMFMicroManager_h 1

#include "G4StatMFMicroPartition.hh"
#include "G4StatMFParameters.hh"
#include "G4StatMFChannel.hh"

#include "G4Fragment.hh"
#include "Randomize.hh"


class G4StatMFMicroManager {

public:

    // G4StatMFMicroManager class must be initialized with a G4Fragment, multiplicity,
    // free internal energy and the entropy of the compund nucleus.
    G4StatMFMicroManager(const G4Fragment & theFragment, const G4int multiplicity,
			 const G4double FreeIntE, const G4double SCompNuc);

    // destructor
    ~G4StatMFMicroManager();

private:
    // default constructor
    G4StatMFMicroManager() {};


    // copy constructor
    G4StatMFMicroManager(const G4StatMFMicroManager &right);


    // operators
    G4StatMFMicroManager & operator=(const G4StatMFMicroManager & right);

public:
    G4bool operator==(const G4StatMFMicroManager & right) const;
    G4bool operator!=(const G4StatMFMicroManager & right) const;


public:

    // Choice of fragment atomic numbers and charges.
    G4StatMFChannel * ChooseChannel(const G4double A0, const G4double Z0, const G4double MeanT);
	
    G4double GetProbability(void) const {return _WW;}

    void Normalize(const G4double Norm);
	
    G4double GetMeanMultiplicity(void) const {return _MeanMultiplicity; }

    G4double GetMeanTemperature(void) const {return _MeanTemperature; }

    G4double GetMeanEntropy(void) const {return _MeanEntropy; }

private:

    // Initailization method
    void Initialize(const G4Fragment & theFragment, const G4int m,
		    const G4double FreeIntE, const G4double SCompNuc);

    G4bool MakePartition(const G4int k, G4int * ANumbers); 
								


	
// Data members
private:


    // Partitions vector
    std::vector<G4StatMFMicroPartition*> _Partition;
	

    // Statistical weight
    G4double _WW;

    G4double _Normalization;
	
    G4double _MeanMultiplicity;

    G4double _MeanTemperature;
	
    G4double _MeanEntropy;

  struct DeleteFragment 
  {
    template<typename T>
    void operator()(const T* ptr) const
    {
      delete ptr;
    }
  };
  
};

#endif






