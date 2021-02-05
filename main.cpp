#include <stdio.h>

#include <QApplication>
#include <QMainWindow>
#include <QSize>

#include "gui/window.h"
#include "scharm.h"

/*
Interface :
 tbd:
  patchcords
  holdbutton

  communication with state
  - warnstate and check state are implemented for knobs

  proper dark mode : 
  - knob text color isn't right

  radios:
  - add mouse press event

  knobs :
  - change statekeys of "stepX"

  waveslider:
  - add mousepressevent


State :
 tbd: 
  patchbay
  radios
  wavesliders

*/

int main(int argc, char **argv) {
    printf("Hello\n");
    QApplication app(argc, argv);

    Scharm scharm;

	Window window;
    window.show();

    window.resize(QSize(950, 620));

	app.exec();

    printf("app.exec closed\n");

    scharm.close();

}