#include "sequencer.h"

#include "../defs.h"
#include <string>


QWidget *createSequencerSection(Window* parent, int seqNumber){
    // cout << "creating sequencer section\n";
	Section *section = new Section(parent, QString::fromStdString("Sequencer" + to_string(seqNumber)));

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 4; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText("Step" + to_string(i+1));
		knob->setStyleSheet(
			defaultKnobStyleSheet(seq));
        knob->setMinimum(-127);

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
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
