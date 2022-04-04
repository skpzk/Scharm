#include "general.h"

#include "../defs.h"
#include "../../state/state.h"


QWidget *createGeneralSection(Window* parent){
    // cout << "creating general section\n";
	Section *section = new Section(parent, "General");
	string titles[] = {"Vol", "Tempo"};

	QHBoxLayout *hbox = new QHBoxLayout;

	Knob * debugKnob;

	for(int i=0; i < 2; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText(titles[i]);
		knob->setKnobType(general);

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		knob->checkState();
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		hbox->addWidget(knob);

		if(i)
			State::params("activetempo")->attachCallback((void*) knob, Knob::changeTitleColorCallback);
		// debugKnob = knob;
	}


	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	section->vbox->addLayout(hbox);

    return section;
    // return box;
}
