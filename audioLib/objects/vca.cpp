#include "vca.h"
#include "../utils/SignalUtils.h"

Vca::Vca(){
  eg = new AudioObject;
  eg->setDefaultValue(1);
  input = new AudioObject;
  // input->setDefaultValue(0);
}

void Vca::setEnv(AudioObject * envPtr){
  eg = envPtr;
}
void Vca::setInput(AudioObject * inPtr){
  input = inPtr;
}

void Vca::output(void* outputBuffer){
  sample_t * out = (sample_t*) outputBuffer;
  initBuffer(env);
  eg->output(env);
  initBuffer(audio);
  input->output(audio);

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += audio[2*i] * ((float) env[2*i + 1])/MAX;
    *out++ += audio[2*i +1] * ((float) env[2*i + 1])/MAX;
  }
}