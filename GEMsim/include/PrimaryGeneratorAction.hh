#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4Alpha.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();    
  virtual ~PrimaryGeneratorAction();
  
  virtual void GeneratePrimaries(G4Event*);         
  
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
private:
  G4ParticleGun*        fParticleGun;
  G4Box*                fEnvelopeBox;
};

#endif