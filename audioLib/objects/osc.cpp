#include "osc.h"
#include "../utils/midi.h"
#include "../utils/quantize.h"
#include "../../state/state.h"

Osc::Osc() : 
AudioObject()
{
  this->setFreq(440);
  this->volume = 1;
  this->phase = 0;
  this->vco_or_sub = 0; // 0: vco
  wave = Wave(0);
}

void Osc::setWave(float type){
  if(vco_or_sub == 0){
    wave = Wave(type);
  }else{
    if(type == 3){
      wave = Wave(2);
    }else{
      wave = Wave(type);
    }
  }
}

void Osc::setFreq(float freq){
  this->freq = freq;
  // printf("freq = %f\n", freq);
  this->updatePhaseIncrement();
}

void Osc::setNoteMidi(float note){
  this->setFreq(mtof(note));
}

void Osc::updatePhaseIncrement(){
  this->phaseIncrement = this->freq * TABLE_SIZE / SAMPLE_RATE;
}

void Osc::output(void* outputBuffer){
  // printf("osc output\n");
  this->outputWave(outputBuffer);
}

void Osc::outputWave(void* outputBuffer){
  // printf("osc output\n");
  sample_t* out = (sample_t*) outputBuffer;
  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += this->wave.wave[(int)this->phase] * this->volume;  // mono/left
    *out++ += this->wave.wave[(int)this->phase] * this->volume;  // right
    this->phase += (this->phaseIncrement);
    if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
  }
}

void Osc::CVOutput(void* outputBuffer){
  printf("Getting data from AudioObject, something must be wrong\n");
  printf("This method should be overridden by objects that inherit from AudioObject\n");
  sample_t* out = (sample_t*) outputBuffer;
  for(int i=0; i<FRAMES_PER_BUFFER; i++){
      out[i] += SILENCE;
  }
}

Vco::Vco() :
Osc(){
  vco_or_sub = 0;
}

void Vco::updateWaveType(int newWaveType){
  if(newWaveType != waveType){
    this->setWave(newWaveType);
    this->waveType = newWaveType;
  }
}

void Vco::checkValues(){
  // get the value of the gui frequency knob via state
  // if(stateKeys.freq == "") return;
  knobFreq = trim(*State::params(stateKeys.freq), 127);
  // printf("%f\n", 60 + knobFreq/2);
  knobFreq = mtof(60 + knobFreq/2);

  // get the quantize value :
  quantValue = trim(*State::params(stateKeys.quant), 4);

  int newWaveType = 4 - trim(*State::params(stateKeys.wave), 4);
  updateWaveType(newWaveType);

  volume = trim(*State::params(stateKeys.volume), 127)/127;

}

int Vco::getWave(){
  return waveType;
}

float Vco::getFreq(){
  return freq;
}

void Vco::updateFreq(){
  this->freq = knobFreq;
  // quantize freq
  quantizeFloat(&this->freq, quantValue);

  // setFreq(knobFreq);
  updatePhaseIncrement();
}

void Vco::output(void* outputBuffer){
  // check values from state
  checkValues();
  // printf("Vco1knobfreq = %f\n", *State::params(stateKeys.freq));
  // update Freq
  updateFreq();

  
  // freq CV input is gathered in updateFreq

  // update Phase Increment

  // update phase

  // compute pwm
  // pwm CV inputs are gathered in computePwm

  // output
  // apply volume
  this->outputWave(outputBuffer);

  

}

Sub::Sub() :
Vco(){
  vco_or_sub = 1;
  vco = nullptr;
}

void Sub::setVco(Vco* vco){
  this->vco = vco;
}

void Sub::checkValues(){
  knobDiv = trim(*State::params(stateKeys.div), 16);

  volume = trim(*State::params(stateKeys.volume), 127)/127;
}

void Sub::computeDiv(){
  // check CV inputs

  // check sequence

  // create an array with the div values

  div = knobDiv;
}

void Sub::output(void* outputBuffer){
  // set wave from parent vco
  if(vco != nullptr)
  updateWaveType(vco->getWave());

  // check values from state
  checkValues();

  computeDiv();

  // update Freq
  if(vco != nullptr)
  freq = this->vco->getFreq() / this->div;

  // update Phase Increment
  updatePhaseIncrement();

  // update phase

  // compute pwm
  // pwm CV inputs are gathered in computePwm

  // output
  // apply volume
  this->outputWave(outputBuffer);
}