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
// $Id: G4IronStopping.cc,v 1.6 2008-08-27 10:00:03 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

//---------------------------------------------------------------------------
//
// GEANT4 Class file
//
// Description: Data on stopping power
//
// Author:    A.Ivanchenko 10.08.2008
//
// Modifications:
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4IronStopping.hh"
#include "G4EmCorrections.hh"
#include "G4LPhysicsFreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4IronStopping::G4IronStopping(G4EmCorrections* corr, G4bool splineFlag) 
{
  spline = splineFlag;
  Initialise(corr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4IronStopping::~G4IronStopping()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4IronStopping::GetDEDX(G4int idxMaterial, G4double kinEnergy)
{
  G4double res = 0.0;
  if(idxMaterial < 0 || idxMaterial > 15) return res; 
  G4bool b;
  G4double scaledEnergy = kinEnergy/55.847;
  G4double emin = .025*MeV;
  if(scaledEnergy < emin) {
    res = (dedx[idxMaterial])->GetValue(emin, b)*sqrt(scaledEnergy/emin);
  } else {
    res = (dedx[idxMaterial])->GetValue(scaledEnergy, b);
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4IronStopping::GetDEDX(const G4String& NameMaterial, G4double kinEnergy)
{
  return GetDEDX(GetMaterialIndex(NameMaterial), kinEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int
G4IronStopping::GetMaterialIndex(const G4String& NameMaterial)
{
  for (G4int idxMaterial=0; idxMaterial<16; idxMaterial++){
    if(MatName[idxMaterial] == NameMaterial) return idxMaterial;
  }
  return -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4IronStopping::GetDensity(G4int idxMaterial)
{
  if( idxMaterial < 0 || idxMaterial > 15) return 0.0;
  return Density[idxMaterial];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String G4IronStopping::GetMaterialName(G4int idxMaterial)
{
  G4String s = "";
  if( idxMaterial< 0 || idxMaterial> 15) return s;
  return MatName[idxMaterial];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhysicsVector* G4IronStopping::GetPhysicsVector(G4int idxMaterial)
{
  if(idxMaterial< 0 || idxMaterial> 15) return 0; 
  return dedx[idxMaterial];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhysicsVector* 
G4IronStopping::GetPhysicsVector(const G4String& NameMaterial)
{
  return GetPhysicsVector(GetMaterialIndex(NameMaterial));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void G4IronStopping::AddData(G4double* energy, G4double* stoppower, 
				       G4double factor)
{
  G4LPhysicsFreeVector* pv = new G4LPhysicsFreeVector(31,energy[0],energy[30]);
  pv->SetSpline(spline);
  dedx.push_back(pv);
  for(G4int i=0;i<31;i++) {
    pv->PutValues(i,energy[i],stoppower[i]*factor);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4IronStopping::Initialise(G4EmCorrections* corr)
{
G4int i, j=0;
dedx.reserve(1*16);

//..List of ions
G4double factor = 1000.*MeV/cm;
G4double dens[16]={8.988E-05, 1.785E-04, 1.848, 1.9, .00125, .001429, 8.99E-04, 2.699, 2.329, .0017837, 4.508, 7.873, 8.907, 8.933, 5.323, .003743};
G4String NameMaterial[16]={"G4_H", "G4_He", "G4_Be", "G4_C", "G4_N", "G4_O", "G4_Ne", "G4_Al", "G4_Si", "G4_Ar", "G4_Ti", "G4_Fe", "G4_Ni", "G4_Cu", "G4_Ge", "G4_Kr"};
  
for(i=0;i<16;i++){
MatName[i]=NameMaterial[i];
Density[i]=dens[i]*gram/cm3;
}

  //..Reduced energies
G4double E[53] = {.025,.03,.04,.05,.06,.07,0.08,0.09,0.1,0.15,0.2,0.25,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.5,2,2.5,3,4,5,6,7,8,9,10,15,20,25,30,40,50,60,70,80,90,100,150,200,250,300,400,500,600,700,800,900,1000};
for(i=0; i<53; i++) {E[i] *= MeV;}

G4double H_Fe[53] ={2.760E+01, 3.046E+01, 3.563E+01, 4.023E+01, 4.441E+01, 4.828E+01, 5.191E+01, 5.535E+01, 5.862E+01, 7.281E+01, 8.345E+01, 9.110E+01, 9.654E+01, 1.032E+02, 1.068E+02, 1.085E+02, 1.093E+02, 1.095E+02, 1.092E+02, 1.086E+02, 1.041E+02, 9.890E+01, 9.387E+01, 8.923E+01, 8.115E+01, 7.443E+01, 6.878E+01, 6.397E+01, 5.981E+01, 5.619E+01, 5.300E+01, 4.143E+01, 3.416E+01, 2.915E+01, 2.550E+01, 2.053E+01, 1.731E+01, 1.505E+01, 1.338E+01, 1.209E+01, 1.107E+01, 1.024E+01, 7.662E+00, 6.326E+00, 5.510E+00, 4.960E+00, 4.272E+00, 3.864E+00, 3.597E+00, 3.412E+00, 3.279E+00, 3.180E+00, 3.104E+00};
 AddData(E,H_Fe,dens[j]*factor);j++;

G4double He_Fe[53] ={9.071E+00, 9.992E+00, 1.168E+01, 1.322E+01, 1.465E+01, 1.601E+01, 1.731E+01, 1.856E+01, 1.977E+01, 2.534E+01, 3.009E+01, 3.392E+01, 3.691E+01, 4.096E+01, 4.337E+01, 4.482E+01, 4.569E+01, 4.620E+01, 4.645E+01, 4.653E+01, 4.569E+01, 4.406E+01, 4.229E+01, 4.053E+01, 3.730E+01, 3.447E+01, 3.202E+01, 2.988E+01, 2.801E+01, 2.635E+01, 2.489E+01, 1.948E+01, 1.603E+01, 1.365E+01, 1.192E+01, 9.560E+00, 8.040E+00, 6.981E+00, 6.200E+00, 5.601E+00, 5.126E+00, 4.741E+00, 3.553E+00, 2.939E+00, 2.563E+00, 2.310E+00, 1.994E+00, 1.806E+00, 1.684E+00, 1.599E+00, 1.538E+00, 1.493E+00, 1.458E+00};
 AddData(E,He_Fe,dens[j]*factor);j++;

G4double Be_Fe[53] ={7.231E+00, 8.001E+00, 9.429E+00, 1.074E+01, 1.198E+01, 1.315E+01, 1.426E+01, 1.532E+01, 1.632E+01, 2.070E+01, 2.417E+01, 2.687E+01, 2.894E+01, 3.186E+01, 3.377E+01, 3.507E+01, 3.597E+01, 3.657E+01, 3.697E+01, 3.722E+01, 3.722E+01, 3.633E+01, 3.519E+01, 3.398E+01, 3.163E+01, 2.948E+01, 2.756E+01, 2.585E+01, 2.431E+01, 2.294E+01, 2.171E+01, 1.705E+01, 1.402E+01, 1.190E+01, 1.035E+01, 8.254E+00, 6.906E+00, 5.972E+00, 5.289E+00, 4.768E+00, 4.357E+00, 4.025E+00, 3.010E+00, 2.489E+00, 2.172E+00, 1.960E+00, 1.693E+00, 1.535E+00, 1.432E+00, 1.361E+00, 1.310E+00, 1.272E+00, 1.243E+00};
 AddData(E,Be_Fe,dens[j]*factor);j++;

G4double C_Fe[53] ={6.513E+00, 7.256E+00, 8.631E+00, 9.900E+00, 1.109E+01, 1.222E+01, 1.329E+01, 1.431E+01, 1.528E+01, 1.972E+01, 2.371E+01, 2.695E+01, 2.943E+01, 3.276E+01, 3.481E+01, 3.617E+01, 3.708E+01, 3.770E+01, 3.813E+01, 3.842E+01, 3.865E+01, 3.799E+01, 3.701E+01, 3.592E+01, 3.370E+01, 3.160E+01, 2.969E+01, 2.795E+01, 2.638E+01, 2.496E+01, 2.368E+01, 1.874E+01, 1.545E+01, 1.313E+01, 1.142E+01, 9.087E+00, 7.588E+00, 6.550E+00, 5.792E+00, 5.215E+00, 4.761E+00, 4.396E+00, 3.282E+00, 2.714E+00, 2.369E+00, 2.137E+00, 1.848E+00, 1.677E+00, 1.565E+00, 1.488E+00, 1.433E+00, 1.392E+00, 1.361E+00};
 AddData(E,C_Fe,dens[j]*factor);j++;

G4double N_Fe[53] ={7.196E+00, 7.991E+00, 9.457E+00, 1.080E+01, 1.207E+01, 1.328E+01, 1.443E+01, 1.552E+01, 1.657E+01, 2.116E+01, 2.497E+01, 2.817E+01, 3.072E+01, 3.422E+01, 3.634E+01, 3.768E+01, 3.855E+01, 3.912E+01, 3.948E+01, 3.969E+01, 3.965E+01, 3.883E+01, 3.774E+01, 3.657E+01, 3.426E+01, 3.212E+01, 3.017E+01, 2.840E+01, 2.682E+01, 2.538E+01, 2.407E+01, 1.907E+01, 1.573E+01, 1.337E+01, 1.163E+01, 9.250E+00, 7.718E+00, 6.657E+00, 5.883E+00, 5.293E+00, 4.831E+00, 4.458E+00, 3.323E+00, 2.746E+00, 2.396E+00, 2.161E+00, 1.868E+00, 1.694E+00, 1.581E+00, 1.503E+00, 1.447E+00, 1.405E+00, 1.374E+00}; 
 AddData(E,N_Fe,dens[j]*factor);j++;

G4double O_Fe[53] ={5.927E+00, 6.602E+00, 7.858E+00, 9.021E+00, 1.012E+01, 1.118E+01, 1.220E+01, 1.319E+01, 1.415E+01, 1.850E+01, 2.219E+01, 2.529E+01, 2.784E+01, 3.147E+01, 3.372E+01, 3.515E+01, 3.609E+01, 3.671E+01, 3.712E+01, 3.738E+01, 3.753E+01, 3.687E+01, 3.594E+01, 3.491E+01, 3.283E+01, 3.087E+01, 2.906E+01, 2.742E+01, 2.593E+01, 2.457E+01, 2.333E+01, 1.856E+01, 1.535E+01, 1.306E+01, 1.137E+01, 9.051E+00, 7.553E+00, 6.516E+00, 5.757E+00, 5.180E+00, 4.727E+00, 4.362E+00, 3.253E+00, 2.688E+00, 2.346E+00, 2.117E+00, 1.831E+00, 1.661E+00, 1.551E+00, 1.475E+00, 1.420E+00, 1.380E+00, 1.349E+00};
 AddData(E,O_Fe,dens[j]*factor);j++;

G4double Ne_Fe[53] ={4.522E+00, 5.012E+00, 5.923E+00, 6.771E+00, 7.576E+00, 8.354E+00, 9.113E+00, 9.858E+00, 1.059E+01, 1.406E+01, 1.714E+01, 1.983E+01, 2.214E+01, 2.574E+01, 2.821E+01, 2.989E+01, 3.102E+01, 3.180E+01, 3.233E+01, 3.270E+01, 3.324E+01, 3.287E+01, 3.221E+01, 3.141E+01, 2.975E+01, 2.812E+01, 2.659E+01, 2.518E+01, 2.389E+01, 2.270E+01, 2.161E+01, 1.733E+01, 1.440E+01, 1.229E+01, 1.072E+01, 8.553E+00, 7.146E+00, 6.167E+00, 5.451E+00, 4.906E+00, 4.478E+00, 4.133E+00, 3.084E+00, 2.552E+00, 2.229E+00, 2.012E+00, 1.742E+00, 1.582E+00, 1.479E+00, 1.407E+00, 1.356E+00, 1.318E+00, 1.289E+00};
 AddData(E,Ne_Fe,dens[j]*factor);j++;

G4double Al_Fe[53] ={5.170E+00, 5.727E+00, 6.746E+00, 7.678E+00, 8.549E+00, 9.369E+00, 1.015E+01, 1.089E+01, 1.160E+01, 1.473E+01, 1.729E+01, 1.934E+01, 2.100E+01, 2.358E+01, 2.550E+01, 2.697E+01, 2.809E+01, 2.892E+01, 2.953E+01, 2.997E+01, 3.072E+01, 3.045E+01, 2.984E+01, 2.912E+01, 2.760E+01, 2.611E+01, 2.472E+01, 2.344E+01, 2.227E+01, 2.120E+01, 2.020E+01, 1.629E+01, 1.359E+01, 1.163E+01, 1.017E+01, 8.131E+00, 6.803E+00, 5.876E+00, 5.196E+00, 4.677E+00, 4.270E+00, 3.941E+00, 2.942E+00, 2.434E+00, 2.127E+00, 1.921E+00, 1.664E+00, 1.512E+00, 1.413E+00, 1.345E+00, 1.296E+00, 1.260E+00, 1.233E+00};
 AddData(E,Al_Fe,dens[j]*factor);j++;

G4double Si_Fe[53] ={5.179E+00, 5.792E+00, 6.906E+00, 7.894E+00, 8.779E+00, 9.583E+00, 1.033E+01, 1.105E+01, 1.176E+01, 1.509E+01, 1.774E+01, 1.979E+01, 2.143E+01, 2.390E+01, 2.572E+01, 2.711E+01, 2.818E+01, 2.901E+01, 2.964E+01, 3.011E+01, 3.100E+01, 3.079E+01, 3.022E+01, 2.950E+01, 2.798E+01, 2.650E+01, 2.511E+01, 2.382E+01, 2.264E+01, 2.155E+01, 2.056E+01, 1.661E+01, 1.387E+01, 1.189E+01, 1.039E+01, 8.322E+00, 6.967E+00, 6.020E+00, 5.324E+00, 4.794E+00, 4.377E+00,  4.040E+00, 3.017E+00, 2.497E+00, 2.182E+00, 1.971E+00, 1.708E+00, 1.552E+00, 1.451E+00, 1.381E+00, 1.331E+00, 1.295E+00, 1.266E+00};
 AddData(E,Si_Fe,dens[j]*factor);j++;

G4double Ar_Fe[53] ={4.640E+00, 5.184E+00, 6.169E+00, 7.042E+00, 7.836E+00, 8.580E+00, 9.290E+00, 9.972E+00, 1.063E+01, 1.362E+01, 1.604E+01, 1.796E+01, 1.949E+01, 2.168E+01, 2.315E+01, 2.417E+01, 2.491E+01, 2.545E+01, 2.586E+01, 2.617E+01, 2.681E+01, 2.669E+01, 2.625E+01, 2.567E+01, 2.438E+01, 2.311E+01, 2.191E+01, 2.081E+01, 1.979E+01, 1.886E+01, 1.801E+01, 1.461E+01, 1.225E+01, 1.052E+01, 9.221E+00, 7.405E+00, 6.211E+00, 5.374E+00, 4.757E+00, 4.286E+00, 3.914E+00,  3.615E+00, 2.701E+00, 2.236E+00, 1.955E+00, 1.766E+00, 1.530E+00, 1.391E+00, 1.301E+00, 1.239E+00, 1.194E+00, 1.161E+00, 1.136E+00};
 AddData(E,Ar_Fe,dens[j]*factor);j++;

G4double Ti_Fe[53] ={3.463E+00, 3.857E+00, 4.576E+00, 5.231E+00, 5.840E+00, 6.417E+00, 6.971E+00, 7.510E+00, 8.037E+00, 1.056E+01, 1.286E+01, 1.484E+01, 1.650E+01, 1.899E+01, 2.064E+01, 2.176E+01, 2.255E+01, 2.312E+01, 2.353E+01, 2.385E+01, 2.452E+01, 2.450E+01, 2.420E+01, 2.377E+01, 2.273E+01, 2.165E+01, 2.060E+01, 1.962E+01, 1.870E+01, 1.786E+01, 1.708E+01, 1.396E+01, 1.175E+01, 1.014E+01, 8.910E+00, 7.185E+00, 6.043E+00, 5.239E+00, 4.644E+00, 4.189E+00, 3.829E+00, 3.538E+00, 2.651E+00, 2.198E+00, 1.923E+00, 1.738E+00, 1.508E+00, 1.372E+00, 1.284E+00, 1.223E+00, 1.180E+00, 1.148E+00, 1.123E+00};
 AddData(E,Ti_Fe,dens[j]*factor);j++;

G4double Fe_Fe[53] ={2.849E+00, 3.182E+00, 3.799E+00, 4.367E+00, 4.900E+00, 5.406E+00, 5.896E+00, 6.375E+00, 6.847E+00, 9.117E+00, 1.120E+01, 1.305E+01, 1.461E+01, 1.702E+01, 1.875E+01, 2.000E+01, 2.088E+01, 2.151E+01, 2.197E+01, 2.230E+01, 2.301E+01, 2.302E+01, 2.277E+01, 2.239E+01, 2.148E+01, 2.052E+01, 1.959E+01, 1.870E+01, 1.786E+01, 1.709E+01, 1.637E+01, 1.345E+01, 1.138E+01, 9.844E+00, 8.673E+00, 7.018E+00, 5.918E+00, 5.139E+00, 4.562E+00, 4.119E+00, 3.768E+00, 3.484E+00, 2.615E+00, 2.171E+00, 1.901E+00, 1.720E+00, 1.493E+00, 1.360E+00, 1.273E+00, 1.213E+00, 1.171E+00, 1.139E+00, 1.115E+00};
 AddData(E,Fe_Fe,dens[j]*factor);j++;

G4double Ni_Fe[53] ={2.634E+00, 2.937E+00, 3.504E+00, 4.033E+00, 4.536E+00, 5.017E+00, 5.480E+00, 5.930E+00, 6.372E+00, 8.530E+00, 1.058E+01, 1.247E+01, 1.412E+01, 1.669E+01, 1.851E+01, 1.983E+01, 2.078E+01, 2.147E+01, 2.197E+01, 2.234E+01, 2.310E+01, 2.311E+01, 2.285E+01, 2.247E+01, 2.156E+01, 2.061E+01, 1.969E+01, 1.881E+01, 1.798E+01, 1.721E+01, 1.650E+01, 1.359E+01, 1.151E+01, 9.971E+00, 8.793E+00, 7.127E+00, 6.015E+00, 5.227E+00, 4.643E+00, 4.193E+00, 3.838E+00, 3.550E+00, 2.666E+00, 2.214E+00, 1.939E+00, 1.754E+00, 1.524E+00, 1.387E+00, 1.299E+00, 1.238E+00, 1.195E+00, 1.163E+00, 1.138E+00};
 AddData(E,Ni_Fe,dens[j]*factor);j++;

G4double Cu_Fe[53] ={2.375E+00, 2.651E+00, 3.165E+00, 3.642E+00, 4.090E+00, 4.517E+00, 4.929E+00, 5.331E+00, 5.729E+00, 7.707E+00, 9.605E+00, 1.135E+01, 1.289E+01, 1.528E+01, 1.699E+01, 1.823E+01, 1.915E+01, 1.982E+01, 2.031E+01, 2.066E+01, 2.143E+01, 2.148E+01, 2.125E+01, 2.092E+01, 2.010E+01, 1.924E+01, 1.839E+01, 1.759E+01, 1.683E+01, 1.612E+01, 1.545E+01, 1.276E+01, 1.082E+01, 9.381E+00, 8.279E+00, 6.716E+00, 5.673E+00, 4.933E+00, 4.383E+00, 3.963E+00, 3.625E+00, 3.354E+00, 2.521E+00, 2.094E+00, 1.835E+00, 1.660E+00, 1.443E+00, 1.314E+00, 1.231E+00, 1.173E+00, 1.132E+00, 1.102E+00, 1.079E+00};
 AddData(E,Cu_Fe,dens[j]*factor);j++;

G4double Ge_Fe[53] ={2.136E+00, 2.387E+00, 2.854E+00, 3.284E+00, 3.678E+00, 4.043E+00, 4.391E+00, 4.730E+00, 5.064E+00, 6.677E+00, 8.224E+00, 9.597E+00, 1.079E+01, 1.278E+01, 1.434E+01, 1.557E+01, 1.651E+01, 1.723E+01, 1.777E+01, 1.819E+01, 1.913E+01, 1.925E+01, 1.910E+01, 1.883E+01, 1.814E+01, 1.740E+01, 1.667E+01, 1.597E+01, 1.531E+01, 1.469E+01, 1.410E+01, 1.171E+01, 9.967E+00, 8.666E+00, 7.664E+00, 6.237E+00, 5.279E+00, 4.597E+00, 4.090E+00, 3.699E+00, 3.389E+00, 3.137E+00, 2.363E+00, 1.966E+00, 1.723E+00, 1.561E+00, 1.358E+00, 1.237E+00, 1.160E+00, 1.106E+00, 1.068E+00, 1.040E+00, 1.018E+00};
 AddData(E,Ge_Fe,dens[j]*factor);j++;

G4double Kr_Fe[53] ={2.888E+00, 3.222E+00, 3.838E+00, 4.386E+00, 4.875E+00, 5.326E+00, 5.754E+00, 6.164E+00, 6.563E+00, 8.427E+00, 9.958E+00, 1.119E+01, 1.221E+01, 1.383E+01, 1.508E+01, 1.608E+01, 1.689E+01, 1.753E+01, 1.802E+01, 1.840E+01, 1.922E+01, 1.926E+01, 1.903E+01, 1.870E+01, 1.792E+01, 1.712E+01, 1.635E+01, 1.563E+01, 1.496E+01, 1.433E+01, 1.375E+01, 1.139E+01, 9.689E+00, 8.425E+00, 7.454E+00, 6.071E+00, 5.143E+00, 4.482E+00, 3.990E+00, 3.610E+00, 3.309E+00, 3.064E+00, 2.309E+00, 1.921E+00, 1.685E+00, 1.526E+00, 1.327E+00, 1.209E+00, 1.133E+00, 1.081E+00, 1.044E+00, 1.016E+00, 9.949E-01};
 AddData(E,Kr_Fe,dens[j]*factor);j++;

if(corr) {
  for(j=0; j<16; j++) {
    corr->AddStoppingData(26, 56, NameMaterial[j], dedx[j]);
  }
}
}



