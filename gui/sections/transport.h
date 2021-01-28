#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>
#include <QGroupBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QDial>

#include "section.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/button.h"
#include "../window.h"

using namespace::std;

QWidget *createTransportSection(Window*);

#endif // TRANSPORT_H