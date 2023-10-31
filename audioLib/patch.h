#ifndef AUDIOLIB_PATCH_H
#define AUDIOLIB_PATCH_H

#include "objects/audioObject.h"
#include "objects/stateKeys.h"

#include <vector>

enum patchInputs {oscillo1, oscillo2};

class Patch: public AudioObject{
  public:
    Patch();

    void output(void*);
    void update();

    StateKeys stateKeys;

  private:
    AudioObject* outputObj;
    AudioObject* masterClock;
    AudioObject ** utils;
    int utilsNumber = 1;

    sample_t audio[FRAMES_PER_BUFFER*2];

    int counter=0;
};

#endif /* AUDIOLIB_PATCH_H */
