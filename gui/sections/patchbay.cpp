#include "patchbay.h"
#include "section.h"
#include "../utils.h"

Patchbay::Patchbay(QWidget *parent):
QWidget(parent)
{
    Section *section = new Section(parent, "Rhythm");
}