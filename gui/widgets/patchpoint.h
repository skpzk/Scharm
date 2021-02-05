#ifndef PATCHPOINT_H
#define PATCHPOINT_H

#include <iostream>
#include <string>
#include <tuple>

#include <QWidget>
#include <QPainter>
#include <QEnterEvent>

class Patchpoint;
#include "patchcordList.h"


/*

IMPORTANT NOTE FOR LATER :
The mouse movement handling is a bit convoluted, 
we should have used a drag and drop implementation, see : https://doc.qt.io/qt-5/dnd.html

*/

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

    PatchCordList * pcs;

    QPointF getCenter();

    string ioType;

    tuple<bool, Patchpoint*> isMouseReleaseOnPp(QPointF);

private:

    virtual void paintEvent(QPaintEvent*) override;

    void computeCenter();

    QColor bgColor;
    QColor pointColor;
    QColor ppColor;

    bool hasFixedSize, hasFixedFontSize, hasPcGrabbed;
    float fixedSize, fixedFontSize;
    float fontsizefactor, fontsize;

    float mainRadius, ppMargin;

    QPolygonF createPoly(int, float, float, float);
    void setPcToHoveredIfCursorOnPp(QMouseEvent *);

    

    // double knobRadius_;

    // double knobMargin_;

    // double size_;
    // double radius;
    
    QPointF center;

    QString text_;
    

    // QColor ringColor;

    // bool coloredTitle = false;
    
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent *ev) override;
    virtual void leaveEvent(QEvent *ev) override;

};

#endif // PATCHPOINT_H