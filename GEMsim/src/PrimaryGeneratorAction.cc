#include "PrimaryGeneratorAction.hh"

#include "Randomize.hh"

//____________________________________________________________________________________________________________________________________________________________
PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),
fParticleGun(0)
{
  fParticleGun  = new G4ParticleGun(1);
  G4String particleName;
}

//____________________________________________________________________________________________________________________________________________________________
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//____________________________________________________________________________________________________________________________________________________________
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  G4double twopi=6.28318530718;
  
  // Randomize location of particle emission on circular source
  G4double alpha = twopi*G4UniformRand();
  G4double r = 0.39*G4UniformRand()*cm;
  fParticleGun->SetParticlePosition(G4ThreeVector(r*std::cos(alpha), r*std::sin(alpha), -5.15*cm));
  
  // Randomize direction of the particle (hit active volume only)                                                                                                                                                        
  G4double cosTheta = G4UniformRand();
  G4double phi = twopi*G4UniformRand();
  G4double sinTheta = sqrt(1. - cosTheta*cosTheta);
  fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(sinTheta*std::cos(phi), sinTheta*std::sin(phi), cosTheta));
  
  // Mixed source - 239Pu + 241Am + 244Cm
  G4double rmd1 = G4UniformRand();
  G4double rmd2 = G4UniformRand();
  // 239Pu contribution
  G4double e = 5.105*MeV;
  if(rmd1<= 1./3. && rmd2 > 0.115 && rmd2 <= 0.266) e=5.143*MeV;
  else if(rmd1<= 1./3. && rmd2 > 0.266) e=5.155*MeV;
  // 241Am contribution
  else if(rmd1 > 1./3. && rmd1 <= 2./3. && rmd2 <= 0.014) e=5.388*MeV;
  else if(rmd1 > 1./3. && rmd1 <= 2./3. && rmd2 > 0.014 && rmd2 <= 0.142) e=5.443*MeV;
  else if(rmd1 > 1./3. && rmd1 <= 2./3. && rmd2 > 0.142) e=5.486*MeV;  
  // 244Cm contribution
  else if(rmd1 > 2./3. && rmd2 <= 0.233) e=5.763*MeV;
  else if(rmd1 > 2./3. && rmd2 > 0.233) e=5.805*MeV;
  
  fParticleGun->SetParticleEnergy(e);
  
  fParticleGun->SetNumberOfParticles(1);
  fParticleGun->SetParticleDefinition(G4Alpha::AlphaDefinition());
  fParticleGun->GeneratePrimaryVertex(anEvent);
}