#include "oscillo.h"
#include "section.h"
#include "../utils.h"

#include "../../state/state.h"

#include <QVBoxLayout>

#include <QGraphicsLineItem>
#include <QResizeEvent>

// #include <functional>
#include <stdio.h>
#include <math.h>
#include <iostream>


using namespace::std;

// string Patchpoint::getName(){
// 	string name = text_.toStdString();
// 	lowerWithoutSpaces(&name);
// 	return name;
// }

CustomComboBox::CustomComboBox(QWidget *parent):QComboBox(parent){

}

void CustomComboBox::keyPressEvent(QKeyEvent* e){
    e->ignore(); 
    // send event to parent to be able to quit by pressing 'q'
    this->parentWidget()->eventFilter(this, (QEvent*) e);
}

OscilloSection::OscilloSection(QWidget *parent):
QWidget(parent)
{
    Section *section = new Section(parent, "Oscillo  ");

    // cout << "patchbay instanciated\n";

    this->setLayout(section->vbox);

    //check state
    // checkState();

    scene = new QGraphicsScene(this);
    scene->setSceneRect(-100, -80, 200, 160);

    scene->setBackgroundBrush(Qt::black);

    QColor bgColor = QColor("red");

    QPen pen = QPen(bgColor);
    // QBrush brush = QBrush(QColor("blue"));

    // QGraphicsRectItem *rect = scene->addRect(QRectF(0, 0, 20, 20), pen);

    // QGraphicsItem *item = scene->itemAt(0, 0, QTransform());

    // view = new CustomView(scene);
    view = new QGraphicsView(this);
    view->setContentsMargins(0, 0, 0, 0);
    view->setScene(scene);
    view->fitInView(scene->sceneRect());
    view->show();
    // QSizePolicy *qsp = new QSizePolicy;
    // qsp->setHeightForWidth(true);

    // view->setSizePolicy(*qsp);
    // view->setBaseSize(5, 4);

    // view->setSizePolicy(QSizePolicy::Preferred, *qsp);

    AspectRatioWidget *arwidget = new AspectRatioWidget(view, 5, 4, this);

    plotAxes();

    

    // QGridLayout *grid = new QGridLayout;

    section->vbox->addWidget(arwidget);
    // QLabel *label = new QLabel("TEST");
    // cbox = new QComboBox(this);

    QStringList sources_tmp = {"VCA", "VCO 1", "VCO 1 SUB 1", "VCO 1 SUB 2",
                    "VCA EG", "VCO 2", "VCO 2 SUB 1", "VCO 2 SUB 2",
                    "VCF EG",
                    "SEQ 1", "SEQ 1 CLK", "SEQ 2", "SEQ 2 CLK", "NOISE",
                    "CLOCK", "SH", "MIDI CLK", "MIDI"};
    sources = sources_tmp;

    stateKeys.push_back("oscillo1");
    stateKeys.push_back("oscillo2");

    for(int i=0;i<stateKeys.size();i++){
        CustomComboBox * cbox = new CustomComboBox(this);
        cbox->stateKey = stateKeys.at(i); 
        cbox->index = i;
        cbox->addItems(sources);
        cbox->os = this;
        section->vbox->addWidget(cbox);
        currentInputs.push_back("");
        State::params(stateKeys.at(i))
            ->attachCallback((void*) cbox, 
            &CustomComboBox::receiveSignal);
        connect(cbox, &QComboBox::currentTextChanged, cbox, &CustomComboBox::signalSelection);
        cboxes.push_back(cbox);

        signalsVector.push_back(vector<float> {0});
    }
    cboxes.at(0)->color = QColor("#f8c60d");
    cboxes.at(0)->setStyleSheet("background-color: #111111; color: #f8c60d; selection-background-color: #37373d;");
    cboxes.at(1)->color = QColor("#1e3aa1");
    cboxes.at(1)->setStyleSheet("background-color: #111111; color: #1e3aa1; selection-background-color: #37373d;");



    // State::connections.connect("oscillo", "vco1");
    
    checkState();

    // section->vbox->addLayout(grid, 1);
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    section->vbox->addSpacerItem(verticalSpacer);

    // section->vbox->addWidget(&view);
    section->vbox->setStretch(0, 0);
    // section->vbox->setStretch(1, 1);
    // section->vbox->setStretch(2, 1);
    // section->vbox->setStretch(1, 100);

    connect(this, &OscilloSection::repaintDisplay, this, &OscilloSection::repaintSlot);
}

// CustomView::CustomView(QGraphicsScene *scene):
// QGraphicsView(scene){
// }

// int CustomView::heightForWidth(int w) const{
//     return 4*w/5;
// }

void OscilloSection::checkState(){
    for(int i=0;i<stateKeys.size();i++){
        string stateKey = stateKeys.at(i);
        vector<string> inputsList = State::connections.getConnectionsToPpIn(stateKey);
        if(inputsList.size() > 1){
            for(string input : inputsList){
                State::connections.disconnect(stateKey, input);
            }
            State::connections.connect(stateKey, inputsList.at(0));
        }
        if(inputsList.size() == 0) continue;
        currentInputs.at(i) = inputsList.at(0);
        QString input = "";
        string input_str;
        for(QString source : sources){
            input_str = source.toLower().replace(" ", "").toStdString();
            if(input_str == currentInputs.at(i)){
                input = source;
                break;
            }
        }

        int index = cboxes.at(i)->findText(input);
        if ( index != -1 ) { // -1 for not found
            printf("Index found\n\n");
            cboxes.at(i)->setCurrentIndex(index);
            cboxes.at(i)->setCurrentText(input);
            cboxes.at(i)->update();
        }else{
            printf("Index not found\n\n");
            cout << "currentInput = " << currentInputs.at(0) <<endl;
            cout << "input = " << input.toStdString() << endl;
            cout << "cbox[0] = " << cboxes.at(i)->itemData(0).toString().toStdString() << endl << endl;
        }
    }
    // cbox->setCurrentText(QString(currentInput.c_str()));
}

