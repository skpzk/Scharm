#ifndef OLD_OTHERFILTERS_H
#define OLD_OTHERFILTERS_H

#include "../audioObject.h"
#include "../audioDetector.h"

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


#endif /* OLD_OTHERFILTERS_H */
