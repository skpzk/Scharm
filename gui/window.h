#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>

// for sigint
#include <csignal>
#include <iostream>
#include "sections/patchbay.h"


using namespace::std;

class Window : public QMainWindow
{
    Q_OBJECT
    public:
        explicit Window(QWidget *parent = 0);

        GuiPatchbay *patchbay;
    private:
        QWidget *createMainSection();
        QWidget *createTopControls();
        QWidget *createBottomControls();
        QWidget *createSequencerControls();
        bool eventFilter(QObject*, QEvent*);
        // int NbDials;

        void resizeEvent(QResizeEvent* event);

};

#endif // WINDOW_H