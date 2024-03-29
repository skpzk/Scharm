#ifndef OBJECTS_AUDIOOBJECT_H
#define OBJECTS_AUDIOOBJECT_H
#include <iostream>
#include <vector>
#include "../../audioApi/AudioConstants.h"

class AudioObject;

#include "patchbay.h"

class AudioObject{
    public:
        AudioObject();
        AudioObject(sample_t);
        // AudioObject(int nbCVInputs, int nbCVOutputs);
        virtual void output(void*);
        // this is not really satisfying to have to set yet another output
        // todo : find another solution ?
        virtual void altOutput(void*); 
        virtual void CVOutput(void*);
        virtual void update();
        void setDefaultValue(sample_t);

        void setNumberOfCVInputs(int);
        void setCVInput(int, PpIn*);
        
    protected:
        sample_t defaultValue;
        // int nbCVInputs, nbCVOutputs;

        std::vector<PpIn*> CVinputs;
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

class ArbitraryAudioObject : public AudioObject{
  public:
    ArbitraryAudioObject();

    std::vector<double> data;
    void altOutput(void*);
    


};
#endif /* OBJECTS_AUDIOOBJECT_H */
