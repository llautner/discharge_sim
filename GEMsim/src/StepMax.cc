#include "StepMax.hh"
#include "G4VDiscreteProcess.hh"
#include "TRandom3.h"
#include "G4Electron.hh"
#include "G4OpticalPhoton.hh"
#include "G4Alpha.hh"
#include "G4MuonMinus.hh"

StepMax::StepMax (const G4String& processName) :   G4VDiscreteProcess(processName), stepLimit(110000000)    {
  SetProcessType(fGeneral);
  SetProcessSubType(401);
//   stepLimit = 0.005 *CLHEP::mm;
  stepLimit = 1*CLHEP::mm;

}

//=======================================================================================================================================================================================================================================
StepMax::~StepMax(void){
}

//=======================================================================================================================================================================================================================================
void StepMax::setStepLimit(G4double limit){
  stepLimit = limit;
}

//=======================================================================================================================================================================================================================================
G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle){
  return (particle.GetPDGCharge() != 0.0 && !particle.IsShortLived());
}

//=======================================================================================================================================================================================================================================
G4double StepMax::PostStepGetPhysicalInteractionLength(const G4Track& aTrack, G4double, G4ForceCondition* condition){
  
  *condition = NotForced;
  G4double ProposedStep = stepLimit;
  return ProposedStep;
}

//=======================================================================================================================================================================================================================================
G4VParticleChange* StepMax::PostStepDoIt(const G4Track& aTrack, const G4Step&)
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}