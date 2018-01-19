#ifndef INPUTHANDLER_hh
#define INPUTHANDLER_hh 1

#include "TRandom3.h"
#include "G4ThreeVector.hh"
#include "TString.h"
#include <iostream>
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

struct Output
{
  TString filename;
};



struct GasFlag
{
  G4int gasParameter;
  G4double WmeanNeCO2N2;
  G4double WmeanNeCO2;
  G4double WmeanAr_90CO2_10;
  G4double WmeanAr_70CO2_30;
};

class InputHandler {
public:
  static InputHandler* GetInstance();
  ~InputHandler();
  
  void Init(char** argv);
  
  float GetDriftField() const {return driftField;}
  Bool_t UseVis(){return visualizationOn;};
  TRandom3* getTRandom(){return random;};  
  Output GetOutput(){return output;};
  GasFlag GetGasFlag(){return gasFlag;};
  GasFlag GetWmeanNeCO2N2(){return WmeanNeCO2N2;};
  GasFlag GetWmeanNeCO2(){return WmeanNeCO2;};
  GasFlag GetWmeanAr_90CO2_10(){return WmeanAr_90CO2_10;};
  GasFlag GetWmeanAr_70CO2_30(){return WmeanAr_70CO2_30;};
  
private:
  InputHandler(void);
  static InputHandler* inputHandler;
  
  TRandom3* random;
  Bool_t visualizationOn;
  Output output;
  float driftField;
  GasFlag gasFlag;
  GasFlag WmeanNeCO2N2;
  GasFlag WmeanNeCO2;
  GasFlag WmeanAr_90CO2_10;
  GasFlag WmeanAr_70CO2_30;
};

#endif /* INPUTHANDLER_HH_ */
