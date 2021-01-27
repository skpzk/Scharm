#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QDial>

#include "section.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/knob.h"
#include "../window.h"

using namespace::std;

QWidget *createFilterSection(Window*);

#endif // FILTER_H