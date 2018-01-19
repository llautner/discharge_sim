#include "Detector.hh"
#include "G4EmCalculator.hh"
#include "G4Alpha.hh"
#include "G4ClassicalRK4.hh"

//____________________________________________________________________________________________________________________________________________________________
Detector::Detector(G4LogicalVolume* world_, InputHandler* input_) {
  world = world_;
  fInput = input_;
}

//____________________________________________________________________________________________________________________________________________________________
Detector::~Detector() {
}

//____________________________________________________________________________________________________________________________________________________________
void Detector::Create(G4LogicalVolume* mother){
  G4int detectorFlag = fInput->GetGasFlag().gasParameter;
  float EMfieldFlag = fInput->GetDriftField();
  
  G4NistManager* nist = G4NistManager::Instance();
  
  G4String name, symbol;
  G4int ncomponents, natoms;
  G4double fractionmass;
  G4double density;
  
  G4Element* elH  = nist->FindOrBuildElement("H");
  G4Element* elC  = nist->FindOrBuildElement("C");
  G4Element* elN  = nist->FindOrBuildElement("N");
  G4Element* elNe = nist->FindOrBuildElement("Ne");
  G4Element* elAr = nist->FindOrBuildElement("Ar");
  G4Material *CO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
  
  G4double densityN2    = 1.1694*mg/cm3;
  G4Material* N2 = new G4Material(name="Nitrogen", densityN2, ncomponents=1);
  N2->AddElement(elN, natoms=2);

  G4Material *SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  
  //FR_4 (Glass + Epoxy)
  density = 1.2*g/cm3;
  G4Material* Epoxy = new G4Material("Epoxy" , density, ncomponents=2);
  Epoxy->AddElement(elH, natoms=2);
  Epoxy->AddElement(elC, natoms=2);
  
  density = 1.86*g/cm3;
  G4Material* FR_4 = new G4Material("FR_4"  , density, ncomponents=2);
  FR_4->AddMaterial(SiO2, fractionmass=0.528);
  FR_4->AddMaterial(Epoxy, fractionmass=0.472);
  
  //===================================================================================================================================
  //Detector gases
  
  // 90% Ar + 10% CO2, STP                                                                                                                                                                                         
  density = 1.6689*mg/cm3 ;   //1.8036*mg/cm3 ;                                                                                                                                                                    
  G4Material *Ar_90CO2_10 = new G4Material(name="ArCO2_9010", density, ncomponents=2, kStateGas,300*kelvin,1.*atmosphere);
  Ar_90CO2_10->AddElement(elAr, fractionmass = 0.891);
  Ar_90CO2_10->AddMaterial(CO2, fractionmass = 0.109);
  Ar_90CO2_10->GetIonisation()->SetMeanEnergyPerIonPair(fInput->GetGasFlag().WmeanAr_90CO2_10 *eV);
  
  // 70% Ar + 30% CO2, STP                                                                                                                                                                                         
  density = 1.7067*mg/cm3 ;   //1.8391*mg/cm3 ;                                                                                                                                                                    
  G4Material *Ar_70CO2_30 = new G4Material(name="ArCO2_7030", density, ncomponents=2, kStateGas,300*kelvin,1.*atmosphere);
  Ar_70CO2_30->AddElement(elAr, fractionmass = 0.68);
  Ar_70CO2_30->AddMaterial(CO2, fractionmass = 0.32);
  Ar_70CO2_30->GetIonisation()->SetMeanEnergyPerIonPair(fInput->GetGasFlag().WmeanAr_70CO2_30 *eV);
  
  // ALICE mixture TPC_Ne-CO2                                                                                                                                                                                      
  density = 0.93572*mg/cm3 ;
  G4Material *NeCO2 = new G4Material(name="NeCO2_9010", density, ncomponents=2, kStateGas,300*kelvin,1.*atmosphere);
  NeCO2->AddElement(elNe, fractionmass = 0.8035);
  NeCO2->AddMaterial(CO2, fractionmass = 0.1965);
  NeCO2->GetIonisation()->SetMeanEnergyPerIonPair(fInput->GetGasFlag().WmeanNeCO2 *eV);
  
  // ALICE upgrade mixture TPC_Ne-CO2-N2                                                                                                                                                                         
  density = 0.94645*mg/cm3 ;
  G4Material *NeCO2N2 = new G4Material(name="NeCO2N2_90105", density, ncomponents=3, kStateGas,300*kelvin,1.*atmosphere);
  NeCO2N2->AddElement(elNe, fractionmass = 0.7565);
  NeCO2N2->AddMaterial(CO2, fractionmass = 0.1846);
  NeCO2N2->AddMaterial(N2, fractionmass = 0.0589);
  NeCO2N2->GetIonisation()->SetMeanEnergyPerIonPair(fInput->GetGasFlag().WmeanNeCO2N2 *eV);
  
  G4Material *detectorGas = NeCO2N2;
  if(detectorFlag == 1) detectorGas = NeCO2;
  if(detectorFlag == 2) detectorGas = Ar_90CO2_10;
  if(detectorFlag == 3) detectorGas = Ar_70CO2_30;
  
  //___________________________________________________________________________________________________________________________________
  // Definition of active detector volume
  
  G4ThreeVector ActiveVolumePos = G4ThreeVector(0, 0, 0);
  ActiveVolumeBox = new G4Box("ActiveVolumeBox", 5*cm, 5*cm, 5*cm);
  ActiveVolumeLog = new G4LogicalVolume(ActiveVolumeBox, detectorGas, "ActiveVolumeLog", 0, 0, 0);
  new G4PVPlacement(0, ActiveVolumePos, ActiveVolumeLog,"gas",mother,0,0,true);
    
  sensitiveDetector = new SensitiveDetector("Detector",0);
  ActiveVolumeLog->SetSensitiveDetector(sensitiveDetector);
    
  //___________________________________________________________________________________________________________________________________
  // Definition of the cathode

  PCBBox   = new G4Box("PCBBox", 5*cm, 5*cm,0.15*cm);
  cylinder = new G4Tubs("cylinder",0.,0.39*cm,0.151*cm,0.*deg,360.*deg);
  subtract = new G4SubtractionSolid("subtract", PCBBox, cylinder, 0, G4ThreeVector(0.,0.,0.));
  
  cylinderLog = new G4LogicalVolume(cylinder, detectorGas, "cylinderLog", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0*cm, 0*cm,-5.15*cm), cylinderLog,"gas",mother,0,0,true);
  
  PCBLog   = new G4LogicalVolume(subtract, FR_4, "PCBLog", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0.,0.,-5.15*cm), PCBLog,"PCB", mother, 0,0,true);
  
//   G4EmCalculator cal;
//   std::cout << cal.ComputeTotalDEDX(1*MeV, G4Alpha::AlphaDefinition(), detectorGas);
  
  
  //___________________________________________________________________________________________________________________________________
  // Definition of the electric field
  
  
  fEMfield = new G4UniformElectricField(G4ThreeVector(0.0,0.0, -EMfieldFlag*volt/cm));
  
  // Create an equation of motion for this field
  fEquation = new G4EqMagElectricField(fEMfield); 
  
  G4int nvar = 8;
  fStepper = new G4ClassicalRK4( fEquation, nvar );       
  
  // Get the global field manager 
  fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();

  // Set this field to the global field manager 
  fFieldManager->SetDetectorField(fEMfield );
  
  fMinStep     = 0.001*mm ; // minimal step of 1 microns
  
  fIntgrDriver = new G4MagInt_Driver(fMinStep, fStepper, fStepper->GetNumberOfVariables() );
  
  fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );  
}
