#include "audioObject.h"

AudioObject::AudioObject(){
    defaultValue = 0;
}
void AudioObject::output(void* outputBuffer){
    printf("Getting data from AudioObject, something must be wrong\n");
    printf("This method should be overridden by objects that inherit from AudioObject\n");
    sample_t* out = (sample_t*) outputBuffer;
    for(int i=0; i<FRAMES_PER_BUFFER; i++){
        out[i] = defaultValue;
    }
}
void AudioObject::CVOutput(void* outputBuffer){
    printf("Getting data from AudioObject, something must be wrong\n");
    printf("This method should be overridden by objects that inherit from AudioObject\n");
    sample_t* out = (sample_t*) outputBuffer;
    for(int i=0; i<FRAMES_PER_BUFFER; i++){
        out[i] += defaultValue;
    }
}