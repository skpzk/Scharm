#ifndef PATCHBAY_H
#define PATCHBAY_H

#include <iostream>
#include <QWidget>
#include <QVBoxLayout>
#include <string>

#include "../widgets/sectionLabel.h"
#include "../widgets/patchpoint.h"
#include "../widgets/patchcordList.h"



class GuiPatchbay : public QWidget{
    
    Q_OBJECT
    public:
        GuiPatchbay(QWidget *parent = 0);
        
        // QWidget *box;
        // QVBoxLayout *vbox;

        std::string displayPp = "all";

        PatchCordList * pcs;
        void createPC(Patchpoint*);
        void moveLastPC(QPointF);
        void findReleasePp(QPointF);
        void disposeOfLastPc();

        void movePC(PatchCord* pac, Patchpoint *pp);

        void checkState();

    private:
        std::vector<Patchpoint *> pps;
        void resizePcs();

        void connect(std::string inPp, std::string outPp);

        std::vector<std::string> titlesIn;


    protected:
        virtual void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent* event);

};

#endif // PATCHBAY_H