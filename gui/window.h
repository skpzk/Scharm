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


using namespace::std;

class Window : public QMainWindow
{
    Q_OBJECT
    public:
        explicit Window(QWidget *parent = 0);
    private:
        QWidget *createMainSection();
        QWidget *createTopControls();
        QWidget *createBottomControls();
        QWidget *createSequencerControls();
        bool eventFilter(QObject*, QEvent*);
        // int NbDials;

};

#endif // WINDOW_H