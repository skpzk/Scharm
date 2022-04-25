#include "patch.h"

#include "objects/mixer.h"
#include "objects/osc.h"
#include "objects/filter.h"
#include "objects/vca.h"

#include "objects/clock.h"
#include "objects/sequencer.h"
#include "objects/env.h"
#include "objects/patchbay.h"

#include "../state/state.h"

Patch::Patch(){
  Mixer *mixer = new Mixer;

  Vco *vco1 = new Vco;
  Vco *vco2 = new Vco;


  vco1->setNoteMidi(69);
  vco1->stateKeys.freq = "vco1knobfreq";
  vco1->stateKeys.wave = "vco1wave";
  vco1->stateKeys.volume = "vco1level";
  vco1->stateKeys.assignvco = "seq1assignToVco1";

  mixer->addInput(vco1);

  vco2->setNoteMidi(69);
  vco2->stateKeys.freq = "vco2knobfreq";
  vco2->stateKeys.wave = "vco2wave";
  vco2->stateKeys.volume = "vco2level";
  vco2->stateKeys.assignvco = "seq2assignToVco2";
  
  mixer->addInput(vco2);


  Sub * vco1sub1 = new Sub;

  vco1sub1->stateKeys.div = "vco1sub1div";
  vco1sub1->stateKeys.volume = "vco1sub1level";
  vco1sub1->stateKeys.assignvco = "seq1assignToSub1";
  vco1sub1->setVco(vco1);

  mixer->addInput(vco1sub1);


  Sub * vco1sub2 = new Sub;

  vco1sub2->stateKeys.div = "vco1sub2div";
  vco1sub2->stateKeys.volume = "vco1sub2level";
  vco1sub2->stateKeys.assignvco = "seq1assignToSub2";
  vco1sub2->setVco(vco1);

  mixer->addInput(vco1sub2);


  Sub * vco2sub1 = new Sub;

  vco2sub1->stateKeys.div = "vco2sub1div";
  vco2sub1->stateKeys.volume = "vco2sub1level";
  vco2sub1->setVco(vco2);
  vco2sub1->stateKeys.assignvco = "seq2assignToSub1";

  mixer->addInput(vco2sub1);


  Sub * vco2sub2 = new Sub;

  vco2sub2->stateKeys.div = "vco2sub2div";
  vco2sub2->stateKeys.volume = "vco2sub2level";
  vco2sub2->stateKeys.assignvco = "seq2assignToSub2";
  vco2sub2->setVco(vco2);

  mixer->addInput(vco2sub2);

  Clock * clock = new Clock();
  clock->stateKeys.guiCallback = "activetempo";

  RhythmGenerator * rg1 = new RhythmGenerator();
  rg1->stateKeys.div = "clk1";
  rg1->stateKeys.guiCallback = "activerhythm1";
  rg1->setInput(clock);

  RhythmGenerator * rg2 = new RhythmGenerator();
  rg2->stateKeys.div = "clk2";
  rg2->stateKeys.guiCallback = "activerhythm2";
  rg2->setInput(clock);

  RhythmGenerator * rg3 = new RhythmGenerator();
  rg3->stateKeys.div = "clk3";
  rg3->stateKeys.guiCallback = "activerhythm3";
  rg3->setInput(clock);

  RhythmGenerator * rg4 = new RhythmGenerator();
  rg4->stateKeys.div = "clk4";
  rg4->stateKeys.guiCallback = "activerhythm4";
  rg4->setInput(clock);

  clock->addRhythm(rg1);
  clock->addRhythm(rg2);
  clock->addRhythm(rg3);
  clock->addRhythm(rg4);

  Sequencer * seq1 = new Sequencer();
  seq1->stateKeys.step1 = "seq1step1";
  seq1->stateKeys.step2 = "seq1step2";
  seq1->stateKeys.step3 = "seq1step3";
  seq1->stateKeys.step4 = "seq1step4";
  seq1->stateKeys.clk1 = "seq1clk1";
  seq1->stateKeys.clk2 = "seq1clk2";
  seq1->stateKeys.clk3 = "seq1clk3";
  seq1->stateKeys.clk4 = "seq1clk4";
  seq1->stateKeys.guiCallback = "activeseq1step";

  seq1->addRhythm(rg1);
  seq1->addRhythm(rg2);
  seq1->addRhythm(rg3);
  seq1->addRhythm(rg4);

  vco1->setSequencer(seq1);
  vco1sub1->setSequencer(seq1);
  vco1sub2->setSequencer(seq1);

  clock->addSeq(seq1);


  Sequencer * seq2 = new Sequencer();
  seq2->stateKeys.step1 = "seq2step1";
  seq2->stateKeys.step2 = "seq2step2";
  seq2->stateKeys.step3 = "seq2step3";
  seq2->stateKeys.step4 = "seq2step4";
  seq2->stateKeys.clk1 = "seq2clk1";
  seq2->stateKeys.clk2 = "seq2clk2";
  seq2->stateKeys.clk3 = "seq2clk3";
  seq2->stateKeys.clk4 = "seq2clk4";
  seq2->stateKeys.guiCallback = "activeseq2step";

  seq2->addRhythm(rg1);
  seq2->addRhythm(rg2);
  seq2->addRhythm(rg3);
  seq2->addRhythm(rg4);

  vco2->setSequencer(seq2);
  vco2sub1->setSequencer(seq2);
  vco2sub2->setSequencer(seq2);

  clock->addSeq(seq2);

  Env * filterEg = new Env();
  filterEg->stateKeys.a = "filtera";
  filterEg->stateKeys.d = "filterd";

  filterEg->addRhythm(seq1);
  filterEg->addRhythm(seq2);

  

  Vcf * vcf = new Vcf();

  vcf->stateKeys.freq = "cutoff";
  vcf->stateKeys.reso = "reso";

  // vcf->setInput(mixer);
  vcf->setInput(mixer);
  vcf->setEgInput(filterEg);

  // Osc* osc = new Osc();

  Vca * vca = new Vca();

  vca->setInput(vcf);

  // ConstantAudioObject * aao = new ConstantAudioObject();
  // aao->setDefaultValue(MAX);
  // vca->setEnv(aao);

  Env * eg = new Env();
  eg->stateKeys.a = "a";
  eg->stateKeys.d = "d";

  eg->addRhythm(seq1);
  eg->addRhythm(seq2);

  vca->setEnv(eg);

  // vca->setEnv(clock);

  Patchbay * patchbay = new Patchbay();

  State::connections.setPatchbay(patchbay);

  // patchbay->debug_addOutput(vca, &AudioObject::output);
  patchbay->setOutput("vcaeg", vca, &AudioObject::altOutput);
  patchbay->setOutput("vco1", vco1);
  patchbay->setOutput("vco2", vco2);
  patchbay->setOutput("vco1sub1", vco1sub1);
  patchbay->setOutput("vco1sub2", vco1sub2);
  patchbay->setOutput("vco2sub1", vco2sub1);
  patchbay->setOutput("vco2sub2", vco2sub2);
  patchbay->setOutput("vca", vca);

  vco1->setCVInput(vcoIn_vco, patchbay->getInput("vco1"));
  vco2->setCVInput(vcoIn_vco, patchbay->getInput("vco2"));

  vca->setCVInput(vcaIn, patchbay->getInput("vca"));

  // patchbay->connect("vco1", "vcaeg");

  patchbay->checkState();

  this->outputObj = vca;
  this->masterClock = clock;
}

void Patch::output(void* out){
  initBuffer(out);
  outputObj->output(out);
}

void Patch::update(){
  this->masterClock->update();

}