#include "general.h"


QWidget *createGeneralSection(Window* parent){
    cout << "creating general section\n";
	Section *section = new Section(parent, "General");
	string titles[] = {"Vol", "Tempo"};

	QHBoxLayout *hbox = new QHBoxLayout;

	for(int i=0; i < 2; i++){
		// knob = Knob.Knob(3, titles[i])
		QDial *knob = new QDial;
		// knob.setTitle(titles[i]);


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

// def createGeneralSection():
// 	box, vbox = section("General")
// 	titles = ["Vol", "Tempo"]
// 	hbox = Qtw.QHBoxLayout()
// 	for i in range(2):
// 		knob = Knob.Knob(3, titles[i])

// 		knob.setStyleSheet(knobDefaultStyleSheet('general'))
// 		knob.checkState()
// 		if titles[i] == "Tempo":
// 			State.params.setCallback("activetempo", knob.sequencerCallback)

// 		hbox.addWidget(knob)
// 	hbox.setContentsMargins(0, 0, 0, 0)
// 	hbox.setSpacing(0)

// 	vbox.addLayout(hbox)
// 	box.setLayout(vbox)
// 	return box