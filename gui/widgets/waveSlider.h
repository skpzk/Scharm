#ifndef WAVESLIDER_H
#define WAVESLIDER_H

#include <QSlider>
#include <QColor>
#include <QPointF>
#include <string>
#include <iostream>

#include <QPainterPath>
#include <QPolygonF>

#include <QMouseEvent>

using namespace::std;

QPainterPath compute_tri_path(float x0, float y0, float width, float height);
QPainterPath compute_saw_path(float x0, float y0, float width, float height);
QPainterPath compute_sqr_path(float x0, float y0, float width, float height);
QPolygonF compute_arrow(float x0, float y0, float width, float height);

class WaveSlider : public QSlider
{
    Q_OBJECT

    // Q_PROPERTY(double knobRadius READ getKnobRadius WRITE setKnobRadius) 
    //permits access via stylesheet

    // Q_PROPERTY(double knobMargin READ getKnobMargin WRITE setKnobMargin)
    // Q_PROPERTY(double fixedSize READ getFixedSize WRITE setFixedSize)
    // Q_PROPERTY(QColor ringColor READ getRingColor WRITE setRingColor)

public:

    WaveSlider(int nb_items, int direction);

private:

    virtual void paintEvent(QPaintEvent*) override;

    int steps, direction;
    QPointF handlePos;
    QRectF barRect;

    int getMousePositionInSteps(QPoint);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

};

#endif // WAVESLIDER_H