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

        // void resizeEvent(QResizeEvent* event);
        
        // void setBarsColor(QColor);
        // QColor getBarsColor() const;
        float paintSize, fontsize;
        static float constexpr fontsizefactor = 1.5;
        void computeSize();
        static void computeSizeFromContainingWidgetSize(float, float, float*);
        QString text;

    private:
        int ID;

        QPointF center;
        QRectF textRect;
        float radius;
        // float padding;
        // QColor barsColor;



        virtual void paintEvent(QPaintEvent*) override;
};

#endif // RADIO_H