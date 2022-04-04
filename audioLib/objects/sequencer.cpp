#include "sequencer.h"
#include "../utils/SignalUtils.h"

#include "../../../state/state.h"

#include <algorithm> // just for min, this may be an overkill, to be tested


Sequencer::Sequencer(){

  lastValue = 0;
  stepCounter = 1;

}

void Sequencer::addRhythm(AudioObject* r){
  this->rhythms.push_back(r);
}

void Sequencer::checkValues(){
  steps[0] = * State::params(stateKeys.step1);
  steps[1] = * State::params(stateKeys.step2);
  steps[2] = * State::params(stateKeys.step3);
  steps[3] = * State::params(stateKeys.step4);

  activeRhythms[0] = (bool) *State::params(stateKeys.clk1);
  activeRhythms[1] = (bool) *State::params(stateKeys.clk2);
  activeRhythms[2] = (bool) *State::params(stateKeys.clk3);
  activeRhythms[3] = (bool) *State::params(stateKeys.clk4);

  // std::cout << "checking step 1 : " << stateKeys.step1 << " = " << *State::params(stateKeys.step1) << std::endl;

  // printf("steps =  [");
  // for(int i = 0; i<4; i++){
  //   printf("%.2f, ", steps[i]);
  // }
  // printf("\b\b]\naR = [");
  // for(int i = 0; i<4; i++){
  //   printf("%i, ", activeRhythms[i]);
  // }
  // printf("\b\b]\n");
}

void Sequencer::updateClockSignal(){

  // clockSignal gets the clock signals from the rhythms
  // the it is updated to contain the step sequence
  // it may backfire one day, but for now it saves us a buffer.

  initBuffer(clockSignal);
  initBuffer(stepsSignal);
  for(int i = 0; i<std::min((int) rhythms.size(), 4); i++){
    if(activeRhythms[i])
      rhythms[i]->output(clockSignal);
  }

  // lastValue = 0; // if no signal is active at the first value of clockSignal

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    value = clockSignal[2*i];

    stepCounter = (value>0?(stepCounter + 1) % 4:stepCounter);

    // printf("stepcounter : %i\n", stepCounter);
    //debug
    // if(((sample_t) steps[stepCounter]) > 127 || ((sample_t) steps[stepCounter])< -127){
      
    //   printf("stepcounter : %i\n", stepCounter);
    

    // }

    stepsSignal[2*i] = (sample_t) steps[stepCounter];
    stepsSignal[2*i+1] = (sample_t) steps[stepCounter];

    // lastValue = value;
  }
}

void Sequencer::updateParam(){
  // debug
  int lastValue = stepCounter+1;

  // maybe move the checking to the gui ?
  
  float value = * State::params(stateKeys.guiCallback);

  if(((int)value)!=(stepCounter+1)){
    State::params(stateKeys.guiCallback)->setValue(stepCounter+1);
    // printf("updated param %s, value = %i\n", stateKeys.guiCallback.c_str(), (int) value);
  }
  // printf("last clock value = %d\n", clockSignal[2*FRAMES_PER_BUFFER - 1]);
}

void Sequencer::update(){
  // check values
  checkValues();

  // get clock signal and transform into step signal
  updateClockSignal();

  // update param for gui

  updateParam();

}


void Sequencer::output(void* buffer){
  CVOutput(buffer);
}

void Sequencer::CVOutput(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){

    *out++ += stepsSignal[2*i];  // mono/left
    *out++ += stepsSignal[2*i+1];
  }
}


void Sequencer::altOutput(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){

    *out++ += clockSignal[2*i];  // mono/left
    *out++ += clockSignal[2*i+1];
  }
}