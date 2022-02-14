#include "filter.h"
#include "../utils/SignalUtils.h"
#include "../utils/midi.h"

#include "../../state/state.h"


/*
	useful resources :
		https://www.w3.org/2011/audio/audio-eq-cookbook.html
		https://arachnoid.com/BiQuadDesigner/
		https://en.wikipedia.org/wiki/Digital_biquad_filter
		https://en.wikibooks.org/wiki/Digital_Signal_Processing/IIR_Filter_Design#Chain_of_Second_Order_Sections

  Better filter ?
    http://www.willpirkle.com/Downloads/Addendum_A1_Moog.pdf
*/

BiquadFilter::BiquadFilter(){
  // this->filterType = lpf;
  this->maxFc = SAMPLE_RATE/2;
  this->T = 1. / SAMPLE_RATE;
  this->Q = 0.707;
  this->Qinv = 1./this->Q;
  this->setFc(220.);
  // this->audioOutput = new AudioOutput(this);
}


void BiquadState::update(float x, float y){
  this->xn2 = this->xn1;
  this->yn2 = this->yn1;
  this->xn1 = x;
  this->yn1 = y;
}

void BiquadFilter::setFc(float fc){
  this->fc = trim(fc, this->maxFc);
  this->computeCoefs();
}

void BiquadFilter::setMidiFc(float fc){
  this->setFc(mtof(fc));
}

void BiquadFilter::setQ(float Q){
  this->Q = trim(Q, 0.707, 127);
  this->Qinv = 1./this->Q;
  this->computeCoefs();
}
void BiquadFilter::setMidiQ(float Q){
  this->setQ(Q * 5./127.);
}


void BiquadFilter::computeCoefs(){
  float w0 = 2 * M_PI * this->fc * this->T;
  // printf("M_PI : %f\n", M_PI);
  // float w0 = 2 * M_PI * this->fc;
  // w0 = omega0 : pulsation
  float alpha = sin(w0)/(2*this->Q);
  float a0 = 1 + alpha;
  float a0inv = 1. / a0;
  // a0 != 0 as long as fc <= samplerate/2
  float cosw0 = cos(w0);
  float b0 = (1 - cosw0) * .5 * a0inv;

  this->coefs.b0 = b0 ;
  this->coefs.b1 = b0 * 2;
  this->coefs.b2 = b0;

  this->coefs.a1 = -2 * cosw0 * a0inv;
  this->coefs.a2 = (2 - a0) * a0inv;
}

void BiquadFilter::setInput(AudioObject* in){
  this->input = in;
}

void BiquadFilter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;
  float data;

  // BiquadState tmpState;

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    data = in[i*2] * this->coefs.b0 
          + this->coefs.b1 * this->state.xn1 + this->coefs.b2 * this->state.xn2
    	    - this->coefs.a1 * this->state.yn1 - this->coefs.a2 * this->state.yn2;

    this->state.update(in[i*2], data);

    *out++ = (sample_t) data;  // mono/left
    *out++ = (sample_t) data; //right


  }

}

void BiquadFilter::output(void* out){
  this->filter(out);
}

Vcf::Vcf(){

}

void Vcf::checkValues(){
  knobCutoff = trim(*State::params(stateKeys.freq), 127);
  knobCutoff = mtof(ftom(20) + (ftom(20000) - ftom(20)) * knobCutoff/127);

  setFc(knobCutoff);

  knobReso = trim(*State::params(stateKeys.reso), 127);
  knobReso = knobReso/127 * 5 + .1;

  setQ(knobReso);
}

void Vcf::output(void* buf){
  // check values
  checkValues();
  // get env
  // compute fc based on values and env
  // filter
  filter(buf);
}
