#include "clock.h"
#include "../../../state/state.h"
#include "../utils/midi.h"

Clock::Clock(){

  stateKeys.freq = "tempo";

  wave = Wave(SQR, 1);// wave type : square, maxValue : 1

}

void Clock::setFreq(float freq){
  this->freq = freq;
  // printf("freq = %f\n", freq);
  this->updatePhaseIncrement();
}

void Clock::updatePhaseIncrement(){
  this->phaseIncrement = this->freq * TABLE_SIZE / SAMPLE_RATE;
}

void Clock::updateClockSignal(){

  if(!playing) {initBuffer(clockSignal); return;}

  double value;
  // double maxClock = 0, minClock = MAX;
  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    value = this->wave.wave[(int)this->phase];
    value = (value + 1.)/2;

    // maxClock = (value > maxClock?value:maxClock);
    // minClock = (value < minClock?value:minClock);

    clockSignal[2*i] = value ;  // mono/left
    clockSignal[2*i +1 ] = value ;  // mono/left

    this->phase += (this->phaseIncrement);
    if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
  }
  // printf("max Clock = %f\n", maxClock);
  // printf("min Clock = %f\n", minClock);

  // printf("d : %d\n", clockSignal[0]);
  // printf("f : %f\n", clockSignal[0]);
      

}
void Clock::checkValues(){
  float freq = trim(*State::params(stateKeys.freq), 127)/127;
  freq = mtof(ftom(1./3) + freq * (ftom(50)-ftom(1./3)));
  // printf("freq = %f\n", freq);
  // printf("ftom(50) = %f\n", ftom(50));
  // printf("ftom(1./3) = %f\n", ftom(1./3));
  // printf("ftom(50 - 1./3) = %f\n", ftom(50 - 1./3));
  // printf("ftom(50) = %f\n", ftom(50));
  setFreq(freq);

  playing = (bool) State::params("play")->getValue();
}

void Clock::addRhythm(AudioObject* r){
  this->rhythms.push_back(r);
}

void Clock::addSeq(AudioObject* r){
  this->sequences.push_back(r);
}

void Clock::tick(){
  // check values:
  checkValues();

  
  // updates self
  updateClockSignal();
  
  // updates all children

  for(AudioObject* ao: rhythms){
    ao->update();
  }
  for(AudioObject* ao: sequences){
    ao->update();
  }

}

void Clock::updateParam(){
  float value = * State::params(stateKeys.guiCallback);
  if(value!=clockSignal[2*FRAMES_PER_BUFFER - 1])
    State::params(stateKeys.guiCallback)->setValue(clockSignal[2*FRAMES_PER_BUFFER-1]);
  // printf("last clock value = %d\n", clockSignal[2*FRAMES_PER_BUFFER - 1]);
}

void Clock::update(){
  // std::cout << "updating clock\n";
  tick();
  updateParam();
}

void Clock::CVOutput(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){

    *out++ += clockSignal[2*i];  // mono/left
    *out++ += clockSignal[2*i+1];
  }

}

void Clock::output(void* buffer){
  CVOutput(buffer);
}

RhythmGenerator::RhythmGenerator(){
  div=1;
  accum = 0;
  lastValueIn = 0;
  lastValueOut = 1;
  reset=false;

}

void RhythmGenerator::setInput(AudioObject* masterClk){
  masterClock = masterClk;
}

void RhythmGenerator::checkValues(){
  div = (int) *State::params(stateKeys.div);
  // printf("div = %d\n", div);
  // setFreq(freq);
  bool tmpReset = State::params("reset")->getValue();
  if(tmpReset != reset){ // reset only when the button is toggled, not held
    reset = tmpReset;
    if(reset){
      accum = 0;
      lastValueIn = 0;
      lastValueOut = 1;
    }
  }
}

void RhythmGenerator::updateClockSignal(){
  // TODO:
  // comment this!
  float value;
  initBuffer(clockSignal);
  initBuffer(risingEdgeSignal);
  masterClock->output((void*) clockSignal);
  
  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    value = clockSignal[2*i]; // get pulse
    accum += (lastValueIn != value?1:0); // detect change in pulse
    // each time there is a change, increment accum;

    lastValueIn = value;

    lastValueOut = (accum == div?1-lastValueOut:lastValueOut);

    risingEdgeSignal[2*i] = (accum == div) * lastValueOut;
    risingEdgeSignal[2*i + 1] = (accum == div) * lastValueOut;

    accum = (accum>=div?0:accum);

    // if(accum == div){
    //   accum = 0;
    //   lastValueOut = 1-lastValueOut;
    // }
    // value = (value + 1.)/2;

    // maxClock = (value > maxClock?value:maxClock);
    // minClock = (value < minClock?value:minClock);

    clockSignal[2*i] = lastValueOut ;  // mono/left
    clockSignal[2*i + 1] = lastValueOut ;  


  }
}

void RhythmGenerator::updateParam(){
  // debug
  // int lastValue = clockSignal[2*FRAMES_PER_BUFFER-1];

  // maybe move the checking to the gui ?
  
  float value = * State::params(stateKeys.guiCallback);

  if(value!=clockSignal[2*FRAMES_PER_BUFFER - 1]){
    State::params(stateKeys.guiCallback)->setValue(clockSignal[2*FRAMES_PER_BUFFER-1]);
    // printf("updated param %s\n", stateKeys.guiCallback.c_str());
  }
  // printf("last clock value = %d\n", clockSignal[2*FRAMES_PER_BUFFER - 1]);
}

void RhythmGenerator::update(){
  // check values
  checkValues();
  // update clockSignal
  updateClockSignal();
  // update state parameter
  updateParam();
}

void RhythmGenerator::output(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){

    *out++ += risingEdgeSignal[2*i];  // mono/left
    *out++ += risingEdgeSignal[2*i+1];
  }
}

void RhythmGenerator::CVOutput(void* buffer){
  sample_t* out = (sample_t*) buffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){

    *out++ += clockSignal[2*i];  // mono/left
    *out++ += clockSignal[2*i+1];
  }
}
