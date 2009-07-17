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
#ifndef G4FTFParameters_h
#define G4FTFParameters_h 1
//
// $Id: G4FTFParameters.hh,v 1.3 2009-07-17 12:47:14 vuzhinsk Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "G4Proton.hh"
class G4FTFParameters
{

  public:
  	G4FTFParameters(const G4ParticleDefinition * , G4double theA, 
                                                       G4double theZ, 
                                                       G4double s);

	~G4FTFParameters();

// --------- Set geometrical parameteres -----------------------------
        void SethNcmsEnergy(const G4double s);
        void SetTotalCrossSection(const G4double Xtotal);
        void SetElastisCrossSection(const G4double Xelastic);
        void SetInelasticCrossSection(const G4double Xinelastic);
        void SetProbabilityOfElasticScatt(const G4double Xtotal, const G4double Xelastic);
        void SetProbabilityOfElasticScatt(const G4double aValue);

        void SetRadiusOfHNinteractions2(const G4double Radius2);
        void SetSlope(const G4double Slope);
        void SetGamma0(const G4double Gamma0);

        G4double GammaElastic(const G4double impactsquare) 
        {return (FTFGamma0 * std::exp(-FTFSlope * impactsquare));};

// --------- Set parameters of elastic scattering --------------------
        void SetAvaragePt2ofElasticScattering(const G4double aPt2);

// --------- Set parameters of excitations ---------------------------
        void SetProjMinDiffMass(const G4double aValue); // Uzhi 19.04.08
        void SetProjMinNonDiffMass(const G4double aValue);
        void SetProbabilityOfProjDiff(const G4double aValue);

        void SetTarMinDiffMass(const G4double aValue);  // Uzhi 19.04.08
        void SetTarMinNonDiffMass(const G4double aValue);
        void SetProbabilityOfTarDiff(const G4double aValue);

        void SetAveragePt2(const G4double aValue);
   // Perevod mass*GeV Pt2*GeV*GeV

// --------- Get geometrical parameteres -----------------------------
	G4double GetTotalCrossSection();
	G4double GetElasticCrossSection();
	G4double GetInelasticCrossSection();
        G4double GetProbabilityOfElasticScatt();

        G4double GetSlope();

	G4double GetProbabilityOfInteraction(const G4double impactsquare);
	G4double GetInelasticProbability(const G4double impactsquare);

// --------- Get parameters of elastic scattering --------------------
        G4double GetAvaragePt2ofElasticScattering();

// --------- Get parameters of excitations ---------------------------
        G4double GetProjMinDiffMass(); // Uzhi 19.04.08
        G4double GetProjMinNonDiffMass();
        G4double GetProbabilityOfProjDiff();

        G4double GetTarMinDiffMass();  // Uzhi 19.04.08
        G4double GetTarMinNonDiffMass();
        G4double GetProbabilityOfTarDiff();


        G4double GetAveragePt2();

//  private: 

	G4FTFParameters();

// ------------ Initial energy of hN interactions --------------------	
        G4double FTFhNcmsEnergy;                // Initial hN CMS energy

// ------------ Geometrical parameteres ------------------------------
        G4double FTFXtotal;                     // Total X in mb
        G4double FTFXelastic;                   // Elastic X in mb
        G4double FTFXinelastic;                 // Inelastic X in mb
        G4double ProbabilityOfElasticScatt;     // Xel/Xtot
        G4double RadiusOfHNinteractions2;       // Xtot/pi, in fn^2
        G4double FTFSlope;                      // in fm^-1
        G4double AvaragePt2ofElasticScattering; // in MeV^2
        G4double FTFGamma0;

// --------- Parameters of excitations -------------------------------
        G4double ProjMinDiffMass; // Uzhi 19.04.08
        G4double ProjMinNonDiffMass;
        G4double ProbabilityOfProjDiff;

        G4double TarMinDiffMass;  
        G4double TarMinNonDiffMass;
        G4double ProbabilityOfTarDiff;

        G4double AveragePt2;
};

// --------------------------------------------------------------------
inline  void G4FTFParameters::SethNcmsEnergy(const G4double s)
             {FTFhNcmsEnergy = s;}

// --------- Set geometrical parameteres ------------------------------
inline  void G4FTFParameters::SetTotalCrossSection(const G4double Xtotal)
             {FTFXtotal = Xtotal;}

inline  void G4FTFParameters::SetElastisCrossSection(const G4double Xelastic)
             {FTFXelastic = Xelastic;}

inline  void G4FTFParameters::SetInelasticCrossSection(const G4double Xinelastic)
             {FTFXinelastic = Xinelastic;}

