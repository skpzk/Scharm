#ifndef KNOB_H
#define KNOB_H

#include <QDial>

class Knob : public QDial
{
    Q_OBJECT

    // Q_PROPERTY(double knobRadius READ getKnobRadius WRITE setKnobRadius) 
    //permits access via stylesheet

    // Q_PROPERTY(double knobMargin READ getKnobMargin WRITE setKnobMargin)
    // Q_PROPERTY(double fixedSize READ getFixedSize WRITE setFixedSize)

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

    // void setText(const QString& text); 
    // QString getText() const;


private:

    virtual void paintEvent(QPaintEvent*) override;

    double knobRadius_;

    double knobMargin_;

    double size_;
    bool hasFixedSize_ = false;

    QString text_;
    
protected:
    void mousePressEvent(QMouseEvent *ev) override;

};

#endif