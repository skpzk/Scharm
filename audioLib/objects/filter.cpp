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

MFilter::MFilter(){
  // this->filterType = lpf;
  this->maxFc = SAMPLE_RATE/2;
  this->T = 1. / SAMPLE_RATE;
  this->Q = 1;
  this->setFc(1000.);
  // this->audioOutput = new AudioOutput(this);
  this->computeCoefs();
  for(int i = 0; i<4; i++){
    s[i] = 0;
  }
}

void MFilter::setFc(float fc){
  if(fc != this->fc){
    this->fc = trim(fc, this->maxFc);
    this->computeCoefs();
    printf("fc = %f\n", fc);
  }
}

void MFilter::computeCoefs(){
  double w0 = 2 * M_PI * this->fc;
  double RC = 1. / w0;
  a0 = T / (RC + T);
  G = a0;
  K = Q;

  for(int i = 0; i<4; i++){
    gamma[i] = pow(G, i);
  }

  alpha0 = 1. / (1. + K * pow(G, 4));

  // printf("a0 = %f, G = %f, K = %f, alpha0 = %f\n", a0, G, K, alpha0);
}

void MFilter::setQ(float Q){
  this->Q = 1 + 3 * trim(Q, 0, 127)/127;
  this->computeCoefs();
}

void MFilter::setInput(AudioObject* in){
  this->input = in;
}

void MFilter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;
  float data;

  // BiquadState tmpState;

  

  double xn = 0;
  double un;

  double bassBoost_Pct = 100.0;
  double compensationGain = 1.0 + (bassBoost_Pct / 100.0) * K;

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    for(int ii = 0; i<4; i++){
      s[i] = y[i] * (1-a0);
    }

    un = ((double) in[i*2]) / MAX;


    un *= compensationGain;

    xn = alpha0 * (un - K * (gamma[3] * s[0] + gamma[2] * s[1] + gamma[1] * s[2] + gamma[0] * s[3]));

    y[0] = xn * a0 + s[0];
    y[1] = y[0] * a0 + s[1];
    y[2] = y[1] * a0 + s[2];
    y[3] = y[2] * a0 + s[3];

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y[3] * MAX);  // mono/left
    *out++ = (sample_t) (y[3] * MAX);  // right


  }

}
void MFilter::output(void* out){
  this->filter(out);
}

O1Filter::O1Filter(){
  // this->filterType = lpf;
  this->maxFc = SAMPLE_RATE/2;
  this->T = 1. / SAMPLE_RATE;
  this->setFc(1000.);
  // this->audioOutput = new AudioOutput(this);
  this->computeCoefs();

}

void O1Filter::setFc(float fc){
  if(fc != this->fc){
    this->fc = trim(fc, this->maxFc);
    this->computeCoefs();
    // printf("fc = %f\n", fc);
  }
}

void O1Filter::setQ(float q){

}

void O1Filter::computeCoefs(){
  double w0 = 2 * M_PI * this->fc;
  double RC = 1. / w0;
  a0 = T / (RC + T);

  // printf("a0 = %f, G = %f, K = %f, alpha0 = %f\n", a0, G, K, alpha0);
}

void O1Filter::setInput(AudioObject* in){
  this->input = in;
}

void O1Filter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;

  // BiquadState tmpState;
  double un;

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    un = ((double) in[i*2]) / MAX;

    y0 = un * a0 + (1-a0) * y0;

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y0 * MAX);  // mono/left
    *out++ = (sample_t) (y0 * MAX);  // right


  }

}
void O1Filter::output(void* out){
  this->filter(out);
}

Vcf::Vcf(){

}

void Vcf::checkValues(){
  knobCutoff = trim(*State::params(stateKeys.freq), 127);
  knobCutoff = mtof(ftom(20) + (ftom(20000) - ftom(20)) * knobCutoff/127);

  setFc(knobCutoff);

  knobReso = trim(*State::params(stateKeys.reso), 127);
  // knobReso = knobReso/127 * 5 + .1;

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
