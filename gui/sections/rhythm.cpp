#include "rhythm.h"

#include "../defs.h"
#include "../widgets/button.h"
#include "../../state/state.h"
#include <string>

#include <QLabel>

#include <iostream>


QWidget *createRhythmSection(Window* parent){
    // cout << "creating sequencer section\n";
	Section *section = new Section(parent, "Rhythm");

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 4; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		// std::cout << "state::clk" << i+1 << " = " << *State::params("clk" + to_string(i+1)) << std::endl;
		Knob *knob = new Knob;
		knob->setText("Clk" + to_string(i+1));
		knob->setKnobType(rhythm);
		

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		// knob.checkState()
		// std::cout << "state::clk" << i+1 << " = " << *State::params("clk" + to_string(i+1)) << std::endl;
		// std::cout << "checking state\n";
		knob->checkState();
		// std::cout << "state::clk" << i+1 << " = " << *State::params("clk" + to_string(i+1)) << std::endl;
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)
		knob->setMinimum(1);
		knob->setMaximum(16);
		// std::cout << "callback key for clock " << i+1 << " activerhythm" + to_string(i+1) << std::endl;

		State::params("activerhythm" + to_string(i+1))->attachCallback((void*) knob, Knob::changeTitleColorCallback);

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

    QGridLayout *grid = new QGridLayout;

    string title;
    for(int i=0; i<8; i++){
        title = "Seq" + to_string(i/4 + 1);
        Button *button = new Button;
        
        button->setText(title);
        

        button->setStyleSheet(buttonDefaultStyleSheet());
        button->setCheckable(true);
        

        button->setStateParamText("seq" + to_string(i/4 + 1) + "clk" + to_string(i%4 + 1));
        // button->checkState();

        grid->addWidget(button, i/4, i%4);
    }

    grid->setContentsMargins(5, 10, 5, 0);
	grid->setSpacing(10);

	section->vbox->addLayout(hbox);
	section->vbox->addLayout(grid);

    QLabel *dummylabel = new QLabel;

	section->vbox->addWidget(dummylabel);


    section->vbox->setStretch(1, 1);
	section->vbox->setStretch(2, 1);

    return section;
    // return box;
}
