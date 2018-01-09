#include "WorldConstruction.hh"

//____________________________________________________________________________________________________________________________________________________________
WorldConstruction::WorldConstruction(InputHandler* input_) {
  input = input_;     
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    
  G4VisAttributes *invisible = new G4VisAttributes(G4Colour(0.,0.,0.));
  invisible->SetVisibility(false);
  invisible->SetForceSolid(true);
  
  WorldBox = new G4Box("WorldBox", 1*m, 1*m, 1*m);
  WorldLog = new G4LogicalVolume(WorldBox, vacuum, "WorldLog", 0, 0, 0);
  WorldPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), WorldLog, "world",  0, false, 0);
  WorldLog->SetVisAttributes(invisible);
}


//____________________________________________________________________________________________________________________________________________________________
WorldConstruction::~WorldConstruction(){
  delete WorldLog;
  delete WorldBox;
  delete detector;
}


//____________________________________________________________________________________________________________________________________________________________
G4VPhysicalVolume* WorldConstruction::Construct(void){
  detector = new Detector(WorldLog,input);
  detector->Create(WorldLog); 
  
  return WorldPhys;
}

