#include <stdio.h>

#include <QApplication>
#include <QMainWindow>

#include "gui/window.h"
#include "scharm.h"

int main(int argc, char **argv) {
    printf("Hello\n");
    QApplication app(argc, argv);

	Window window;
    window.show();

    Scharm scharm;

	app.exec();

    printf("app.exec closed\n");

    scharm.close();

}