#include "patchbay.h"
#include "section.h"
#include "../utils.h"

#include "../widgets/patchcord.h"



#include <QGridLayout>
#include <QString>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QSize>

#include <vector>

Patchbay::Patchbay(QWidget *parent):
QWidget(parent)
{
    Section *section = new Section(parent, "Patchbay  ");

    this->setLayout(section->vbox);

    QGridLayout *grid = new QGridLayout;

    vector<string> titlesIn = {"VCO 1", "VCO 1 SUB", "VCO 1 PWM",
                "VCA", "VCO 2", "VCO 2 SUB", "VCO 2 PWM",
                "CUTOFF", "PLAY", "RESET", "TRIGGER",
                // "CUTOFF", "RESET", "TRIGGER",
                "RHYTHM 1", "RHYTHM 2", "RHYTHM 3", "RHYTHM 4",
                "CLOCK",
                "SEQ 1", "SEQ 2"};

    vector<string> titlesOut = {"VCA", "VCO 1", "VCO 1 SUB 1", "VCO 1 SUB 2",
                    "VCA EG", "VCO 2", "VCO 2 SUB 1", "VCO 2 SUB 2",
                    "VCF EG",
                    "SEQ 1", "SEQ 1 CLK", "SEQ 2", "SEQ 2 CLK", "NOISE",
                    "CLOCK", "SH", "MIDI CLK", "MIDI"};

    vector<int> titlesIndex ={3, 4,
                        4, 4,
                        4, 1,
                        4, 5,
                        1, 4,
                        2, 0};

    vector<string> titles, types;
    
    for(int i=0; i<(titlesIndex.size() / 2); i++){
        for(int j=0; j<titlesIndex[2 * i]; j++){
            titles.push_back(titlesIn[0]);
            titlesIn.erase(titlesIn.begin());
            types.push_back("in");
        }
        for(int j=0; j<titlesIndex[2 * i + 1]; j++){
            titles.push_back(titlesOut[0]);
            titlesOut.erase(titlesOut.begin());
            types.push_back("out");
        }
    }

    // cout << "titles contains:";
    // for (vector<string>::iterator it = titles.begin(); it != titles.end(); ++it)
    //     cout << ' ' << *it << endl;    
    // cout << '\n';

    int ID;
    for(int i=0; i<4; i++){
        for(int j=0; j<9; j++){
            ID = i + j * 4;
            Patchpoint *pp = new Patchpoint(this, QString::fromStdString(titles[ID]), types[ID]);
            // set stylesheet ?
            grid->addWidget(pp, j, i);
            pps.push_back(pp);
        }
    }

    section->vbox->setStretch(0, 0);
    section->vbox->addLayout(grid, 1);
    /*

    text = """<html><head/><body><p>IN / <span style=" color:%s;background-color:%s">OUT</span></p></body></html>""" %(backgroundColor, pointColor)

    # print("text = ", text)

    fontsize = .38 * 36 / 2
    # print("fontsize = ", fontsize)


    label = Qtw.QLabel(text)
    label.setAlignment(QtCore.Qt.AlignCenter)
    font = label.font()
    font.setPointSizeF(fontsize)
    label.setFont(font)
    vbox.addWidget(label)

    self.setLayout(vbox)

    self.pcs = PatchCordList()

    self.displayPp = "all"  # or "in" or "out"

    self.resizePcs()

    self.checkState()
    */

   this->pcs = new PatchCordList;
    
}


void Patchbay::createPC(Patchpoint* pp){

    cout << "Patchbay :: creating pc...\n";

    PatchCord *pc = new PatchCord(this);

    cout << "setting points \n";
    pc->setStartPp(pp);
    pc->setPos(pp);



    cout << "done\n";

    displayPp = pc->endPoint_io;

    cout << "adding pc to list\n";
    pcs->add(pc);

    cout << "PC created\n";
}

void Patchbay::moveLastPC(QPointF pos){
    pcs->last()->setPos(pos);
    // # print(type(pos))
    repaint();
}

void Patchbay::disposeOfLastPc(){
    PatchCord *pc = pcs->last();
    pcs->remove(pc);
    pc->deleteFromPpLists();
    pc->deleteLater();
    displayPp = "all";
}

void Patchbay::findReleasePp(QPointF pos){

    bool isPpFound = false;
    Patchpoint *pprelease = nullptr;

    for(int i=0; i<pps.size(); i++){
        tie(isPpFound, pprelease) = pps[i]->isMouseReleaseOnPp(pos);
        if(isPpFound){
            break;
        }
    }

    if(!isPpFound){
        disposeOfLastPc();
    }else{
        if(pprelease->ioType != pcs->last()->endPoint_io){
            disposeOfLastPc();
        }else{
            pcs->last()->setEndPp(pprelease);
            pcs->last()->connectPc();

            displayPp = "all";
        }

    }
    repaint();
}

void Patchbay::movePC(PatchCord* pc, Patchpoint *pp){
    pcs->add(pc);
    pcs->last()->disconnectPc(pp);
    displayPp = pcs->last()->endPoint_io;
    repaint();
}

void Patchbay::resizeEvent(QResizeEvent* event){
    resizePcs();
}

void Patchbay::resizePcs(){
    for(int i=0; i<pcs->pcs.size(); i++){
        // cout << "Patchbay : pc[" << i << "]->isHovered = " << pcs->pcs[i]->isHovered << endl;
        // cout << "No bug so far" << endl;
        // cout << "size = " << size().width() << ", " << size().height() << endl;
        pcs->pcs[i]->resize(this->size());
    }
}

void Patchbay::paintEvent(QPaintEvent *ev){

    QStyleOption opt = QStyleOption();
    opt.initFrom(this);
    QPainter painter(this);

    // painter.setBackgroundMode(Qt.TransparentMode)
    QStyle *s = this->style();

    s->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    resizePcs();
}