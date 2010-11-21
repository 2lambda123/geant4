// Class Description
// Container of LEND (Low Energy Nuclear Data) target (nucleus) 
// LEND is Geant4 interface for GIDI (General Interaction Data Interface) 
// which gives a discription of nuclear and atomic reactions, such as
//    Binary collision cross sections
//    Particle number multiplicity distributions of reaction products
//    Energy and angular distributions of reaction products
//    Derived calculational constants
// GIDI is developped at Lawrence Livermore National Laboratory
// Class Description - End

// 071025 First implementation done by T. Koi (SLAC/SCCS)
// 101118 Name modifications for release T. Koi (SLAC/PPA)

#include "G4LENDUsedTarget.hh"
#include "G4LENDManager.hh"

void G4LENDUsedTarget::searchTarget()
{

   G4LENDManager* endl_manager = G4LENDManager::GetInstance();

   target = NULL;
   target = endl_manager->GetLENDTarget( proj , wanted_Evaluation , wanted_Z , wanted_A , wanted_M );

   if ( target != NULL ) 
   {
      actual_Z = wanted_Z;
      actual_A = wanted_A;
      actual_M = wanted_M;
      actual_Evaluation = wanted_Evaluation; 

      return;
   }

   if ( allow_nat == true || allow_anything == true ) 
   {
      target = endl_manager->GetLENDTarget( proj , wanted_Evaluation , wanted_Z , 0 , wanted_M );
      if ( target != NULL ) 
      {
         actual_Z = wanted_Z;
         actual_A = 0;
         actual_M = wanted_M;
         actual_Evaluation = wanted_Evaluation; 

         return;  
      }
   }

   G4int iZ;
   G4int iA;
   G4int iM;

   if ( allow_anything == true ) 
   {

      // Loop Z
      G4int dZ = 0;
      G4int pZ = 1;
      while ( dZ < max_Z - min_Z )
      {
         iZ = wanted_Z + pZ*dZ; 

         // Loop A
         G4int dA = 0;
         G4int pA = 1;
         while ( dA < max_A - min_A )
         {
            iA = wanted_A + pA*dA; 

            // Loop M
            G4int pM = 1;
            G4int dM = 0;
            while ( dM < max_M - min_M )
            {

               iM = wanted_M + pM*dM;

               if ( iZ < min_Z ) iZ = min_Z;
               if ( iA < min_A ) iA = 0;
               if ( iM < min_M ) iM = min_M;

               if ( iZ > max_Z ) iZ = max_Z;
               if ( iA > max_A ) iA = max_A;
               if ( iM > max_M ) iM = max_M;

               //Loop Evaluations choice the first available

               if ( endl_manager->IsLENDTargetAvailable( proj , iZ , iA , iM ).size() > 0 ) 
               {
                  //Choice the first available
                  actual_Evaluation = endl_manager->IsLENDTargetAvailable( proj , iZ , iA , iM ).front();
         
                  actual_Z = iZ;
                  actual_A = iA;
                  actual_M = iM;
                  target = endl_manager->GetLENDTarget( proj , actual_Evaluation , iZ , iA , iM ); 
                  return;
               }

               if ( pM > 0 )  
                  pM = -1; 
               else
                  dM++;
            }

            if ( pA > 0 )  
               pA = -1; 
            else
               dA++;
         }       

         if ( pZ > 0 )  
            pZ = -1; 
         else
            dZ++;
      }

   }

}
