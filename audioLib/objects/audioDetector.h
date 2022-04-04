#ifndef OBJECTS_AUDIODETECTOR_H
#define OBJECTS_AUDIODETECTOR_H

#include "audioObject.h"

// --- constants
const unsigned int TLD_AUDIO_DETECT_MODE_PEAK = 0;
const unsigned int TLD_AUDIO_DETECT_MODE_MS = 1;
const unsigned int TLD_AUDIO_DETECT_MODE_RMS = 2;
const double TLD_AUDIO_ENVELOPE_ANALOG_TC =
  -0.99967234081320612357829304641019; // ln(36.7%)

class AudioDetector{
  public:
    AudioDetector();


    // void output(void*);
    // void setInput(AudioObject*);

    double processAudioSample(double);

  private:
    double attackTime_mSec = 0.0;
    double releaseTime_mSec = 0.0;
    unsigned int detectMode = TLD_AUDIO_DETECT_MODE_PEAK;
    bool detect_dB = false; /* DEFAULT = false (linear NOT log) */
    bool clampToUnityMax = true;

    double lastEnvelope, attackTime, releaseTime, threshold_dB, makeUpGain_dB;

    void setAttackTime(double attack_in_ms);
    void setReleaseTime(double attack_in_ms);

    double computeGain(double detect_dB);


    double processSample(double);
    
    // void processChunk(sample_t* out, sample_t in);
};

#endif /* OBJECTS_AUDIODETECTOR_H */
