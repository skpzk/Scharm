#include "vcos.h"

#include "../defs.h"
#include "../utils.h"
#include "../widgets/waveSlider.h"
#include "../widgets/sectionLabel.h"
#include "../widgets/radio.h"
#include "../widgets/button.h"


#include <string>

QHBoxLayout *firstLine(){
    QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	hbox->addWidget(new SectionLabel(nullptr, "Vco1"));
	hbox->addWidget(new SectionLabel(nullptr, "Vco2"));

    return hbox;
}

QHBoxLayout *secondLine(int *stretches){
	QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	WaveSlider *ws1 = new WaveSlider(5, 0);
	WaveSlider *ws2 = new WaveSlider(5, 1);

	ws1->setStateParamText("vco1Wave");
	ws2->setStateParamText("vco2Wave");

	hbox->addWidget(ws1);
	ws1->checkState();

	Knob *knob0 = new Knob;
	knob0->setText("Vco1");
	knob0->setStyleSheet(defaultKnobStyleSheet(vco1));
	knob0->setStateParamText("Vco1knobfreq");

	hbox->addWidget(knob0);

	// range radio

	QVBoxLayout *vboxrange = new QVBoxLayout;
	layoutConf(vboxrange);

	int ranges[] = {5, 2, 1};
	vboxrange->addStretch(1);

	QWidget *rangeGroup = new QWidget;
	rangeGroup->setContentsMargins(0, 0, 0, 0);

	for(int i=0; i<3; i++){
		Radio *rangeRadio = new Radio(QString::fromStdString("±" + to_string(ranges[i])), i);
		
		rangeRadio->setStateParam("rangeradio");

		rangeRadio->setValue(ranges[i]);

		// rangeRadio->stateValue = "r" + str(ranges[i]);
		rangeRadio->checkState();

		vboxrange->addWidget(rangeRadio);
		// rangeGroup.addButton(rangeRadio)
		vboxrange->setStretch(i +1, 1);
	}
	// rangeRadio.setChecked(True)

	QLabel *label = new QLabel("Range");
	label->setAlignment(Qt::AlignCenter);
	vboxrange->addWidget(label);

	vboxrange->addStretch(1);

	rangeGroup->setLayout(vboxrange);
	// hbox.addLayout(vboxrange)
	hbox->addWidget(rangeGroup);


	Knob *knob1 = new Knob;
	knob1->setText("Vco2");
	knob1->setStyleSheet(defaultKnobStyleSheet(vco2));

	knob1->setStateParamText("Vco2knobfreq");

	hbox->addWidget(knob1);

	hbox->addWidget(ws2);
	ws2->checkState();

	for(int i=0; i<5; i++){
		hbox->setStretch(i, stretches[i]);
	}

	return hbox;
}

