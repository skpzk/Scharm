#ifndef OBJECTS_AUDIOOBJECT_H
#define OBJECTS_AUDIOOBJECT_H
#include <iostream>
#include <vector>
#include "../../audioApi/AudioConstants.h"

class AudioObject{
    public:
        AudioObject();
        // AudioObject(int nbCVInputs, int nbCVOutputs);
        virtual void output(void*);
        // this is not really satisfying to have to set yet another output
        // todo : find another solution ?
        virtual void altOutput(void*); 
        virtual void CVOutput(void*);
        virtual void update();
        void setDefaultValue(sample_t);
    protected:
        sample_t defaultValue;
        // int nbCVInputs, nbCVOutputs;

        std::vector<AudioObject*> CVinputs;
};


class ConstantAudioObject: public AudioObject{
    public:
        ConstantAudioObject();
        // AudioObject(int nbCVInputs, int nbCVOutputs);
        void output(void*);
        void CVOutput(void*);

    protected:
        // sample_t defaultValue;
        
};
#endif /* OBJECTS_AUDIOOBJECT_H */
