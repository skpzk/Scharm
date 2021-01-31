#ifndef PATCHPOINT_H
#define PATCHPOINT_H

#include <iostream>
#include <string>

#include <QWidget>
#include <QPainter>

using namespace::std;

class Patchpoint : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float fixedSize READ getFixedSize WRITE setFixedSize)
    Q_PROPERTY(QColor ppColor READ getPpColor WRITE setPpColor)

public:

    explicit Patchpoint(QWidget * parent = nullptr,
               const QString& text = "", string ioType = "in");
    
    void setFixedSize(float size);
    float getFixedSize() const;

    void setPpColor(QColor size);
    QColor getPpColor() const;

    

private:

    virtual void paintEvent(QPaintEvent*) override;

    void computeCenter();

    QColor bgColor;
    QColor pointColor;
    QColor ppColor;

    bool hasFixedSize, hasFixedFontSize;
    float fixedSize, fixedFontSize;
    float fontsizefactor, fontsize;

    float mainRadius, ppMargin;

    QPolygonF createPoly(int, float, float, float);

    

    // double knobRadius_;

    // double knobMargin_;

    // double size_;
    // double radius;
    
    QPointF center;

    QString text_;
    string ioType;

    // QColor ringColor;

    // bool coloredTitle = false;
    
protected:
    // void mousePressEvent(QMouseEvent *ev) override;

};

#endif // PATCHPOINT_H