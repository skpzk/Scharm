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

  proper dark mode

State :
 tbd: 
  everything

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