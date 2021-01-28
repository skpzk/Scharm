#include "button.h"

#include <QColor>
#include <QSizePolicy>
#include <QBrush>

#include "../defs.h"
#include <string>

Button::Button(QWidget * parent,
    const QString& text): 
QAbstractButton(parent),
  text_(text)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setSizePolicy(Qtw.QSizePolicy.Ignored, Qtw.QSizePolicy.Ignored);
    setMinimumHeight(10);
}

void Button::setText(string text){
    text_ = QString::fromStdString(text);
}

void Button::getColorFromStyleSheet(){
    QString s = this->styleSheet();
    // cout << s.toStdString() << endl;
    vector<string> sSplitted = split(s.toStdString(), "}");
    string style = "";

    pointColor = QColor("black");
    bgColor = QColor("white");

    // for(int i=0; i<sSplitted.size(); i++){
    //     cout << sSplitted[i] <<endl;
    // }
    if(isDown()){
        for(int i=0; i<sSplitted.size(); i++){
            if(sSplitted[i].find("Pressed") != string::npos){
                style = sSplitted[i];
                // cout << "Is down, style is : " << style <<endl;
                break;
            }
        }
    }else if(isChecked()){
        for(int i=0; i<sSplitted.size(); i++){
            if(sSplitted[i].find("Checked") != string::npos){
                style = sSplitted[i];
                // cout << "Is checked, style is : " << style <<endl;
                break;
            }
        }
    }else{
        for(int i=0; i<sSplitted.size(); i++){
            if(sSplitted[i].find("Button{") != string::npos){
                style = sSplitted[i];
                // cout << "Else, style is : " << style <<endl;
                break;
            }
        }
    }
    if(style != ""){
        vector<string> styleSplitted = split(style, ";");
        // cout << "style is : " << style << endl;
        for(int i=0; i<styleSplitted.size(); i++){
            // cout << "stylesplitted[" << i << "] = " << styleSplitted[i] << endl;
            if(styleSplitted[i].find("background") != string::npos){
                vector<string> colorsSplitted = split(styleSplitted[i], " ");
                // cout << "bgcolor found, bgcolor = " << colorsSplitted[colorsSplitted.size() - 1] << endl;
                bgColor = QColor(QString::fromStdString(colorsSplitted[colorsSplitted.size() - 1]));
            }
            if((styleSplitted[i].find("color") != string::npos) &&
                (styleSplitted[i].find("background") == string::npos)){
                vector<string> colorsSplitted = split(styleSplitted[i], " ");
                // cout << "ptcolor found, ptcolor = " << colorsSplitted[colorsSplitted.size() - 1] << endl;
                pointColor = QColor(QString::fromStdString(colorsSplitted[colorsSplitted.size() - 1]));
            }
        }
    }
    
}

void Button::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setBrush(QColor("black"));

    p.setBackgroundMode(Qt::OpaqueMode);
    p.setRenderHint(QPainter::Antialiasing);

    // cout << "button paintevent, width = " << this->width() << endl;
    // cout << "button paintevent, parent width = " << QAbstractButton::width() << endl;

    // QColor bgColor = p.background().color();
    // QColor pointColor = QColor(p.pen().color());

    getColorFromStyleSheet();
    

    p.setBrush(QBrush(bgColor));

    float w = this->width();
    float h = this->height();
    p.drawRect(0, 0, this->width(), this->height());

    // draw text
    p.setPen(pointColor);
    p.setBackgroundMode(Qt::TransparentMode);
    p.drawText(QRectF(0, 0, w, h), Qt::AlignHCenter| Qt::AlignVCenter,  text_);
}