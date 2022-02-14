#include "midi.h"

// Converts midi note number to frequency (A 440Hz -> midi note 69)

float ftom(float f){
	float m = 69 + log(f/440.)/log(pow(2, 1./12));
	return m;
}

float mtof(float m){
	float f = 440. * pow(pow(2, 1./12), m - 69);
	return f;
}