#include "sequencer.h"

#include "../defs.h"
#include "../../../state/state.h"
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
		knob->setKnobType(seq);
		knob->setId(i+1);

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		knob->setMinimum(-127);
		// if titles[i] == "Tempo":
		knob->checkState();
		knob->setStateParamText("seq" + to_string(seqNumber) + "step" + to_string(i+1));
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		State::params("activeseq" + to_string(seqNumber) + "step")->attachCallback((void*) knob, Knob::changeTitleColorCallback);

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	section->vbox->addLayout(hbox);

    return section;
    // return box;
}
