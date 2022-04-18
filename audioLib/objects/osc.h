#ifndef OBJECTS_OSC_H
#define OBJECTS_OSC_H

#include <iostream>
#include <cmath>

#include "audioObject.h"
#include "Wave.h"
#include "../../audioApi/AudioConstants.h"
#include "stateKeys.h"


class Osc : public AudioObject{
	public:
		Osc();

		void output(void*);
		// void CVOutput(void*);

		void setFreq(float);
		void setNoteMidi(float);
		void setWave(float);
		StateKeys stateKeys;
		double poly_blep(double t);

		// void setLocalMaxToOne();
		// void resetLocalMax();

	private:
		// Wavetables sample_t
		Wave wave;
		float phaseIncrement;
		float phase;

		float lastOutput;
		// sample_t localMax = MAX;

		float freqArray[FRAMES_PER_BUFFER];
		


	protected:
		int vco_or_sub;// 0 is for vco, 1 is for sub
		float freq;

		float volume;

		int waveType;

		void updatePhaseIncrement();
		void outputWave(void*);

		virtual void updateFreq(int);

		sample_t sequence[2*FRAMES_PER_BUFFER];

};

class Vco : public Osc{
	public:
		Vco();

		void output(void*);
		int getWave();
		float getFreq();

		void setSequencer(AudioObject*);

		void computeFreq(int);
	private:
		void checkValues();

		// gui values :
		float knobFreq = 0;
		int quantValue;
		int range;
		
		void updateFreq();
		virtual void updateFreq(int );


	protected:
		AudioObject * seq;
		bool seqActive;
		void updateWaveType(int);
};

class Sub : public Vco{
	public:
		Sub();

		void output(void*);
		void setVco(Vco*);

	private:
		void checkValues();

		float knobDiv;
		float div;

		void computeDiv();
		void updateFreq(int );
		Vco * vco;
};

#endif /* OBJECTS_OSC_H */
