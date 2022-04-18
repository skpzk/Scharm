#ifndef OBJECTS_CLOCK_H
#define OBJECTS_CLOCK_H

#include <vector>
#include "audioObject.h"
#include "Wave.h"
#include "stateKeys.h"

// class RhythmGenerator;


class Clock: public AudioObject{
  public:
    Clock();

    void CVOutput(void*);
    void output(void*);
    void update();

    StateKeys stateKeys;

    void setFreq(float);
    void updatePhaseIncrement();

    void addRhythm(AudioObject*);
    void addSeq(AudioObject*);


  private:
    void tick();
    void checkValues();
    float freq;
    bool playing;
    
    sample_t clockSignal[2*FRAMES_PER_BUFFER];
    sample_t risingEdgeSignal[2*FRAMES_PER_BUFFER];

    Wave wave;

    float phaseIncrement;
		float phase;

		// float lastOutput;

    void updateClockSignal();
    void updateParam();
    std::vector<AudioObject*> rhythms;
    std::vector<AudioObject*> sequences;

};


/*
each rhythm sets a parameter in state,
to this parameter is set a callback to the gui that repaints the knob title

*/
class RhythmGenerator: public AudioObject{
  public:
    RhythmGenerator();

    void CVOutput(void*);
    void output(void*);
    void update();

    StateKeys stateKeys;

    void setInput(AudioObject*);

  private:
    void tick();
    void checkValues();
    bool reset;

    sample_t clockSignal[2*FRAMES_PER_BUFFER];
    sample_t risingEdgeSignal[2*FRAMES_PER_BUFFER];

		float lastOutput;

    int div, accum;

    void updateClockSignal();
    void updateParam();
    AudioObject * masterClock;

    float lastValueIn, lastValueOut;

};

#endif /* OBJECTS_CLOCK_H */
