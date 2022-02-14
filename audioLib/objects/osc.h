#ifndef OSC_H_
#define OSC_H_

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
		void CVOutput(void*);

		void setFreq(float);
		void setNoteMidi(float);
		void setWave(float);
		StateKeys stateKeys;

	private:
		// Wavetables sample_t
		Wave wave;
		float phaseIncrement;
		float phase;

	protected:
		int vco_or_sub;// 0 is for vco, 1 is for sub
		float freq;
		float volume;
		void updatePhaseIncrement();
		void outputWave(void*);

};

class Vco : public Osc{
	public:
		Vco();

		void output(void*);
		int getWave();
		float getFreq();

	private:
		void checkValues();

		// gui values :
		float knobFreq = 0;
		int quantValue;
		int waveType;
		void updateFreq();

	protected:
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
		Vco * vco;
};

#endif // OSC_H_