#include "mixer.h"
#include "../utils/SignalUtils.h"

Mixer::Mixer(){

}

void Mixer::addInput(AudioObject * in){
  this->inputs.push_back(in);
}

void Mixer::output(void* outputBuffer){
  sample_t * out = (sample_t*) outputBuffer;
  initBuffer(out, FRAMES_PER_BUFFER*CHANNELS, 0);
  for(AudioObject* it : this->inputs){
    it->output(outputBuffer);
  }
}