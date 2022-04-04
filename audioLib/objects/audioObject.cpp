#include "audioObject.h"

AudioObject::AudioObject(){
    // AudioObject(0,0);
    defaultValue = 0;
}

void AudioObject::setDefaultValue(sample_t def){
    defaultValue = def;
}

// AudioObject::AudioObject(int nbCVInputs, int nbCVOutputs){
//     defaultValue = 0;
//     this->nbCVInputs = nbCVInputs;
//     this->nbCVOutputs = nbCVOutputs;
// }
void AudioObject::output(void* outputBuffer){
    printf("Getting data from AudioObject, something must be wrong\n");
    printf("This method should be overridden by objects that inherit from AudioObject\n");
    sample_t* out = (sample_t*) outputBuffer;
    for(int i=0; i<FRAMES_PER_BUFFER*2; i++){
        out[i] = defaultValue;
    }
}
void AudioObject::CVOutput(void* outputBuffer){
    printf("Getting data from AudioObject, something must be wrong\n");
    printf("This method should be overridden by objects that inherit from AudioObject\n");
    sample_t* out = (sample_t*) outputBuffer;
    for(int i=0; i<FRAMES_PER_BUFFER*2; i++){
        out[i] += defaultValue;
    }
}

void AudioObject::altOutput(void* outputBuffer){
    output(outputBuffer);
}

void AudioObject::update(){
    printf("Updating AudioObject, something must be wrong\n");
}

ConstantAudioObject::ConstantAudioObject(){
    ;
}

void ConstantAudioObject::output(void* outputBuffer){
    sample_t* out = (sample_t*) outputBuffer;
    for(int i=0; i<FRAMES_PER_BUFFER*2; i++){
        out[i] = defaultValue;
    }
}

void ConstantAudioObject::CVOutput(void* outputBuffer){
    output(outputBuffer);
}
