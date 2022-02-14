#include "filter.h"
#include "../defs.h"


QWidget *createFilterSection(Window* parent){
    // cout << "creating filter section\n";
	Section *section = new Section(parent, "Filter");
	string titles[] = {"Cutoff", "Reso", "EG Amount", "A", "D"};

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 5; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText(titles[i]);
		knob->setStyleSheet(
			defaultKnobStyleSheet(filter));

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		// knob->setStateParamText("cutoff");
		knob->checkState();
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	section->vbox->addLayout(hbox);

    return section;
    // return box;
}
