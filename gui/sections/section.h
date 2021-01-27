#ifndef SECTION_H
#define SECTION_H


#include <iostream>
#include <QWidget>
#include <QVBoxLayout>

#include "../widgets/sectionLabel.h"

using namespace::std;

class Section : public QWidget{
    
    Q_OBJECT
    public:
        explicit Section(QWidget *parent = 0, const QString& text="");
        void setTitle(string title);
        // QWidget *box;
        QVBoxLayout *vbox;
        string getTitle();

};

#endif // SECTION_H