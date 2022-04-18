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

#include <string>




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

        void setStateParam(QString);
        void checkState();

        void setValue(int);

    private:
        int ID;
        int value;

        QPointF center;
        QRectF textRect;
        float radius;
        // float padding;
        // QColor barsColor;

        std::string stateKey;
        // int stateValue;

        virtual void paintEvent(QPaintEvent*) override;
    
    public slots:
        void warnState(int);
};

#endif // RADIO_H