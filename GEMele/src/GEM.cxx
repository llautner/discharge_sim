#include "GEM.h"
#include <iostream>
#include <fstream>
#include <sstream> 
#include "TClonesArray.h"

class GEM;


//____________________________________________________________________________________________________________________________________________________________ 
GEM::GEM()
:
fPitch(0),
fPadsLong(0),
fPadsShort(0),
fReadout(0)
{}

//____________________________________________________________________________________________________________________________________________________________ 
GEM::GEM(double pitch, double readoutSize)
{
  // Pitch in micrometer, length of one side of the quadratic readout area in millimeter
  // is converted to millimeter
  fReadout = readoutSize;
  fPitch = pitch/1000.;
  fPadsLong = round(readoutSize/fPitch +0.49999);                       // number of pads in a row which starts with a hexagon side
  fPadsShort = 1+round((readoutSize-fPitch/2.)/fPitch +0.49999);        // number of pads in a row which starts witha hole-centered hexagon
}

//____________________________________________________________________________________________________________________________________________________________ 
GEM::~GEM(){
  fReadout = 0;
  fPitch = 0;
  fPadsLong = 0;
  fPadsShort = 0;
}

//____________________________________________________________________________________________________________________________________________________________ 
int GEM::GetPadID(double x, double y){
  // find the exact ID of the GEM hole, where the electron goes
  // vector product: playchilla.com/how-to-check-if-a-point-is-inside-a-hexagon

  if(fabs(x) > fReadout || fabs(y) > fReadout) return -991;
  
  double height = sqrt(3)/2. * fPitch;
    
  // find first estimate of GEM hole coordinate
  double row = round(y/height);

  // take into account shift between pads from one row to the next one
  double offsetX = 0;
  
  if(int(row)%2==1){
    offsetX = fPitch/2.;
  }
    
  double pad = round((x+offsetX)/fPitch);
  double posX = (int(pad)*fPitch) - offsetX;
  double posY = double(row)*height;
 
  double absX      = fabs(x - posX);
  double absY      = fabs(y - posY);
  double absXright = fabs(x - (posX+fPitch/2.));
  double absXleft  = fabs(x - (posX-fPitch/2));
  double absYup    = fabs(y - (posY+height));
  double absYdown  = fabs(y - (posY-height));
  
  double hexagonX = fPitch/2.;
  double hexagonY = fPitch/(2*sqrt(3));
  
  bool found = kFALSE;
  // check whether the first estimate is right
  if(2*hexagonX*hexagonY - hexagonY*absX - hexagonX*absY >= 0){
    found = kTRUE;
  }
  // y position is ambigious - check left above
  else if(2*hexagonX*hexagonY - hexagonY*absXleft - hexagonX*absYup >= 0){
    ++row;
    --pad;
    pad+=int(row)%2;
    found = kTRUE;
  }
  // y position is ambigious - check right above
  else if(2*hexagonX*hexagonY - hexagonY*absXright - hexagonX*absYup >= 0){           
    ++row;
    pad+=int(row)%2;
    found = kTRUE;
  }
  // y position is ambigious - check left below
  else if(2*hexagonX*hexagonY - hexagonY*absXleft - hexagonX*absYdown >= 0){
    --row;
    --pad;
    pad+=int(row)%2;
    found = kTRUE;
  }
  // else electrons go to right below
  else{
    --row;
    pad+=int(row)%2;
    found = kTRUE;
  }
  
  int padID = 0;
  if(int(row)%2==0){
    // no offset
    padID = int(row/2.) * fPadsLong + int(row/2.) * fPadsShort + pad;
  }
  else{
    // with offset 
    padID = (int(row/2.)+1) * fPadsLong + int(row/2.) * fPadsShort + pad;
  }
  
  if(found==kFALSE) padID = -999;
  
  return padID;
}

//____________________________________________________________________________________________________________________________________________________________ 
double GEM::GetPosX(int id){
  
}

//____________________________________________________________________________________________________________________________________________________________ 
double GEM::GetPosY(int id){
  
}
