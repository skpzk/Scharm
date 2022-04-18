#include "filter.h"


#include <fftw3.h>
#include <iostream>

#include <cmath>
#include <vector>

#include "../audioLib/objects/osc.h"
#include "../audioLib/objects/filter.h"
#include "../audioLib/objects/audioObject.h"

#include "../audioApi/AudioConstants.h"

#include <pybind11/embed.h>  // py::scoped_interpreter
#include <pybind11/stl.h>    // bindings from C++ STL containers to Python types
#include <pybind11/numpy.h>

#include <complex.h> // must be included after pybind !

namespace py = pybind11;

using namespace std;

std::vector<double>make_vector_from_1d_numpy_array( py::array_t<double>py_array )
{
    return std::vector<double>(py_array.data(), py_array.data() + py_array.size());
}

vector<double> linspace(float start, float stop, float size){
  vector<double> signal;

  py::object np = py::module_::import("numpy");
  py::object linspace = np.attr("linspace");
  py::array_t<double> sig = linspace(start, stop, (int) size);

  signal = make_vector_from_1d_numpy_array(sig);

  return signal;
}

void plotExample(){
  std::vector<double> signal(1024);
    for (size_t i = 0; i < signal.size(); ++i)
        signal[i] = std::exp(i / -256.0) * std::cos(2 * M_PI * 8 * i / 1024.0);

    // Start the Python interpreter
    py::scoped_interpreter guard{};
    using namespace py::literals;

    // Save the necessary local variables in a Python dict
    py::dict locals = py::dict{
        "signal"_a = signal,
    };

    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    plt.plot(signal)
    plt.show()
    
    )",
             py::globals(), locals);
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

void plotSpectrum(vector<double> freqs, vector<double> gain, vector<double> phase, double fc, int order, bool newFigure){
  
  
  vector<double> gainDb, minus3Db, minusPiOver4, asymptotes;
  for(int i =0; i<gain.size(); i++){
    gainDb.push_back(20*log10(gain.at(i)));
    minus3Db.push_back(-3);
    minusPiOver4.push_back(-M_PI/4 * order);
    if(freqs.at(i) <= fc){
      asymptotes.push_back(0);
    }else{
      asymptotes.push_back(-20 * order * log10(freqs.at(i)/fc));
    }
    while(phase.at(i) > 0){
      phase.at(i) = phase.at(i) - 2 * M_PI;
    }
  }
  // if(newFigure){ plt::figure();}
  // plt::subplot(2,1,0);
  // plt::hold(plt::on);
  // plt::semilogx(freqs, gainDb);
  // plt::semilogx(freqs,minus3Db);
  // plt::semilogx(freqs,asymptotes);
  // plt::subplot(2,1,1);
  // plt::hold(plt::on);
  // plt::semilogx(freqs, phase);
  // plt::semilogx(freqs,minusPiOver4);

  using namespace py::literals; // _a defined here

  py::dict locals = py::dict{
        "freqs"_a = freqs,
        "gainDb"_a = gainDb,
        "asymptotes"_a = asymptotes,
        "phase"_a = phase,
        "fc"_a = fc,
        "newFigure"_a = newFigure,

    };
    M_PI;
    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    if newFigure:
      plt.figure()
      plt.subplot(211)
      plt.axvline(fc, color="black", lw=0.707)
      plt.plot(asymptotes, color="black", lw=0.707)
      plt.axhline(-3, color="black", lw=0.707)
      plt.subplot(212)
      plt.axvline(fc, color="black", lw=0.707)
      plt.axhline(-3.14159265358979323846, color="black", lw=0.707)
    plt.subplot(211)
    plt.semilogx(freqs, gainDb)
    plt.subplot(212)
    plt.semilogx(freqs, phase)

    )",
             py::globals(), locals);
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

  vector<double> freqs = linspace(0, SAMPLE_RATE/2, FRAMES_PER_BUFFER/2);
  printf("size freqs = %d\n", freqs.size());
  if(returnAll){
    vector<double> negFreqs = linspace(-SAMPLE_RATE/2, 0, FRAMES_PER_BUFFER/2);
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

// class ArbitraryAudioObject : public AudioObject{
//   public:
//     ArbitraryAudioObject(){};

//     vector<double> data;

//     void output(void* outBuffer){
//       sample_t* out = (sample_t*) outBuffer;
//       for(int i = 0; i<FRAMES_PER_BUFFER; i++){
//         *out++ = (sample_t) data.at(i);
//         *out++ = (sample_t) data.at(i);
//       }
//     };


// };

void filterDirac(MFilter filter, vector<double>* y){

  // MFilter filter;
  sample_t outBuffer[2*FRAMES_PER_BUFFER];

  filter.output((void*) &outBuffer);

  for(int i = 0; i<FRAMES_PER_BUFFER; i++){
    y->push_back((double) outBuffer[2*i]/MAX);
  }
}

void resetVectors(vector<double>* y, vector<double>* freqs, 
  vector<double>* gain, vector<double>* phase){
  y->clear();
  freqs->clear();
  gain->clear();
  phase->clear();
}

void debug_plotter_spectrum(){
  // using namespace matplot;

  py::scoped_interpreter guard{};
  
  printf("chunk size = %d\n", FRAMES_PER_BUFFER);

  std::vector<double> y;

  // Osc * osc = new Osc();

  // osc->setFreq(880);

  // appendAudioObjToVector(osc, &y);

  using namespace py::literals; // _a defined here

  py::dict locals = py::dict{};

  py::exec(R"(
  import matplotlib.pyplot as plt
  )",
    py::globals(), locals);

  MFilter filter;
  vector<double> gain, phase, freqs;
  vector<complex<double>> S;

  vector<double> dirac = createDirac(FRAMES_PER_BUFFER);
  ArbitraryAudioObject * aao = new ArbitraryAudioObject;
  aao->data = dirac;
  filter.setInput(aao);
  filter.setFc(1000.);

  filter.setK(1);
  filterDirac(filter, &y);
  S = getSpectrum(y);
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  plotSpectrum(freqs, gain, phase, 1000., 4, true);  
  resetVectors(&y, &freqs, &gain, &phase);

  filter.setK(2);
  filter.reset();
  filterDirac(filter, &y);
  S = getSpectrum(y);
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  plotSpectrum(freqs, gain, phase, 1000., 4, false);
  resetVectors(&y, &freqs, &gain, &phase);  

  filter.setK(3);
  filter.reset();
  filterDirac(filter, &y);
  S = getSpectrum(y);
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  plotSpectrum(freqs, gain, phase, 1000., 4, false);
  resetVectors(&y, &freqs, &gain, &phase);  

  filter.setK(4);
  filter.reset();
  filterDirac(filter, &y);
  S = getSpectrum(y);
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  plotSpectrum(freqs, gain, phase, 1000., 4, false);
  resetVectors(&y, &freqs, &gain, &phase);  

  filter.setK(5);
  filter.reset();
  filterDirac(filter, &y);
  S = getSpectrum(y);
  getFreqsGainAndPhase(S, &freqs, &gain, &phase);
  plotSpectrum(freqs, gain, phase, 1000., 4, false);  

  
  
  

  // Execute Python code, using the variables saved in `locals`
  py::exec(R"(
  import matplotlib.pyplot as plt
  plt.show()
  )",
    py::globals(), locals);
  

  // plt::show();
  
}




void testPyBind(){
  
  
  
  vector<double> signal = linspace(0, 1, 100);
  

  int a = signal.size();

  printf("size signal = %d\n", a);

}