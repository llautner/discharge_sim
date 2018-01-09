#ifndef WORLDCONSTRUCTION_HH_
#define WORLDCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "Detector.hh"
#include "InputHandler.hh"
#include "G4NistManager.hh"

class WorldConstruction : public G4VUserDetectorConstruction {
public:
  WorldConstruction(InputHandler* input);
  virtual ~WorldConstruction();
  
  G4VPhysicalVolume* Construct(void);
  
  
private:
  InputHandler *input;
  G4Box* WorldBox;
  G4LogicalVolume* WorldLog;
  G4VPhysicalVolume* WorldPhys;
  
  Detector* detector;
};

#endif /* WORLDCONSTRUCTION_HH_ */
