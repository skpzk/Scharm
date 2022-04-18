#include "plotter.h"

#include <iostream>

#include <pybind11/embed.h>  // py::scoped_interpreter
#include <pybind11/stl.h>    // bindings from C++ STL containers to Python types
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace std;

using namespace py::literals;

#include "plot.h"

#include "../audioLib/objects/env.h"

#include "../audioLib/utils/SignalUtils.h"

#include "../state/state.h"





void plotEnv(){
  cout << "Plotting envelope" <<endl;

  State::params("a")->setValue(0.2);
  State::params("eg")->setValue(on);

  Env* env = new Env();
  env->stateKeys.a = "a";

  ArbitraryAudioObject * aao = new ArbitraryAudioObject;

  std::vector<double> clock;

  for(int i=0;i<FRAMES_PER_BUFFER;i++){
    clock.push_back(0);
  }

  clock[0] = MAX;


  clock[217] = MAX;
  aao->data = clock;

  env->addRhythm(aao);



  sample_t buffer[2*FRAMES_PER_BUFFER];
  initBuffer(buffer);

  env->output(buffer);
  // Start the Python interpreter
  py::scoped_interpreter guard{};

  plt::plot(buffer);
  plt::plot(clock);
  plt::show();
}