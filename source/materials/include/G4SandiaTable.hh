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
// $Id: G4SandiaTable.hh,v 1.12 2004-12-07 08:50:02 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

// class description
//
// This class is an interface to G4StaticSandiaData.
// it provides - Sandia coeff for an element, given its Z
//             - sandia coeff for a material, given a pointer to it
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
//
// History:
//
// 05.03.04 V.Grichine, new methods for old sorting algorithm for PAI model
// 03.04.01 fnulcof[4] added; returned if energy < emin
// 30.01.01 major bug in the computation of AoverAvo and in the units (/g!)
//          in GetSandiaCofPerAtom(). mma
// 18.11.98 simplified public interface; new methods for materials.  mma
// 10.06.97 created. V. Grichine
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#ifndef G4SANDIATABLE_HH
#define G4SANDIATABLE_HH

#include "G4OrderedTable.hh"      
#include "G4ios.hh"
#include "globals.hh"
#include <assert.h>

class G4Material;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

class G4SandiaTable
{
public:  // with description

    G4SandiaTable(G4int);	         
    G4SandiaTable(G4Material*);	         

	         
   ~G4SandiaTable();

    //per atom of an Element:   
    static G4int     GetNbOfIntervals   (G4int Z);
    static G4double  GetSandiaCofPerAtom(G4int Z, G4int, G4int);
    static G4double* GetSandiaCofPerAtom(G4int Z, G4double energy);
    static G4double  GetIonizationPot   (G4int Z);
    static G4double  GetZtoA            (G4int Z);
    
    //per volume of a material:
           G4int     GetMatNbOfIntervals()  {return fMatNbOfIntervals;};
           G4double  GetSandiaCofForMaterial(G4int,G4int);
           G4double* GetSandiaCofForMaterial(G4double energy);
	   

private:
       
    void ComputeMatSandiaMatrix();
    
private:

    static const G4double        fSandiaTable[981][5];
    static const G4int           fNbOfIntervals[101];
    static       G4int           fCumulInterval[101];
    static const G4double        fZtoAratio[101];
    static const G4double        fIonizationPotentials[101];
               
    static       G4double        fSandiaCofPerAtom[4];
    
                 G4Material*     fMaterial;
                 G4int           fMatNbOfIntervals;
                 G4OrderedTable* fMatSandiaMatrix;
		 
		 G4double        fnulcof[4];
		   

/////////////////////////////////////////////////////////////////////
//
// Methods for PAI model

public:  // without description


    inline void SandiaSwap( G4double** da,
                                 G4int i,
                                 G4int j );

    void SandiaSort( G4double** da,
                          G4int sz );

    G4int SandiaIntervals( G4int Z[],
                                G4int el );

    G4int SandiaMixing(       G4int Z[],
                             const G4double fractionW[],
                                   G4int el,
                                   G4int mi );

    inline G4double GetPhotoAbsorpCof(G4int i , G4int j) const;

    inline G4int GetMaxInterval() const { return fMaxInterval;};

    G4OrderedTable* GetSandiaMatTable() const {return fMatSandiaMatrix;};
    inline G4double  GetSandiaMatTable(G4int,G4int) const;

private:

    void ComputeMatTable();


//////////////////////////////////////////////////////////////////////////
//
// data members for PAI model

private:
         static const G4int    fNumberOfElements  ;
         static const G4int    fIntervalLimit ;
         static const G4int    fNumberOfIntervals  ;
		
         // G4double fPhotoAbsorptionCof[101][5] ; // SandiaTable  for mixture

	 G4double** fPhotoAbsorptionCof ;	// SandiaTable  for mixture

         // G4OrderedTable*
	 G4int fMaxInterval ;

//
//
//////////////////////////////////////////////////////////////////////////
  
};
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

///////////////////////////////////////////////////////////////////////
//
// Inline methods for PAI model

inline
void
G4SandiaTable::SandiaSwap( G4double** da ,
                           G4int i,
                           G4int j )
{
  G4double tmp = da[i][0] ;
  da[i][0] = da[j][0] ;
  da[j][0] = tmp ;
}

/////////////////////////////////////////////////////////////////////////
//
//

inline
G4double G4SandiaTable::GetPhotoAbsorpCof(G4int i, G4int j) const
{
   G4double     unitCof ;
   if(j == 0)   unitCof = keV ;
   else         unitCof = (cm2/g)*std::pow(keV,(G4double)j) ;
   
   return  fPhotoAbsorptionCof[i][j]*unitCof ;
}

///////////////////////////////////////////////////////////////////////
//
//

inline G4double  G4SandiaTable::GetSandiaMatTable(G4int interval, G4int j) const
{
   assert (interval >= 0 && interval < fMaxInterval && j >= 0 && j < 5 );
   G4double     unitCof ;
   if(j == 0)   unitCof = keV ;
   else         unitCof = (cm2/g)*std::pow(keV,(G4double)j);                      
   return ( (*(*fMatSandiaMatrix)[interval])[j] )*unitCof; 
}

//
//
////////////////////////////////////////////////////////////////////////////


#endif 

