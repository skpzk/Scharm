#define _USE_MATH_DEFINES
#include <cmath>

#include <QPainter>
#include <QColor>
#include <QBrush>

#include <QRectF>
#include <QMouseEvent>

#include "knob.h"

#include "../utils.h"
#include "../../utils/utils.h"

#include "../../state/state.h"

#include <set>



std::set<std::string> implementedKnobs = {};

QColor titleColor = QColor("#fd971f");



Knob::Knob(QWidget * parent,
    const QString& text,
    double knobRadius,
    double knobMargin): QDial(parent),
    knobRadius_(knobRadius),
    knobMargin_(knobMargin),
    text_(text)
{
    // Default range
    QDial::setRange(0,127);
    ringColor = QColor("black");
    sizeType = 1;
    stateKey = text_.toStdString();
    lowerWithoutSpaces(&stateKey);

    // this->valueChanged.connect();
    // connect(lineedit, &QLineEdit::textChanged, this, &YourWidget::MySlot);
    connectWarnState();

    // trying this to prevent recursive repaint and segfaults
    connect(this, &Knob::repaintTitle, this, &Knob::repaintSlot);

    debug_print_rects = true;
    debug_print_rects = false;

    // extent is the angle of the colored line around the knob, 
    // and of the total travel of the knob
    // offset is used to display it with vertical symmetry
    extent = 1.5 * M_PI;
    offset = 1.25 * M_PI;

    id = -1;
    kType = general;
}

void Knob::setKnobType(knobType kt){
    kType = kt;
    setStyleSheet(defaultKnobStyleSheet(kType));
}

void Knob::setId(int i){
    id = i;
}

void Knob::connectWarnState(){
    connect(this, &QDial::valueChanged, this, &Knob::warnState);
}

void Knob::disconnectWarnState(){
    disconnect(this, &QDial::valueChanged, this, &Knob::warnState);
}

void Knob::setMinimum(int m){
    disconnectWarnState();
    QDial::setMinimum(m);
    connectWarnState();
}

void Knob::setMaximum(int M){
    disconnectWarnState();
    QDial::setMaximum(M);
    connectWarnState();
}

void Knob::setText(string text){
    text_ = QString::fromStdString(text);
    stateKey = text_.toStdString();
    lowerWithoutSpaces(&stateKey);
}

void Knob::setStateParamText(string text){
    stateKey = text;
    lowerWithoutSpaces(&stateKey);
    checkState();
}
// void Knob::mousePressEvent(QMouseEvent *ev){

// }

void Knob::warnState(int _){

    // cout << "warning state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) value() / maximum() << endl;
    State::params(stateKey)->setValue((float) value());
}

void Knob::checkState(){

    // cout << "checking state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) *State::params(stateKey) << endl;
    // State::params(stateKey)->setValue(value() / maximum());
    setValue(*State::params(stateKey));
}

void Knob::setRingColor(QColor color){
    ringColor = QColor(color);
}

QColor Knob::getRingColor() const{
    return QColor("black");
}

void Knob::mousePressEvent(QMouseEvent* ev){
    if(distance(ev->pos(), this->center) < this->radius){
        mouseEvent(ev);
        ignoresMouse = false;
    }
    else{
        // printf("mouse outside of knob\n");
        ignoresMouse = true;
    }
}

void Knob::mouseMoveEvent(QMouseEvent* ev){
    if(!ignoresMouse)
        mouseEvent(ev);
}

void Knob::mouseReleaseEvent(QMouseEvent* ev){
    if(!ignoresMouse){
        mouseEvent(ev);
        ignoresMouse = true;
    }
}

void Knob::mouseEvent(QMouseEvent* ev){
    // super(Knob, self).mousePressEvent(me)
    ev->accept();
    // printf("event accepted\n");
    double angle = atan2(ev->pos().x() - this->center.x(), -ev->pos().y() + this->center.y());
    angle += M_PI;

    angle = angle * 1/extent - (2*M_PI - extent)/(2*extent);
    // printf("angle = %0.3f\n", angle);
    this->setValue(this->minimum() + angle * (this->maximum() - this->minimum()));
}

void Knob::changeTitleColorCallback(void* voidInstance, float value){
    Knob* instance = (Knob*) voidInstance;
    // string title = instance->text_.toStdString();
    // printf("knob %s called, value = %0.2f\n", title.c_str(), value);
    // std::cout << title << std::endl;
    if((instance->kType == seq) && (instance->id != (int) value))
        value = false;

    instance->coloredTitle = (bool) value;
    emit instance->repaintTitle();
    // instance->repaint();

}

void Knob::repaintSlot(){
    repaint();
}

