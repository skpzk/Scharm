#include "window.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>

#include "sections/filter.h"
#include "sections/general.h"
#include "sections/env.h"
#include "sections/transport.h"
#include "sections/sequencer.h"
#include "sections/rhythm.h"
#include "sections/vcos.h"

#include "sections/oscillo.h"

#include "widgets/radio.h"
#include "defs.h"



Window::Window(QWidget *parent) :
QMainWindow(parent)
{
    // printf("Window\n");
    //create filter for events
    this->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(this->createMainSection());

    layout->setStretch(0, 580);

    patchbay = new GuiPatchbay(this);
    // patchbay->checkState();
    layout->addWidget(patchbay);
    layout->setStretch(1, 280);

    oscillo = new OscilloSection(this);
    layout->addWidget(oscillo);
    layout->setStretch(2, 280);

    
    layout->setSpacing(8);

    QWidget *centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    setStyleSheet(mainWindowStyleSheet());

    

    

    this->setWindowTitle("Scharm");
    // path = join("gui", "images", "icon.svg")
    // this->setWindowIcon(QIcon(path));
    // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setFocus();
}

bool Window::eventFilter(QObject *object, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = (QKeyEvent*)ev;

        if(keyEvent->key() == Qt::Key_Q)
        {
            QApplication::quit();
            // std::raise(SIGINT);
        }else{
            // cout << "Key pressed : " << keyEvent->text().toStdString() <<"\n";
        }
    }
    return false;
}

QWidget *Window::createMainSection(){
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(this->createTopControls());
    layout->addWidget(this->createBottomControls());

    layout->setStretch(0, 5);
    layout->setStretch(1, 1);

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    return widget;
}

QWidget *Window::createTopControls(){
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;


    layout->addWidget(this->createSequencerControls());
    layout->addStretch(1);
    layout->addWidget(createVcoSection(this));

    layout->setStretch(0, 8);
    // layout->setStretch(1, 1);
    layout->setStretch(2, 8);

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    return widget;
}

QWidget *Window::createBottomControls(){
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(createGeneralSection(this));
    layout->addWidget(createFilterSection(this));

    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);

    layout->setStretch(0, 2);
    layout->setStretch(1, 5);


    return widget;
}

void Window::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    
    QList<Radio *> widgets = this->findChildren<Radio *>("");
    QList<QWidget *> groupWidgets = this->findChildren<QWidget *>("quantizeGroup");
    int lenList = widgets.size();

    float sizes[2];

    Radio::computeSizeFromContainingWidgetSize(groupWidgets[0]->width(), groupWidgets[0]->height()/7, sizes);
      
    float minsize = sizes[0], fontsize = sizes[1];

    if(minsize > 0){
        
        for(int i=0; i<lenList; i++){
            if(widgets[i]->paintSize != minsize){
                widgets[i]->paintSize = minsize;
                widgets[i]->repaint();
            }
        }
        QList<QLabel *> labels = this->findChildren<QLabel *>("");
        int lenLabels = labels.size();


        for(int j=0; j<lenLabels; j++){
            if(labels[j]->text() == "Range" || labels[j]->text() == "Quantize"){
                
                if(fontsize != 0 && (labels[j]->font().pointSizeF() != fontsize)){
                    QFont font = labels[j]->font();
                    font.setPointSizeF(fontsize);
                    labels[j]->setFont(font);
                    labels[j]->repaint();
                }
            }
        }
    }
    // event->accept();*/
}

QWidget *Window::createSequencerControls(){
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(createSequencerSection(this, 1));
    layout->addWidget(createSequencerSection(this, 2));
    layout->addWidget(createRhythmSection(this));

    QHBoxLayout *hbox = new QHBoxLayout;

    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->addWidget(createTransportSection(this));
    hbox->addWidget(createEnvSection(this));
    hbox->setStretch(0, 1);
    hbox->setStretch(1, 1);

    layout->addLayout(hbox);

    layout->setContentsMargins(0, 0, 0, 0);
    widget->setContentsMargins(0, 0, 0, 0);
    layout->setStretch(0, 2);
    layout->setStretch(1, 2);
    layout->setStretch(2, 4);
    layout->setStretch(3, 2);

    widget->setLayout(layout);

    return widget;
}
