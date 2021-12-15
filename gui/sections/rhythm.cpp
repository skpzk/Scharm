#include "rhythm.h"

#include "../defs.h"
#include "../widgets/button.h"
#include <string>

#include <QLabel>


QWidget *createRhythmSection(Window* parent){
    // cout << "creating sequencer section\n";
	Section *section = new Section(parent, "Rhythm");

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 4; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText("Step" + to_string(i+1));
		knob->setStyleSheet(
			defaultKnobStyleSheet(rhythm));
        knob->setMinimum(1);
        knob->setMaximum(16);

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		// knob.checkState()
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

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
        

        // button.setStateParam(stateParam);
        // button.checkState();

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
