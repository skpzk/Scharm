#ifndef OBJECTS_SEQUENCER_H
#define OBJECTS_SEQUENCER_H

#include "audioObject.h"
#include "stateKeys.h"
#include <vector>


class Sequencer : public AudioObject{
  public:
    Sequencer();

    void output(void*);
    void CVOutput(void*);
    void altOutput(void*);

    void update();
    void addRhythm(AudioObject*);

    StateKeys stateKeys;


  private:
    std::vector<AudioObject*> rhythms;
    float steps[4];
    bool activeRhythms[4];

    int reset;
    bool next;

    sample_t clockSignal[2*FRAMES_PER_BUFFER];
    sample_t stepsSignal[2*FRAMES_PER_BUFFER];

    void checkValues();
    void updateClockSignal();
    void updateParam();

    sample_t lastValue, value;
    int stepCounter;

};

#endif /* OBJECTS_SEQUENCER_H */
