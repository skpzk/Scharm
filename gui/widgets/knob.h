#ifndef KNOB_H
#define KNOB_H

#include <QDial>
#include <QColor>
#include <QPointF>
#include <string>
#include <iostream>

using namespace::std;

class Knob : public QDial
{
    Q_OBJECT

    // Q_PROPERTY(double knobRadius READ getKnobRadius WRITE setKnobRadius) 
    //permits access via stylesheet

    // Q_PROPERTY(double knobMargin READ getKnobMargin WRITE setKnobMargin)
    // Q_PROPERTY(double fixedSize READ getFixedSize WRITE setFixedSize)
    Q_PROPERTY(QColor ringColor READ getRingColor WRITE setRingColor)

public:

    Knob(QWidget * parent = nullptr,
               const QString& text = "",
               double knobRadius = 5,
               double knobMargin = 3);

    // void setKnobRadius(double radius);
    // double getKnobRadius() const;

    // void setKnobMargin(double margin);
    // double getKnobMargin() const;

    // void setFixedSize(double size);
    // double getFixedSize() const;

    void setRingColor(QColor);
    QColor getRingColor() const;

    // void setText(const QString& text); 
    // QString getText() const;

    void setText(string);

    int sizeType;
    float fontsize1;

    void checkState();

private:

    virtual void paintEvent(QPaintEvent*) override;

    double knobRadius_;

    double knobMargin_;

    double size_;
    double radius;
    
    QPointF center;

    QString text_;

    QColor ringColor;

    bool coloredTitle = false;
    
    string stateKey;

public slots:
    void warnState(int);
    
protected:
    // void mousePressEvent(QMouseEvent *ev) override;

};

#endif