#ifndef GEMEvent_hpp
#define GEMEvent_hpp

#include "TObject.h"
#include "TString.h"
#include "TMath.h"   
#include "TRandom.h"   
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TVector3.h"

class TDirectory;

//____________________________________________________________________________________________________________________________________________________________
class Hit : public TObject 
{
  
private:
  Double_t      fX;
  Double_t      fY;
  Double_t      fZ;
  Double_t      fEdep;
  Double_t      fSteplength;
  Int_t         fPID;
  
public:
  Hit(){ };
  virtual ~Hit() { };
  
  void  SetX(Double_t d) {fX = d;}
  void  SetY(Double_t d) {fY = d;}
  void  SetZ(Double_t d) {fZ = d;}
  void  SetEdep(Double_t d) {fEdep = d;}
  void  SetSteplength(Double_t d) {fSteplength = d;}
  void  SetPID(Int_t i) {fPID = i;}
  
  Double_t GetX()       const {return fX;}
  Double_t GetY()       const {return fY;}
  Double_t GetZ()       const {return fZ;}
  Double_t GetEdep()    const {return fEdep;}
  Double_t GetStep()    const {return fSteplength;}
  Int_t    GetPID()     const {return fPID;}
  
  ClassDef(Hit,1)  //A track hit
};

//____________________________________________________________________________________________________________________________________________________________
class GEMEvent : public TObject
{     
  
private:
  TVector3      fVertex;            //vertex coordinates

  Int_t         fNhit;             //Number of hist in detector
  Int_t         fNevt;
  Double_t      fEdep;
  Double_t      fRange;
  Double_t      fGunEnergy;
  TClonesArray  *fHits;             //->array of hits in detector
  
public :
  
  GEMEvent()
  {
    fHits  = new TClonesArray("Hit", 10000000);
    fNhit = 0;
    fNevt = 0;
    fRange = 0;
    fEdep = 0;
    fGunEnergy = 0;
    fVertex = TVector3(0,0,0);
  }
  
  virtual ~GEMEvent () {Reset(0);}
  
  //____________________________________________________________________________________________________________________________________________________________
  void Clear(Option_t *option)
  {
    fNhit = 0;
    fNevt = 0;
    fRange = 0;
    fEdep = 0;
    fGunEnergy = 0;
    fVertex = TVector3(0,0,0);
    fHits->Clear(option);
  }
  
  //____________________________________________________________________________________________________________________________________________________________
  void Reset(Option_t *)
  {
    delete fHits;  fHits = 0;
  }
  
  //____________________________________________________________________________________________________________________________________________________________
  
  TClonesArray  *GetHit()       const {return fHits;}
  Int_t         GetNhit()       const { return fNhit;}
  Int_t         GetNevt()       const { return fNevt;}
  Double_t      GetEdep()       const {return fEdep;}
  Double_t      GetRange()      const {return fRange;}
  TVector3      GetVertex()     const {return fVertex;}
  Double_t      GetGunEnergy()  const {return fGunEnergy;}
  

  void  SetNevt(Int_t i)        {fNevt = i;}
  void  SetVertex(TVector3 v)   {fVertex = v;}
  void  SetEdep(Double_t d)     {fEdep = d;}
  void  SetRange(Double_t d)    {fRange = d;}
  void  SetGunEnergy(Double_t d) {fGunEnergy = d;}
  
  //____________________________________________________________________________________________________________________________________________________________
  Hit *AddHit(Double_t posX, Double_t posY, Double_t posZ, Double_t Edep, Double_t Steplength, Int_t pid)
  {
    TClonesArray &hits = *fHits;
    Hit *hit = new(hits[fNhit++]) Hit();
    hit->SetX(posX);
    hit->SetY(posY);
    hit->SetZ(posZ);
    hit->SetEdep(Edep);
    hit->SetSteplength(Steplength);
    hit->SetPID(pid);
    return hit;
  }
  
public:
  ClassDef (GEMEvent,1) ; 
  
} ;


#endif