void Knob::paintEvent(QPaintEvent*){



    QPainter painter(this);

    // So that we can use the background color
    // painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackgroundMode(Qt::TransparentMode);

    // Smooth out the circle
    painter.setRenderHint(QPainter::Antialiasing);

    // Use background color
    QColor bgColor = painter.background().color();
    painter.setBrush(painter.background());
   

    // Store color from stylesheet, pen will be overridden
    QColor pointColor = QColor(painter.pen().color());

    // print(QDial.width(self), QDial.height(self))

    // draw widget borders
    QPen pen = QPen(QColor(this->ringColor), 1);
    pen.setCapStyle(Qt::SquareCap);
    painter.setPen(pen);
    painter.setBrush(QColor("transparent"));

    // uncomment the following line to draw outer rect
    if(debug_print_rects)
    painter.drawRect(0, 0, QDial::width(), QDial::height());

    // No border
    
    painter.setPen(QPen(Qt::NoPen));

    // the height of the widget is 2*radius + 2*fontsize1 + 2*fontsize2
    // where fontsize1 = .4radius and fontsize2 = .9*.4*radius
    // so QDial.height = radius * (2+.4*2+.4*.9*2)
    //

    float fontsize1factor = .4;
    float fontsize2reduction = .9;
    float fontsize2factor = fontsize1factor*fontsize2reduction;

    float center_x = QDial::width() / 2.0;
    float center_y = QDial::height() / 2.0;

    radius = fmin(QDial::width() / 2. - knobMargin_,
        QDial::height() / (2.+ 2*fontsize1factor + 2*fontsize2factor) - knobMargin_);
    radius = fmax(radius, 1);
    // print("Radius = ", radius, ", height = ", QDial.height(self), ", width = ", QDial.width(self))
    center_y = center_y - radius * (fontsize1factor + fontsize2factor);

    QRectF rectangle = QRectF(center_x - radius,
                            center_y - radius,
                            2 * radius,
                            2 * radius);

    // Draw arc
    

    /*The startAngle and spanAngle must be specified in 1/16th of a degree, 
    i.e. a full circle equals 5760 (16 * 360). 
    Positive values for the angles mean counter-clockwise 
    while negative values mean the clockwise direction. 
    Zero degrees is at the 3 o'clock position.*/

    float linewidth = ((float) radius) / 30. * 2.;

    // linewidth = 1
    pen = QPen(QColor(ringColor), linewidth);
    pen.setCapStyle(Qt::RoundCap);
    // pen.setCapStyle(Qt.FlatCap)

    painter.setPen(pen);

    // adapt to linewidth to make it more pleasant to the eye
    float capRadius = linewidth/4;
    float angleCap = asin(capRadius/radius);

    float start_deg = (90 - rad2deg(extent/2)) + rad2deg(angleCap);
    float start_16deg = start_deg * 16;

    float extent_deg =  rad2deg(extent) - 2 * rad2deg(angleCap);
    float extent_16deg = extent_deg * 16;

    painter.drawArc(rectangle, start_16deg, extent_16deg);

    //draw inner disk

    painter.setBackgroundMode(Qt::OpaqueMode);

    pen = QPen(QColor(pointColor), linewidth);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);
    painter.setBrush(QColor(bgColor));
    if(implementedKnobs.find(this->text_.toStdString()) == implementedKnobs.end()){
        painter.setBrush(QBrush(QColor("#393939")));
        // painter.setBrush(QBrush(QColor("red")));
    }

    float radius_inner = 15./20. * radius;
    painter.drawEllipse(QPointF(center_x, center_y), radius_inner, radius_inner);

    center = QPointF(center_x, center_y);

    double value = static_cast<double> (QDial::value());

    double ratio = (value - QDial::minimum()) / (QDial::maximum() - QDial::minimum());

    // cout << "Ratio = " << ratio << endl;

    // The maximum amount of degrees is 270, offset by 225
    double angle = ratio * extent - offset;

    // Draw the indicator
    painter.setBrush(QBrush(pointColor));

    float a_y = center_y + sin(angle) * (radius -.1);
    float a_x = center_x + cos(angle) * (radius -.1);

    pen = QPen(pointColor, linewidth);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.drawLine(a_x, a_y, center_x, center_y);

    float fontsize1 = radius * fontsize1factor;

    if(sizeType == 1 && fontsize1 != Knob::fontsize1){
        Knob::fontsize1 = fontsize1;
    }else{
        fontsize1 = Knob::fontsize1;
    }
    float fontsize2 = fontsize1 * fontsize2reduction;

    QRectF textRect_ = QRectF(0, center_y + radius, QDial::width(), 2*fontsize1);

    if(coloredTitle)
    painter.setPen(titleColor);

    QFont f = painter.font();
    f.setPointSizeF(fontsize1);
    painter.setFont(f);
    // painter.setBrush(QColor("transparent"));
    painter.setBackgroundMode(Qt::TransparentMode);
    // painter.drawRect(textRect_)
    painter.drawText(textRect_, Qt::AlignHCenter | Qt::AlignTop, text_);
    // painter.drawText(textRect_, Qt.AlignHCenter | Qt.AlignTop, str(fontsize1))

    QRectF textRect = QRectF(0, center_y + radius + fontsize1*2, QDial::width(), 2*fontsize2);

    if(this->hasFocus())
        painter.setPen(QColor("red"));

    f.setPointSizeF(fontsize2);
    painter.setFont(f);
    // painter.drawRect(textRect_)
    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, QString::number(QDial::value()));

    painter.end();

}