inline  void G4FTFParameters::SetProbabilityOfElasticScatt(const G4double Xtotal, 
                                                           const G4double Xelastic)
                 { 
                  if(Xtotal==0.) {ProbabilityOfElasticScatt = 0.;}
                  else           {ProbabilityOfElasticScatt = Xelastic/Xtotal;};
                 } 

inline  void G4FTFParameters::SetProbabilityOfElasticScatt(const G4double aValue)
             {ProbabilityOfElasticScatt = aValue;}

inline  void G4FTFParameters::SetRadiusOfHNinteractions2(const G4double Radius2)
             {RadiusOfHNinteractions2 = Radius2;}

inline  void G4FTFParameters::SetSlope(const G4double Slope)
             {FTFSlope = 12.84/Slope;} // Slope is in GeV^-2, FTFSlope in fm^-2

inline  void G4FTFParameters::SetGamma0(const G4double Gamma0)
             {FTFGamma0 = Gamma0;}

// --------- Set parameters of elastic scattering ---------------------
inline  void G4FTFParameters::SetAvaragePt2ofElasticScattering(const G4double aPt2)
                 {
//G4cout<<"Pt2 El "<<aPt2<<" "<<std::sqrt(aPt2)<<G4endl;
//G4int Uzhi; G4cin>>Uzhi;
AvaragePt2ofElasticScattering = aPt2;}

// --------- Set parameters of excitations ----------------------------
inline  void G4FTFParameters::SetProjMinDiffMass(const G4double aValue)   // Uzhi 19.04.08
             {ProjMinDiffMass = aValue*GeV;}
inline  void G4FTFParameters::SetProjMinNonDiffMass(const G4double aValue)
             {ProjMinNonDiffMass = aValue*GeV;}
inline  void G4FTFParameters::SetProbabilityOfProjDiff(const G4double aValue)
             {ProbabilityOfProjDiff = aValue;}

inline  void G4FTFParameters::SetTarMinDiffMass(const G4double aValue)  // Uzhi 19.04.08
             {TarMinDiffMass = aValue*GeV;}
inline  void G4FTFParameters::SetTarMinNonDiffMass(const G4double aValue)
             {TarMinNonDiffMass = aValue*GeV;}
inline  void G4FTFParameters::SetProbabilityOfTarDiff(const G4double aValue)
             {ProbabilityOfTarDiff = aValue;}

inline  void G4FTFParameters::SetAveragePt2(const G4double aValue)
             {AveragePt2 = aValue*GeV*GeV;}

// --------- Get geometrical parameteres ------------------------------
inline  G4double G4FTFParameters::GetTotalCrossSection()     {return FTFXtotal;}
inline  G4double G4FTFParameters::GetElasticCrossSection()   {return FTFXelastic;}
inline  G4double G4FTFParameters::GetInelasticCrossSection() {return FTFXinelastic;}

inline  G4double G4FTFParameters::GetSlope()                 {return FTFSlope;}

inline  G4double G4FTFParameters::GetProbabilityOfInteraction(const G4double impactsquare)
                 {
                  if(RadiusOfHNinteractions2 > impactsquare) {return 1.;}
                  else                                       {return 0.;}
                 } 

inline  G4double G4FTFParameters::GetProbabilityOfElasticScatt()
                 {return ProbabilityOfElasticScatt;}

inline  G4double G4FTFParameters::GetInelasticProbability( const G4double impactsquare)
        {
         G4double Gamma = GammaElastic(impactsquare);
         return 2 * Gamma - Gamma *Gamma;
        }

// --------- Get parameters of elastic scattering ---------------------
inline  G4double G4FTFParameters::GetAvaragePt2ofElasticScattering()
                 {return AvaragePt2ofElasticScattering;}

// --------- Get parameters of excitations ---------------------------
inline  G4double G4FTFParameters::GetProjMinDiffMass()        {return ProjMinDiffMass;}
inline  G4double G4FTFParameters::GetProjMinNonDiffMass()     {return ProjMinNonDiffMass;}
inline  G4double G4FTFParameters::GetProbabilityOfProjDiff()  {return ProbabilityOfProjDiff;} 

inline  G4double G4FTFParameters::GetTarMinDiffMass()         {return TarMinDiffMass;}
inline  G4double G4FTFParameters::GetTarMinNonDiffMass()      {return TarMinNonDiffMass;}
inline  G4double G4FTFParameters::GetProbabilityOfTarDiff()   {return ProbabilityOfTarDiff;}

inline  G4double G4FTFParameters::GetAveragePt2()             {return AveragePt2;}

#endif
