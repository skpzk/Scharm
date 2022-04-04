#include "env.h"

#include "../utils/SignalUtils.h"
#include "../../state/state.h"

#include <cmath>

//debug 
// #include <iostream>

#define MIN_A 0.001
#define MAX_A 10.0
#define MIN_D 0.005
#define MAX_D 10.0

Env::Env(){
  elapsed = 0;
  T = 1./SAMPLE_RATE;
  envValue = 0;

  tau_attack = 0.5;
}

void Env::addRhythm(AudioObject* r){
  this->rhythms.push_back(r);
}

void Env::computeAlpha(bool gate){
  alpha = T / (T + (gate?tau_attack/5.:tau_decay/5.));
}

void Env::checkValues(){
  // attack from 1ms to 10sec
  // log

  float a = State::params(stateKeys.a)->getValue() / 127.;
  float d = State::params(stateKeys.d)->getValue() / 127.;

  tau_attack = exp(a * log(MAX_A + (1-MIN_A))) - (1-MIN_A);

  // decay from 5ms to 10 sec 
  tau_decay = exp(d * log(MAX_D + (1-MIN_D))) - (1-MIN_D);

  // std::cout << "a          = "<<a          << "\nd         = "<<d <<std::endl;
  // std::cout << "tau attack = "<<tau_attack << "\ntau decay = "<<tau_decay <<std::endl;
}

sample_t Env::computeEnv(bool reset, float gate){

  // reset the env if time elapsed is > than attack time
  elapsed = (reset && (elapsed > tau_attack))?0:elapsed;

  gate = (elapsed <= tau_attack ? 1:(0 + gate));

  // computeAlpha(gate);

  alpha = T / (T + (gate?tau_attack/5.:tau_decay/5.));

  envValue = alpha * gate + (1-alpha) * envValue;

  elapsed += T;

  return (sample_t) (envValue * MAX);
}

void Env::update(){
  // check values
  checkValues();

  // get clockSignal
  updateClockSignal();

  // compute env
}

void Env::updateClockSignal(){

  // clockSignal gets the clock signals from the rhythms
  // the it is updated to contain the envelope
  // it may backfire one day, but for now it saves us a buffer.

  initBuffer(clockSignal);

  // TODO:
  // it should get its rhythms from the sequencers.
  for(int i = 0; i<std::min((int) rhythms.size(), 2); i++){
    rhythms[i]->altOutput(clockSignal);
  }

  float value;
  sample_t envValue_sample;
  float gate = 0; // gate could come from the patchbay

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    value = clockSignal[2*i];

    // stepCounter = (value>0?(stepCounter + 1) % 4:stepCounter);

    // envValue = ((value>0)&&is?computeAttack():computeDecay());

    // value > 0 && is in decay : reset attack, compute attack
    // value > 0 && is in attack: continue computing attack
    // value == 0 : continue previous action

    envValue_sample = computeEnv(value>0, gate);


    clockSignal[2*i] = envValue_sample;
    clockSignal[2*i+1] = envValue_sample;

  }
}

void Env::output(void* buffer){
  update();
  CVOutput(buffer);
}

void Env::CVOutput(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *out++ += clockSignal[2*i];  // mono/left
    *out++ += clockSignal[2*i+1];
  }
}