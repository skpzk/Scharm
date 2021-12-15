#ifndef SCHARM_H
#define SCHARM_H

#include <iostream>
#include "audioApi/audio.h"



class Scharm{
    public:
        Scharm();
        void close();
        int test = 12;

        static void callback(void*, float);

        Audio * audio;
};


#endif // SCHARM_H