#ifndef VCOS_H
#define VCOS_H

#include <iostream>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QDial>
#include <QString>

#include "section.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/knob.h"
#include "../window.h"

using namespace::std;

QWidget *createVcoSection(Window*);
QHBoxLayout *firstLine();
QHBoxLayout *secondLine(int*);
QHBoxLayout *thirdLine();
QHBoxLayout *fourthLine();
QHBoxLayout *fifthLine();
QHBoxLayout *sixthLine(int*);
QHBoxLayout *seventhLine();

#endif // VCOS_H