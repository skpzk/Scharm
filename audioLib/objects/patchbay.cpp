#include "patchbay.h"

#include <iostream>

#include "../utils/SignalUtils.h"
#include "../../state/state.h"

using namespace std;

void PpOutVector::set(string n, AudioObject* ao, void (AudioObject::*out)(void*)){
  for(int i=0;i<this->size();i++){
    if((*this)[i]->name == n){
      (*this)[i]->ao = ao;
      (*this)[i]->output = out;
      return;
    }
  }
}

void PpOutVector::add(string n, AudioObject* ao, void (AudioObject::*out)(void*)){
  PpOut pp = {n, ao, out};
  push_back(&pp);
}

int PpOutVector::pop(string n){
  for(int i=0;i<this->size();i++){
    if((*this)[i]->name == n){
      this->erase(this->begin() + i);
      return 0;
    }
  }
  return 1;
}


PpIn::PpIn(){
  name = "noname";
}

PpIn::PpIn(string n){
  name = n;
}

void PpIn::output(void* outbuffer){
  blockDisconnect = true;
  // initBuffer(outbuffer);
  for(PpOut* pp : ppOutVector){
    ((pp->ao)->*(pp->output))(outbuffer);
    // (ao->*outputFunction)(test);
  }
  blockDisconnect = false;
}

void PpIn::connect(PpOut* ppOut){
  ppOutVector.push_back(ppOut);
}

void PpIn::disconnect(PpOut* ppOut){
  while(blockDisconnect){}
  ppOutVector.pop(ppOut->name);
}


Patchbay::Patchbay(){
  // cout << "creating patchbay Object\n";

  outs[ppo_vco1] = {"vco1",  new AudioObject(), &AudioObject::output};
  outs[ppo_vco2] = {"vco2",  new AudioObject(), &AudioObject::output};

  outs[ppo_vco1sub1] = {"vco1sub1",  new AudioObject(), &AudioObject::output};
  outs[ppo_vco1sub2] = {"vco1sub2",  new AudioObject(), &AudioObject::output};
  outs[ppo_vco2sub1] = {"vco2sub1",  new AudioObject(), &AudioObject::output};
  outs[ppo_vco2sub2] = {"vco2sub2",  new AudioObject(), &AudioObject::output};

  outs[ppo_vca] = {"vca",  new AudioObject(), &AudioObject::output};
  outs[ppo_vcaeg] = {"vcaeg",  new AudioObject(MAX), &AudioObject::output};

  outs[ppo_seq1] = {"seq1",  new AudioObject(), &AudioObject::output};
  outs[ppo_seq2] = {"seq2",  new AudioObject(), &AudioObject::output};

  outs[ppo_vcfeg] = {"vcfeg",  new AudioObject(MAX), &AudioObject::output};

  outs[ppo_noise] = {"noise",  new AudioObject(), &AudioObject::output};
  outs[ppo_sh] = {"sh",  new AudioObject(), &AudioObject::output};

  outs[ppo_clock] = {"clock",  new AudioObject(), &AudioObject::output};
  outs[ppo_seq1clk] = {"seq1clk",  new AudioObject(), &AudioObject::output};
  outs[ppo_seq2clk] = {"seq2clk",  new AudioObject(), &AudioObject::output};

  ins[0] = PpIn("vco1");
  ins[1] = PpIn("vco1sub");
  ins[2] = PpIn("vco1pwm");
  ins[3] = PpIn("vca");
  ins[4] = PpIn("vco2");
  ins[5] = PpIn("vco2sub");
  ins[6] = PpIn("vco2pwm");
  ins[7] = PpIn("cutoff");
  ins[8] = PpIn("play");
  ins[9] = PpIn("reset");
  ins[10] = PpIn("trigger");
  ins[11] = PpIn("rhythm1");
  ins[12] = PpIn("rhythm2");
  ins[13] = PpIn("rhythm3");
  ins[14] = PpIn("rhythm4");
  ins[15] = PpIn("clock");
  ins[16] = PpIn("seq1");
  ins[17] = PpIn("seq2");


}

void Patchbay::checkState(){
  vector<string> ppOuts;
  for(PpIn ppIn:ins){
    ppOuts = State::connections.getConnectionsToPpIn(ppIn.name);
    for(string ppOut:ppOuts){
      connect(ppIn.name, ppOut);
    }
  }
}

void Patchbay::debug_addOutput(AudioObject* ao, void (AudioObject::*outputFunction)(void*) ){
  // outs.push_back(outputFunction);
  cout << "output function added, calling function :\n";
  
  // void* test;
  
  // (ao->*outputFunction)(test);

  PpOut ppOut;
  ppOut.ao = ao;
  ppOut.output = outputFunction;

  // outs.push_back(ppOut);

}

void Patchbay::debug_printOuts(){
  // for(auto p : outs){
  //   cout << "function pointer : " << p.output << endl;
  // }
}

void Patchbay::setOutput(std::string n, AudioObject* ao, void (AudioObject::*out)(void*)){
  for(int i=0; i<PPOUTLENGTH; i++){
    if(outs[i].name == n){
      outs[i].ao = ao;
      outs[i].output = out;
      return;
    }
  }
}
void Patchbay::setOutput(std::string n, AudioObject* ao){
  setOutput(n, ao, &AudioObject::CVOutput);
}

PpIn* Patchbay::getInput(std::string inPpName){ 
  for(int i=0; i<PPINLENGTH; i++){
    if(ins[i].name == inPpName){
      return &(ins[i]);
    }
  }
  return nullptr;
}

void Patchbay::connect(std::string inPpName, std::string outPpName){

  PpOut * ppOut = nullptr;
  
  PpIn * ppIn = getInput(inPpName);

  for(int i=0; i<PPOUTLENGTH; i++){
    if(outs[i].name == outPpName){
      ppOut = &(outs[i]);
    }
  }

  if(ppIn ==nullptr || ppOut == nullptr)
    return;

  ppIn->connect(ppOut);

}

void Patchbay::disconnect(std::string inPpName, std::string outPpName){

  PpOut * ppOut = nullptr;
  
  PpIn * ppIn = getInput(inPpName);

  for(int i=0; i<PPOUTLENGTH; i++){
    if(outs[i].name == outPpName){
      ppOut = &(outs[i]);
    }
  }

  if(ppIn ==nullptr || ppOut == nullptr)
    return;

  ppIn->disconnect(ppOut);

}