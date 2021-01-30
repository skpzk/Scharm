#ifndef PATCHBAY_H
#define PATCHBAY_H

#include <iostream>
#include <QWidget>
#include <QVBoxLayout>

#include "../widgets/sectionLabel.h"

using namespace::std;

class Patchbay : public QWidget{
    
    Q_OBJECT
    public:
        explicit Patchbay(QWidget *parent = 0);
        
        // QWidget *box;
        QVBoxLayout *vbox;
        

};

#endif // PATCHBAY_H