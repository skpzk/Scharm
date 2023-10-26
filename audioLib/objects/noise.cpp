#include "noise.h"
#include "../utils/SignalUtils.h"

#include "../../state/state.h"

#include <iostream>

NoiseGenerator::NoiseGenerator(){
  setNumberOfCVInputs(0);
}

// duplicate of CVOutput, to be removed
void NoiseGenerator::output(void* outputBuffer){

  // std::cout << "output function from Noise\n";

  sample_t * out = (sample_t*) outputBuffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += audio[2*i];
    *out++ += audio[2*i +1];
  }
}

void NoiseGenerator::CVOutput(void* outputBuffer){

  // std::cout << "CVOutput function from Noise\n";

  sample_t * out = (sample_t*) outputBuffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += audio[2*i];// * ((float) env[2*i + 1])/MAX;
    *out++ += audio[2*i +1];// * ((float) env[2*i + 1])/MAX;
  }
}

void NoiseGenerator::update(){
  // std::cout << "update function from Noise\n";

  sample_t n;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    n = rand() % MAX;
    audio[2*i] = n * volume;
    audio[2*i+1] = n * volume;
  }
}