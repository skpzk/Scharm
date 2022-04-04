#include "SignalUtils.h"

float trim(float f, int minvalue, int maxvalue){
    if(f<minvalue){
        return minvalue;
    }else if(f > maxvalue){
        return maxvalue;
    }
    return f;
}

float trim(float f, int maxvalue){
    return trim(f, 0, maxvalue);
}

void initBuffer(sample_t* buffer, int frames, int value){
    for(int i=0; i<(frames); i++){     
        *buffer++ = value;
    }
}

void initBuffer(sample_t* buffer, int value){
    for(int i=0; i<FRAMES_PER_BUFFER; i++){     
        *buffer++ = value;
        *buffer++ = value;
    }
}

void initBuffer(sample_t* buffer){
    initBuffer(buffer, 0);
}

void initBuffer(void* buffer){
    sample_t* buf = (sample_t*) buffer;
    initBuffer(buf, 0);
}