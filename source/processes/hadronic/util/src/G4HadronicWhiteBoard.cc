#include "G4HadronicWhiteBoard.hh"

G4HadronicWhiteBoard & G4HadronicWhiteBoard::Instance()
{
  static G4HadronicWhiteBoard theInstance;
  return theInstance;
}

  const G4HadProjectile * G4HadronicWhiteBoard::GetProjectile() {return theProjectile;}
  const G4Nucleus & G4HadronicWhiteBoard::GetTargetNucleus() { return theTarget; }
  G4ParticleDefinition * G4HadronicWhiteBoard::GetPDef() {return theDef;}
  G4String G4HadronicWhiteBoard::GetParticleName() {return theName;}
  G4double G4HadronicWhiteBoard::GetEnergy() {return theE;}
  G4double G4HadronicWhiteBoard::GetPx(){return thePx;}
  G4double G4HadronicWhiteBoard::GetPy(){return thePy;}
  G4double G4HadronicWhiteBoard::GetPz(){return thePz;}
  G4double G4HadronicWhiteBoard::GetA(){return theA;}
  G4double G4HadronicWhiteBoard::GetZ(){return theZ;}
  
  
  void G4HadronicWhiteBoard::SetProjectile(const G4HadProjectile & aProjectile)
  {
    theProjectile = const_cast<G4HadProjectile*>(& aProjectile);
    theDef = const_cast<G4ParticleDefinition*>(theProjectile->GetDefinition());
    theName = const_cast<char *>(theDef->GetParticleName().c_str() );
    theE = theProjectile->Get4Momentum().t();
    thePx = theProjectile->Get4Momentum().vect().x();
    thePy = theProjectile->Get4Momentum().vect().y();
    thePz = theProjectile->Get4Momentum().vect().z();
  }
    
  void G4HadronicWhiteBoard::SetTargetNucleus(const G4Nucleus & aTarget) 
  {
    theTarget = aTarget;
    theA = theTarget.GetN();
    theZ = theTarget.GetZ();
  }

  void G4HadronicWhiteBoard::Dump()
  {
    std::cerr << std::endl;
    std::cerr << "GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD"<<std::endl;
    std::cerr << "Dumping the registered hadronic state information "<<std::endl;
    std::cerr << "Nucleus A, Z = "<<theA<<" "<<theZ<<std::endl;
    std::cerr << "Projectile was a "<<theName<<std::endl;
    std::cerr << "projectile momentum (px, py, pz) = ("<<thePx<<", "<<thePy<<", "<<thePz<<")"<<std::endl;
    std::cerr << "Projectile energy = "<< theE<<std::endl;
    std::cerr << "GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD_GHAD"<<std::endl;
    std::cerr << std::endl;
  }
