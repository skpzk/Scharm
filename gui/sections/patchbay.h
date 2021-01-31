#ifndef PATCHBAY_H
#define PATCHBAY_H

#include <iostream>
#include <QWidget>
#include <QVBoxLayout>
#include <string>

#include "../widgets/sectionLabel.h"
#include "../widgets/patchpoint.h"

using namespace::std;

class Patchbay : public QWidget{
    
    Q_OBJECT
    public:
        explicit Patchbay(QWidget *parent = 0);
        
        // QWidget *box;
        // QVBoxLayout *vbox;

        string displayPp = "all";

    private:
        vector<Patchpoint *> pps;
        

};

#endif // PATCHBAY_H