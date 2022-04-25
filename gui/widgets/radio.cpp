#include "radio.h"

#include <QStyleOption>
#include <vector>
#include <QRegularExpression>
#include <QList>
#include <QLabel>

#include "../../state/state.h"

using namespace std;

vector<QString> colors = {
    "#38e643",
    "#1e62ff",
    "#f91c1c",
    "#ff7d00",
    "#1e62ff",
    "#f91c1c"
};

Radio::Radio(const QString& text, int id):
QRadioButton(nullptr), text(text)
{
    QRadioButton::setText(text);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ID = id;

    value = id;

    setMinimumWidth(10);
    setMinimumHeight(15);

    center = QPointF(0, 0);
    radius = 1;

    textRect = QRect(0, 0, 0, 0);

    paintSize = 0;
    fontsize = 0;

    connect(this, &QRadioButton::toggled, this, &Radio::warnState);
    
}

void Radio::setValue(int v){
    value = v;
}

void Radio::computeSize(){
    paintSize = min(1./8. * ((float)this->width()), ((float)this->height())/4.);
    fontsize = paintSize * fontsizefactor;
}

void Radio::computeSizeFromContainingWidgetSize(float w, float h, float* out){
    out[0] = min(1./8. * w, h/4.);
    out[1] = out[0] * fontsizefactor;
}

void Radio::setStateParam(QString text){
    stateKey = text.toStdString();
}

void Radio::warnState(int _){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    // cout << "warning state :\n";
    // cout << "key = " << stateKey << ", value = " << (int) value << endl;
    State::params(stateKey)->setValue((int) value);
}

void Radio::checkState(){
    if(*State::params(stateKey) == value){
        setChecked(true);
    }
}

void Radio::paintEvent(QPaintEvent*){

    QStyleOption opt = QStyleOption();
    
    opt.initFrom(this);
    QPainter painter(this);

    float size = paintSize;

    float linewidth = 1;

    painter.setRenderHint(QPainter::Antialiasing);

    QColor pointColor = QColor(painter.pen().color());
    QColor bgColor = QColor(painter.background().color());
    // bgColor = QColor("white")

    painter.setBrush(QBrush(QColor("transparent")));
    painter.setPen(QPen(QColor(pointColor), linewidth));
    // painter.setBackgroundMode(Qt.TransparentMode)

    center = QPointF((this->width()/2), this->height()/2);
    
    this->radius = size;

    painter.drawEllipse(center, size, size);

    float margin = .4 * size;

    // painter.drawRect(text_rect)
    // self.text()

    float fontsize = size * fontsizefactor;
    // self.fontsize = fontsize

    QFont f = painter.font();
    f.setPointSizeF(fontsize);
    painter.setFont(f);

    QRectF text_rect = QRectF(center.x() + size + margin, center.y() - fontsize, 100, 2 * fontsize);
    // painter.drawRect(text_rect)
    // cout << "Radio text = " << this->text.toStdString() << endl;
    painter.drawText(text_rect, Qt::AlignLeft | Qt::AlignVCenter, this->text);

    this->textRect = text_rect;


    if(this->isChecked()){
        painter.setBrush(QBrush(QColor(colors[this->ID])));
        // painter.setPen(QPen(Qt.NoPen))
        painter.drawEllipse(center, size, size);
    }
    // painter.drawLine(QtCore.QPointF(self.width()/2, 0), QtCore.QPointF(self.width()/2, self.height()))

    
}