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
#include "../audioLib/objects/clock.h"
#include "../audioLib/objects/sequencer.h"

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

void plotClock(){
  // for this to work, set #define FRAMES_PER_BUFFER   (5000) in AudioConstants.h

  cout << "Plotting clock" <<endl;
  // Start the Python interpreter
  py::scoped_interpreter guard{};

  State::params("tempo")->setValue(127);
  State::params("play")->setValue(1);
  State::params("clk1")->setValue(1);
  State::params("clk2")->setValue(0);
  State::params("clk3")->setValue(1);
  State::params("clk4")->setValue(1);
  State::params("reset")->setValue(0);

  State::params("seq1step1")->setValue(0);
  State::params("seq1step2")->setValue(1);
  State::params("seq1step3")->setValue(2);
  State::params("seq1step4")->setValue(3);

  State::params("seq1clk1")->setValue(0);
  State::params("seq1clk2")->setValue(1);
  State::params("seq1clk3")->setValue(0);
  State::params("seq1clk4")->setValue(0);

  State::params("seq2step1")->setValue(0);
  State::params("seq2step2")->setValue(1);
  State::params("seq2step3")->setValue(2);
  State::params("seq2step4")->setValue(3);

  State::params("seq2clk1")->setValue(0);
  State::params("seq2clk2")->setValue(0);
  State::params("seq2clk3")->setValue(1);
  State::params("seq2clk4")->setValue(0);
  

  Clock* clk = new Clock();

  RhythmGenerator * rg1 = new RhythmGenerator();
  rg1->stateKeys.div = "clk1";
  rg1->setInput(clk);
  clk->addRhythm(rg1);

  RhythmGenerator * rg2 = new RhythmGenerator();
  rg2->stateKeys.div = "clk2";
  rg2->setInput(clk);
  clk->addRhythm(rg2);

  RhythmGenerator * rg3 = new RhythmGenerator();
  rg3->stateKeys.div = "clk3";
  rg3->setInput(clk);
  clk->addRhythm(rg3);

  RhythmGenerator * rg4 = new RhythmGenerator();
  rg4->stateKeys.div = "clk4";
  rg4->setInput(clk);
  clk->addRhythm(rg4);

  Sequencer * seq1 = new Sequencer();
  seq1->stateKeys.step1 = "seq1step1";
  seq1->stateKeys.step2 = "seq1step2";
  seq1->stateKeys.step3 = "seq1step3";
  seq1->stateKeys.step4 = "seq1step4";
  seq1->stateKeys.clk1 = "seq1clk1";
  seq1->stateKeys.clk2 = "seq1clk2";
  seq1->stateKeys.clk3 = "seq1clk3";
  seq1->stateKeys.clk4 = "seq1clk4";

  seq1->addRhythm(rg1);
  seq1->addRhythm(rg2);
  seq1->addRhythm(rg3);
  seq1->addRhythm(rg4);

  Sequencer * seq2 = new Sequencer();
  seq2->stateKeys.step1 = "seq2step1";
  seq2->stateKeys.step2 = "seq2step2";
  seq2->stateKeys.step3 = "seq2step3";
  seq2->stateKeys.step4 = "seq2step4";
  seq2->stateKeys.clk1 = "seq2clk1";
  seq2->stateKeys.clk2 = "seq2clk2";
  seq2->stateKeys.clk3 = "seq2clk3";
  seq2->stateKeys.clk4 = "seq2clk4";

  seq2->addRhythm(rg1);
  seq2->addRhythm(rg2);
  seq2->addRhythm(rg3);
  seq2->addRhythm(rg4);

  clk->addSeq(seq1);
  clk->addSeq(seq2);

  sample_t buffer[2*FRAMES_PER_BUFFER];
  initBuffer(buffer);

  clk->update();
  clk->output(buffer);
  plt::plot(buffer);

  sample_t bufferRg1[2*FRAMES_PER_BUFFER];
  sample_t buffer2Rg1[2*FRAMES_PER_BUFFER];
  initBuffer(bufferRg1);
  initBuffer(buffer2Rg1);
  
  rg1->output(bufferRg1);
  rg1->CVOutput(buffer2Rg1);
  plt::plotWithOffset(bufferRg1, 1.5);
  plt::plotWithOffset(buffer2Rg1, 3);

  sample_t bufferRg2[2*FRAMES_PER_BUFFER];
  sample_t buffer2Rg2[2*FRAMES_PER_BUFFER];
  initBuffer(bufferRg2);
  initBuffer(buffer2Rg2);
  
  rg2->output(bufferRg2);
  rg2->CVOutput(buffer2Rg2);
  plt::plotWithOffset(bufferRg2, 4.5);
  plt::plotWithOffset(buffer2Rg2, 6);

  // plot seq1
  sample_t bufferSeq1Output[2*FRAMES_PER_BUFFER];
  // sample_t bufferSeq1CVOutput[2*FRAMES_PER_BUFFER];
  sample_t bufferSeq1AltOutput[2*FRAMES_PER_BUFFER];
  initBuffer(bufferSeq1Output);
  // initBuffer(bufferSeq1CVOutput);
  initBuffer(bufferSeq1AltOutput);

  seq1->output(bufferSeq1Output);
  // seq1->CVOutput(bufferSeq1CVOutput);
  seq1->altOutput(bufferSeq1AltOutput);

  plt::plotHLineAt(7.5);
  plt::plotWithOffsetAndStretch(bufferSeq1Output, 7.5, 0.25);
  // plt::plotWithOffset(bufferSeq1CVOutput, 9);
  plt::plotWithOffset(bufferSeq1AltOutput, 9);

  // plot seq2
  sample_t bufferSeq2Output[2*FRAMES_PER_BUFFER];
  // sample_t bufferSeq2CVOutput[2*FRAMES_PER_BUFFER];
  sample_t bufferSeq2AltOutput[2*FRAMES_PER_BUFFER];
  initBuffer(bufferSeq2Output);
  // initBuffer(bufferSeq2CVOutput);
  initBuffer(bufferSeq2AltOutput);

  seq2->output(bufferSeq2Output);
  // seq2->CVOutput(bufferSeq2CVOutput);
  seq2->altOutput(bufferSeq2AltOutput);

  plt::plotHLineAt(10.5);
  plt::plotWithOffsetAndStretch(bufferSeq2Output, 10.5, 0.25);
  // plt::plotWithOffset(bufferSeq2CVOutput, 9);
  plt::plotWithOffset(bufferSeq2AltOutput, 12);

  plt::show();
}