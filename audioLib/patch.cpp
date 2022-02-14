#include "patch.h"

#include "objects/mixer.h"
#include "objects/osc.h"
#include "objects/filter.h"

Patch::Patch(){
  Mixer *mixer = new Mixer;

  Vco *vco1 = new Vco;
  Vco *vco2 = new Vco;


  vco1->setNoteMidi(69);
  vco1->stateKeys.freq = "vco1knobfreq";
  vco1->stateKeys.wave = "vco1wave";
  vco1->stateKeys.volume = "vco1level";

  mixer->addInput(vco1);

  vco2->setNoteMidi(69);
  vco2->stateKeys.freq = "vco2knobfreq";
  vco2->stateKeys.wave = "vco2wave";
  vco2->stateKeys.volume = "vco2level";
  
  mixer->addInput(vco2);


  Sub * vco1sub1 = new Sub;

  vco1sub1->stateKeys.div = "vco1sub1div";
  vco1sub1->stateKeys.volume = "vco1sub1level";
  vco1sub1->setVco(vco1);

  mixer->addInput(vco1sub1);


  Sub * vco1sub2 = new Sub;

  vco1sub2->stateKeys.div = "vco1sub2div";
  vco1sub2->stateKeys.volume = "vco1sub2level";
  vco1sub2->setVco(vco1);

  mixer->addInput(vco1sub2);


  Sub * vco2sub1 = new Sub;

  vco2sub1->stateKeys.div = "vco2sub1div";
  vco2sub1->stateKeys.volume = "vco2sub1level";
  vco2sub1->setVco(vco2);

  mixer->addInput(vco2sub1);


  Sub * vco2sub2 = new Sub;

  vco2sub2->stateKeys.div = "vco2sub2div";
  vco2sub2->stateKeys.volume = "vco2sub2level";
  vco2sub2->setVco(vco2);

  mixer->addInput(vco2sub2);

  Vcf * vcf = new Vcf();

  vcf->stateKeys.freq = "cutoff";
  vcf->stateKeys.reso = "reso";

  vcf->setInput(mixer);


  this->outputObj = vcf;
}

void Patch::output(void* out){
  outputObj->output(out);
}