
#include "audioDetector.h"
#include "../../audioApi/AudioConstants.h"

#include <cmath>


AudioDetector::AudioDetector(){

  detect_dB = true;
  setAttackTime(5.0);
  setReleaseTime(25.0);
  clampToUnityMax = false;
  detectMode = TLD_AUDIO_DETECT_MODE_PEAK;
  makeUpGain_dB = 0;
  threshold_dB = -3;
  lastEnvelope = 0;

}

void AudioDetector::setAttackTime(double attack_in_ms)
{
  if (attackTime_mSec == attack_in_ms)
    return;
  attackTime_mSec = attack_in_ms;
  attackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_in_ms *
    SAMPLE_RATE * 0.001));
}

void AudioDetector::setReleaseTime(double release_in_ms)
{
  if (releaseTime_mSec == release_in_ms)
    return;
  releaseTime_mSec = release_in_ms;
  releaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_in_ms *
    SAMPLE_RATE * 0.001));
}


double AudioDetector::processAudioSample(double xn){
  return pow(10.0, (makeUpGain_dB/ 20.0)) * xn * computeGain(processSample(xn));
}

double AudioDetector::processSample(double xn)  {
  // --- all modes do Full Wave Rectification
  double input = fabs(xn);
  // --- square it for MS and RMS
  if (detectMode == TLD_AUDIO_DETECT_MODE_MS ||
      detectMode == TLD_AUDIO_DETECT_MODE_RMS)
    input *= input;
  // --- to store current
  double currEnvelope = 0.0;
  // --- do the RC simulator detection with attack or release
  if (input > lastEnvelope)
    currEnvelope = attackTime*(lastEnvelope-input) + input;
  else
    currEnvelope = releaseTime*(lastEnvelope-input) + input;

  // --- we are recursive so need to check underflow
  // checkFloatUnderflow(currEnvelope);

  // --- bound them if desired
  if (clampToUnityMax)
    currEnvelope = fmin(currEnvelope, 1.0);

  // --- can not be (-)
  currEnvelope = fmax(currEnvelope, 0.0);

  // --- store envelope prior to sqrt for RMS version
  lastEnvelope = currEnvelope;

  // --- if RMS, do the SQRT
  if (detectMode == TLD_AUDIO_DETECT_MODE_RMS)
    currEnvelope = pow(currEnvelope, 0.5);
  // --- if not dB, we are done
  if (!detect_dB)
    return currEnvelope;
  // --- setup for log( )
  if (currEnvelope <= 0) return -96.0;

  // --- true log output in dB, can go above 0dBFS!
  return 20.0*log10(currEnvelope);
}

double AudioDetector::computeGain(double detect_dB){
  double output_dB = 0.0;

  // --- defaults - you can change these here
  bool softknee = true;
  double kneeWidth_dB = 10.0;

  // --- hard knee
  if (!softknee)
  {
    // --- below threshold, unity
    if (detect_dB <= threshold_dB)
      output_dB = detect_dB;
    // --- above threshold, compress
    else
      output_dB = threshold_dB;
  }
  else
  {
    // --- calc gain with knee
    // --- left side of knee, outside of width, unity gain zone
    if (2.0*(detect_dB - threshold_dB) < -kneeWidth_dB)
      output_dB = detect_dB;
    // --- inside the knee,
    else if (2.0*(fabs(detect_dB - threshold_dB)) <= kneeWidth_dB)
      output_dB = detect_dB - pow((detect_dB - threshold_dB + (kneeWidth_dB / 2.0)), 2.0) / (2.0*kneeWidth_dB);
    // --- right of knee, compression zone
    else if (2.0*(detect_dB - threshold_dB) > kneeWidth_dB)
      output_dB = threshold_dB;
  }

  // --- convert difference between threshold and detected to raw
  return  pow(10.0, (output_dB - detect_dB) / 20.0);
}