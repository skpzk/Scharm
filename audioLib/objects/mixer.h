#ifndef AUDIOLIB_MIXER_H
#define AUDIOLIB_MIXER_H

#include "audioObject.h"
#include <vector>

class Mixer: public AudioObject{
  public:
    Mixer();

    void output(void*);
    void addInput(AudioObject*);

  private:
    std::vector<AudioObject*> inputs;
};

#endif /* AUDIOLIB_MIXER_H */
