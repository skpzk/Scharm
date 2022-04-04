#ifndef AUDIOLIB_PATCH_H
#define AUDIOLIB_PATCH_H

#include "objects/audioObject.h"

#include <vector>

class Patch: public AudioObject{
  public:
    Patch();

    void output(void*);
    void update();

  private:
    AudioObject* outputObj;
    AudioObject* masterClock;
};

#endif /* AUDIOLIB_PATCH_H */
