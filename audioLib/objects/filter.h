#ifndef OBJECTS_FILTER_H
#define OBJECTS_FILTER_H

// #include <vector>

#include "audioObject.h"
#include "stateKeys.h"

typedef struct{
	float b0, b1, b2, a1, a2;
}BiquadCoefs;


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
  
  protected:
    void filter(void*);
};

class MFilter : public AudioObject{
  public:
    void output(void*);

    void setFc(float);
    void setMidiFc(float);

    // lpf
    void setQ(float);
    void setMidiQ(float);

    // void setFilterType(FilterType);
    MFilter();

    void setInput(AudioObject*);
  private:
    double fc, maxFc, T;
    double Q;

    double gamma[4];
    double s[4];
    double y[4] = {0, 0, 0, 0};
    double G, K, alpha0, a0;

    void computeCoefs();

    void updateQ(); //lpf

    AudioObject* input;
  
  protected:
    void filter(void*);
};

class O1Filter : public AudioObject{
  public:
    void output(void*);

    void setFc(float);

    // lpf
    void setQ(float);

    // void setFilterType(FilterType);
    O1Filter();

    void setInput(AudioObject*);
  private:
    double fc, maxFc, T;


    double a0, y0=0;
    

    void computeCoefs();

    AudioObject* input;
  
  protected:
    void filter(void*);
};

class Vcf : public O1Filter{
  public:
    Vcf();
    void output(void*);
    StateKeys stateKeys;
  private:
    void checkValues();
    float knobCutoff, knobReso;
};

#endif /* OBJECTS_FILTER_H */
