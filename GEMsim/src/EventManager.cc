#include "EventManager.hh"

//____________________________________________________________________________________________________________________________________________________________
EventManager* EventManager::eventManager = NULL;

//____________________________________________________________________________________________________________________________________________________________
EventManager* EventManager::getInstance(void)
{
  if (eventManager == NULL) {
    eventManager = new EventManager();
  }
  return eventManager;
}

//____________________________________________________________________________________________________________________________________________________________
EventManager::EventManager(void){
  eventNo_ = 0;
  eventInformation = new Event;
  detectorCollection = new std::vector<DetectorCollection>;
  verboseLevel = 0;
  output = 0;
  eDep       = 0;
  numElectrons = 0;
  input = InputHandler::GetInstance();
  isnewevent = true;
}

//____________________________________________________________________________________________________________________________________________________________
EventManager::~EventManager(void){
  delete eventInformation;
  delete detectorCollection;
}

//____________________________________________________________________________________________________________________________________________________________
void EventManager::addDetector(G4String detectorName, G4int detectorID){
  DetectorCollection detector;
  detector.detectorName = detectorName;
  detector.detectorID = detectorID;
  detector.endOfEvent = false;
  detectorCollection->push_back(detector);
}

//____________________________________________________________________________________________________________________________________________________________
void EventManager::endOfEvent(G4int detectorID,  std::vector<SdInformation>* hit){
  for (G4int i = 0; i < (G4int) detectorCollection->size(); i++) {
    if (detectorID == detectorCollection->at(i).detectorID) {
      detectorCollection->at(i).endOfEvent = true;
      // output
      DetectorInformation detectorInformation;
      detectorInformation.detectorName = detectorCollection->at(i).detectorName;
      detectorInformation.detectorID   = detectorID;
      for (G4int k = 0; k < (G4int) hit->size(); k++) {
        detectorInformation.sdInformation.push_back(hit->at(k));
      }
      eventInformation->detectors.push_back(detectorInformation);
      break;
    }
  }
  if (checkEndOfEvent() == true) endOfEventAction();
  isnewevent = true;
}

//____________________________________________________________________________________________________________________________________________________________
G4bool EventManager::checkEndOfEvent(void){
  for (G4int i = 0; i < (G4int) detectorCollection->size(); i++) {
    if (detectorCollection->at(i).endOfEvent == false) {
      return false;
    }
  }
  return true;
}

//____________________________________________________________________________________________________________________________________________________________
void EventManager::endOfEventAction(void){
  
  const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  eventInformation->VertexPosition= evt->GetPrimaryVertex()->GetPosition();
  G4PrimaryParticle* pp = evt->GetPrimaryVertex()->GetPrimary();
  eventInformation->gunEnergy = pp->GetKineticEnergy();
  eventInformation->eventNo = eventNo_;
  output->addEvent(eventInformation);
  eventInformation->detectors.clear();
  eventNo_++;
  for (G4int i = 0; i < (G4int) detectorCollection->size(); i++) {
    detectorCollection->at(i).endOfEvent = false;
  }
}

//____________________________________________________________________________________________________________________________________________________________
G4int EventManager::getEventNo(void){
  return eventNo_;
}

//____________________________________________________________________________________________________________________________________________________________
void EventManager::setOutputManager(OutputManager* out){
  output = out;
}