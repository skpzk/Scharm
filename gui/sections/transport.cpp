#include "transport.h"

#include <QPushButton>

#include "../defs.h"


QWidget *createTransportSection(Window* parent){
    // cout << "creating transport section\n";
	Section *section = new Section(parent, "Transport");
	string titles[][3] = {{"Reset", "EG", "Next"}, {"Play", "Trigger"}};
	int lenTitles[] = {3, 2};

	QHBoxLayout *hbox1 = new QHBoxLayout;
	QHBoxLayout *hbox2 = new QHBoxLayout;
	QHBoxLayout *hboxes[] = {hbox1, hbox2};

	string title;


	for(int i=0; i<2; i++){
		for(int j=0; j<lenTitles[i]; j++){

			title = titles[i][j];
			Button *button = new Button;
			// QPushButton *button = new QPushButton;
			button->setText(title);
			// cout << "title ij = " << titles[i][j] << endl;
			button->setStyleSheet(buttonDefaultStyleSheet());

			if(title!="Reset" && title!="Trigger" && title!="Next")
			button->setCheckable(true);

			if(title == "EG")
			button->setHoldable(true);

			button->checkState();
			// string stateParam = title;

			// button.setStateParam(stateParam);
			// button.checkState();

			hboxes[i]->addWidget(button);
		}

	}
	


	hbox1->setSpacing(10);
	hbox2->setSpacing(10);
	hbox1->setContentsMargins(5, 10, 5, 5);
	hbox2->setContentsMargins(5, 5, 5, 10);

	section->vbox->addLayout(hbox1);
	section->vbox->addLayout(hbox2);
	section->vbox->setStretch(0, 0);
	section->vbox->setStretch(1, 2);
	section->vbox->setStretch(2, 2);

	return section;
	// return box;
}
