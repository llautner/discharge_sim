#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmProcessOptions.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//____________________________________________________________________________________________________________________________________________________________
PhysicsList::PhysicsList():  G4VUserPhysicsList(),
fEmPhysicsList(0)
{
  SetVerboseLevel(0);
  fEmPhysicsList = new G4EmLivermorePhysics();
//     fEmPhysicsList = new G4EmStandardPhysics_option4();
  
  G4LossTableManager::Instance();
//   SetDefaultCutValue(1*mm);
  
  //G4ProductionCutsTable::GetProductionCutsTable()->Set..
  
}

//____________________________________________________________________________________________________________________________________________________________
PhysicsList::~PhysicsList() {
  delete fEmPhysicsList;
}

//____________________________________________________________________________________________________________________________________________________________
void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();
  
  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();
  
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();
  
  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();
  
  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
  
  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

//____________________________________________________________________________________________________________________________________________________________
void PhysicsList::ConstructProcess()
{
  AddTransportation();
  
  fEmPhysicsList->ConstructProcess();    //use pre-defined physics list
  
  // the following is from GEANT4 examples - TestEM11
//   G4EmProcessOptions emOptions;
//   emOptions.SetBuildCSDARange(true);
//   emOptions.SetDEDXBinningForCSDARange(8*10);
  
  AddDecay();
  AddStepMax();
  
  // according to TestEM7 only added when using EmStanandardPhysics_opt0
//   AddIonGasModels();
}


//____________________________________________________________________________________________________________________________________________________________
#include "G4Decay.hh"

void PhysicsList::AddDecay()
{
  G4Decay* fDecayProcess = new G4Decay();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    
    if (fDecayProcess->IsApplicable(*particle) && !particle->IsShortLived()) { 
      
      pmanager ->AddProcess(fDecayProcess);
      
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);
      
    }
  }
}


//____________________________________________________________________________________________________________________________________________________________
#include "StepMax.hh"

void PhysicsList::AddStepMax()
{
  stepMaxProcess = new StepMax();
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (particle->GetParticleName() == "alpha"){
      pmanager ->AddDiscreteProcess(stepMaxProcess);
    }
  }
}


//____________________________________________________________________________________________________________________________________________________________
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

void PhysicsList::AddIonGasModels()
{
  G4EmConfigurator* em_config = G4LossTableManager::Instance()->EmConfigurator();
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String partname = particle->GetParticleName();
    if(partname == "alpha" || partname == "He3" || partname == "GenericIon") {
      G4BraggIonGasModel* mod1 = new G4BraggIonGasModel();
      G4BetheBlochIonGasModel* mod2 = new G4BetheBlochIonGasModel();
      G4double eth = 2.*MeV*particle->GetPDGMass()/proton_mass_c2;
      em_config->SetExtraEmModel(partname,"ionIoni",mod1,"",0.0,eth, new G4IonFluctuations());
      em_config->SetExtraEmModel(partname,"ionIoni",mod2,"",eth,100*TeV, new G4UniversalFluctuation());

    }
  }
}