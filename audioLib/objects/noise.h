#ifndef OBJECTS_NOISE_H
#define OBJECTS_NOISE_H

#include "audioObject.h"

class NoiseGenerator : public AudioObject{
  public:
    NoiseGenerator();
    void output(void*);
    void CVOutput(void*);
    void update();
  
  private:
    sample_t audio[FRAMES_PER_BUFFER*2];
    float volume = .5;
};

#endif /* OBJECTS_NOISE_H */
