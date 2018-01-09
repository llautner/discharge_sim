#include "SensitiveDetector.hh"

//____________________________________________________________________________________________________________________________________________________________
SensitiveDetector::SensitiveDetector(G4String detectorName, G4int detectorID) :
G4VSensitiveDetector(detectorName){
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(this);
  
  detectorName_ = detectorName;
  detectorID_   = detectorID;
  eventManager = EventManager::getInstance();
  
  eventManager->addDetector(detectorName_, detectorID_);
  
  hit = new std::vector<SdInformation>;
  verboseLevel = 0;
  input = InputHandler::GetInstance();
  random = input->getTRandom();
}

//____________________________________________________________________________________________________________________________________________________________
SensitiveDetector::~SensitiveDetector(void){
  delete hit;
}

//____________________________________________________________________________________________________________________________________________________________
G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  
  //smear out position between two GEANT hits
  
  G4StepPoint* prePoint  = aStep->GetPreStepPoint();
  G4StepPoint* postPoint = aStep->GetPostStepPoint(); 
  G4ThreeVector P1 = prePoint ->GetPosition();
  G4ThreeVector P2 = postPoint->GetPosition();
  //G4ThreeVector point = P1 + G4UniformRand()*(P2 - P1);
  G4ThreeVector point = P1;  


  SdInformation sdInformation;
  sdInformation.energyDeposit = aStep->GetTotalEnergyDeposit();
//   sdInformation.hitPosition = aStep->GetPreStepPoint()->GetPosition();
  sdInformation.hitPosition = point;
  sdInformation.particleEnergy = aStep->GetPreStepPoint()->GetTotalEnergy();
  sdInformation.particleMomentum = aStep->GetPreStepPoint()->GetMomentum();
  sdInformation.particleId = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  sdInformation.time = aStep->GetPreStepPoint()->GetGlobalTime();
  sdInformation.trackId = aStep->GetTrack()->GetTrackID();
  sdInformation.stepLength = aStep->GetStepLength();
  hit->push_back(sdInformation);

  return true;
}

//____________________________________________________________________________________________________________________________________________________________
void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{  
  eventManager->endOfEvent(detectorID_, hit);
  
  delete hit;
  hit = new std::vector<SdInformation>;
}
