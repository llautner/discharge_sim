#include <iterator>
#include <vector>
#include <iostream>
#include <cmath>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TRandom3.h"
#include "GEMEvent.hpp"
#include "GEM.h"
#include "HitEvent.hpp"
#include "RandomRing.h"
#include "TNtuple.h"
#include "Riostream.h"
#include <fstream>


// ./GEMdiffusion  directory  gasFlag  tIntegration [ns]  pitch [um] nEvents field [V/cm] 

int main(int argc,char** argv){
 
  const char *directory = argv[1];
  const int gasFlag = atoi(argv[2]);                                                                                                                                         
  const float tIntegration = atof(argv[3]);
  const float pitch = atof(argv[4]);
  const int nEvents = atoi(argv[5]);
  const float field = atof(argv[6]);
 
  //____________________________________________________________________________________________________________________________________________________________
  // Gas and detector parameters  
  
  const char *gasDirectory = "/home/ga24get";
  const char *gas="Ne-CO2-N2_90-10-5";
  const char *gasFilename;
  float wIon = 37.3f; float diffL = 0.0218f; float diffT = 0.0223f; float vdrift = 2.52f;
  
  if(gasFlag ==2) {
    
    gas = "Ne-CO2_90-10";
    wIon = 38.1f;
    diffL = 0.0223f;
    diffT = 0.0219f;
    vdrift = 2.66f;
  }

  if(gasFlag == 2) {

    gas = "Ar-CO2_90-10"; 
    wIon = 28.8f;
    diffL = 0.0244f;
    diffT = 0.0268f;
    vdrift = 3.25f;
  } 

  if(gasFlag == 3) {
    gas = "Ar_CO2_70-30";
    wIon = 28.1f;
    diffL = 0.0138f;
    diffT = 0.0145f;
    vdrift = 0.932f;
  }

  wIon /=1000000.f;

//____________________________________________________________________________________________________________________________________
//  Read file and set gas coeff

  gasFilename = Form("%s/%s_fullv.dat", gasDirectory, gas);

  std::ifstream fin(gasFilename);
  if(!fin) {
    std:cerr << "Unable to open " << gasFilename << endl;
  }

  FILE *gasdat = fopen(gasFilename,"r");
  float field_A, diffL_A, diffT_A, vdrift_A;
  int nlines = 0;
  int ncols;

  bool isField = false;

  while(fin.good()) {
    ncols = fscanf(gasdat,"%f %f %f %f", &field_A, &vdrift_A, &diffL_A, &diffT_A);
    if (ncols < 0) break;
    if (std::abs(field_A - field) < 0.01f) {
      isField = true;
      diffL = diffL_A/10000.f;
      diffT = diffT_A/10000.f;
      vdrift = vdrift_A;
      break;
    }      
    ++nlines;	
  }

  if (!isField) {
    std::cout << "No parameters for drift field value!" << endl;
    return 0;
  }
  fclose(gasdat);
 
  const float zIntegration = vdrift * tIntegration * 1/100.f; // vdrift [cm/us], tIntegration [ns] and zIntegration [mm]
  
  const int nSteps = 4;
  const float readout[nSteps] = {10.f, 18.5f, 30.5f, 38.f};
   
  std::cout << "__________________________________________________________________________________________________\n";
  std::cout << "Initializing analysis for " << gas <<  "\n";
  std::cout << "GEM hole pitch: " << pitch << " um \n";
  std::cout << "Drift enabled - integration time " << tIntegration << " ns - drift distance " << zIntegration << " mm \n";
  std::cout << "Drift velocity: " << vdrift << " longitudinal diffusion: " << diffL << " transversal diffusion: " << diffT << endl; 
  //____________________________________________________________________________________________________________________________________________________________
  // Input file
  

  const char *filename = Form("%s/output_%s_%s.root", directory, gas, argv[6]);
  printf("Processing file %s \n", filename);
  TFile *file = new TFile(filename);
  if(!file) return 0;
  TTree *tree = (TTree*)file->Get("GEM");
  if(!tree) return 0;
  
  GEMEvent *event = new GEMEvent();
  tree->SetBranchAddress("GEMEvent", &event);
  
  //____________________________________________________________________________________________________________________________________________________________
  // Output file and processing classes
  
  const char *outfileName = Form("%s/GEMdiffusion_%s_%s_%s_%i.root", directory, gas, argv[6], argv[3], int(pitch));
  
  TFile outfile(Form("%s", outfileName), "RECREATE");
  TTree out("HoleHits","Collection of all hits");
  
  HitEvent hitEvent;
  out.Branch("hitEvent", &hitEvent);
  out.BranchRef();
  
  GEM gem(pitch, 100);
  
  RandomRing random(10000000);
  
  //____________________________________________________________________________________________________________________________________________________________
  // Event loop
  
  TStopwatch timer;
  timer.Start();
      
  std::map <float, std::map<int, int> > GEMhits;
  
  for(int iev=0; iev<tree->GetEntries(); ++iev){
    if(iev == nEvents) break;
    hitEvent.Clear(0);
    hitEvent.SetNevt(iev);
    tree->GetEntry(iev);
    for(int ihit=0; ihit<event->GetNhit(); ++ihit){
      Hit *hit = (Hit*)event->GetHit()->UncheckedAt(ihit);
      const int nele = hit->GetEdep()/wIon;
      if(nele < 1) continue;
      const float hitX = hit->GetX() + 50.f;
      const float hitY = hit->GetY() + 50.f;
      const float hitZ = hit->GetZ() + 50.f;
            
      for(int zStep = 0; zStep<nSteps; ++zStep){
        const float readoutPos = readout[zStep];
        const float sqrtDriftLength = std::sqrt((readoutPos-hitZ) * 0.1f);      // should be in cm
        const float sigmaT = sqrtDriftLength*diffT*10.f;                        // transform back to mm
        const float sigmaL = sqrtDriftLength*diffL*10.f;                        // transform back to mm
        if(hitZ<readoutPos-zIntegration-3.f*sigmaL || hitZ>readoutPos+3.f*sigmaL || (readoutPos-hitZ) < 0.f ) continue;
        for(int ele=0; ele<nele; ++ele){
          //charge spread by diffusion
          const float diffX = (random.getNextValue() * sigmaT) + hitX;
          const float diffY = (random.getNextValue() * sigmaT) + hitY;
          const float diffZ = (random.getNextValue() * sigmaL) + hitZ;
          
          if(diffZ < readoutPos-zIntegration || diffZ > readoutPos) continue;
          GEMhits[readoutPos][gem.GetPadID(diffX, diffY)] +=1;
        }
      }
    }
    
    // this loop does not affect the performance at all!!!
    //for(auto iterGEMpos = GEMhits.begin(); iterGEMpos != GEMhits.end(); ++iterGEMpos) {
    for(std::map <float, std::map<int, int> >::iterator iterGEMpos = GEMhits.begin(); iterGEMpos != GEMhits.end(); ++iterGEMpos) {
      float poszInt = iterGEMpos->first;
      std::map<int, int> &hits = GEMhits.at(iterGEMpos->first);
      int nEleHighest = -1;
      //for(auto iterHit = hits.begin(); iterHit != hits.end(); ++iterHit) {
      for(std::map<int, int>::iterator iterHit = hits.begin(); iterHit != hits.end(); ++iterHit) { 
        if(hits.at(iterHit->first) < 0) continue;
        int currentNEle = hits.at(iterHit->first);
        if(currentNEle > nEleHighest){
          nEleHighest = currentNEle;
        }
      }
      hitEvent.AddHit(poszInt, nEleHighest);
    }
    
    out.Fill();
    GEMhits.clear();
    std::map <float, std::map<int, int> >(GEMhits).swap(GEMhits);  //de-allocate memory of the std::map
  }
  
  timer.Stop();
  timer.Print();
  
  outfile.Write();
  outfile.Close();
  
  delete file;
  delete event;

  return 0;
}
