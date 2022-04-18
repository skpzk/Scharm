#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>

#include <QAbstractButton>
#include <QPainter>
#include <QEvent>
#include <QObject>

// #include <QProperty>

using namespace::std;

class Button : public QAbstractButton
{
    Q_OBJECT

    // Q_PROPERTY(QColor ringColor READ getRingColor WRITE setRingColor)
    // Q_PROPERTY(bool held READ isHeld NOTIFY notifyHeld DESIGNABLE true)
    // Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor DESIGNABLE true)
    // Q_PROPERTY(QColor foregroundColor READ getForegroundColor WRITE setForegroundColor DESIGNABLE true)


public:

    Button(QWidget * parent = nullptr,
               const QString& text = "");

    void setText(string);
    void setStateParamText(string);

    void checkState();
    void warnState(int);
    void uncheckableWarnState();

    void setCheckable(bool);
    void setHoldable(bool);

    bool isHeld();
    bool isHoldable();

    // QColor getBackgroundColor();
    // QColor getForegroundColor();
    
    // void setBackgroundColor(QColor);
    // void setForegroundColor(QColor);



private:

    bool eventFilter(QObject*, QEvent*);

    virtual void paintEvent(QPaintEvent*) override;
    void getColorFromStyleSheet();
    string getStyleFromPseudoState(string pseudostate);

    QColor bgColor;
    QColor pointColor;

    // QColor backgroundColor;
    // QColor foregroundColor;

    // double knobRadius_;

    // double knobMargin_;

    // double size_;
    // double radius;
    
    // QPointF center;
    string stateKey;

    QString text_;

    // QColor ringColor;

    // bool coloredTitle = false;

    bool holdable=false;
    bool held = false;
    
protected:
    // void mousePressEvent(QMouseEvent *ev) override;
    // void mouseMoveEvent(QMouseEvent *ev) override;
    // void mouseReleaseEvent(QMouseEvent *ev) override;
    // void mouseEvent(QMouseEvent *ev);

};

#endif // BUTTON_H