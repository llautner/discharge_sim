#ifndef SENSITIVEDETECTOR_HH_
#define SENSITIVEDETECTOR_HH_

#include "EventManager.hh"

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "InputHandler.hh"
#include "G4SystemOfUnits.hh"


class SensitiveDetector : public G4VSensitiveDetector 
{
  public:
    SensitiveDetector(G4String detectorName, G4int detectorID);
    ~SensitiveDetector(void);

    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

  private:
    InputHandler* input;
    G4String detectorName_;
    G4int detectorID_;
    G4int verboseLevel;
    EventManager* eventManager;
    std::vector<SdInformation>* hit;
    TRandom3* random;
};

#endif /* SENSITIVEDETECTOR_HH_ */