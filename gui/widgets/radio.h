#ifndef RADIO_H
#define RADIO_H

#include <QRadioButton>
#include <QString>
#include <QColor>
#include <QRectF>
#include <QResizeEvent>

#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QFontMetrics> 

#include <iostream>


using namespace::std;

class Radio : public QRadioButton
{
    Q_OBJECT

    // Q_PROPERTY(QColor barsColor READ getBarsColor WRITE setBarsColor)

    public:
        explicit Radio(const QString& text = "", int id=0);

        void resizeEvent(QResizeEvent* event);
        
        // void setBarsColor(QColor);
        // QColor getBarsColor() const;

    private:
        QString text;
        int ID;

        QPointF center;
        QRectF textRect;
        float radius;
        float paintSize, fontsize, fontsizefactor;
        // float padding;
        // QColor barsColor;

        void computeSize();


        virtual void paintEvent(QPaintEvent*) override;
};

#endif // RADIO_H