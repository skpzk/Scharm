#include "filter.h"
#include "../defs.h"
#include "../utils.h"
#include "../widgets/radio.h"


QWidget *createFilterSection(Window* parent){
    // cout << "creating filter section\n";
	Section *section = new Section(parent, "Filter");
	string titles[] = {"Cutoff", "Reso", "EG Amount", "A", "D"};

	QHBoxLayout *hbox = new QHBoxLayout;

	QVBoxLayout *vboxfilters = new QVBoxLayout;
	layoutConf(vboxfilters);

	string filters[] = {"Moog", "Biquad"};
	vboxfilters->addStretch(1);

	QWidget *filtersGroup = new QWidget;
	filtersGroup->setContentsMargins(0, 0, 0, 0);

	for(int i=0; i<2; i++){
		Radio *filterRadio = new Radio(QString::fromStdString(filters[i]), i);
		
		// rangeRadio->setStateParam('rangeradio');
		filterRadio->setStateParam("filtersradio");
		// rangeRadio->stateValue = "r" + str(ranges[i]);
		filterRadio->checkState();

		vboxfilters->addWidget(filterRadio);
		// rangeGroup.addButton(rangeRadio)
		vboxfilters->setStretch(i +1, 1);
	}

	vboxfilters->addStretch(1);
	filtersGroup->setLayout(vboxfilters);
	hbox->addWidget(filtersGroup);

	

	for(int i=0; i < 5; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText(titles[i]);
		knob->setKnobType(filter);

		if(titles[i] == "EG Amount"){
			knob->setMinimum(-127);
		}

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		if(titles[i] == "A" || titles[i] == "D"){
			knob->setStateParamText("filter" + titles[i]);
		}
		knob->checkState();
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	for(int i=0; i<6; i++){
		hbox->setStretch(i, 1);
	}

	section->vbox->addLayout(hbox);

	return section;
	// return box;
}
