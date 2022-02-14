#ifndef AUDIOLIB_AUDIOOBJECT_H
#define AUDIOLIB_AUDIOOBJECT_H
#include <iostream>
#include <vector>
#include "../../audioApi/AudioConstants.h"

class AudioObject{
    public:
        AudioObject();
        // AudioObject(int nbCVInputs, int nbCVOutputs);
        virtual void output(void*);
        virtual void CVOutput(void*);
        void setDefaultValue(sample_t);
    protected:
        sample_t defaultValue;
        // int nbCVInputs, nbCVOutputs;

        std::vector<AudioObject*> CVinputs;
};
#endif /* AUDIOLIB_AUDIOOBJECT_H */
