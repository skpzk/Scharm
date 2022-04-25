#include "audioObject.h"

AudioObject::AudioObject(){
    // AudioObject(0,0);
    defaultValue = 0;
}

AudioObject::AudioObject(sample_t df){
    // AudioObject(0,0);
    defaultValue = df;
}

void AudioObject::setDefaultValue(sample_t def){
    defaultValue = def;
}

void AudioObject::setCVInput(int index, PpIn* ppIn){
    if(CVinputs.size()>index)
        CVinputs[index] = ppIn;
}

void AudioObject::setNumberOfCVInputs(int n){
  for(int i=0;i<3;i++){
    CVinputs.push_back(nullptr);
  }
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

ArbitraryAudioObject::ArbitraryAudioObject(){}

void ArbitraryAudioObject::altOutput(void* outBuffer){
    sample_t* out = (sample_t*) outBuffer;
    for(int i = 0; i<FRAMES_PER_BUFFER; i++){
    // std::cout << "alt output called" << std::endl;
    *out++ = (sample_t) data.at(i);
    *out++ = (sample_t) data.at(i);
    }
}
