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
  float      fX;
  float      fY;
  float      fZ;
  float      fEdep;
  float      fSteplength;
  int        fPID;
  
public:
  Hit(){ };
  virtual ~Hit() { };
  
  void  SetX(float d) {fX = d;}
  void  SetY(float d) {fY = d;}
  void  SetZ(float d) {fZ = d;}
  void  SetEdep(float d) {fEdep = d;}
  void  SetSteplength(float d) {fSteplength = d;}
  void  SetPID(int i) {fPID = i;}
  
  float GetX()       const {return fX;}
  float GetY()       const {return fY;}
  float GetZ()       const {return fZ;}
  float GetEdep()    const {return fEdep;}
  float GetStep()    const {return fSteplength;}
  int   GetPID()     const {return fPID;}
  
  ClassDef(Hit,2)  //A track hit
};

//____________________________________________________________________________________________________________________________________________________________
class GEMEvent : public TObject
{     
  
private:
  TVector3      fVertex;            //vertex coordinates
  
  int           fNhit;             //Number of hist in detector
  int           fNevt;
  float         fEdep;
  float         fRange;
  float         fGunEnergy;
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
  
  virtual ~GEMEvent () {
    fHits->Clear("C");
    fHits = 0;
  }
  
  //____________________________________________________________________________________________________________________________________________________________
  void Clear(Option_t *option)
  {
    fNhit = 0;
    fNevt = 0;
    fRange = 0;
    fEdep = 0;
    fGunEnergy = 0;
    fVertex = TVector3(0,0,0);
    fHits->Clear("C");
  }

  //____________________________________________________________________________________________________________________________________________________________
  
  TClonesArray  *GetHit()       const {return fHits;}
  int           GetNhit()       const { return fNhit;}
  int           GetNevt()       const { return fNevt;}
  float         GetEdep()       const {return fEdep;}
  float         GetRange()      const {return fRange;}
  TVector3      GetVertex()     const {return fVertex;}
  float         GetGunEnergy()  const {return fGunEnergy;}
  
  
  void  SetNevt(int i)        {fNevt = i;}
  void  SetVertex(TVector3 v)   {fVertex = v;}
  void  SetEdep(float d)     {fEdep = d;}
  void  SetRange(float d)    {fRange = d;}
  void  SetGunEnergy(float d) {fGunEnergy = d;}
  
  //____________________________________________________________________________________________________________________________________________________________
  Hit *AddHit(float posX, float posY, float posZ, float Edep, float Steplength, int pid)
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
  ClassDef (GEMEvent,2) ; 
  
} ;


#endif
