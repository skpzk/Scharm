#include <matplot/matplot.h>
#include <cmath>
#include <vector>

#include "../audioLib/objects/osc.h"
#include "../audioLib/objects/filter.h"
#include "../audioApi/AudioConstants.h"


double poly_blep(double t, double dt){
  // t = phase / Tablelength % 1;

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

void appendAudioObjToVector(AudioObject* ao, std::vector<double> * v){
  const sample_t * audioBuffer = new sample_t[2*FRAMES_PER_BUFFER];

  sample_t * buf = (sample_t *) audioBuffer;

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    *buf++ = 0;  // mono/left
    *buf++ = 0;
  }
  buf = (sample_t*) audioBuffer;

  ao->output(buf);

  for(int i=0; i<FRAMES_PER_BUFFER; i++){
    v->push_back(buf[2*i]);
  }
}

void debug_plotter(){
  using namespace matplot;
  std::vector<double> x = linspace(0, FRAMES_PER_BUFFER, FRAMES_PER_BUFFER);
  std::vector<double> y;
  std::vector<double> y2;


  
  
  Osc * osc = new Osc();
  Osc * osc2 = new Osc();

  osc->setFreq(880);
  osc2->setFreq(880);
  BiquadFilter * vcf = new BiquadFilter();
  vcf->setFc(1000);

  vcf->setInput(osc2);

  appendAudioObjToVector(osc, &y);
  // appendAudioObjToVector(osc, &y);
  // appendAudioObjToVector(osc, &y);

  appendAudioObjToVector(vcf, &y2);
  // appendAudioObjToVector(vcf, &y2);
  // appendAudioObjToVector(vcf, &y2);

  plot(y);
  hold(on);

  plot(y2);
  show();

  
}