void CustomComboBox::receiveSignal(void * _cb, vector<float> signal){
    CustomComboBox * cb = (CustomComboBox*) _cb;
    cb->os->receiveSignal(signal, cb->index);
    // cout << "signal received, index = " << cb->index <<endl;
}

void OscilloSection::receiveSignal(vector<float> signal, int i){
    // printf("signal received\n");
    // for(float s : signal){
    //     cout << s <<endl;
    // }
    // QList<QPointF> *signal;
    signalsVector.at(i) = signal;
    if(signalsVector.size()-1 == i)
        emit repaintDisplay();
}

void OscilloSection::repaintSlot(){
    repaint();
}

void CustomComboBox::signalSelection(QString text){
    printf("text changed to ");
    cout << text.toStdString() <<endl;
    State::connections.disconnect(stateKey, os->currentInputs.at(index));

    os->currentInputs.at(index) = text.toLower().replace(" ", "").toStdString();

    State::connections.connect(stateKey, os->currentInputs.at(index));

}
void OscilloSection::signalSelection(QString text){
    // printf("text changed to ");
    // cout << text.toStdString() <<endl;
    // State::connections.disconnect("oscillo", currentInput);

    // currentInput = text.toLower().replace(" ", "").toStdString();

    // State::connections.connect("oscillo", currentInput);

}

void OscilloSection::plotSignals(){
    for(int i=0;i<stateKeys.size();i++){
        QPolygonF poly;

        float y_display_max = 60;
        float signal_max = 1;
        float x_width = 200;

        float signal_v_stretch = y_display_max/signal_max;
        float signal_h_stretch = x_width/signalsVector.at(i).size();
        float xmin = -100;

        for(int j=0; j < signalsVector.at(i).size(); j++){
            float s = signalsVector.at(i).at(j) * (-1) * signal_v_stretch;
            float posx = xmin + j*signal_h_stretch;
            poly << QPointF(posx, s);
        }
        // poly << signal;
        QPen *pen = new QPen(cboxes.at(i)->color);

        QPainterPath *path = new QPainterPath();
        path->addPolygon(poly);
        scene->addPath(*path, *pen);
    }
}

void OscilloSection::plotAxes()
{
    int xmin, xmax, ymin, ymax;
    xmin = scene->sceneRect().x();
    xmax = scene->sceneRect().width() + xmin;
    ymin = scene->sceneRect().y();
    ymax = scene->sceneRect().height() + ymin;
    // printf("ymin = %d", ymin);
    // printf("ymax = %d", ymax);
    // printf("height = %d", (int) scene->sceneRect().height());
    // float ycenter = (ymax+ymin)/2;
    QPen *dashedLine {new QPen(QBrush(Qt::green), .5, Qt::DashLine)};
    QPen *smallerDashedLine {new QPen(QBrush(Qt::green), .2, Qt::DashLine)};
    // QLineF xline(view->mapToScene(xmin, 0), view->mapToScene(xmax, 0));
    // QGraphicsLineItem *xAxisAlt {scene->addLine(xline, *dashedLine)};
    QGraphicsLineItem *xAxis {scene->addLine(xmin, 0, xmax, 0, *dashedLine)};
    QGraphicsLineItem *yAxis {scene->addLine(0, ymin, 0, ymax, *dashedLine)};
    // bool toggled {axesToggle->isChecked()};
    xAxis->setVisible(true);
    yAxis->setVisible(true);
    for(int i=0;i<8;i++){
        int posy = ymin + i * (ymax-ymin)/8;
        scene->addLine(xmin, posy, xmax, posy, *smallerDashedLine);
    }
    for(int i=0;i<10;i++){
        int posx = xmin + i * (xmax-xmin)/8;
        scene->addLine(posx, ymin, posx, ymax, *smallerDashedLine);
    }
    // scene->addLine(xmin, 50, xmax, 50, *dashedLine);

    scene->update();
}

AspectRatioWidget::AspectRatioWidget(QWidget *widget, float width, float height, QWidget *parent) :
    QWidget(parent), arWidth(width), arHeight(height)
{
    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    // add spacer, then your widget, then spacer
    layout->addItem(new QSpacerItem(0, 0));
    layout->addWidget(widget);
    layout->addItem(new QSpacerItem(0, 0));
}

void OscilloSection::resizeEvent(QResizeEvent *event)
{
    view->fitInView(scene->sceneRect());
    scene->update();
}

void OscilloSection::paintEvent(QPaintEvent *ev){
    view->fitInView(scene->sceneRect());
    scene->clear();
    plotAxes();
    plotSignals();
    scene->update();
}

void AspectRatioWidget::resizeEvent(QResizeEvent *event)
{
    float thisAspectRatio = (float)event->size().width() / event->size().height();
    int widgetStretch, outerStretch;

    if (thisAspectRatio > (arWidth/arHeight)) // too wide
    {
        layout->setDirection(QBoxLayout::LeftToRight);
        widgetStretch = height() * (arWidth/arHeight); // i.e., my width
        outerStretch = (width() - widgetStretch) / 2 + 0.5;
    }
    else // too tall
    {
        layout->setDirection(QBoxLayout::TopToBottom);
        widgetStretch = width() * (arHeight/arWidth); // i.e., my height
        outerStretch = (height() - widgetStretch) / 2 + 0.5;
    }

    layout->setStretch(0, outerStretch);
    layout->setStretch(1, widgetStretch);
    layout->setStretch(2, outerStretch);
}