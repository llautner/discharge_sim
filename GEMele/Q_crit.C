#include "HitEvent.hpp"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStopwatch.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom3.h"
#include "RandomRing.h"

// ./qCrit [Qcrit] [gasFlag] [pitch] [tInt] [filename] [field] [readoutSize] [Ecoll]

int main(int argc,char** argv){

  const int ClusterSize = atoi(argv[1]);
  const int gasFlag = atoi(argv[2]);
  const int pitch = atoi(argv[3]);
  const char *tIntegration = argv[4];
  const char *filename = argv[5];
  const int field = atoi(argv[6]); 
  const float readoutSize = atof(argv[7]);
  const float Ecoll = atof(argv[8]);

 //___________________________________________________________________________________________
 // Initialize environment and parameters
      
  const char *gas="Ne-CO2-N2_90-10-5";
  Double_t SigmaOverMu = 0.78;
  if(gasFlag == 0) gas = "Ne-CO2-N2_90-10-5"; SigmaOverMu = 0.78;
  if(gasFlag == 1) gas = "Ne-CO2_90-10"; SigmaOverMu = 0.74;
  if(gasFlag == 2) gas = "Ar-CO2_90-10"; SigmaOverMu = 0.84; 
  if(gasFlag == 3) gas = "Ar-CO2_70-30"; SigmaOverMu = 0.84;
  if(gasFlag == 4) gas = "Ar-CH4_50-50"; SigmaOverMu = 0.84;
  
  const int nSteps = 100;

  const Int_t nMulti = 62;
  float multiplication[nMulti] = {178., 190., 205., 220., 231., 235., 243., 250., 269., 275., 279., 302., 325., 340., 363., 375, 
409., 425., 475., 525., 575., 600., 625., 675., 725., 775., 825., 875., 925., 947., 975., 980., 990., 1010., 1025., 1031., 1049., 1059., 1075., 1084., 1087., 1096., 1098., 1126., 1130., 1139., 1144., 1152., 1154., 1175., 1200., 1225., 1275., 1279., 1325., 1330., 1375., 1436., 1500., 1600., 1800., 2000.};
  const float readout[nSteps] = {10.f, 18.5f, 30.5f, 35.5f, 36.f, 38.f};
  double HitCounter[nSteps];
  double discharge[nSteps][nMulti];
  double discharge_Ecoll[nSteps][nMulti];
  double dischargeFluct[nSteps][nMulti];

  TFile *file = new TFile(Form("%s", filename));
  TTree *tree = (TTree*)file->Get("HoleHits");
  
  HitEvent *hitEvent = new HitEvent();
  tree->SetBranchAddress("hitEvent", &hitEvent);
  const double nev = tree->GetEntries();
  
  //POLYA-DISTRIBUTION FOR THE GAIN FLUCTUATIONS
  //const float kappa = 1/(SigmaOverMu*SigmaOverMu);
  //const float s = 1/kappa;
  //const float theta = 1/SigmaOverMu -1;  
  //TF1 *fPolya = new TF1("Polya", Form("1/(TMath::Gamma(%e)*%e) *pow(x/%e, (%e)) *exp(-x/%e)", kappa, s, s, kappa-1, s), 0, 1000);
  //TRandom3 *rand = new TRandom3;
  
  RandomRing random(10000000);
  
  //_____________________________________________________________________________________
  // Event loop  
  
  // reset variables
  for(int bins=0; bins<nSteps; ++bins){
    HitCounter[bins]=0;
    for(Int_t mult=0; mult<nMulti; ++mult){
      discharge[bins][mult]=0;
      dischargeFluct[bins][mult]=0;
    }
  }
  
  int noDoubleCount[nSteps][nMulti];
  int noDoubleCount_Ecoll[nSteps][nMulti];
  int noDoubleCountFluct[nSteps][nMulti];
  
  TStopwatch timerPost;
  timerPost.Start();
  
  for(Int_t iev=0; iev<int(nev); ++iev){
    tree->GetEntry(iev);
    
    // reset double counting prevention
    for(int bins=0; bins<nSteps; ++bins){
      for(Int_t mult=0; mult<nMulti; ++mult){
        noDoubleCount[bins][mult]=0;
        noDoubleCountFluct[bins][mult]=0;
      }
    }
    
    for(Int_t ihit=0; ihit<int(hitEvent->GetNhit()); ++ihit){
      HoleHit *holeHit = (HoleHit*)hitEvent->GetHit()->UncheckedAt(ihit);
      
      const double nEle = holeHit->GetNele();
      const int readoutPos = int(holeHit->GetZPos());
      
      ++HitCounter[readoutPos];
          
      // gain fluctuations according to Polya distribution
      //const float gainFluct = fPolya->GetRandom();
      const double nEleFluct = (random.getNextValue() * std::sqrt(nEle)) + nEle;
      
      // fluctuations of critical charge according to measurements of the hole diameter (2.5 %) - enter quadratically - effective area of the hole -> charge density
      //const float QcritFluct = rand->Gaus(1., 0.025/2);
      const double QcritFluct = (random.getNextValue() * 0.0125) + 1.;
      
      for(Int_t mult=0; mult<nMulti; ++mult){
	const double totalCharge = nEle*multiplication[mult];
       
//         const float totalChargeFluct = nEle * multiplication[mult]/kappa * gainFluct * QcritFluct * QcritFluct;
        const double totalChargeFluct = nEleFluct * multiplication[mult] * QcritFluct * QcritFluct;
        
        if(totalCharge > ClusterSize && noDoubleCount[readoutPos][mult] == 0){
          ++discharge[readoutPos][mult];
          noDoubleCount[readoutPos][mult] = 1;
        }
        if(totalCharge*Ecoll > ClusterSize && noDoubleCount_Ecoll[readoutPos][mult] == 0){
          ++discharge_Ecoll[readoutPos][mult];
          noDoubleCount_Ecoll[readoutPos][mult] = 1;
        }
        if(totalChargeFluct > ClusterSize && noDoubleCountFluct[readoutPos][mult] == 0){
          ++dischargeFluct[readoutPos][mult];
          noDoubleCountFluct[readoutPos][mult] = 1;
        }
      }
    }
  }
  
  timerPost.Stop();
  timerPost.Print();
  
  //_______________________________________________________________________________________________
  // Post-processing
 
  const char *directory = "~/Results";
  TFile *outFile = new TFile(Form("%s/simulation_%s_%s_%i_%i_%i.root", directory, gas, tIntegration, field, int(readoutSize), int(ClusterSize)), "RECREATE");

  double normalization = nev;
//   double multi, nDischM, nDischMFluct, nDischR;
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // M-curves, no fluctuations
  TGraphErrors grMultiplication[nSteps];
  
  int i[nSteps];
  for(int bin=0; bin<nSteps; ++bin){
    i[bin]=0;
  }
  
  for(int bin=0; bin<nSteps; ++bin){
    if(HitCounter[int(readout[bin])] == 0) continue;
    grMultiplication[int(readout[bin])].SetTitle(Form("z=%.1f mm ;Multiplication;Discharge probability", float(int(readout[bin]))));
    for(Int_t mult=0; mult<nMulti; ++mult){
      const double multi=multiplication[mult];
      const double nDischM=discharge[int(readout[bin])][mult];
      grMultiplication[int(readout[bin])].SetPoint(i[bin], multi, nDischM/normalization);
      grMultiplication[int(readout[bin])].SetPointError(i[bin], 0, sqrt(nDischM/(normalization*normalization) + (nDischM*nDischM)/(normalization*normalization*normalization)));
      ++i[bin];
    }
    grMultiplication[int(readout[bin])].Write(Form("grMultiplication[%i]", int(readout[bin])));
  }
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // M-curves, no fluctuations, collection efficiency
  TGraphErrors grMultiplication_Ecoll[nSteps];
  
  for(int bin=0; bin<nSteps; ++bin){
    i[bin]=0;
  }
  
  for(int bin=0; bin<nSteps; ++bin){
    if(HitCounter[int(readout[bin])] == 0) continue;
    grMultiplication_Ecoll[int(readout[bin])].SetTitle(Form("z=%.1f mm ;Multiplication;Discharge probability; Ecoll=%.3f", float(int(readout[bin])),float(Ecoll)));
    for(Int_t mult=0; mult<nMulti; ++mult){
      const double multi=multiplication[mult];
      const double nDischM_Ecoll=discharge_Ecoll[int(readout[bin])][mult];
      grMultiplication_Ecoll[int(readout[bin])].SetPoint(i[bin], multi, nDischM_Ecoll/normalization);
      grMultiplication_Ecoll[int(readout[bin])].SetPointError(i[bin], 0, sqrt(nDischM_Ecoll/(normalization*normalization) + (nDischM_Ecoll*nDischM_Ecoll)/(normalization*normalization*normalization)));
      ++i[bin];
    }
    grMultiplication_Ecoll[int(readout[bin])].Write(Form("grMultiplication_Ecoll[%i]", int(readout[bin])));
  }
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // M-curves, fluctuations
  TGraphErrors grMultiplicationFluct[nSteps];
  for(int bin=0; bin<nSteps; ++bin){
    i[bin]=0;
  }
  for(int bin=0; bin<nSteps; ++bin){
    if(HitCounter[int(readout[bin])] == 0) continue;
    grMultiplicationFluct[int(readout[bin])].SetTitle(Form("z=%.1f mm (fluct);Multiplication;Discharge probability", float(int(readout[bin]))));
    for(Int_t mult=0; mult<nMulti; ++mult){
      const double multi=multiplication[mult];
      const double nDischMFluct=dischargeFluct[int(readout[bin])][mult];
      grMultiplicationFluct[int(readout[bin])].SetPoint(mult, multi, nDischMFluct/normalization);
      grMultiplicationFluct[bin].SetPointError(mult, 0, sqrt(nDischMFluct/(normalization*normalization) + (nDischMFluct*nDischMFluct)/(normalization*normalization*normalization)));
      ++i[bin];
    }
    grMultiplicationFluct[int(readout[bin])].Write(Form("grMultiplicationFluct[%i]", int(readout[bin])));
  }
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Range curves, no fluctuations
  TGraphErrors grRange[nMulti];
    
  int j[nMulti];
  for(Int_t mult=0; mult<nMulti; ++mult){
    j[mult]=0;
  }
  
  for(Int_t mult=0; mult<nMulti; ++mult){
    grRange[mult].SetTitle(Form("Multiplication=%.0f ;#it{d}_{source} [cm];Discharge probability", multiplication[mult]));
    for(int bin=0; bin<nSteps; ++bin){      
      if(HitCounter[int(readout[bin])] == 0) continue;
      const double nDischR=discharge[int(readout[bin])][mult];
      grRange[mult].SetPoint(j[mult], (readout[bin]+1.5)/10., nDischR/normalization);
      grRange[mult].SetPointError(j[mult], 0, sqrt(nDischR/(normalization*normalization) + nDischR*nDischR/(normalization*normalization*normalization)));
      ++j[mult];
    }
    grRange[mult].Write(Form("grRange[%.0f]", multiplication[mult]));
  }
  
   
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Range curves, no fluctuations, collection efficiency
  TGraphErrors grRange_Ecoll[nMulti];
    
  for(Int_t mult=0; mult<nMulti; ++mult){
    j[mult]=0;
  }
  
  for(Int_t mult=0; mult<nMulti; ++mult){
    grRange_Ecoll[mult].SetTitle(Form("Multiplication=%.0f ;#it{d}_{source} [cm];Discharge probability; Ecoll = %.3f", multiplication[mult], float(Ecoll)));
    for(int bin=0; bin<nSteps; ++bin){      
      if(HitCounter[int(readout[bin])] == 0) continue;
      const double nDischR_Ecoll=discharge_Ecoll[int(readout[bin])][mult];
      grRange_Ecoll[mult].SetPoint(j[mult], (readout[bin]+1.5)/10., nDischR_Ecoll/normalization);
      grRange_Ecoll[mult].SetPointError(j[mult], 0, sqrt(nDischR_Ecoll/(normalization*normalization) + nDischR_Ecoll*nDischR_Ecoll/(normalization*normalization*normalization)));
      ++j[mult];
    }
    grRange_Ecoll[mult].Write(Form("grRange[%.0f]", multiplication[mult]));
  }

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Range curves, no fluctuations
  
  TGraphErrors grRangeFluct[nMulti];
  for(Int_t mult=0; mult<nMulti; ++mult){
    j[mult]=0;
  }
  
  for(Int_t mult=0; mult<nMulti; ++mult){
    grRangeFluct[mult].SetTitle(Form("Multiplication=%.0f (fluct);#it{d}_{source} [cm];Discharge probability", multiplication[mult]));
    for(int bin=0; bin<nSteps; ++bin){  
      if(HitCounter[int(readout[bin])] == 0) continue;
      const double nDischRFluct=dischargeFluct[int(readout[bin])][mult];
      grRangeFluct[mult].SetPoint(j[mult], (readout[bin]+1.5)/10., nDischRFluct/normalization);
      grRangeFluct[mult].SetPointError(j[mult], 0, sqrt(nDischRFluct/(normalization*normalization) + nDischRFluct*nDischRFluct/(normalization*normalization*normalization)));
      ++j[mult];    
    }
    grRangeFluct[mult].Write(Form("grRangeFluct[%.0f]", multiplication[mult]));
  }
  
  outFile->Close();  
}
