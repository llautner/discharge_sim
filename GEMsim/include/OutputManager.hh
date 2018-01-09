#ifndef OUTPUTMANAGER_HH_
#define OUTPUTMANAGER_HH_

#include "InputHandler.hh"

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "TH1F.h"
#include "TCanvas.h"
#include "G4Proton.hh"
#include <sstream>
#include "TAxis.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TVector3.h"
#include "GEMEvent.hpp"
#include "G4SystemOfUnits.hh"

struct SdInformation{
  G4int particleId;
  G4int trackId;
  G4double particleEnergy;
  G4double time;
  G4double energyDeposit;
  G4double beta;
  G4ThreeVector hitPosition;
  G4ThreeVector particleMomentum;
  G4double stepLength;
};

struct DetectorInformation{
  G4String detectorName;
  G4int detectorID;
  std::vector<SdInformation> sdInformation;
};

struct Event{
  std::vector<DetectorInformation> detectors;
  G4ThreeVector VertexPosition;
  G4double gunEnergy;
  G4int eventNo;
  G4int numElectrons;
};

struct DetectorCollection{
  G4String detectorName;
  G4int detectorID;
  G4bool endOfEvent;
};



class OutputManager {
public:
  OutputManager(InputHandler*);
  virtual ~OutputManager();
  
  
  void addEvent(Event*);
  void analyseEvent(Event);
  void printOutput();
  
private:
  GEMEvent *fGEMEvent;
  Hit *hit;
  InputHandler* input;
  TFile* file;
  TTree* tree;
  Bool_t isUsed(Int_t,std::vector<Int_t>*);
};

#endif /* OUTPUTMANAGER_HH_ */
