#include "env.h"

#include "../defs.h"


QWidget *createEnvSection(Window* parent){
    // cout << "creating env section\n";
	Section *section = new Section(parent, "Env");
	string titles[] = {"A", "D"};

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 2; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText(titles[i]);
		knob->setStyleSheet(
			defaultKnobStyleSheet(env));

		// knob.setStyleSheet(knobDefaultStyleSheet('env'))
		// knob.checkState()
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
