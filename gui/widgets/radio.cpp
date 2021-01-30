#include "radio.h"

#include <QStyleOption>
#include <vector>
#include <QRegularExpression>
#include <QList>
#include <QLabel>

vector<QString> colors = {
    "0x38e643",
    "0x1e62ff",
    "0xf91c1c",
    "0xff7d00",
    "0x1e62ff",
    "0xf91c1c"
};

Radio::Radio(const QString& text, int id):
QRadioButton(nullptr), text(text)
{
    QRadioButton::setText(text);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ID = id;

    setMinimumWidth(10);
    setMinimumHeight(15);

    center = QPointF(0, 0);
    radius = 1;

    textRect = QRect(0, 0, 0, 0);

    paintSize = 0;
    fontsize = 0;
    fontsizefactor = 1.5;
}

void Radio::computeSize(){
    paintSize = min(1./8. * ((float)this->width()), ((float)this->height())/4.);
    fontsize = paintSize * fontsizefactor;
}

void Radio::resizeEvent(QResizeEvent* event){
    paintSize = 10;
    fontsize = 5;
    QWidget *parent = this->topLevelWidget();
    // print(parent)
    // QList<T> radios = parent->findChildren(QRegularExpression('QWidget'));
    // QList<QWidget> radios = parent->findChildren(QWidget);
    QList<Radio *> widgets = parent->findChildren<Radio *>("");
    int lenList = widgets.size();

    // for(int i=0; i<lenList; i++){
    //     cout << widgets[i]->text.toStdString() << endl;
    // }
    
    this->computeSize();
    float minsize = this->paintSize;
    for(int i=0; i<lenList; i++){
        // cout << widgets[i]->text.toStdString() << endl;
        if(widgets[i] != this){
            widgets[i]->computeSize();
            minsize = min(minsize, widgets[i]->paintSize);
            // cout << "minsize = " <<  minsize << endl;
        }
    }

    if(minsize > 0){
        // cout << "found minsize !\n";
        paintSize = minsize;
        for(int i=0; i<lenList; i++){
            if(widgets[i]->paintSize != minsize){
                widgets[i]->paintSize = minsize;
                widgets[i]->repaint();
            }
        }
        QList<QLabel *> labels = parent->findChildren<QLabel *>("");
        int lenLabels = labels.size();
        
        for(int j=0; j<lenLabels; j++){
            if(labels[j]->text() == "Range" || labels[j]->text() == "Quantize"){
                if(fontsize != 0){
                    QFont font = labels[j]->font();
                    font.setPointSizeF(fontsize);
                    labels[j]->setFont(font);
                }
            }
        }
    }
}

void Radio::paintEvent(QPaintEvent*){

    QStyleOption opt = QStyleOption();
    
    opt.initFrom(this);
    QPainter painter(this);

    // print("minimum size : ", self.minimumSize())
    // print("radio: hasHforW : ", self.hasHeightForWidth())
    // print("radio: size : ", self.size())
    // print("radio : minimumSizeHint() :", self.minimumSizeHint())

    // s = self.style()

    // s.drawPrimitive(QStyle.PE_Widget, opt, painter, self)

    this->computeSize();
    float size = paintSize;
    // size = np.min([1/8 * self.width(), self.height()/4])

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