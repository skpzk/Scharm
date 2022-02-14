#ifndef signalutils_h_
#define signalutils_h_

#include "../../audioApi/AudioConstants.h"

//to be implemented
//sample_t *trim(sample_t*, int, int);
//sample_t *trim(sample_t*, int, int, int);

float trim(float, int, int);
float trim(float, int);
void initBuffer(sample_t* buffer, int frames, int value);
 


#endif