#include "otherFilters.h"

#include "../../utils/SignalUtils.h"

#include <cmath>


OnFilter::OnFilter(){
  // this->filterType = lpf;
  this->maxFc = SAMPLE_RATE/2;
  this->T = 1. / SAMPLE_RATE;
  this->setFc(1000.);
  // this->audioOutput = new AudioOutput(this);
  this->computeCoefs();

}

void OnFilter::setFc(float fc){
  if(fc != this->fc){
    this->fc = trim(fc, this->maxFc);
    this->computeCoefs();
    // printf("fc = %f\n", fc);
  }
}

void OnFilter::setQ(float q){

}

void OnFilter::computeCoefs(){
  double w0 = 2 * M_PI * this->fc;
  double RC = 1. / w0;
  a0 = T / (RC + T);

  // printf("a0 = %f, G = %f, K = %f, alpha0 = %f\n", a0, G, K, alpha0);
}

void OnFilter::setInput(AudioObject* in){
  this->input = in;
}

void OnFilter::filter(void* outputBuffer){
}
void OnFilter::output(void* out){
  this->filter(out);
}

O1Filter::O1Filter() : OnFilter(){}
void O1Filter::output(void* out){
  this->filter(out);
}

void O1Filter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;

  // BiquadState tmpState;
  double un;

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    un = ((double) in[i*2]);
    // un = un/MAX;

    y0 = un * a0 + (1-a0) * y0;

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y0 * MAX);  // mono/left
    *out++ = (sample_t) (y0 * MAX);  // right


  }

}

O2Filter::O2Filter() : OnFilter(){}
void O2Filter::output(void* out){
  this->filter(out);
}

void O2Filter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;

  // BiquadState tmpState;
  double un;
  // printf("y0 = %f, y1 = %f\n", y0, y1);

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    un = ((double) in[i*2]);
    un = un/MAX;

    y0 = un * a0 + (1-a0) * y0;
    y1 = y0 * a0 + (1-a0) * y1;

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y1 * MAX);  // mono/left
    *out++ = (sample_t) (y1 * MAX);  // right


  }

}

O4Filter::O4Filter() : OnFilter(){}
void O4Filter::output(void* out){
  this->filter(out);
}

void O4Filter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;

  // BiquadState tmpState;
  double un;
  // printf("y0 = %f, y1 = %f\n", y0, y1);

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    un = ((double) in[i*2]);
    un = un/MAX;

    y0 = un * a0 + (1-a0) * y0;
    y1 = y0 * a0 + (1-a0) * y1;
    y2 = y1 * a0 + (1-a0) * y2;
    y3 = y2 * a0 + (1-a0) * y3;

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y3 * MAX);  // mono/left
    *out++ = (sample_t) (y3 * MAX);  // right


  }

}

O4FdbFilter::O4FdbFilter() : OnFilter(){}
void O4FdbFilter::output(void* out){
  this->filter(out);
}

void O4FdbFilter::filter(void* outputBuffer){
  sample_t in[2*FRAMES_PER_BUFFER];

  initBuffer(in);

  this->input->output(in);
  
  sample_t *out = (sample_t*)outputBuffer;

  // BiquadState tmpState;
  double un;
  // printf("y0 = %f, y1 = %f\n", y0, y1);

	for(int i=0; i<FRAMES_PER_BUFFER; i++){ 

    un = ((double) in[i*2]);
    un = un/MAX;

    y0 = un * a0 + (1-a0) * y0;
    y1 = y0 * a0 + (1-a0) * y1;
    y2 = y1 * a0 + (1-a0) * y2;
    y3 = y2 * a0 + (1-a0) * y3;

    // printf("y3 = %f\n", y[3]);

    // double limiterOutput = audioLimiter ? peakLimiter.processAudioSample(LPF3) : LPF3;

    *out++ = (sample_t) (y3 * MAX);  // mono/left
    *out++ = (sample_t) (y3 * MAX);  // right


  }

}