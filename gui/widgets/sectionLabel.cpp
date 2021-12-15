#include "sectionLabel.h"

using namespace::std;

SectionLabel::SectionLabel(QWidget* parent, const QString& text):
QLabel(parent), text_(text)
{
    // cout<< "test class sectionLabel\n";
    // QLabel::setText(text_);
    QLabel::setText(text);
    padding = .2;
    barsColor = QColor("black");
}

void SectionLabel::setBarsColor(QColor color){
    barsColor = QColor(color);
}

QColor SectionLabel::getBarsColor() const{
    return barsColor;
}


void SectionLabel::paintEvent(QPaintEvent*){
    QPainter p(this);

    QFont font = p.font();
    float fontsize = font.pointSizeF();

    p.setBackgroundMode(Qt::TransparentMode);

    QColor pointColor = QColor(p.pen().color());
    QColor bgColor = QColor(p.background().color());

    p.drawRect(QRectF(0, 0, QLabel::width()-1, QLabel::height()-1));
    

    QFontMetrics metrics(font);

    QRectF rect = metrics.boundingRect(QLabel::text());

    // p.drawRect(rect);

    QPointF center(QLabel::width()/2, QLabel::height()/2 + 1);
    float paintPadding = padding * fontsize;

    QRectF text_rect(center.x() - rect.width()/2 - paintPadding,
		            center.y() - rect.height()/2,rect.width() + 2 * paintPadding, rect.height());

    p.setBrush(QBrush(pointColor));
    p.drawRect(text_rect);
    text_rect.setX(text_rect.x() + paintPadding);
    p.setPen(QColor(bgColor));
    // p.setPen(QtGui.QColor("red"))
    // print("label text = ", type(text))
    p.drawText(text_rect, QLabel::text());

    p.setPen(barsColor);
    p.drawLine(QPointF(center.x() + rect.width()/2 + paintPadding*4, center.y()),
                QPointF(QLabel::width(), center.y()));
    p.drawLine(QPointF(center.x() - rect.width() / 2 - paintPadding * 4, center.y()),
                QPointF(0, center.y()));

}
