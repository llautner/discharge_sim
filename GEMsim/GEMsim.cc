
#include "InputHandler.hh"
#include "PhysicsList.hh"
#include "WorldConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventManager.hh"
#include "OutputManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "RunManager.hh"

#include "Randomize.hh"

int main(int argc,char** argv)
{
  // [nEvents] [Visualisation VIS/ no visualization SIM] [Gas: 0 - Ne-CO2-N2 (90-10-5), 1 - Ne-CO2 (90-10), 2 - Ar-CO2 (90-10), 3 - Ar-CO2 (70-30)] [Drift field] 
  InputHandler* input = InputHandler::GetInstance();
  input->Init(argv);
  
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  OutputManager* output = new OutputManager(input);
  EventManager* eventManager = EventManager::getInstance();
  eventManager->setOutputManager(output);
    
  RunManager *runManager = new RunManager(input);
  PhysicsList* physicsList = new PhysicsList();
  runManager->SetPhysicsList(physicsList);
 
  PrimaryGeneratorAction* primaryGenerator = new PrimaryGeneratorAction();
  runManager->SetGun(primaryGenerator);
  WorldConstruction* worldConstruction = new WorldConstruction(input);
  runManager->SetGeometry(worldConstruction);
    
  runManager->Initialize();
  
  if (input->UseVis()) {
    G4cout << "*************************************************************" << G4endl;
    G4cout << "Visualization enabled" << G4endl;
    G4cout << "*************************************************************" << G4endl;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4VisManager* visManager = new G4VisExecutive;  
    visManager->Initialize();
    UImanager->ApplyCommand("/vis/open OGL 600x600-0+0");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh false");
    UImanager->ApplyCommand("/vis/verbose errors");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector -1 0 0");
    UImanager->ApplyCommand("/vis/viewer/set/lightsVector -1 0 0");
    UImanager->ApplyCommand("/vis/viewer/set/style wireframe");
    UImanager->ApplyCommand("/vis/viewer/set/auxiliaryEdge true");
    UImanager->ApplyCommand("/vis/viewer/set/lineSegmentsPerCircle 100");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByCharge");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2");
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    UImanager->ApplyCommand("/vis/scene/add/scale");
    UImanager->ApplyCommand("/vis/scene/add/eventID");
    UImanager->ApplyCommand("/vis/scene/add/date");
    UImanager->ApplyCommand("/vis/set/colour red");
    UImanager->ApplyCommand("/vis/set/lineWidth 2");
    UImanager->ApplyCommand("/vis/scene/add/frame");
    UImanager->ApplyCommand("/vis/set/colour");
    UImanager->ApplyCommand("/vis/set/lineWidth");
    UImanager->ApplyCommand("/vis/geometry/set/visibility WorldLog 0 false");
    UImanager->ApplyCommand("/vis/viewer/set/style surface");
    UImanager->ApplyCommand("/vis/viewer/set/hiddenMarker true");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 60 150");
//     UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90 0");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/verbose warnings");
    UImanager->ApplyCommand("/vis/reviewKeptEvents");
  } 
  else {
    G4cout << "*************************************************************" << G4endl;
    G4cout << "Visualization disabled - RUN STARTS" << G4endl;
    G4cout << "*************************************************************" << G4endl;
  }
  int nev = G4UIcommand::ConvertToInt(argv[1]);
  runManager->StartRun(nev);
  
  output->printOutput();
  
  delete eventManager;
  delete output;
  return 0;
}
