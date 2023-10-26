#ifndef OBJECTS_OSC_H
#define OBJECTS_OSC_H

#include <iostream>
#include <cmath>

#include "audioObject.h"
#include "Wave.h"
#include "../../audioApi/AudioConstants.h"
#include "stateKeys.h"

#include "patchbay.h"


class Osc : public AudioObject{
	public:
		Osc();

		void output(void*);
		// void CVOutput(void*);

		void setFreq(float);
		void setNoteMidi(float);
		void setWave(int);
		StateKeys stateKeys;
		double poly_blep(double t);

		// void setLocalMaxToOne();
		// void resetLocalMax();

	private:
		// Wavetables sample_t
		Wave wave;
		float phaseIncrement;
		

		float lastOutput;
		// sample_t localMax = MAX;

		float freqArray[FRAMES_PER_BUFFER];
		float getPwmPhaseFromPhase(int);
		


	protected:
		bool isSubOsc;
		float freq;
		float phase;

		float volume;

		int waveType; // selected by the gui, SAW_OR_SQR (3) is sqr with pwm for vco and saw for sub 
		int outputWaveType; // actual output, there is no SAW_OR_SQR possible for this one

		void updatePhaseIncrement();
		void outputWave(void*);

		virtual void updateFreq(int);

		sample_t sequence[2*FRAMES_PER_BUFFER];
		sample_t cvFreq[2*FRAMES_PER_BUFFER];
		sample_t cvDiv[2*FRAMES_PER_BUFFER];
		sample_t cvPwm[2*FRAMES_PER_BUFFER];

		sample_t computedWave[2*FRAMES_PER_BUFFER];

};

enum vcoInputs {vcoIn_vco, vcoIn_sub, vcoIn_pwm, vcoIn_pwm_default};

class Vco : public Osc{
	public:
		Vco();

		void output(void*);
		void CVOutput(void*);
		int getWave();
		float getFreq();
		int getDiv(int);

		void setSequencer(AudioObject*);

		void computeFreq(int);
		

	private:
		void checkValues();

		// gui values :
		float knobFreq = 0;
		float quantizedFreq = 0;
		int quantValue;
		int range;
		
		void updateFreq();
		virtual void updateFreq(int );
		// float getPwmPhaseFromPhase(int);

		


	protected:
		AudioObject * seq;
		bool seqActive;
		void updateWaveType(int);

};

class Sub : public Vco{
	public:
		Sub();

		void output(void*);
		void altOutput(void*);
		void setVco(Vco*);

	private:
		void checkValues();
		// float getPwmPhaseFromPhase(int);

		float knobDiv;
		float div;

		void computeDiv();
		void updateFreq(int );
		Vco * vco;
};

#endif /* OBJECTS_OSC_H */
