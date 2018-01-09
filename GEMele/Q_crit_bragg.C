#include "HitEvent.hpp"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStopwatch.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom3.h"


// ./qCrit_bragg [Qcrit] [gasFlag] [pitch] [tInt] [filename]
int main(int argc,char** argv){

  const int ClusterSize = atoi(argv[1]);
  const int gasFlag = atoi(argv[2]);
  const int pitch = atoi(argv[3]);
  const int tIntegration = atoi(argv[4]);
  const char *filename = argv[5];
  
  //____________________________________________________________________________________________________________________________________________________________
  // Initialize environment and parameters
      
  //TODO: Sigma over Mu  values for Ar-based mixtures!
  const char *gas="Ne-CO2-N2_90-10-5";
  Double_t SigmaOverMu = 0.78;
  if(gasFlag == 1) gas = "Ne-CO2_90-10"; SigmaOverMu = 0.74;
  if(gasFlag == 2) gas = "Ar-CO2_90-10"; SigmaOverMu = 0.84; 
  if(gasFlag == 3) gas = "Ar-CO2_70-30"; SigmaOverMu = 0.84;
  
  const int nSteps = 100;
  const Int_t nMulti = 31;
  float multiplication[nMulti] = {275, 325., 375, 425., 475., 525., 575., 600., 625., 675., 725., 775., 825., 875., 925., 975., 1025., 1075., 1125., 1175., 1200., 1225., 1275., 1325., 1375., 1500, 1750, 2000., 3000., 4000., 5000.};
  const float readout[nSteps] = {2, 4, 6, 8, 10, 12, 13, 14, 16, 18, 20, 22, 23, 24, 26, 28, 30.5, 32, 34, 36, 38, 40, 42, 44, 46, 48, 49.7, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 71, 73, 76, 79, 82};
  float HitCounter[nSteps];
  float discharge[nSteps][nMulti];
  float dischargeFluct[nSteps][nMulti];

  TFile *file = new TFile(Form("%s", filename));
  TTree *tree = (TTree*)file->Get("HoleHits");
  
  HitEvent *hitEvent = new HitEvent();
  tree->SetBranchAddress("hitEvent", &hitEvent);
  const float nev = tree->GetEntries();
  
  //POLYA-DISTRIBUTION FOR THE GAIN FLUCTUATIONS
  const float kappa = 1/(SigmaOverMu*SigmaOverMu);
  const float s = 1/kappa;
  const float theta = 1/SigmaOverMu -1;  
  TF1 *fPolya = new TF1("Polya", Form("1/(TMath::Gamma(%e)*%e) *pow(x/%e, (%e)) *exp(-x/%e)", kappa, s, s, kappa-1, s), 0, 1000);
  
  TRandom3 *rand = new TRandom3;
  
  //____________________________________________________________________________________________________________________________________________________________
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
      
      const float nEle = holeHit->GetNele();
      const int readoutPos = int(holeHit->GetZPos());
      
      ++HitCounter[readoutPos];
          
      // gain fluctuations according to Polya distribution
      const float gainFluct = fPolya->GetRandom();
      
      // fluctuations of critical charge according to measurements of the hole diameter (2.5 %) - enter quadratically - effective area of the hole -> charge density
      const float QcritFluct = rand->Gaus(1., 0.025/2);
      
      for(Int_t mult=0; mult<nMulti; ++mult){
        const float totalCharge = nEle*multiplication[mult];
        const float totalChargeFluct = nEle * multiplication[mult]/kappa * gainFluct * QcritFluct * QcritFluct;

        if(totalCharge > ClusterSize && noDoubleCount[readoutPos][mult] == 0){
          ++discharge[readoutPos][mult];
          noDoubleCount[readoutPos][mult] = 1;
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
  
  //____________________________________________________________________________________________________________________________________________________________
  // Post-processing
 
  const char *directory = "~/Results";
  TFile *outFile = new TFile(Form("%s/bragg_%s_%i_%i.root", directory, gas, int(tIntegration), int(ClusterSize)), "RECREATE");
  
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
      const float multi=multiplication[mult];
      const float nDischM=discharge[int(readout[bin])][mult];
      grMultiplication[int(readout[bin])].SetPoint(i[bin], multi, nDischM/normalization);
      grMultiplication[int(readout[bin])].SetPointError(i[bin], 0, sqrt(nDischM/(normalization*normalization) + (nDischM*nDischM)/(normalization*normalization*normalization)));
      ++i[bin];
    }
    grMultiplication[int(readout[bin])].Write(Form("grMultiplication[%i]", int(readout[bin])));
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
      const float multi=multiplication[mult];
      const float nDischMFluct=dischargeFluct[int(readout[bin])][mult];
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
      const float nDischR=discharge[int(readout[bin])][mult];
      grRange[mult].SetPoint(j[mult], (readout[bin]+1.5)/10., nDischR/normalization);
      grRange[mult].SetPointError(j[mult], 0, sqrt(nDischR/(normalization*normalization) + nDischR*nDischR/(normalization*normalization*normalization)));
      ++j[mult];
    }
    grRange[mult].Write(Form("grRange[%.0f]", multiplication[mult]));
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
