#ifndef wave_h_
#define wave_h_
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include "../../audioApi/AudioConstants.h"
#include <math.h>
#include "../utils/SignalUtils.h"

class Wave{
  public:
    Wave(int);
    Wave();
    sample_t wave[TABLE_SIZE];
  private:
    sample_t maxValue = MAX;
    void sine();
    void tri();
    void square();
    void saw();
};


class WaveShape {
public:
    WaveShape();
    int numWaves;
    sample_t** waveforms = NULL;
    void addWave(Wave*);
    void selectWaveShape(int);
    
private:
    void basicShapes();
    void organShapes();
};

#endif