/*
 * EventManager.hh
 *
 *  Created on: Feb 13, 2014
 *      Author: tkunz
 */

#ifndef EVENTMANAGER_HH_
#define EVENTMANAGER_HH_

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "OutputManager.hh"
#include "InputHandler.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4PrimaryParticle.hh"


class EventManager {
public:
  static EventManager* getInstance(void);
  ~EventManager(void);

  G4int getEventNo(void);

  void addDetector(G4String detectorName,G4int detectorID);
  void endOfEvent(G4int detectorID, std::vector<SdInformation>* hit);
//   void addPhotons(Int_t);
//   void addPhoton(Double_t wavelength); 
//   void addElectrons(Int_t);
//   void addEdepAlpha(Double_t eDep);
  bool isnewevent;
  void setOutputManager(OutputManager*);
private:
  EventManager(void);
  static EventManager* eventManager;
  G4int verboseLevel;

  OutputManager* output;
  G4int eventNo_;
  G4bool checkEndOfEvent(void);
  void endOfEventAction(void);


  std::vector<DetectorCollection>* detectorCollection;
  Event* eventInformation;
//   G4int numPhotons;
  G4double eDep;

  InputHandler* input;
  Int_t numElectrons;
};

#endif /* EVENTMANAGER_HH_ */
