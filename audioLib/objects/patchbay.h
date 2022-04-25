#ifndef OBJECTS_PATCHBAY_H
#define OBJECTS_PATCHBAY_H

#define PPOUTLENGTH 16
#define PPINLENGTH 18

class PpIn;
#include "audioObject.h"
#include <string>

#include <vector>

typedef struct{
  std::string name;
  AudioObject* ao = nullptr;
  void (AudioObject::*output)(void*) = nullptr;
}PpOut;

// well, this is nice and all, but not very useful
enum ppOutNames {
  ppo_vco1, ppo_vco2, ppo_vco1sub1, ppo_vco1sub2, ppo_vco2sub1, 
  ppo_vco2sub2, ppo_vca, ppo_vcaeg, ppo_seq1, ppo_seq2, 
  ppo_vcfeg, ppo_noise, ppo_sh, ppo_clock, ppo_seq1clk, 
  ppo_seq2clk};  // 16 names

// enum ppInNames {
//   ppi_vco1, ppi_vco1sub, ppi_vco1pwm, ppi_vca, ppi_vco2,
//   ppi_vco2sub, ppi_vco2pwm, ppi_cutoff, ppi_play, ppi_reset,
//   ppi_trigger, ppi_rhythm1, ppi_rhythm2, ppi_rhythm3, ppi_rhythm4,
//   ppi_clock, ppi_seq1, ppi_seq2}; // 18 names

class PpOutVector: public std::vector<PpOut*>{
  public:
    void set(std::string, AudioObject*, void (AudioObject::*)(void*));
    void add(std::string, AudioObject*, void (AudioObject::*)(void*));

    int pop(std::string n);
};

class PpIn{
  public:
    PpIn();
    PpIn(std::string);
    std::string name;

    void output(void*);

    void connect(PpOut*);
    void disconnect(PpOut*);

  private:

    bool blockDisconnect=false;
    PpOutVector ppOutVector;

};

class Patchbay{
  public:
    Patchbay();

    void connect(std::string inPp, std::string outPp);
    void disconnect(std::string inPp, std::string outPp);

    void debug_addOutput(AudioObject*, void (AudioObject::*)(void*));
    void debug_printOuts();

    void setOutput(std::string, AudioObject*, void (AudioObject::*)(void*));
    void setOutput(std::string, AudioObject*);

    PpIn* getInput(std::string);

    void checkState();

  private:
    
    PpIn ins[PPINLENGTH];
    PpOut outs[PPOUTLENGTH];
};

#endif /* OBJECTS_PATCHBAY_H */
