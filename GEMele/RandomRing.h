#include <vector>

#include "TF1.h"
#include "TRandom3.h"

class RandomRing
{
  public:
    RandomRing();

    RandomRing(const size_t size = 5000000);

    void initialize(const size_t size = 5000000);

    float getNextValue()
    {
      const float value = mRandomNumbers[mRingPosition];
      ++mRingPosition %= mRandomNumbers.size();
      return value;
    }

    unsigned int getRingPosition() const { return  mRingPosition; }

  private:
    std::vector<float> mRandomNumbers;
    unsigned int mRingPosition;

    RandomRing(const RandomRing &);

    void operator=(const RandomRing &) {}

};



inline RandomRing::RandomRing(const size_t size)
  : mRandomNumbers(size)
  ,  mRingPosition(0)

{
  initialize(size);
}


inline void RandomRing::initialize(const size_t size)
{
  mRandomNumbers.resize(size);
  TRandom3 rand;
  rand.SetSeed(0);
  for (std::vector<float>::iterator iter = mRandomNumbers.begin(); iter != mRandomNumbers.end(); ++iter) { 
      (*iter) = rand.Gaus(0.f, 1.f);
  }
}
