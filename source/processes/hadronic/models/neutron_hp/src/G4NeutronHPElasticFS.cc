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
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4NeutronHPElasticFS.hh"
#include "G4ReactionProduct.hh"
#include "G4Nucleus.hh"
#include "G4Proton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"
#include "G4ParticleTable.hh"
#include "G4NeutronHPDataUsed.hh"

  void G4NeutronHPElasticFS::Init (G4double A, G4double Z, G4String & dirName, G4String & )
  {
    G4String tString = "/FS/";
    G4bool dbool;
    G4NeutronHPDataUsed aFile = theNames.GetName(static_cast<G4int>(A), static_cast<G4int>(Z), dirName, tString, dbool);
    G4String filename = aFile.GetName();
    theBaseA = aFile.GetA();
    theBaseZ = aFile.GetZ();
    if(!dbool)
    {
      hasAnyData = false;
      hasFSData = false; 
      hasXsec = false;
      return;
    }
    std::ifstream theData(filename, std::ios::in);
    theData >> repFlag >> targetMass >> frameFlag;
    if(repFlag==1)
    {
      G4int nEnergy;
      theData >> nEnergy; 
      theCoefficients = new G4NeutronHPLegendreStore(nEnergy);
      theCoefficients->InitInterpolation(theData);
      G4double temp, energy;
      G4int tempdep, nLegendre;
      G4int i, ii;
      for (i=0; i<nEnergy; i++)
      {
        theData >> temp >> energy >> tempdep >> nLegendre;
        energy *=eV;
        theCoefficients->Init(i, energy, nLegendre);
        theCoefficients->SetTemperature(i, temp);
        G4double coeff=0;
        for(ii=0; ii<nLegendre; ii++)
        {
          // load legendre coefficients.
          theData >> coeff;
          theCoefficients->SetCoeff(i, ii+1, coeff); // @@@HPW@@@
        }
      }
    }
    else if (repFlag==2)
    {
      G4int nEnergy;
      theData >> nEnergy;
      theProbArray = new G4NeutronHPPartial(nEnergy, nEnergy);
      theProbArray->InitInterpolation(theData);
      G4double temp, energy;
      G4int tempdep, nPoints;
      for(G4int i=0; i<nEnergy; i++)
      {
        theData >> temp >> energy >> tempdep >> nPoints;
        energy *= eV;
        theProbArray->InitInterpolation(i, theData);
        theProbArray->SetT(i, temp);
        theProbArray->SetX(i, energy);
        G4double prob, costh;
        for(G4int ii=0; ii<nPoints; ii++)
        {
          // fill probability arrays.
          theData >> costh >> prob;
          theProbArray->SetX(i, ii, costh);
          theProbArray->SetY(i, ii, prob);
        }
      }
    }
    else if (repFlag==0)
    {
      theData >> frameFlag;
    }
    else
    {
      G4cout << "unusable number for repFlag: repFlag="<<repFlag<<G4endl;
      throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPElasticFS::Init -- unusable number for repFlag");
    }
    theData.close();
  }
  G4HadFinalState * G4NeutronHPElasticFS::ApplyYourself(const G4HadProjectile & theTrack)
  {  
//    G4cout << "G4NeutronHPElasticFS::ApplyYourself+"<<G4endl;
    theResult.Clear();
    G4double eKinetic = theTrack.GetKineticEnergy();
    const G4HadProjectile *incidentParticle = &theTrack;
    G4ReactionProduct theNeutron( const_cast<G4ParticleDefinition *>(incidentParticle->GetDefinition()) );
    theNeutron.SetMomentum( incidentParticle->Get4Momentum().vect() );
    theNeutron.SetKineticEnergy( eKinetic );
//    G4cout << "G4NeutronHPElasticFS::ApplyYourself++"<<eKinetic<<" "<<G4endl;
//    G4cout << "CMSVALUES 0 "<<theNeutron.GetTotalMomentum()<<G4endl;
    
    G4ReactionProduct theTarget; 
    G4Nucleus aNucleus;
    G4ThreeVector neuVelo = (1./incidentParticle->GetDefinition()->GetPDGMass())*theNeutron.GetMomentum();
    theTarget = aNucleus.GetBiasedThermalNucleus( targetMass, neuVelo, theTrack.GetMaterial()->GetTemperature());
//     G4cout << "Nucleus-test"<<" "<<targetMass<<" ";
//     G4cout << theTarget.GetMomentum().x()<<" ";
//     G4cout << theTarget.GetMomentum().y()<<" ";
//     G4cout << theTarget.GetMomentum().z()<<G4endl;
    
    // neutron and target defined as reaction products.

// prepare lorentz-transformation to Lab.

    G4ThreeVector the3Neutron = theNeutron.GetMomentum();
    G4double nEnergy = theNeutron.GetTotalEnergy();
    G4ThreeVector the3Target = theTarget.GetMomentum();
//    cout << "@@@" << the3Target<<G4endl;
    G4double tEnergy = theTarget.GetTotalEnergy();
    G4ReactionProduct theCMS;
    G4double totE = nEnergy+tEnergy;
    G4ThreeVector the3CMS = the3Target+the3Neutron;
    theCMS.SetMomentum(the3CMS);
    G4double cmsMom = std::sqrt(the3CMS*the3CMS);
    G4double sqrts = std::sqrt((totE-cmsMom)*(totE+cmsMom));
    theCMS.SetMass(sqrts);
    theCMS.SetTotalEnergy(totE);
    
    // data come as fcn of n-energy in nuclear rest frame
    G4ReactionProduct boosted;
    boosted.Lorentz(theNeutron, theTarget);
    eKinetic = boosted.GetKineticEnergy(); // get kinetic energy for scattering
    G4double cosTh = -2;
    if(repFlag == 1)
    {
      cosTh = theCoefficients->SampleElastic(eKinetic);
    }
    
    else if (repFlag==2)
    {
      cosTh = theProbArray->Sample(eKinetic);
    }
    else if (repFlag==0)
    {
      cosTh = 2.*G4UniformRand()-1.;
    }
    else
    {
      G4cout << "unusable number for repFlag: repFlag="<<repFlag<<G4endl;
      throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPElasticFS::Init -- unusable number for repFlag");
    }
    if(cosTh<-1.1) return NULL;
    G4double phi = twopi*G4UniformRand();
    G4double theta = std::acos(cosTh);
    G4double sinth = std::sin(theta);
    if (frameFlag == 1) // final state data given in target rest frame.
    {
      // we have the scattering angle, now we need the energy, then do the
      // boosting.
      // relativistic elastic scattering energy angular correlation:
      theNeutron.Lorentz(theNeutron, theTarget);
      G4double e0 = theNeutron.GetTotalEnergy();
      G4double p0 = theNeutron.GetTotalMomentum();
      G4double mN = theNeutron.GetMass();
      G4double mT = theTarget.GetMass();
      G4double eE = e0+mT;
      G4double ap = (mT+eE)*(mT-eE) + (p0+mN)*(p0-mN);
      G4double a = 4*(eE+p0*cosTh)*(eE-p0*cosTh);
      G4double b = 4*ap*p0*cosTh;
      G4double c = (2.*eE*mN-ap)*(2.*eE*mN+ap);
      G4double en = (-b+std::sqrt(b*b - 4*a*c) )/(2*a);
      G4ThreeVector tempVector(en*sinth*std::cos(phi), en*sinth*std::sin(phi), en*std::cos(theta) );
      theNeutron.SetMomentum(tempVector);
      theNeutron.SetTotalEnergy(std::sqrt(en*en+theNeutron.GetMass()*theNeutron.GetMass()));
      // first to lab     
      theNeutron.Lorentz(theNeutron, -1.*theTarget);
      // now to CMS
      theNeutron.Lorentz(theNeutron, theCMS);
      theTarget.SetMomentum(-theNeutron.GetMomentum());
      theTarget.SetTotalEnergy(theNeutron.GetTotalEnergy());
      // and back to lab
      theNeutron.Lorentz(theNeutron, -1.*theCMS);
      theTarget.Lorentz(theTarget, -1.*theCMS);      
    }
    else if (frameFlag == 2) // CMS
    {
      theNeutron.Lorentz(theNeutron, theCMS);
      theTarget.Lorentz(theTarget, theCMS);
      G4double en = theNeutron.GetTotalMomentum(); // already in CMS.
      G4ThreeVector cmsMom=theNeutron.GetMomentum(); // for neutron direction in CMS
      G4double cms_theta=cmsMom.theta();
      G4double cms_phi=cmsMom.phi();
      G4ThreeVector tempVector;
      tempVector.setX(std::cos(theta)*std::sin(cms_theta)*std::cos(cms_phi)
                      +std::sin(theta)*std::cos(phi)*std::cos(cms_theta)*std::cos(cms_phi)
                      -std::sin(theta)*std::sin(phi)*std::sin(cms_phi)  );
      tempVector.setY(std::cos(theta)*std::sin(cms_theta)*std::sin(cms_phi)
                      +std::sin(theta)*std::cos(phi)*std::cos(cms_theta)*std::sin(cms_phi)
                      +std::sin(theta)*std::sin(phi)*std::cos(cms_phi)  );
      tempVector.setZ(std::cos(theta)*std::cos(cms_theta)
                      -std::sin(theta)*std::cos(phi)*std::sin(cms_theta)  );
      tempVector *= en;
      theNeutron.SetMomentum(tempVector);
      theTarget.SetMomentum(-tempVector);
      G4double tP = theTarget.GetTotalMomentum();
      G4double tM = theTarget.GetMass();
      theTarget.SetTotalEnergy(std::sqrt((tP+tM)*(tP+tM)-2.*tP*tM));
      theNeutron.Lorentz(theNeutron, -1.*theCMS);
      theTarget.Lorentz(theTarget, -1.*theCMS);
    }
    else
    {
      G4cout <<"Value of frameFlag (1=LAB, 2=CMS): "<<frameFlag;
      throw G4HadronicException(__FILE__, __LINE__, "G4NeutronHPElasticFS::ApplyYourSelf frameflag incorrect");
    }
    // now all in Lab
// nun den recoil generieren...und energy change, momentum change angeben.
    theResult.SetEnergyChange(theNeutron.GetKineticEnergy());
    theResult.SetMomentumChange(theNeutron.GetMomentum().unit());
    G4DynamicParticle* theRecoil = new G4DynamicParticle;
    if(targetMass<4.5)
    {
      if(targetMass<1)
      {
        // proton
        theRecoil->SetDefinition(G4Proton::Proton());
      }
      else if(targetMass<2 )
      {
        // deuteron
        theRecoil->SetDefinition(G4Deuteron::Deuteron());
      }
      else if(targetMass<2.999 )
      {
        // 3He 
        theRecoil->SetDefinition(G4He3::He3());
      }
      else if(targetMass<3 )
      {
        // Triton
        theRecoil->SetDefinition(G4Triton::Triton());
      }
      else
      {
        // alpha
        theRecoil->SetDefinition(G4Alpha::Alpha());
      }
    }
    else
    {
      theRecoil->SetDefinition(G4ParticleTable::GetParticleTable()
                               ->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA), 0, static_cast<G4int>(theBaseZ)));
    }
    theRecoil->SetMomentum(theTarget.GetMomentum());
    theResult.AddSecondary(theRecoil);
//    G4cout << "G4NeutronHPElasticFS::ApplyYourself 10+"<<G4endl;
    // postpone the tracking of the primary neutron
     theResult.SetStatusChange(suspend);
    return &theResult;
  }
