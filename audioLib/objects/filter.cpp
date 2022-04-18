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
  this->Q = trim(Q, 1, 127);
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

void BiquadFilter::setEgInput(AudioObject* ao){
  egInput = ao;
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


    double dData = data/MAX;

    this->state.update(in[i*2], data);

    dData = peakLimiter.processAudioSample(dData);

    data = dData * MAX;


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
  this->reset();

  this->egInput = new AudioObject;
}

void MFilter::setFc(float fc){
  if(fc != this->fc){
    this->fc = trim(fc, this->maxFc);
    this->computeCoefs();
    // printf("fc = %f\n", fc);
  }
}

void MFilter::computeCoefs(){
  double w0 = 2 * M_PI * this->fc / SAMPLE_RATE;
  double g = cos(w0) / (1+sin(w0));
  a0 = (1-g)/2;
  a1 = a0;
  b1 = -g;
  G = a0;
  K = Q;

  g0 = 1.;
  g1 = G;
  g2 = pow(G,2);
  g3 = pow(G,3);

  alpha0 = 1. / (1. + K * pow(G, 4));

  // printf("a0 = %f, G = %f, K = %f, alpha0 = %f\n", a0, G, K, alpha0);
}

void MFilter::computeCoefsInLoop(double loopFc){
  w0 = 2 * M_PI * loopFc / SAMPLE_RATE;
  g = cos(w0) / (1+sin(w0));
  a0 = (1-g)/2;
  // a1 = a0;
  // b1 = -g;
  // K = Q;

  alpha0 = 1. / (1. + Q * pow(a0, 4));

  // printf("a0 = %f, G = %f, K = %f, alpha0 = %f\n", a0, G, K, alpha0);
}

void MFilter::setQ(float Q){
  this->Q = 4 * trim(Q, 0, 127)/127;
  this->computeCoefs();
}

void MFilter::setK(float k){
  this->Q = k;
  this->K = k;
  this->computeCoefs();
}

void MFilter::setEgAmount(float e){
  egAmount = e;
}

void MFilter::setEgInput(AudioObject* ao){
  egInput = ao;
}

void MFilter::setInput(AudioObject* in){
  this->input = in;
}

void MFilter::reset(){
  for(int i = 0; i<4; i++){
    y11=0;
    y21=0;
    y31=0;
    y41=0;
    x11=0;
    x21=0;
    x31=0;
    x41=0;
  }
}

void MFilter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);
  initBuffer(env);

  this->input->output(in);
  this->egInput->output(env);
  
  sample_t *out = (sample_t*)outputBuffer;
  float data;

  // BiquadState tmpState;

  

  double xn = 0;
  double un;

  double bassBoost_Pct = 100.0;
  double compensationGain = 1.0 + (bassBoost_Pct / 100.0) * K;

  double y1, y2, y3, y4;

  // std::cout << trim(mtof(ftom(fc) + egAmount * 127 * ((double)env[0])/MAX), 21000) <<std::endl;

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    

    xn = ((double) in[i*2]);
    xn = xn / MAX;

    // xn *= compensationGain;

    // s1 = a1 * x11 - b1 * y11;
    // s2 = a1 * x21 - b1 * y21;
    // s3 = a1 * x31 - b1 * y31;
    // s4 = a1 * x41 - b1 * y41;

    computeCoefsInLoop(trim(mtof(ftom(fc) + egAmount * 64 * ((double)env[2*i])/MAX), 21000));

    s1 = a0 * x11 + g * y11;
    s2 = a0 * x21 + g * y21;
    s3 = a0 * x31 + g * y31;
    s4 = a0 * x41 + g * y41;

    un = alpha0 * (xn - Q * (a0 * a0 * a0 * s1 + a0 * a0 * s2 + a0 * s3 + s4));


    y1 = un * a0 + s1;
    y2 = y1 * a0 + s2;
    y3 = y2 * a0 + s3;
    y4 = y3 * a0 + s4;

    y11 = y1;
    y21 = y2;
    y31 = y3;
    y41 = y4;

    x11 = un;
    x21 = y1;
    x31 = y2;
    x41 = y3;

    y4 = peakLimiter.processAudioSample(y4);

    *out++ = (sample_t) (y4 * MAX);  // mono/left
    *out++ = (sample_t) (y4 * MAX);  // right
    // *out++ = (sample_t) (y4 * MAX);  // right
    // *out++ = (sample_t) (y4 * MAX);  // right


  }

}
void MFilter::output(void* out){
  this->filter(out);
}



Vcf::Vcf(){

}

void Vcf::checkValues(){
  knobCutoff = trim(*State::params(stateKeys.freq), 127);

  // log progression :
  knobCutoff = mtof(ftom(20) + (ftom(20000) - ftom(20)) * knobCutoff/127);

  // linear progression :
  // knobCutoff = 20 + (20000 - 20) * knobCutoff/127;

  mFilter.setFc(knobCutoff);
  bqFilter.setFc(knobCutoff);

  knobReso = trim(*State::params(stateKeys.reso), 127);
  // knobReso = knobReso/127 * 5 + .1;

  mFilter.setQ(knobReso);
  bqFilter.setQ(knobReso);

  // check filterType

  float ft = *State::params(stateKeys.filterType);
  fType =(FilterType) ((int) ft);

  // eg amount

  float egAmount = State::params("egamount")->getValue() / 127;

  mFilter.setEgAmount(egAmount);


}

void Vcf::output(void* buf){
  // check values
  checkValues();
  // get env
  // compute fc based on values and env
  // filter
  switch (fType)
  {
  case moog:
    mFilter.filter(buf);
    break;

  case biquad:
    bqFilter.filter(buf);
    break;
  
  default:
    mFilter.filter(buf);
    break;
  }
  
}


void Vcf::setInput(AudioObject* in){
  mFilter.setInput(in);
  bqFilter.setInput(in);
}

void Vcf::setEgInput(AudioObject* in){
  mFilter.setEgInput(in);
  bqFilter.setEgInput(in);
}