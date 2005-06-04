// G4HadronValues.cc

#include "globals.hh"
#include "G4HadronValues.hh"


 void 
 G4HadronValues::GetHadronValues(const G4DynamicParticle* aHadron)
  {

       G4ParticleDefinition * dHadron = aHadron->GetDefinition();

       G4int iHadron(-1);

         if(dHadron == G4Proton::Proton()        ||
            dHadron == G4Neutron::Neutron()      ||
            dHadron == G4Lambda::Lambda()        || 
            dHadron == G4SigmaPlus::SigmaPlus()  ||
            dHadron == G4SigmaMinus::SigmaMinus()|| 
            dHadron == G4SigmaZero::SigmaZero()  ||
            dHadron == G4XiMinus::XiMinus()      ||
            dHadron == G4XiZero::XiZero()        || 
            dHadron == G4OmegaMinus::OmegaMinus())         iHadron=0;

   else  if(dHadron == G4AntiProton::AntiProton()        ||
            dHadron == G4AntiNeutron::AntiNeutron()      ||
            dHadron == G4AntiLambda::AntiLambda()        ||
            dHadron == G4AntiSigmaPlus::AntiSigmaPlus()  ||
            dHadron == G4AntiSigmaMinus::AntiSigmaMinus()||
            dHadron == G4AntiSigmaZero::AntiSigmaZero()  ||
            dHadron == G4AntiXiMinus::AntiXiMinus()      ||
            dHadron == G4AntiXiZero::AntiXiZero()        || 
            dHadron == G4AntiOmegaMinus::AntiOmegaMinus())  iHadron=1;

   else  if(dHadron == G4PionPlus::PionPlus())              iHadron=2;

   else  if(dHadron == G4PionMinus::PionMinus())            iHadron=3;

   else  if(dHadron == G4KaonPlus::KaonPlus())              iHadron=4;

   else  if(dHadron == G4KaonMinus::KaonMinus())            iHadron=5;

   else {   
  G4Exception(" There is not method for this hadron ");
        }

       G4double mHadr      = aHadron->GetMass()/1000.;         // In GeV
       G4double HadrEnergy = aHadron->GetTotalEnergy()/1000.;  // In GeV
       G4double sHadr      = 2*HadrEnergy*0.938+0.938*0.938+mHadr*mHadr;
       G4double sqrS       = std::sqrt(sHadr);
       G4double Ecm        = (sHadr-mHadr*mHadr+0.938*.938)/2/sqrS;
                MomentumCM = std::sqrt(Ecm*Ecm-0.938*0.938);

   if(HadrEnergy<1.0) 
    {
     G4cout<<HadrEnergy<<G4endl;
     G4Exception(" The hadron Energy is very low for this method!");
    }
        switch (iHadron)
        {

         case 0:                  //  proton

        G4double Delta;

          Delta=1;

              if(HadrEnergy<40)
                  Delta = 0.916+0.0021*HadrEnergy;
              HadrTot   = 5.2+5.2*std::log(HadrEnergy)
                          +51*std::pow(HadrEnergy,-0.35);            //  mb
              HadrSlope = 6.44+0.88*std::log(sHadr)-1;               //  GeV-2 
              HadrReIm  = 0.13*std::log(sHadr/350)*std::pow(sHadr,-0.18);
              DDSect2   = 11;                                    //mb*GeV-2
              DDSect3   = 3;                                     //mb*GeV-2

//    if(HadrEnergy>1000) HadrReIm=0.15; 

        if( dHadron == G4Lambda::Lambda()        ||
            dHadron == G4SigmaPlus::SigmaPlus()  ||
            dHadron == G4SigmaMinus::SigmaMinus()||
            dHadron == G4SigmaZero::SigmaZero())
            {
              HadrTot   *=0.80;
              HadrSlope *=0.85;
            }

        if( dHadron == G4XiMinus::XiMinus()      ||
            dHadron == G4XiZero::XiZero())
            {
              HadrTot   *=0.70;
              HadrSlope *=0.75;
            }           

        if( dHadron == G4OmegaMinus::OmegaMinus())
            {
              HadrTot   *=0.60;
              HadrSlope *=0.65;
            }

             break;

         case 1:              //   antiproton

              sqrS      = std::sqrt(sHadr);
              HadrTot   = 5.2+5.2*std::log(HadrEnergy)
                          +123.2*std::pow(HadrEnergy,-0.5);           //  mb
              HadrSlope = 8.32+0.57*std::log(sHadr); //GeV-2 
           if(HadrEnergy<1000)
              HadrReIm  =0.06*(sqrS-2.236)*(sqrS-14.14)*std::pow(sHadr,-0.8);
           else
              HadrReIm  = 0.6*std::log(sHadr/350)*std::pow(sHadr,-0.25);

              DDSect2   = 11;                                     //mb*GeV-2
              DDSect3   = 3;                                      //mb*GeV-2

//    if(HadrEnergy>1000) HadrReIm=0.15; 

        if( dHadron == G4AntiLambda::AntiLambda()        ||
            dHadron == G4AntiSigmaPlus::AntiSigmaPlus()  ||
            dHadron == G4AntiSigmaMinus::AntiSigmaMinus()||
            dHadron == G4AntiSigmaZero::AntiSigmaZero())
            {
              HadrTot   *=0.75;
              HadrSlope *=0.85;
            }
     
        if( dHadron == G4AntiXiMinus::AntiXiMinus()      ||
            dHadron == G4AntiXiZero::AntiXiZero())
            {
              HadrTot   *=0.65;
              HadrSlope *=0.75;
            }

        if( dHadron == G4AntiOmegaMinus::AntiOmegaMinus())
            {
              HadrTot   *=0.55;
              HadrSlope *=0.65;
            }

            break;

         case 2:             //   pi plus

              HadrTot   = 10.6+2.*std::log(HadrEnergy)+
                          25*std::pow(HadrEnergy,-0.43);               // mb 
              HadrSlope = 7.28+0.245*std::log(sHadr);                  //GeV-2 
              HadrReIm  = 0.2*std::log(sHadr/100)*std::pow(sHadr,-0.15);
              DDSect2   = 4.6;                                    //mb*GeV-2
              DDSect3   = 1.33;                                   //mb*GeV-2
            break;

         case 3:             //   pi minus

              HadrTot   = 10.6+2*std::log(HadrEnergy)+
                          30*std::pow(HadrEnergy,-0.43);             // mb 
              HadrSlope = 7.28+0.245*std::log(sHadr);               // GeV-2 
              HadrReIm  = 0.2*std::log(sHadr/100)*std::pow(sHadr,-0.15);
              DDSect2   = 4.6;                                 //mb*GeV-2
              DDSect3   = 1.33;                                //mb*GeV-2
            break;

         case 4:            //  K plus

              HadrTot   = 10.6+1.8*std::log(HadrEnergy)+        
                               9.0*std::pow(HadrEnergy,-0.55);     // mb 
         if(HadrEnergy>100) HadrSlope = 15.0;
         else
              HadrSlope = 5.28+1.76*std::log(sHadr)-
                              2.84*std::pow(sHadr,-0.5);           // GeV-2
              HadrReIm  = 0.4*(sHadr-20)*(sHadr-150)*std::pow(sHadr+50,-2.1);
              DDSect2   = 3.5;                                //mb*GeV-2
              DDSect3   = 1.03;                               //mb*GeV-2
            break;

         case 5:              //   K minus

              HadrTot   = 10+1.8*std::log(HadrEnergy)
                               +25*std::pow(HadrEnergy,-0.5);   // mb 
              HadrSlope = 6.98+0.127*std::log(sHadr);           // GeV-2 
//         if(HadrEnergy<8) HadrReIm = 0.7;
//         else
              HadrReIm  = 0.4*(sHadr-20)*(sHadr-20)*std::pow(sHadr+50,-2.1);
              DDSect2   = 3.5;                             //mb*GeV-2
              DDSect3   = 1.03;                            //mb*GeV-2
            break;
      }     
  }

/* end of file */
