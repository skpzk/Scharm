#ifndef RHYTHM_H
#define RHYTHM_H

#include <iostream>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDial>
#include <QString>

#include "section.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/knob.h"
#include "../window.h"

using namespace::std;

QWidget *createRhythmSection(Window*);

#endif // RHYTHM_H