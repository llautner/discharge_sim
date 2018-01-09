#ifndef PhysicsList_hh
#define PhysicsList_hh 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
// #include "PhysListEmStandard.hh"
// #include "PhysListEmStandardSS.hh"
// #include "PhysListEmStandardNR.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4Decay.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"
#include "G4OpticalPhoton.hh"
#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Scintillation.hh"
#include "globals.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4ionIonisation.hh"
#include "G4BetheBlochIonGasModel.hh"
#include "G4BraggIonGasModel.hh"
#include "G4EmConfigurator.hh"
#include "G4LossTableManager.hh"
#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonConstructor.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4VPhysicsConstructor.hh"
#include "G4ProductionCuts.hh"
#include "G4EmModelActivator.hh"
#include "G4VModularPhysicsList.hh"

#include "StepMax.hh"

class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();
  
protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

    // these methods Construct physics processes and register them
    void AddDecay();
    void AddStepMax();
    void AddIonGasModels();
    
    G4VPhysicsConstructor* fEmPhysicsList;
    
    // step size
    StepMax* stepMaxProcess;

};

#endif

 
