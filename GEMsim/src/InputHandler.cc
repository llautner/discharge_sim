#include "InputHandler.hh"

#include <iostream>

//____________________________________________________________________________________________________________________________________________________________
InputHandler* InputHandler::inputHandler = NULL;

//____________________________________________________________________________________________________________________________________________________________
InputHandler* InputHandler::GetInstance()
{
  if (inputHandler == NULL) {
    inputHandler = new InputHandler();
  }
  return inputHandler;
}

//____________________________________________________________________________________________________________________________________________________________
InputHandler::InputHandler() {
        visualizationOn = false;
        random = new TRandom3(0);
}

//____________________________________________________________________________________________________________________________________________________________
InputHandler::~InputHandler() {
  delete random;
}

//____________________________________________________________________________________________________________________________________________________________
void InputHandler::Init(char** argv)
{
  int nev = G4UIcommand::ConvertToInt(argv[1]);
  driftField = G4UIcommand::ConvertToDouble(argv[4]);
  gasFlag.gasParameter = G4UIcommand::ConvertToInt(argv[3]);
  TString flag = argv[2];
  if(flag == "VIS"){
    visualizationOn = true;
  }
  
  TString gasAppendix = "Ne-CO2-N2_90-10-5";
  if(gasFlag.gasParameter == 1) gasAppendix = "Ne-CO2_90-10";
  if(gasFlag.gasParameter == 2) gasAppendix = "Ar-CO2_90-10";
  if(gasFlag.gasParameter == 3) gasAppendix = "Ar-CO2_70-30";
  
  gasFlag.WmeanNeCO2N2 = 37.3;
  gasFlag.WmeanNeCO2 = 38.1;
  gasFlag.WmeanAr_90CO2_10 = 28.8;
  gasFlag.WmeanAr_70CO2_30 = 28.1;
  
  output.filename = "output_" +  gasAppendix + "_" + argv[4] +".root"; 
  
  G4cout << "*************************************************************" << G4endl;
  G4cout << "Number of events " << nev << G4endl; 
  G4cout <<  "Writing to file " << output.filename << G4endl;
  G4cout << "Gas parameter " << gasFlag.gasParameter << G4endl;
  G4cout << "Drift Field " << driftField << G4endl;
  G4cout << "*************************************************************" << G4endl;
}
