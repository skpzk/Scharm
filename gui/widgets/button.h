#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>

#include <QAbstractButton>
#include <QPainter>

using namespace::std;

class Button : public QAbstractButton
{
    Q_OBJECT

    // Q_PROPERTY(QColor ringColor READ getRingColor WRITE setRingColor)

public:

    Button(QWidget * parent = nullptr,
               const QString& text = "");

    void setText(string);

private:

    virtual void paintEvent(QPaintEvent*) override;
    void getColorFromStyleSheet();

    QColor bgColor;
    QColor pointColor;

    // double knobRadius_;

    // double knobMargin_;

    // double size_;
    // double radius;
    
    // QPointF center;

    QString text_;

    // QColor ringColor;

    // bool coloredTitle = false;
    
protected:
    // void mousePressEvent(QMouseEvent *ev) override;

};

#endif // BUTTON_H