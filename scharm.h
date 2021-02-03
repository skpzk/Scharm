#ifndef SCHARM_H
#define SCHARM_H

#include <iostream>

using namespace::std;

class Scharm{
    public:
        Scharm();
        void close();
        int test = 12;

        static void callback(void*, float);
};


#endif // SCHARM_H