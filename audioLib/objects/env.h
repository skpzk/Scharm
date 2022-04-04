#ifndef OBJECTS_ENV_H
#define OBJECTS_ENV_H

#include "audioObject.h"
#include "stateKeys.h"
// #include "../../audioApi/AudioConstants.h" // included in audioObject.h

#include <vector>

class Env: public AudioObject{
  public:
    Env();
    void output(void*);
    void CVOutput(void*);
    void update();

    void addRhythm(AudioObject*);

    StateKeys stateKeys;

  private:

    float elapsed, T;
    float attackTime;

    float tau_attack, tau_decay, alpha;
    float envValue;

    std::vector<AudioObject*> rhythms;

    void updateClockSignal();

    // sample_t computeAttack();
    // sample_t computeDecay();

    void computeAlpha(bool);

    sample_t computeEnv(bool reset, float);

    sample_t clockSignal[2*FRAMES_PER_BUFFER];

    void checkValues();

};

#endif /* OBJECTS_ENV_H */
