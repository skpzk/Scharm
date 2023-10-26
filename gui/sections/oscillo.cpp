#include "oscillo.h"
#include "section.h"
#include "../utils.h"

#include "../../state/state.h"

#include <QVBoxLayout>

#include <QGraphicsLineItem>
#include <QResizeEvent>
#include <QComboBox>

#include <stdio.h>
#include <math.h>
#include <iostream>


using namespace::std;

// string Patchpoint::getName(){
// 	string name = text_.toStdString();
// 	lowerWithoutSpaces(&name);
// 	return name;
// }

OscilloSection::OscilloSection(QWidget *parent):
QWidget(parent)
{
    Section *section = new Section(parent, "Oscillo  ");

    // cout << "patchbay instanciated\n";

    this->setLayout(section->vbox);

    //check state
    // checkState();

    scene = new QGraphicsScene;
    scene->setSceneRect(-100, -80, 200, 160);

    scene->setBackgroundBrush(Qt::black);

    QColor bgColor = QColor("red");

    QPen pen = QPen(bgColor);
    // QBrush brush = QBrush(QColor("blue"));

    // QGraphicsRectItem *rect = scene->addRect(QRectF(0, 0, 20, 20), pen);

    // QGraphicsItem *item = scene->itemAt(0, 0, QTransform());

    // view = new CustomView(scene);
    view = new QGraphicsView();
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

    

    QGridLayout *grid = new QGridLayout;

    section->vbox->addWidget(arwidget);
    QLabel *label = new QLabel("TEST");
    QComboBox * cbox = new QComboBox();

    QStringList sources = {"VCA", "VCO 1", "VCO 1 SUB 1", "VCO 1 SUB 2",
                    "VCA EG", "VCO 2", "VCO 2 SUB 1", "VCO 2 SUB 2",
                    "VCF EG",
                    "SEQ 1", "SEQ 1 CLK", "SEQ 2", "SEQ 2 CLK", "NOISE",
                    "CLOCK", "SH", "MIDI CLK", "MIDI"};

    cbox->addItems(sources);
    section->vbox->addWidget(cbox);

    currentInput = "vco1";

    State::connections.connect("oscillo", "vco1");
    State::params("oscillo")->attachCallback((void*) this, OscilloSection::receiveSignal);
    

    // section->vbox->addLayout(grid, 1);
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    section->vbox->addSpacerItem(verticalSpacer);

    // section->vbox->addWidget(&view);
    section->vbox->setStretch(0, 0);
    // section->vbox->setStretch(1, 1);
    // section->vbox->setStretch(2, 1);
    // section->vbox->setStretch(1, 100);

    connect(this, &OscilloSection::repaintDisplay, this, &OscilloSection::repaintSlot);
    connect(cbox, &QComboBox::currentTextChanged, this, &OscilloSection::signalSelection);
    
}

// CustomView::CustomView(QGraphicsScene *scene):
// QGraphicsView(scene){
// }

// int CustomView::heightForWidth(int w) const{
//     return 4*w/5;
// }
void OscilloSection::receiveSignal(void * _os, vector<float> signal){

    OscilloSection * os = (OscilloSection*) _os;
    // printf("signal received\n");
    // for(float s : signal){
    //     cout << s <<endl;
    // }
    // QList<QPointF> *signal;
    os->signal = signal;
    emit os->repaintDisplay();
}

void OscilloSection::repaintSlot(){
    repaint();
}

void OscilloSection::signalSelection(QString text){
    printf("text changed to ");
    cout << text.toStdString() <<endl;
    State::connections.disconnect("oscillo", currentInput);

    currentInput = text.toLower().replace(" ", "").toStdString();

    State::connections.connect("oscillo", currentInput);

}

void OscilloSection::plotSignal(){
    QPolygonF poly;

    float y_display_max = 60;
    float signal_max = 1;
    float x_width = 200;

    float signal_v_stretch = y_display_max/signal_max;
    float signal_h_stretch = x_width/signal.size();
    float xmin = -100;

    for(int i=0; i < signal.size(); i++){
        float s = signal.at(i) * (-1) * signal_v_stretch;
        float posx = xmin + i*signal_h_stretch;
        poly << QPointF(posx, s);
    }
    // poly << signal;
    QPen *pen = new QPen("red");

    QPainterPath *path = new QPainterPath();
    path->addPolygon(poly);
    scene->addPath(*path, *pen);
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
    plotSignal();
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