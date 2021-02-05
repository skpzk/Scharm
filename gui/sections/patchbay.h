#ifndef PATCHBAY_H
#define PATCHBAY_H

#include <iostream>
#include <QWidget>
#include <QVBoxLayout>
#include <string>

#include "../widgets/sectionLabel.h"
#include "../widgets/patchpoint.h"
#include "../widgets/patchcordList.h"

using namespace::std;

class Patchbay : public QWidget{
    
    Q_OBJECT
    public:
        explicit Patchbay(QWidget *parent = 0);
        
        // QWidget *box;
        // QVBoxLayout *vbox;

        string displayPp = "all";

        PatchCordList * pcs;
        void createPC(Patchpoint*);
        void moveLastPC(QPointF);
        void findReleasePp(QPointF);
        void disposeOfLastPc();

        void movePC(PatchCord* pac, Patchpoint *pp);

    private:
        vector<Patchpoint *> pps;
        void resizePcs();


    protected:
        virtual void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent* event);

};

#endif // PATCHBAY_H