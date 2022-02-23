#include "filter.h"

#include <complex.h>
#include <fftw3.h>
#include <iostream>

#include <matplot/matplot.h>
#include <cmath>
#include <vector>

#include "../audioLib/objects/osc.h"
#include "../audioLib/objects/filter.h"
#include "../audioLib/objects/audioObject.h"
#include "../audioApi/AudioConstants.h"

// #include "../audioLib/"

namespace plt = matplot;
using namespace std;

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

void plotSpectrum(vector<double> freqs, vector<double> gain, vector<double> phase){
  vector<double> gainDb, minus3Db, minusPiOver4;
  for(int i =0; i<gain.size(); i++){
    gainDb.push_back(20*log10(gain.at(i)));
    minus3Db.push_back(-3);
    minusPiOver4.push_back(-M_PI/4);
  }
  plt::figure();
  plt::subplot(2,1,0);
  plt::semilogx(freqs, gainDb);
  plt::hold(plt::on);
  plt::semilogx(freqs,minus3Db);
  plt::subplot(2,1,1);
  plt::semilogx(freqs, phase);
  plt::hold(plt::on);
  plt::semilogx(freqs,minusPiOver4);
}

vector<complex<double>> getSpectrum(vector<double> s){
  
  fftw_complex *in, *out;
  fftw_plan p;

  int N = FRAMES_PER_BUFFER;

  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_MEASURE);

  for(int i = 0; i< N; i++){
    in[i][0] = s.at(i);
    in[i][1] = 0.;
  }

  fftw_execute(p);
  
  vector<complex<double>> S;

  for(int i = 0; i< N; i++){
    S.push_back(out[i][0] + 1i * out[i][1]);
  }

  fftw_destroy_plan(p);
  fftw_free(in); fftw_free(out);

  return S;
}

void getFreqsGainAndPhase(vector<complex<double>> S, vector<double>* f, vector<double>* gain, vector<double>* phase, bool returnAll){
  int N = FRAMES_PER_BUFFER;
  if(!returnAll){
    N = N/2;
  }
  
  for(int i = 0; i < N; i++){
    gain->push_back(abs(S.at(i)));
  }

  for(int i = 0; i < N; i++){
    phase->push_back(arg(S.at(i)));
  }

  vector<double> freqs = plt::linspace(0, SAMPLE_RATE/2, FRAMES_PER_BUFFER/2);
  if(returnAll){
    vector<double> negFreqs = plt::linspace(-SAMPLE_RATE/2, 0, FRAMES_PER_BUFFER/2);
    freqs.insert(freqs.end(), negFreqs.begin(), negFreqs.end());
  }
  *f = freqs;
}

void getFreqsGainAndPhase(vector<complex<double>> S, vector<double>* f, vector<double>* gain, vector<double>* phase){
  getFreqsGainAndPhase(S, f, gain, phase, false);
}

vector<double> createDirac(int N){
  vector<double> d;
  d.push_back(1);

  for(int i=1; i<N; i++){
    d.push_back(0);
  }
  return d;
}

class ArbitraryAudioObject : public AudioObject{
  public:
    ArbitraryAudioObject(){};

    vector<double> data;

    void output(void* outBuffer){
      sample_t* out = (sample_t*) outBuffer;
      for(int i = 0; i<FRAMES_PER_BUFFER; i++){
        *out++ = (sample_t) data.at(i);
        *out++ = (sample_t) data.at(i);
      }
    };


};

void debug_plotter_spectrum(){
  using namespace matplot;
  
  std::vector<double> y;

  // Osc * osc = new Osc();

  // osc->setFreq(880);

  // appendAudioObjToVector(osc, &y);

  vector<double> dirac = createDirac(FRAMES_PER_BUFFER);

  O1Filter filter;


  ArbitraryAudioObject * aao = new ArbitraryAudioObject;
  aao->data = dirac;

  filter.setInput(aao);

  sample_t outBuffer[2*FRAMES_PER_BUFFER];

  filter.setFc(1000.);
  filter.output((void*) &outBuffer);

  for(int i = 0; i<FRAMES_PER_BUFFER; i++){
    y.push_back((double) outBuffer[2*i]/MAX);
  }

  vector<complex<double>> S = getSpectrum(y);
  vector<double> gain, phase, freqs;
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  
  plotSpectrum(freqs, gain, phase);  

  show();
  
}