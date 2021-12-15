#ifndef OSC_H_
#define OSC_H_

#include <iostream>
#include <cmath>

#include "audioObject.h"

class Osc : public AudioObject{
    public:
        Osc();

        void output(void*);
        void CVOutput(void*);
    private:
        // Wavetables sample_t
};

class Vco : public Osc{

};

class Sub : public Osc{

};

#endif // OSC_H_