#ifndef GEM_h
#define GEM_h 1

//____________________________________________________________________________________________________________________________________________________________ 
class GEM{
public:
  GEM();
  GEM(double pitch, double readoutSize);
  virtual ~GEM();
  
  int           GetPadID(double x, double y);
  double        GetPosX(int id);
  double        GetPosY(int id);
  
private:
  double fPitch;
  double fPadsLong;
  double fPadsShort;
  double fReadout;
  
};

#endif