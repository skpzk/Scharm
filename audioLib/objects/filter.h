#ifndef OBJECTS_FILTER_H
#define OBJECTS_FILTER_H

// #include <vector>

#include "audioObject.h"
#include "stateKeys.h"
#include "audioDetector.h"

typedef struct{
	float b0, b1, b2, a1, a2;
}BiquadCoefs;

enum FilterType { moog, biquad };

class Vcf;


class BiquadState{
  public:
    //BiquadState();
    void update(float, float);
    float xn1=0, xn2=0, yn1=0, yn2=0;
};

class BiquadFilter : public AudioObject{
  public:
    void output(void*);

    void setFc(float);
    void setMidiFc(float);

    // lpf
    void setQ(float);
    void setMidiQ(float);

    // void setFilterType(FilterType);
    BiquadFilter();

    void setInput(AudioObject*);

  private:
    double fc, maxFc, T;
    float Q, Qinv, BW;
    BiquadCoefs coefs;
    BiquadState state;
    void computeCoefs();

    void updateQ(); //lpf
    void updateBW(); //bpf
    // FilterType filterType;
    // bool has_input=false;

    

    AudioObject* input;
    AudioDetector peakLimiter;
  
  protected:
    void filter(void*);

  friend Vcf;
};



class MFilter : public AudioObject{
  public:
    void output(void*);

    void setFc(float);
    void setMidiFc(float);

    // lpf
    void setQ(float);
    void setMidiQ(float);
    void setK(float);

    // void setFilterType(FilterType);
    MFilter();

    void setInput(AudioObject*);

    void reset();
  private:
    double fc, maxFc, T;
    double Q;

    double g1, g2, g3, g0;
    double s1, s2, s3, s4;
    double y11, y21, y31, y41;
    double x11, x21, x31, x41;
    double G, K, alpha0, a0, a1, b1;

    void computeCoefs();

    void updateQ(); //lpf

    AudioObject* input;

    AudioDetector peakLimiter;
  
  protected:
    void filter(void*);

  friend Vcf;
};

class OnFilter : public AudioObject{
  public:
    void output(void*);

    void setFc(float);

    // lpf
    void setQ(float);

    // void setFilterType(FilterType);
    OnFilter();

    void setInput(AudioObject*);
  protected:
    double fc, maxFc, T;


    double a0, y0=0;
    

    void computeCoefs();

    AudioObject* input;
  
    void filter(void*);
};

class O1Filter : public OnFilter{
  public:
    O1Filter();
    void output(void*);
  protected:
    void filter(void*);
};

class O2Filter : public OnFilter{
  public:
    O2Filter();
    void output(void*);
  protected:
    void filter(void*);
    double y1=0;
};

class O4Filter : public OnFilter{
  public:
    O4Filter();
    void output(void*);
  protected:
    void filter(void*);
    double y1=0;
    double y2=0;
    double y3=0;
};

class O4FdbFilter : public OnFilter{
  public:
    O4FdbFilter();
    void output(void*);
  protected:
    void filter(void*);
    double y1=0;
    double y2=0;
    double y3=0;
};

// class Vcf : public BiquadFilter{
class Vcf : public AudioObject{
  public:
    Vcf();
    void output(void*);
    StateKeys stateKeys;
    void setInput(AudioObject*);
  private:
    MFilter mFilter;
    BiquadFilter bqFilter;
    void checkValues();
    float knobCutoff, knobReso;

    FilterType fType;

    
};



#endif /* OBJECTS_FILTER_H */
