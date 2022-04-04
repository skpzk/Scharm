#include <stdio.h>

/*
Last thing I was doing :
  audioLib: 
  - creating vcos
  - figuring out how to implement CVInputs
  + testing load : works with 500 oscs
  + write an extensive doc for params
  + correcting range for osc : can it have a quarter tone scale by default (no quantize)
  + adding check wavetype in vco, switching wavetype
  + adding sub oscs
    todo: change behaviour of quantize ET to mimic that of quantize JI
  + adding a filter
  - integrate a moog filter, based on the pdf, see filter.cpp

Interface :
 tbd:
  holdbutton
  
  patchcords:
  - add communication with state

  communication with state
  - warnstate and check state are implemented for knobs

  proper dark mode : 
  + knob text color isn't right

  radios:
  - add mouse press event
  - bug : when max sizing, mouse press hitbox is moved to the left

  knobs :
  - change statekeys of "stepX"
  - bug: knob doesn't follow mouse correctly
  - bug: when clicking on the title, it sets the value, but it should only focus on the knob
  - when clicking on the value, add an option to set the number via text
  - when pressing shift and moving the knob, switch to float values
  - when pressing ctrl, round to ints

  waveslider:
  + add mousepressevent


State :
 tbd: 
  patchbay
  radios
  wavesliders

*/

/*
BUGS :

  - Rarely, this happens :
      Saving data to "state.sharm"... done.
      terminate called after throwing an instance of 'std::bad_alloc'
        what():  std::bad_alloc
      Aborted (core dumped)
    No idea why.
    It may have been because I wasn't stopping the audio before. This is now done by the Scharm instance.


  - When two osc (typically two subs) have the same frequency, they can have opposite phases, thus cancelling each other
    maybe I could find a way to synchronize their phases on some special conditions, 
    like check if they have the same freq and then phase sync ?
    this is especially noticeable with a square wave
*/

/*
Improvement ideas:
  - wavesliders could be continuous instead of discrete !
  - quantize 8ET, 12JI and 8JI : choose the reference note in a menu

*/

// for sleep function :
// #include <unistd.h>

#include <QApplication>
#include <QMainWindow>
#include <QSize>



#include "gui/window.h"
#include "scharm.h"

int app(int argc, char **argv){
  printf("Hello\n");

  QApplication app(argc, argv);

  Scharm scharm;

	Window window;
  window.show();

  window.resize(QSize(950, 620));

	app.exec();

  // printf("app.exec closed\n");

  scharm.close();
  return 1;
}

// #include "design/filter.h"





int main(int argc, char **argv) {
  app(argc, argv);
  // debug_plotter_spectrum();
  
  
  return 1;
}