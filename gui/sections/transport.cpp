#include "transport.h"

#include <QPushButton>

#include "../defs.h"


QWidget *createTransportSection(Window* parent){
    cout << "creating transport section\n";
	Section *section = new Section(parent, "Transport");
	string titles[][3] = {{"Reset", "EG", "Next"}, {"Play", "Trigger"}};
    int lenTitles[] = {3, 2};

	QHBoxLayout *hbox1 = new QHBoxLayout;
	QHBoxLayout *hbox2 = new QHBoxLayout;
    QHBoxLayout *hboxes[] = {hbox1, hbox2};


    for(int i=0; i<2; i++){
        for(int j=0; j<lenTitles[i]; j++){
            Button *button = new Button;
            // QPushButton *button = new QPushButton;
            button->setText(titles[i][j]);
            // cout << "title ij = " << titles[i][j] << endl;

            button->setStyleSheet(buttonDefaultStyleSheet());
            button->setCheckable(true);
			string stateParam = titles[i][j];

			// button.setStateParam(stateParam);
			// button.checkState();

			hboxes[i]->addWidget(button);
        }

    }
	


	hbox1->setSpacing(10);
	hbox2->setSpacing(10);
	hbox1->setContentsMargins(5, 10, 5, 10);
	hbox2->setContentsMargins(5, 0, 5, 5);

	section->vbox->addLayout(hbox1);
	section->vbox->addLayout(hbox2);

    return section;
    // return box;
}
