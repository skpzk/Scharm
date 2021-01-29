#ifndef SEQUENCER_H
#define SEQUENCER_H

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

QWidget *createSequencerSection(Window*, int);

#endif // SEQUENCER_H