#ifndef OBJECTS_VCA_H
#define OBJECTS_VCA_H

#include "audioObject.h"

class Vca : public AudioObject{
  public:
    Vca();
    void output(void*);
    void setEnv(AudioObject*);
    void setInput(AudioObject*);
  
  private:
    AudioObject* eg=nullptr;
    AudioObject* input=nullptr;

    sample_t env[FRAMES_PER_BUFFER*2];
    sample_t audio[FRAMES_PER_BUFFER*2];

};

#endif /* OBJECTS_VCA_H */
