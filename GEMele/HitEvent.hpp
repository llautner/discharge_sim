#ifndef HitEvent_hpp
#define HitEvent_hpp

#include "TObject.h"
#include "TString.h"
#include "TMath.h"   
#include "TRandom.h"   
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TVector3.h"

class TDirectory;

//____________________________________________________________________________________________________________________________________________________________
class HoleHit : public TObject 
{
public:
  HoleHit(){ };
  virtual ~HoleHit() { };
  
  
  void SetZpos(int n) {mZpos = n;}
  void SetNele(float n) {mEle = n;}
  
  int  GetNele() const {return mEle;}
  float GetZPos() const {return mZpos;}
  
private:
  UShort_t           mEle;
  float              mZpos; 
  
  ClassDef(HoleHit,1)  //A track hit
};

//____________________________________________________________________________________________________________________________________________________________
class HitEvent : public TObject
{     
  
private:
  UShort_t         mNhit;             //Number of hist in detector
  UShort_t         mNevt;
  TClonesArray     *mHits;             //->array of hits in detector
  
public :
  
  HitEvent()
  {
    mHits  = new TClonesArray("HoleHit", 10000000);
    mNhit = 0;
    mNevt = 0;
  }
  
  virtual ~HitEvent () {
    mHits->Clear("C");  
    mHits = 0;
  }
  
  //____________________________________________________________________________________________________________________________________________________________
  void Clear(Option_t *option)
  {
    mNhit = 0;
    mNevt = 0;
    mHits->Clear(option);
  }
   
  //____________________________________________________________________________________________________________________________________________________________
  
  TClonesArray  *GetHit()       const { return mHits;}
  int           GetNhit()       const { return mNhit;}
  int           GetNevt()       const { return mNevt;}

  void  SetNevt(Int_t i)        {mNevt = i;}
  
  //____________________________________________________________________________________________________________________________________________________________
  HoleHit *AddHit(float zPos, int nEle)
  {
    TClonesArray &hits = *mHits;
    HoleHit *hit = new(hits[mNhit++]) HoleHit();
    hit->SetZpos(zPos);
    hit->SetNele(nEle);
    return hit;
  }
  
public:
  ClassDef (HitEvent,1) ; 
  
} ;


#endif
