// IntegrHadrNucleus.cc

#include "globals.hh"
#include "G4IntegrHadrNucleus.hh"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++
 void  G4IntegrHadrNucleus::
           GetIntegralCrSec(G4Nucleus *  aNucleus)
 {
    G4int      i, /*k,*/ l/*, m*/;
    G4double   N, N1, N2, N3, N4, Delta, Inel1/*, Inel3*/;
    G4double   Tot0, Inel0, /*Inel2,*/ Prod0, Prod1, 
                ak, Delt, Delt2, Delt3;
    G4double   Rnucl, R0, Stot, Bhad, Asq, MbToB, Pi1;
    G4double   Dtot, /*Dinel,*/ Dprod, Rnuc2, RB, R2B, bk, bd;

      G4int  Anucleus = static_cast<G4int>( aNucleus->GetN() );

 if(Anucleus<4)
         {
  G4Exception(" This nucleus is very light for this model !!!");
         }

  if(Anucleus>238)
         {
  G4Exception(" This nucleus is very heavy for this model !!!");
         }

          MbToB   = 2.568;
          Pi1     = 3.1416;

          Stot    = HadrTot*MbToB;                     //{In GeV-2}
          Bhad    = HadrSlope;                         //{In GeV-2}
          Asq     = 1+HadrReIm*HadrReIm;

          R0      = sqrt(0.99);                        //{ This is fermi}
          if (Anucleus >10)  R0 = sqrt(0.84);          
          if (Anucleus >20)  R0 = sqrt((35.34+0.5*Anucleus)
                                  /(40.97+Anucleus));
          if (Anucleus == 16) R0 = sqrt(0.75);
          if (Anucleus == 58) R0 = sqrt(0.6);

//          R0      =  sqrt(0.64);

          Rnucl   = R0*pow(static_cast<float>(Anucleus),0.3333);            //{In Fermi }

        if(Anucleus == 4) Rnucl = 1.2;

          Rnuc2   = Rnucl*Rnucl*MbToB*10;               //{ In GeV-2}
          RB      = Rnuc2+Bhad;
          R2B     = RB+Bhad;
          Delta   = Stot/R2B/2/Pi1;
          Delt    = Delta*Asq*0.5;
          Delt2   = Delta*2;
          Delt3   = Stot/RB/Bhad/16/Pi1*Asq*R2B;

          Tot0    = 0; 
          Inel0 = 0; 
          Inel1=0;
          N = N1  = -1/Delta; 
          N3      = -1/Delt2;
          Prod0   = 0;

          for (i=1; i<= Anucleus; i++)

             {
               N     = -N*Delta*(Anucleus-i+1)/i;
               N1    = -N1*Delta*(2*Anucleus-i+1)/i;
               N3    = -N3*Delt2*(Anucleus-i+1)/i;
               Tot0  = Tot0+N/i;
               Inel0 = Inel0+N1/i;
               N2    = 1;
               N4    = 1;
               Inel1 = 0;
               Prod1 = 0;

                  for (l=0; l<= i; l++) 

                     {
//                       Inel1   = Inel1+N2/(i+l);
                         Prod1 = Prod1+N4*RB/(i*RB+l*Bhad);
//                       N2      = -N2*Delt*(i-l)/(l+1);
                         N4    = -N4*Delt3*(i-l)/(l+1);
                     } //  l

//                 Inel2      = Inel2+Inel1*N3;
                 Prod0    = Prod0+Prod1*N3;
          if(abs(N1/i/Inel0) < 0.0001)  break;
              }  // i

             Tot0         = Tot0*HadrTot;
             Inel0        = Inel0*HadrTot*0.5;
//             Inel2      = Inel2*HadrTot;
             Prod0        = Prod0*HadrTot;
             Tot00        = Tot0;

             ak           = (Rnuc2*2*Pi1/Stot);
    G4double DDSect1      = (DDSect2+DDSect3*log(1.06*2*HadrEnergy
                             /Rnucl/sqrt(25.68)/4));

             Dtot         = 8*Pi1*ak/HadrTot*(1-(1+Anucleus/ak)
                              *exp(-Anucleus/ak))*DDSect1/MbToB;
             DTot00       = Dtot;
             
             bk           = (1-1/ak)/Stot/(1-1/ak/4);

             bd           = bk*bk*DDSect1*(1-(1+Anucleus/ak*(1-1/ak/4))*
                             exp(-Anucleus/ak*(1-1/4/ak)))*Rnuc2;

             Dprod        = bd*4*Pi1*Pi1*MbToB;

             TotalCrSec   = Tot0-Dtot;
             InelCrSec    = Inel0-Dprod;
//             InelCrSec1 = Inel2;
             ProdCrSec    = Prod0-Dprod;
             ElasticCrSec = TotalCrSec-InelCrSec;
             QuasyElasticCrSec = InelCrSec-ProdCrSec;
   }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  G4double 
  G4IntegrHadrNucleus::GetElasticCrossSection(
                                     const G4DynamicParticle * aHadron,
                                           G4Nucleus         * aNucleus)

         {       
                HadrEnergy = aHadron->GetTotalEnergy()/1000;

if(HadrEnergy < 1.5)
  G4cout << " The hadron energy is very low for this model !!!"<<G4endl;

                G4HadronValues::GetHadronValues(aHadron);
                GetIntegralCrSec(aNucleus);
                return(ElasticCrSec);
         }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  G4double 
  G4IntegrHadrNucleus::GetTotalCrossSection(
                                     const G4DynamicParticle * aHadron,
                                           G4Nucleus         * aNucleus)
        {
                HadrEnergy = aHadron->GetTotalEnergy()/1000;

if(HadrEnergy < 1.5)
  G4cout <<" The hadron energy is very low for this model !!!"<<G4endl;

                G4HadronValues::GetHadronValues(aHadron);
                GetIntegralCrSec(aNucleus);
                return(TotalCrSec);
        }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  G4double 
  G4IntegrHadrNucleus::GetInelasticCrossSection(
                                     const G4DynamicParticle * aHadron,
                                           G4Nucleus         * aNucleus)
       {
                HadrEnergy = aHadron->GetTotalEnergy()/1000;

if(HadrEnergy < 1.5)
  G4cout <<" The hadron energy is very low for this model !!!"<<G4endl;

                G4HadronValues::GetHadronValues(aHadron);
                GetIntegralCrSec(aNucleus);
                return(InelCrSec);
       }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  G4double 
  G4IntegrHadrNucleus::GetProductionCrossSection(
                                     const G4DynamicParticle * aHadron,
                                           G4Nucleus         * aNucleus)
       {
                HadrEnergy = aHadron->GetTotalEnergy()/1000;

if(HadrEnergy < 1.5)
  G4cout <<" The hadron energy is very low for this model !!!"<<G4endl;

                G4HadronValues::GetHadronValues(aHadron);
                GetIntegralCrSec(aNucleus);
                return(ProdCrSec);
       }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  G4double 
  G4IntegrHadrNucleus::GetQuasyElasticCrossSection(
                                     const G4DynamicParticle * aHadron,
                                           G4Nucleus         * aNucleus)
       {
                HadrEnergy = aHadron->GetTotalEnergy()/1000;

if(HadrEnergy < 1.5)
  G4cout <<" The hadron energy is very low for this model !!!"<<G4endl;

                G4HadronValues::GetHadronValues(aHadron);
                GetIntegralCrSec(aNucleus);
                return(QuasyElasticCrSec);
       }

/* end of file */
