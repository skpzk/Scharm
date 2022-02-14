#include "Wave.h"

// WAVE CLASS

Wave::Wave(int type) {
    switch (type) {
    case 0: //sine wave
        this->sine();
        break;

    case 1: //tri wave : created with a sqr wave, integrated by the oscillator
        this->square();
        break;

    case 2: //saw wave
        this->saw();
        break;

    case 3: //this is used by the main VCO, but not the subs, 
        this->square();
        break;

    case 4: //square wave
        this->square();
        break;

    case -1: //custom wave
        initBuffer(wave, FRAMES_PER_BUFFER, 0);
        break;
    }
}

Wave::Wave()
{
    this->sine();
}

void Wave::sine(){
  for(int i=0; i<TABLE_SIZE; i++ )
  {
      this->wave[i] = SILENCE + (sample_t) (this->maxValue * sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2.));
  }
}

void Wave::square(){
  int middle = (int) (TABLE_SIZE / 2);
  for(int i=0; i<middle; i++ )
  {
      this->wave[i] = SILENCE + (sample_t) (this->maxValue);
  }
  for(int i=middle; i<TABLE_SIZE; i++ )
  {
      this->wave[i] = SILENCE - (sample_t) (this->maxValue);
  }
}

void Wave::tri(){
  int middle = (int) (TABLE_SIZE / 2);
  for(int i=0; i<middle; i++ )
  {
      this->wave[i] = SILENCE + (sample_t) (this->maxValue - 2. * this->maxValue
        * (float) i / middle);
  }
  for(int i=middle; i<TABLE_SIZE; i++ )
  {
      this->wave[i] = SILENCE + (sample_t) (- this->maxValue + 2. * this->maxValue
        * (float) (i - middle) / middle);
  }
}

void Wave::saw(){
  for(int i=0; i<TABLE_SIZE; i++ )
  {
      this->wave[i] = SILENCE + (sample_t) (-this->maxValue + 2. * this->maxValue
        * (float) i / TABLE_SIZE);
  }
}


// WAVETABLE CLASS

WaveShape::WaveShape()
{
    this->numWaves = 0;
    this->waveforms = new sample_t* [MAX_TABLE_NUM];
}

void WaveShape::selectWaveShape(int type) {
    switch (type) {
    case 0:
        this->basicShapes();
        break;
    case 1:
        this->organShapes();
        break;
    default:
        this->basicShapes();
        break;
    }
}

void WaveShape::basicShapes()
{
    this->addWave(new Wave(SINE));
    this->addWave(new Wave(SQR));
    this->addWave(new Wave(TRI));
    this->addWave(new Wave(SAW));
}

void sine(int harm, sample_t* wave){
    for(int i=0; i<TABLE_SIZE; i++ )
    {
        sample_t data=0;
        for(int h=0; h<harm; h++){
            //data = (1/h) * sin(h*t*2pi) where t=i/tablesize
            data += (sample_t) (MAX / harm * sin( ((double) h * (double)i/(double)TABLE_SIZE) * M_PI * 2.));
        }
        wave[i] = data;
    }
}

void WaveShape::organShapes()
{
    // cout<<dline()<<"organShapesCalled\n";
    this->addWave(new Wave(SINE));
    this->addWave(new Wave(-1));
    sine(2., this->waveforms[this->numWaves-1]);
    this->addWave(new Wave(-1));
    sine(3., this->waveforms[this->numWaves-1]);
    this->addWave(new Wave(-1));
    sine(4., this->waveforms[this->numWaves-1]);
}

void WaveShape::addWave(Wave* wave)
{
    if (this->numWaves < MAX_TABLE_NUM + 1) {
        this->waveforms[this->numWaves] = wave->wave;
        ++this->numWaves;
    }
    else {
        printf("Maximum number of waveforms exceeded\n");
    }
}