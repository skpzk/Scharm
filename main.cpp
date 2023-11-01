#include <stdio.h>

/*
Last thing I was doing :
  audioLib: 
  + creating vcos
  + figuring out how to implement CVInputs
  + testing load : works with 500 oscs
  + write an extensive doc for params
  + correcting range for osc : can it have a quarter tone scale by default (no quantize)
  + adding check wavetype in vco, switching wavetype
  + adding sub oscs
    todo: change behaviour of quantize ET to mimic that of quantize JI
  + adding a filter
  + integrate a moog filter, based on the pdf, see filter.cpp
  + patchbay set filter eg output
  + patchbay/filter : set up cutoff input

Interface :
 tbd:
  holdbutton
  
  patchcords:
  + add communication with state

  communication with state
  + warnstate and check state are implemented for knobs

  proper dark mode : 
  + knob text color isn't right

  radios:
  + add mouse press event
  + bug : when max sizing, mouse press hitbox is moved to the left

  knobs :
  - change statekeys of "stepX"
  + bug: knob doesn't follow mouse correctly
  + bug: when clicking on the title, it sets the value, but it should only focus on the knob
  - when clicking on the value, add an option to set the number via text
  - when pressing shift and moving the knob, switch to float values
  - when pressing ctrl, round to ints

  waveslider:
  + add mousepressevent

  + I should be able to leave the app by pressing q everywhere, this doesn't work currently
    > seems to work now, QComboBox was not sending events to parents
  + oscillo : should check state at startup and not connect to cva by default


State :
 tbd: 
  + patchbay
  + radios
  + wavesliders

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
    Not seen this bug since the above fix.


  - When two osc (typically two subs) have the same frequency, they can have opposite phases, thus cancelling each other
    maybe I could find a way to synchronize their phases on some special conditions, 
    like check if they have the same freq and then phase sync ?
    this is especially noticeable with a square wave

  - sometimes the gui patch cables stop working, ie. they don't follow the moving mouse
    this can cause the app to crash (Segmentation fault (core dumped))
    Steps to reproduce : play with the patch cables for a while.

  + bug with rhythmic section :
    steps to reproduce : 
    - erase the contents of state.scharm
    - play, tempo to some value
    - assign one rhythm to a sequencer : it runs backwards, the vca EG is always on, 
      and the sequence is not applied on the vcos or the subs
    bug disappears when we move the knob for a rhythm
    > the div was set to 0 in RhythmGenerator, 
      fixed by trimming the div value for the RG from state to (1,16)
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
  Scharm scharm;

  QApplication app(argc, argv);


	Window window;
  window.show();

  window.resize(QSize(950, 620));

  window.patchbay->repaint();

  scharm.start();

	app.exec();

  // printf("app.exec closed\n");

  scharm.close();
  return 0;
}

// #include "design/plotter.h"


#include <vector>

#include "state/state.h"
void testState(){
  State::print();
  State::params("")->setValue(10);
  State::params("plop")->setValue(10);
  State::params("connexions")->setValue(10);
  State::params("connections")->setValue(10);
  State::connections.print();
  State::connections.connect("test", "test");
  State::connections.connect("test", "test1");
  State::connections.connect("test", "test2");
  State::connections.connect("test2", "test");
  State::connections.connect("test2", "test1");
  State::connections.connect("test3", "test2");
  State::connections.connect("", "test2");
  State::connections.connect("test3", "");
  State::connections.print();
  State::connections.disconnect("test3", "test2");
  State::connections.disconnect("test", "test1");
  State::connections.disconnect("", "test2");
  State::connections.disconnect("test", "");
  // State::connections.print();

  State::print();

  State::save();
}

void testReadState(){
  State::read();
  State::print();
}

#include "gui/widgets/patchpoint.h"

void testOverloadingEquality(int argc, char **argv){

  // Must construct a QApplication before a QWidget
  QApplication app(argc, argv);

  Patchpoint * a, *b;

  a = new Patchpoint(nullptr, "testa", "in");
  b = new Patchpoint(nullptr, "testb", "in");

  cout << "a==b : " << (*a==*b) << endl;

  b = new Patchpoint(nullptr, "testa", "out");
  cout << "a==b : " << (*a==*b) << endl;
  
  b = new Patchpoint(nullptr, "testa", "in");
  cout << "a==b : " << (*a==*b) << endl;
  
}
#include "design/plotter.h"

int main(int argc, char **argv) {
  app(argc, argv);
  // debug_plotter_spectrum();
  // debug_plotter_env();
  // plotEnv();
  // plotClock();
  // testPp();

  // testState();
  // testReadState();

  // testOverloadingEquality(argc, argv);
  
  return 0;
}