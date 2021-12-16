#include <stdio.h>

#include <QApplication>
#include <QMainWindow>
#include <QSize>

#include "gui/window.h"
#include "scharm.h"

#include "utils/logger.h"

/*
Last thing I was doing :
  interface :
  + implementing correct colors on knobs
  + waveslider behaves incorrectly :
    + mouse event are detected on a height equivalent of that of the VCO1 knob instead of the painted area
    + mouse event for the right ws are detected on the symbols, not the selector
  Logger :
    + finished testing singleton design pattern
    + ISO C++ forbids converting a string constant to ‘char*’ ?wut? 
      -> solved by adding 'const' to char* msg
    - update every logger function accordingly


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

  knobs :
  - change statekeys of "stepX"

  waveslider:
  + add mousepressevent


State :
 tbd: 
  patchbay
  radios
  wavesliders

*/

/*
Improvement ideas:
  - wavesliders could be continuous instead of discrete !

*/

// for sleep function :
#include <unistd.h>

int main(int argc, char **argv) {
    printf("Hello\n");

    // Logger *logger = Logger::getInstance();

    // logger->info("testing logger");

    // (*logger)<<"test";

    // cout << logger << "test";

    LoggerInterface logger;

    logger.info("test logger");
    

    // Logger::test();
    // QApplication app(argc, argv);

    // Scharm scharm;

	// Window window;
    // window.show();

    // window.resize(QSize(950, 620));

	// app.exec();

    // printf("app.exec closed\n");

    // scharm.close();

}