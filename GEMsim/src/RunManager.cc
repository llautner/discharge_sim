#include "Randomize.hh"
#include "RunManager.hh"


//____________________________________________________________________________________________________________________________________________________________
RunManager::RunManager(InputHandler* input) {
  
  g4RunManager = new G4RunManager();
  
  primaryGenerator = NULL;
  world = NULL;
  random3 = input->getTRandom();
}

//____________________________________________________________________________________________________________________________________________________________
RunManager::~RunManager() {
}

//____________________________________________________________________________________________________________________________________________________________
void RunManager::SetPhysicsList(PhysicsList* physicsList){
  g4RunManager->SetUserInitialization(physicsList);
}



//____________________________________________________________________________________________________________________________________________________________
void RunManager::StartRun(G4int numEvents){
  std::cout << "Shooting " << numEvents << " particles." << std::endl;
  g4RunManager->BeamOn(numEvents);
  std::cout << "Run finished." << std::endl;
}

//____________________________________________________________________________________________________________________________________________________________
void RunManager::SetGun(PrimaryGeneratorAction* primaryGenerator_){
  if (primaryGenerator == 0) {
    primaryGenerator = primaryGenerator_;
    g4RunManager->SetUserAction(primaryGenerator);
  }
  else{
    std::cerr << "Error in RunManager::setBeamGun. Beam gun has already been set." << std::endl;
  }
}

//____________________________________________________________________________________________________________________________________________________________
void RunManager::SetGeometry(WorldConstruction* world_)
{
  world = world_;
  g4RunManager->SetUserInitialization(world);
}

//____________________________________________________________________________________________________________________________________________________________
void RunManager::Initialize(void)
{
  if (world != NULL && primaryGenerator != NULL) {
    G4int numEntries = 1 + ((G4int) (random3->Rndm() * 100));
    long* seeds = new long[numEntries];
    for (G4int i = 0; i < numEntries; i++) {
      seeds[i] = (long) (1000000 * random3->Rndm());
    }
    CLHEP::HepRandom::setTheSeeds(seeds);
    g4RunManager->Initialize();
  }
  else{
    std::cerr << "Error in RunManager::initialize. World geometry or beam gun has not been set up correctly. RunManager couldn't be initialized." << std::endl;
  }

  CLHEP::HepRandom::showEngineStatus();

}
