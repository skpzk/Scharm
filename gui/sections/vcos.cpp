#include "vcos.h"

#include "../defs.h"
#include "../utils.h"
#include "../widgets/waveSlider.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/radio.h"


#include <string>

QHBoxLayout *firstLine(){
    QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	hbox->addWidget(new SectionLabel(nullptr, "Vco1"));
	hbox->addWidget(new SectionLabel(nullptr, "Vco2"));

    return hbox;
}

QHBoxLayout *secondLine(){
    QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

    WaveSlider *ws1 = new WaveSlider(5, 0);
    WaveSlider *ws2 = new WaveSlider(5, 1);

	hbox->addWidget(ws1);

    Knob *knob0 = new Knob;
    knob0->setText("Vco1");
    knob0->setStyleSheet(defaultKnobStyleSheet(vco1));

    hbox->addWidget(knob0);

    Knob *knob1 = new Knob;
    knob1->setText("Vco2");
    knob1->setStyleSheet(defaultKnobStyleSheet(vco2));

    hbox->addWidget(knob1);

	hbox->addWidget(ws2);

    return hbox;
}


QWidget *createVcoSection(Window* parent){
    // cout << "creating sequencer section\n";
	QWidget *widget = new QWidget;

	QVBoxLayout *vbox = new QVBoxLayout;

    layoutConf(vbox);

    vbox->addLayout(firstLine());

    vbox->addLayout(secondLine());

    widget->setLayout(vbox);
    return widget;

}
