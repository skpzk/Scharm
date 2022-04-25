#include "vca.h"
#include "../utils/SignalUtils.h"

#include "../../state/state.h"

#include <iostream>

Vca::Vca(){
  eg = new AudioObject;
  eg->setDefaultValue(1);
  input = new AudioObject;
  // input->setDefaultValue(0);

  setNumberOfCVInputs(1);
}

void Vca::setEnv(AudioObject * envPtr){
  eg = envPtr;
}
void Vca::setInput(AudioObject * inPtr){
  input = inPtr;
}

void Vca::checkValues(){
  volume = trim(*State::params("vol"), 127)/127.;
}

void Vca::output(void* outputBuffer){

  // std::cout << "output function from VCA !\n";

  checkValues();

  sample_t * out = (sample_t*) outputBuffer;
  initBuffer(env);
  eg->output(env);
  initBuffer(audio);
  input->output(audio);
  initBuffer(processedAudio);

  // volume = 1;
  // std::cout << "volume = " << volume <<std::endl;

  if(CVinputs[vcaIn]!=nullptr)
    CVinputs[vcaIn]->output((void*) audio);

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += audio[2*i] * ((float) env[2*i + 1])/MAX*volume;
    *out++ += audio[2*i +1] * ((float) env[2*i + 1])/MAX*volume;

    processedAudio[2*i] = audio[2*i] * ((float) env[2*i + 1])/MAX*volume;
    processedAudio[2*i+1] = audio[2*i+1] * ((float) env[2*i + 1])/MAX*volume;
  }
}


void Vca::CVOutput(void* outputBuffer){

  // std::cout << "output function from VCA !\n";

  sample_t * out = (sample_t*) outputBuffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += processedAudio[2*i];// * ((float) env[2*i + 1])/MAX;
    *out++ += processedAudio[2*i +1];// * ((float) env[2*i + 1])/MAX;
  }
}


void Vca::altOutput(void* outputBuffer){

  // std::cout << "output function from VCA !\n";

  sample_t * out = (sample_t*) outputBuffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += env[2*i];
    *out++ += env[2*i + 1];
  }
}