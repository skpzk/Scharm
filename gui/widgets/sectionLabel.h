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
    public:
        explicit SectionLabel(QWidget * parent = nullptr, const QString& text = "");
    private:
        QString text_;
        float padding;
        QColor barsColor;

        virtual void paintEvent(QPaintEvent*) override;
};

#endif //SECTIONLABEL_H