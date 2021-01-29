#ifndef SECTIONLABEL_H
#define SECTIONLABEL_H

#include <QLabel>
#include <QString>
#include <QColor>

#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QFontMetrics> 

#include <iostream>


using namespace::std;

class SectionLabel : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(QColor barsColor READ getBarsColor WRITE setBarsColor)

    public:
        explicit SectionLabel(QWidget * parent = nullptr, const QString& text = "");
        
        void setBarsColor(QColor);
        QColor getBarsColor() const;

    private:
        QString text_;
        float padding;
        QColor barsColor;


        virtual void paintEvent(QPaintEvent*) override;
};

#endif //SECTIONLABEL_H