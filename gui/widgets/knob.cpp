#define _USE_MATH_DEFINES
#include <cmath>

#include <QPainter>
#include <QColor>
#include <QBrush>

#include <QRectF>

#include "knob.h"

#include "../utils.h"

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
}

void Knob::setText(string text){
    text_ = QString::fromStdString(text);
}
// void Knob::mousePressEvent(QMouseEvent *ev){

// }

void Knob::setRingColor(QColor color){
    ringColor = QColor(color);
}

QColor Knob::getRingColor() const{
    return QColor("black");
}

void Knob::paintEvent(QPaintEvent*){

    float extent = 1.5 * M_PI;
    float offset = 1.25 * M_PI;

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

    // uncomment the following line to draw outer rect
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

    //draw inner circle

    painter.setBackgroundMode(Qt::OpaqueMode);

    pen = QPen(QColor(pointColor), linewidth);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);
    painter.setBrush(QColor(bgColor));
    if(implementedKnobs.find(this->text_.toStdString()) == implementedKnobs.end()){
        painter.setBrush(QBrush(QColor("#cccccc")));
    }

    float radius_inner = 15./20. * radius;
    painter.drawEllipse(QPointF(center_x, center_y), radius_inner, radius_inner);

    center = QPointF(center_x, center_y);

    double value = static_cast<double> (QDial::value());

    double ratio = (value - QDial::minimum()) / (QDial::maximum() - QDial::minimum());

    cout << "Ratio = " << ratio << endl;

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