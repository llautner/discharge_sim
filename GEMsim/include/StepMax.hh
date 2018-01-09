#ifndef STEPMAX_HH_
#define STEPMAX_HH_

#include "G4VDiscreteProcess.hh"

class StepMax : public G4VDiscreteProcess{
public:
   StepMax(const G4String& processName = "StepLimiter");
   virtual ~StepMax();

   void setStepLimit(G4double);
   G4bool IsApplicable(const G4ParticleDefinition&);
   G4double GetMaxStep(void) {return stepLimit;};
   G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition);
   G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*){return DBL_MAX;};
   virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);
 private:
   G4double stepLimit;

};

#endif /* IONISATION_HH_ */
