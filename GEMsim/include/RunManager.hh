#ifndef RUNMANAGER_HH_
#define RUNMANAGER_HH_

#include "PhysicsList.hh"
#include "TRandom3.h"
#include "G4RunManager.hh"
#include "G4UserLimits.hh"
#include "InputHandler.hh"
#include "WorldConstruction.hh"

#include "PrimaryGeneratorAction.hh"

class RunManager {
public:
  RunManager(InputHandler*);
  virtual ~RunManager();
  
  void SetGun(PrimaryGeneratorAction*);
  void SetGeometry(WorldConstruction*);
  void SetPhysicsList(PhysicsList*);
  void Initialize(void);
  
  void StartRun(G4int numEvents);
  
private:
  G4RunManager* g4RunManager;
  PrimaryGeneratorAction* primaryGenerator;
  WorldConstruction *world;
  TRandom3* random3;
  
};

#endif /* RUNMANAGER_HH_ */
