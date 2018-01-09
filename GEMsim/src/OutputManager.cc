#include "OutputManager.hh"

//____________________________________________________________________________________________________________________________________________________________
OutputManager::OutputManager(InputHandler* input_) {
  input = input_;
  file = new TFile(input->GetOutput().filename,"RECREATE");
  
  fGEMEvent = new GEMEvent();
  
  tree = new TTree("GEM","GEM");
  tree->Branch("GEMEvent", "GEMEvent", &fGEMEvent);
}

//____________________________________________________________________________________________________________________________________________________________
OutputManager::~OutputManager() {
}

//____________________________________________________________________________________________________________________________________________________________
void OutputManager::addEvent(Event* evt){
  Event event;
  event.VertexPosition = evt->VertexPosition;
  event.gunEnergy = evt->gunEnergy;
  event.eventNo = evt->eventNo;
  event.numElectrons = evt->numElectrons;
  for(Int_t i = 0; i < (Int_t) evt->detectors.size(); i++){
    DetectorInformation det;
    det.detectorID = evt->detectors.at(i).detectorID;
    det.detectorName = evt->detectors.at(i).detectorName ;
    for(Int_t j = 0; j < (Int_t) evt->detectors.at(i).sdInformation.size(); j++){
      SdInformation sdInfo;
      sdInfo.beta =  evt->detectors.at(i).sdInformation.at(j).beta;
      sdInfo.energyDeposit =  evt->detectors.at(i).sdInformation.at(j).energyDeposit;
      sdInfo.hitPosition =  evt->detectors.at(i).sdInformation.at(j).hitPosition;
      sdInfo.particleEnergy =  evt->detectors.at(i).sdInformation.at(j).particleEnergy;
      sdInfo.particleId =  evt->detectors.at(i).sdInformation.at(j).particleId;
      sdInfo.particleMomentum =  evt->detectors.at(i).sdInformation.at(j).particleMomentum;
      sdInfo.time =  evt->detectors.at(i).sdInformation.at(j).time;
      sdInfo.trackId =  evt->detectors.at(i).sdInformation.at(j).trackId;
      sdInfo.stepLength = evt->detectors.at(i).sdInformation.at(j).stepLength;
      det.sdInformation.push_back(sdInfo);
    }
    event.detectors.push_back(det);
  }
  
  Bool_t useEvent = false;
  
  if(event.detectors.at(0).sdInformation.size() > 0) useEvent = true;
  if(useEvent){
    analyseEvent(event);
  }
}

//____________________________________________________________________________________________________________________________________________________________
Bool_t OutputManager::isUsed(Int_t trackID,std::vector<Int_t>* vector){
  
  for(Int_t i = 0; i < vector->size();i++){
    if(trackID == vector->at(i)){
      return true;
    }
  }
  vector->push_back(trackID);
  return false;  
}

//____________________________________________________________________________________________________________________________________________________________
void OutputManager::analyseEvent(Event event){
  
  fGEMEvent->Clear(0);
  Int_t eventNo = event.eventNo;
  if(eventNo % 100000 == 0) printf("Processing event %i \n", eventNo);
  fGEMEvent->SetVertex(TVector3(event.VertexPosition.x(), event.VertexPosition.y(), event.VertexPosition.z()));
  fGEMEvent->SetGunEnergy(event.gunEnergy);
  fGEMEvent->SetNevt(eventNo);
  Double_t totalEdep =0;
  Double_t range = 0;  
  
  for(Int_t i = 0; i < (Int_t) event.detectors.size(); i++){
    for(Int_t j = 0; j < (Int_t) event.detectors.at(i).sdInformation.size(); j++)
    {
      Double_t posX = event.detectors.at(i).sdInformation.at(j).hitPosition.x();
      Double_t posY = event.detectors.at(i).sdInformation.at(j).hitPosition.y();
      Double_t posZ = event.detectors.at(i).sdInformation.at(j).hitPosition.z();
      Double_t edep = event.detectors.at(i).sdInformation.at(j).energyDeposit;
      Double_t step = event.detectors.at(i).sdInformation.at(j).stepLength;
      Double_t particleID = event.detectors.at(i).sdInformation.at(j).particleId;
      Double_t particleEnergy = event.detectors.at(i).sdInformation.at(j).particleEnergy;
      Double_t time= event.detectors.at(i).sdInformation.at(j).time;
      if(particleID == 1000020040){
        fGEMEvent->AddHit(posX, posY, posZ, edep, step, particleID);
        totalEdep += edep;
        range += step;
      }
    }
  }
  fGEMEvent->SetEdep(totalEdep);
  fGEMEvent->SetRange(range);
  tree->Fill();
}


//____________________________________________________________________________________________________________________________________________________________
void OutputManager::printOutput(){
  file->Write();
  file->Close();
}
