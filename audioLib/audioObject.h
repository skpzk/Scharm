#ifndef audioobject_h_
#define audioobject_h_
#include <iostream>
#include "../audioApi/AudioConstants.h"

class AudioObject{
    public:
        AudioObject();
        virtual void output(void*);
        virtual void CVOutput(void*);
        void setDefaultValue(float);
    private:
        float defaultValue;
};
#endif
