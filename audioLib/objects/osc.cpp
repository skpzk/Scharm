#include "osc.h"
#include "../utils/midi.h"
#include "../utils/quantize.h"
#include "../../state/state.h"

#include <math.h>

Osc::Osc() : 
AudioObject()
{
  this->setFreq(440);
  this->volume = 1;
  this->phase = 0;
  this->vco_or_sub = 0; // 0: vco
  wave = Wave(0);
  waveType = 0;

  lastOutput = 0;
}

void Osc::setWave(float type){
  waveType = type;
  type = (type==PURE_SQR ? SQR : type);
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

// void Osc::setLocalMaxToOne(){
//   localMax=1;
// }

// void Osc::resetLocalMax(){
//   localMax=MAX;
// }

void Osc::setFreq(float freq){
  this->freq = freq;
  // printf("freq = %f\n", freq);
  this->updatePhaseIncrement();
}

void Osc::setNoteMidi(float note){
  this->setFreq(mtof(note));
}

void Osc::updateFreq(int _){
  ;
}

void Osc::updatePhaseIncrement(){
  this->phaseIncrement = this->freq * TABLE_SIZE / SAMPLE_RATE;
}

void Osc::output(void* outputBuffer){
  // printf("osc output\n");
  this->outputWave(outputBuffer);
}

double Osc::poly_blep(double t){
  // t = phase / Tablelength % 1;

  double dt = this->phaseIncrement / TABLE_SIZE;
  // printf("%f\n", dt);
  // 0 <= t < 1
  if (t < dt) {
    t /= dt;
    return t+t - t*t - 1.0;
  }
  // -1 < t < 0
  else if (t > 1.0 - dt) {
    t = (t - 1.0) / dt;
    return t*t + t+t + 1.0;
  }
  // 0 otherwise
  else return 0.0;
}

void Osc::outputWave(void* outputBuffer){
  // printf("osc output\n");
  sample_t* out = (sample_t*) outputBuffer;
  double dPhaseIncrement;
  // double maxTri = 0;
  // double minTri = 0;
  // double maxTri2 = 0;
  // double minTri2 = 0;

  switch(waveType){

    case SAW:
      for(int i=0; i<FRAMES_PER_BUFFER; i++){
        double value = this->wave.wave[(int)this->phase];
        value -= MAX * poly_blep(this->phase / TABLE_SIZE);

        // maxTri = (value>maxTri?value:maxTri);

        *out++ += value * this->volume;  // mono/left
        // *out++ += poly_blep(this->phase / TABLE_SIZE) * MAX;
        // *out++ += MAX * poly_blep((this->phase / TABLE_SIZE))  ;  // right
        *out++ += value  * this->volume ;  // right
        updateFreq(i);
        this->phase += (this->phaseIncrement);
        if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
      }
      // printf("max Saw = %f\n", maxTri);
      
      break;

    case SQR:
    // printf("here");
      // maxTri = 0;
      for(int i=0; i<FRAMES_PER_BUFFER; i++){
        double value = this->wave.wave[(int)this->phase];
        value += MAX * poly_blep(this->phase / TABLE_SIZE);
        value -= MAX * poly_blep(fmod(this->phase / TABLE_SIZE + 0.5, 1.));

        // maxTri = (value>maxTri?value:maxTri);

        *out++ += value * this->volume;  // mono/left
        // *out++ += poly_blep(this->phase / TABLE_SIZE) * MAX;
        // *out++ += MAX * poly_blep((this->phase / TABLE_SIZE))  ;  // right
        *out++ += value  * this->volume ;  // right
        updateFreq(i);
        this->phase += (this->phaseIncrement);
        if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
      }
      // printf("max Sqr = %f\n", maxTri);
      
      break;

    case TRI:
    // printf("here");
      // maxTri = 0;
      for(int i=0; i<FRAMES_PER_BUFFER; i++){
        double value = this->wave.wave[(int)this->phase];

        dPhaseIncrement = (double) this->phaseIncrement / TABLE_SIZE;

        value += MAX * poly_blep(this->phase / TABLE_SIZE);
        value -= MAX * poly_blep(fmod(this->phase / TABLE_SIZE + 0.5, 1.));

        // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
        value =  ( dPhaseIncrement * value + (1 - dPhaseIncrement) * lastOutput);
        // maxTri2 = (value>maxTri2?value:maxTri2);
        // minTri2 = (value<minTri2?value:minTri2);
        
        lastOutput = value;
        value = value * 5;

        // maxTri = (value>maxTri?value:maxTri);
        // minTri = (value<minTri?value:minTri);

        *out++ += value * this->volume;  // mono/left
        // *out++ += poly_blep(this->phase / TABLE_SIZE) * MAX;
        // *out++ += MAX * poly_blep((this->phase / TABLE_SIZE))  ;  // right
        *out++ += value  * this->volume ;  // right
        updateFreq(i);
        this->phase += (this->phaseIncrement);
        if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
      }
      // printf("max Tri = %f\n", maxTri);
      // printf("min Tri = %f\n", minTri);
      // printf("max Tri2= %f\n", maxTri2);
      // printf("min Tri2= %f\n", minTri2);
      break;

    default:
      for(int i=0; i<FRAMES_PER_BUFFER; i++){
        if(this->phase > TABLE_SIZE){
          printf("phase = %f\n", this->phase);
          printf("i = %d\n", i);
          printf("sequence[i-2] = %f\n", this->sequence[i-2]);
          std::cout << "sequence[i-2] = " << this->sequence[i-2] << std::endl;
        }
        *out++ += this->wave.wave[(int)this->phase] * this->volume;  // mono/left
        *out++ += this->wave.wave[(int)this->phase] * this->volume;  // right
        updateFreq(i);
        this->phase += (this->phaseIncrement);
        if( this->phase >= TABLE_SIZE ) this->phase -= TABLE_SIZE;
      }
      break;
  }
}

// void Osc::CVOutput(void* outputBuffer){
//   printf("Getting data from AudioObject, something must be wrong\n");
//   printf("This method should be overridden by objects that inherit from AudioObject\n");
//   sample_t* out = (sample_t*) outputBuffer;
//   for(int i=0; i<FRAMES_PER_BUFFER; i++){
//       out[i] += SILENCE;
//   }
// }

// seq value -> range -> add to knob freq and freq CV -> quantize
Vco::Vco() :
Osc(){
  vco_or_sub = 0;
  range = 1;
  seq = new AudioObject();
}

void Vco::setSequencer(AudioObject * s){
  seq = s;
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

  range = (int) State::params("rangeradio")->getValue();

  seqActive = (bool) State::params(stateKeys.assignvco)->getValue();

}

int Vco::getWave(){
  return waveType;
}

float Vco::getFreq(){
  return freq;
}

void Vco::updateFreq(){
  // seq value -> range -> add to knob freq and freq CV -> quantize



  this->freq = knobFreq;

  //debug
  // float tmpFreq = freq;


  // quantize freq
  quantizeFloat(&this->freq, quantValue);


  // setFreq(knobFreq);
  updatePhaseIncrement();
}

void Vco::computeFreq(int i){
  this->freq = mtof(((double)sequence[2*i])/127. * range * 12 + ftom(knobFreq));

  // printf("freq = %f\n", freq);

  // quantize freq
  quantizeFloat(&this->freq, quantValue);
}

void Vco::updateFreq(int i){
  // seq value -> range -> add to knob freq and freq CV -> quantize

  // sample_t s = sequence[2*i];
  // double ds = (double) s;
  // double dsCorrected = ((double)sequence[2*i])/127.;
  // double dsCorrectedRange = ((double)sequence[2*i])/127. * range;
  // float kFreq = knobFreq;
  // double mFreq = ftom(knobFreq);
  // double mFreqComplete = ((double)sequence[2*i])/127. * range + ftom(knobFreq);

  // double fFreq = mtof(((double)sequence[2*i])/127. * range + ftom(knobFreq));


  computeFreq(i);


  // setFreq(knobFreq);
  updatePhaseIncrement();
}

void Vco::output(void* outputBuffer){
  // check values from state
  checkValues();
  // printf("Vco1knobfreq = %f\n", *State::params(stateKeys.freq));
  // update Freq

  initBuffer(sequence);

  if(seqActive)
    seq->output((void*) sequence);

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

  seqActive = (bool) State::params(stateKeys.assignvco)->getValue();  
}

void Sub::computeDiv(){
  // check CV inputs

  // check sequence
  initBuffer(sequence);

  if(seqActive)
    seq->output((void*) sequence);

  // create an array with the div values

  div = trim(knobDiv, 1, knobDiv);
}

void Sub::updateFreq(int i){
  // seq value -> range -> add to knob freq and freq CV -> quantize



  if(vco != nullptr){
    this->vco->computeFreq(i);
    freq = this->vco->getFreq() /( (int) trim((this->div + ((double) sequence[2*i])/127. * (-8)), 1, 16));
  }

  // setFreq(knobFreq);
  updatePhaseIncrement();
}

void Sub::output(void* outputBuffer){
  // set wave from parent vco
  if(vco != nullptr)
  updateWaveType(vco->getWave());

  // check values from state
  checkValues();

  computeDiv();

  // update Freq
  

  // update Phase Increment
  updatePhaseIncrement();

  // update phase

  // compute pwm
  // pwm CV inputs are gathered in computePwm

  // output
  // apply volume
  this->outputWave(outputBuffer);
}