QHBoxLayout *thirdLine(){
	QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	string titles[] = {"Sub1", "Sub2"};
	knobType kt[] = {vco1, vco2};

	string stateKeys[] = {"vco1 sub1 div", "vco1 sub2 div", "vco2 sub1 div", "vco2 sub2 div"};

	for(int i=0; i < 4; i++){
		// knob = Knob.Knob(3, titles[i])
		// QDial *knob = new QDial;
		Knob *knob = new Knob;
		knob->setText(titles[i%2]);
		knob->setStyleSheet(
			defaultKnobStyleSheet(kt[i/2]));
		knob->setMinimum(1);
		knob->setMaximum(16);

		knob->setStateParamText(stateKeys[i]);
		knob->checkState();

		// knob.setStyleSheet(knobDefaultStyleSheet('general'))
		// knob.checkState()
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	return hbox;

}

QHBoxLayout *fourthLine(){
    QHBoxLayout *hbox = new QHBoxLayout;


    hbox->setContentsMargins(10, 0, 10, 0);
	hbox->setSpacing(20);

	hbox->addWidget(new SectionLabel(nullptr, "Seq1Assign"));
	hbox->addWidget(new SectionLabel(nullptr, "Seq2Assign"));

	return hbox;

}

QHBoxLayout *fifthLine(){
    QHBoxLayout *hbox = new QHBoxLayout;


    // hbox->setContentsMargins(10, 0, 10, 0);
	// hbox->setSpacing(20);

	// hbox->addWidget(new SectionLabel(nullptr, "Seq1Assign"));
	// hbox->addWidget(new SectionLabel(nullptr, "Seq2Assign"));


	for(int i=0; i<2; i++){
		QWidget *widget = new QWidget;
		
		QHBoxLayout *hbox_i = new QHBoxLayout;
		// layoutConf(hbox_i);

		string titles[] = {"Vco" + to_string(i + 1), "Sub1", "Sub2"};
		for(int ii=0; ii<3; ii++){

			Button *button = new Button;
			button->setText(titles[ii]);
			
			button->setStyleSheet(buttonDefaultStyleSheet());
			button->setCheckable(true);

			button->setStateParamText("seq" + to_string(i+1) + "assignTo" + titles[ii]);


			// stateParam = "seq" + str(i + 1) + "assign" + titles[ii]

			// button.setStateParam(stateParam)
			// button.checkState()

			hbox_i->addWidget(button);
		}
		widget->setLayout(hbox_i);
		// hbox.addLayout(hbox_i)
		hbox->addWidget(widget);
		hbox->setStretch(i, 1);
	}

	return hbox;
}

QHBoxLayout *sixthLine(int *stretches){
    QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	hbox->addStretch(1);

    Knob *knob0 = new Knob;
    knob0->setText("Vco1 level");
    knob0->setStyleSheet(defaultKnobStyleSheet(vco1));

		knob0->checkState();

    hbox->addWidget(knob0);

    // quant radio

    QVBoxLayout *vboxquant = new QVBoxLayout;
	layoutConf(vboxquant);

    
	string quantizes[] = {"12-ET", "8-ET", "12-JI", "8-JI"};
	vboxquant->addStretch(1);

	QWidget *quantGroup = new QWidget;
	quantGroup->setContentsMargins(0, 0, 0, 0);
	quantGroup->setObjectName("quantizeGroup");

	for(int i=0; i<4; i++){
		Radio *quantRadio = new Radio(QString::fromStdString(quantizes[i]), i);
		
		quantRadio->setStateParam("quantradio");
		// quantRadio->stateValue = "r" + str(ranges[i]);
		quantRadio->checkState();

		vboxquant->addWidget(quantRadio);
		// quantGroup.addButton(quantRadio)
		vboxquant->setStretch(i + 1, 1);
	}
	// quantRadio->setChecked(true);

	QLabel *label = new QLabel("Quantize");
	label->setAlignment(Qt::AlignCenter);
	vboxquant->addWidget(label);

	vboxquant->addStretch(1);

	quantGroup->setLayout(vboxquant);
	// hbox.addLayout(vboxquant)
	hbox->addWidget(quantGroup);


	Knob *knob1 = new Knob;
	knob1->setText("Vco2 level");
	knob1->setStyleSheet(defaultKnobStyleSheet(vco2));

	knob1->checkState();

	hbox->addWidget(knob1);

	hbox->addStretch(1);

	for(int i=0; i<5; i++){
		hbox->setStretch(i, stretches[i]);
	}

	return hbox;
}

QHBoxLayout *seventhLine(){
	QHBoxLayout *hbox = new QHBoxLayout;

	layoutConf(hbox);

	string titles[] = {"Sub1 level", "Sub2 level"};
	string stateKeys[] = {"vco1 sub1 level", "vco1 sub2 level", "vco2 sub1 level", "vco2 sub2 level"};
	knobType kt[] = {vco1, vco2};

	for(int i=0; i < 4; i++){
	Knob *knob = new Knob;
	knob->setText(titles[i%2]);
	knob->setStyleSheet(
		defaultKnobStyleSheet(kt[i/2]));

		knob->setStateParamText(stateKeys[i]);
		knob->checkState();

		// knob.checkState()
		// if titles[i] == "Tempo":
		// State.params.setCallback("activetempo", knob.sequencerCallback)

		hbox->addWidget(knob);
	}

	hbox->setContentsMargins(0, 0, 0, 0);
	hbox->setSpacing(0);

	return hbox;

}

QWidget *createVcoSection(Window* parent){
    // cout << "creating sequencer section\n";
	QWidget *widget = new QWidget;

	QVBoxLayout *vbox = new QVBoxLayout;

    int a = 15;
	int b = 20;
	int unit = 100;

	int stretch_slider = a;
	int stretch_radio = 2 * b;
	int stretch_knob = unit - stretch_slider - stretch_radio / 2;

	int stretches[] = {stretch_slider, stretch_knob, stretch_radio, stretch_knob, stretch_slider};
	// cout << stretches << endl;


    layoutConf(vbox);

    vbox->addLayout(firstLine());

    vbox->addLayout(secondLine(stretches));

    vbox->addLayout(thirdLine());

	vbox->addWidget(new QLabel);

    vbox->addLayout(fourthLine());

    vbox->addLayout(fifthLine());

    vbox->addLayout(sixthLine(stretches));

    vbox->addLayout(seventhLine());

	vbox->addWidget(new QLabel);

    widget->setLayout(vbox);

	int mainStretch[] = {1, 6, 4, 1, 1, 2, 6, 4};
	for(int i=0; i < 8; i++){
		vbox->setStretch(i, mainStretch[i]);
	}
    return widget;

}
