#include <math.h>
#include "quantize.h"
#include <stdio.h>

void quantizeFloat(float* freq, int quantizeValue){
  float note;
  int notes8ET[8] = {0, 2, 4, 5, 7, 9, 11, 12};
  double ratios8JI[8] = {1, 9./8, 5./4, 4./3, 3./2, 5./3, 15./8, 2};
  double ratios12JI[13] = {1., 16./15, 9./8, 6./5, 5./4, 4./3, 45./32, 3./2, 8./5, 5./3, 9./5, 15./8, 2};
  int position, scaled;
  float baseA, scaledFreq;
  switch(quantizeValue){
    case 0: // 12ET
      // convert freq to midi
      note = (log(*freq/440) / log(pow(2.,1./12)));
      // note is rounded to an int : 12 tone ET
      // the - 0.00001 is here to correct rounding error :
      // a note of X.50002 is rounded to X
      *freq = 440 * pow(pow(2.,1./12), (int) round(note - 0.00001));

      break;
    case 1: // 8ET
      note = (log(*freq/440) / log(pow(2.,1./12)));
      note = round(note - 0.00001);
      position = ((int) note) %12;
      position = (position <0? position + 12:position);
      scaled = 0;
      for (int i=0; i<7; i++){
        if((position>=notes8ET[i]) && (position<notes8ET[i+1])){
          scaled = notes8ET[i];
          break;
        }
      }
      scaled = note - position + scaled;
      *freq = 440 * pow(pow(2.,1./12), scaled);

      // for(int i = -20; i < 24; i++){
      //   printf("i = %i, i%12 = %i\n", i, i%12);
      // }
      break;
    
    case 2:
    // find closest A
      baseA = 440 * pow(2, floor(log(*freq / 440) / log(2)));
      scaledFreq = baseA*2;

      for(int i=0; i<12; i++){
        if(fabs(*freq - ratios12JI[i] * baseA) <= fabs(*freq - ratios12JI[i+1] * baseA)){

					scaledFreq = ratios12JI[i] * baseA;

					break;
        }
      }

      *freq = scaledFreq;
      break;
    case 3:
      // find closest A
      baseA = 440 * pow(2, floor(log(*freq / 440) / log(2)));
      scaledFreq = baseA*2;

      for(int i=0; i<7; i++){
        if(fabs(*freq - ratios8JI[i] * baseA) <= fabs(*freq - ratios8JI[i+1] * baseA)){

					scaledFreq = ratios8JI[i] * baseA;

					break;
        }
      }

      *freq = scaledFreq;
      break;

    default:
      break;
  }

}