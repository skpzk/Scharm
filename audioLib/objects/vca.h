#ifndef OBJECTS_VCA_H
#define OBJECTS_VCA_H

#include "audioObject.h"

enum vcaInputs {vcaIn};

class Vca : public AudioObject{
  public:
    Vca();
    void output(void*);
    void CVOutput(void*);
    void altOutput(void*);
    void setEnv(AudioObject*);
    void setInput(AudioObject*);

    void checkValues();
  
  private:
    AudioObject* eg=nullptr;
    AudioObject* input=nullptr;

    sample_t env[FRAMES_PER_BUFFER*2];
    sample_t audio[FRAMES_PER_BUFFER*2];
    sample_t processedAudio[FRAMES_PER_BUFFER*2];

    float volume;

};

#endif /* OBJECTS_VCA_H */
