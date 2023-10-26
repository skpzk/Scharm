#include "button.h"

#include <QColor>
#include <QSizePolicy>
#include <QBrush>

#include <Qt>

#include "../defs.h"
#include "../../utils/utils.h"
#include "../../state/state.h"
#include <string>

#include <QGestureEvent>

Button::Button(QWidget * parent,
    const QString& text): 
QAbstractButton(parent),
  text_(text)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setSizePolicy(Qtw.QSizePolicy.Ignored, Qtw.QSizePolicy.Ignored);
    setMinimumHeight(10);
    connect(this, &QAbstractButton::toggled, this, &Button::warnState);
    connect(this, &QAbstractButton::pressed, this, &Button::uncheckableWarnState);
    connect(this, &QAbstractButton::released, this, &Button::uncheckableWarnState);

}

void Button::setCheckable(bool c){
    disconnect(this, &QAbstractButton::pressed, this, &Button::uncheckableWarnState);
    disconnect(this, &QAbstractButton::released, this, &Button::uncheckableWarnState);

    QAbstractButton::setCheckable(c);
}

void Button::setHoldable(bool h){
    if(h){
        this->installEventFilter(this);
        grabGesture(Qt::TapAndHoldGesture);
        holdable = true;
    }else{
        holdable=false;
        this->removeEventFilter(this);
        ungrabGesture(Qt::TapAndHoldGesture);
    }
}

bool Button::eventFilter(QObject *object, QEvent *ev)
{
    if (ev->type() == QEvent::Gesture)
    {
        QGestureEvent * gevent = (QGestureEvent *) ev;
        if(auto g = qobject_cast<QTapAndHoldGesture *>(gevent->gesture(Qt::TapAndHoldGesture))){
            if (g->state() == Qt::GestureFinished){
                // std::cout<<"Tap and hold detected \n";
                held=!held;
                // cout<<"held = " << held << endl;
                setDown(false);
                warnState(held);
            }
        }
    }
    return false;
}


bool Button::isHoldable(){
    return holdable;
}

bool Button::isHeld(){
    return held;
}


void Button::setText(string text){
    text_ = QString::fromStdString(text);
    stateKey = text;
    lowerWithoutSpaces(&stateKey);
}

void Button::setStateParamText(string text){
    stateKey = text;
    lowerWithoutSpaces(&stateKey);
    checkState();
}

void Button::warnState(int _){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    // cout << "warning state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) isChecked()*(!isHeld()) + 2*isHeld() << endl;

    State::params(stateKey)->setValue((float) isChecked()*(!isHeld()) + 2*isHeld());
    // repaint();
    // held = isChecked();
    // emit notifyHeld(held);
}

void Button::uncheckableWarnState(){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    // cout << "uncheckable, warning state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) isDown() << endl;

    State::params(stateKey)->setValue((float) isDown());
}

void Button::checkState(){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    
    
    // cout << "checking state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) *State::params(stateKey) << endl;
    // State::params(stateKey)->setValue(value() / maximum());
    setChecked(*State::params(stateKey));
}

// QColor Button::getBackgroundColor(){
//     return backgroundColor;
// }
// QColor Button::getForegroundColor(){
//     return foregroundColor;
// }


// void Button::setBackgroundColor(QColor c){
//     backgroundColor = c;
// }
// void Button::setForegroundColor(QColor c){
//     foregroundColor = c;
// }

string Button::getStyleFromPseudoState(string pseudostate){
    string s = this->styleSheet().toStdString();
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char x) { return std::isspace(x); }), s.end());
    vector<string> sSplitted = split(s, "}");
    string selector = "Button";
    if(pseudostate != "")
        selector += ":" + pseudostate;
    
    selector += "{";

    for(int i=0; i<sSplitted.size(); i++){
        if(sSplitted[i].find(selector) != string::npos){
            // cout << "Found " << selector <<endl;
            // cout << split(sSplitted[i], selector)[1] <<endl;
            return split(sSplitted[i], selector)[1];
        }
    }
    return "";
}

void setColorFromStyle(QColor * color, string style, string colorSelector){
    // string.erase(std::remove_if(string.begin(), string.end(), [](unsigned char x) { return std::isspace(x); }), string.end());
    style.erase(std::remove_if(style.begin(), style.end(), [](unsigned char x) { return std::isspace(x); }), style.end());
    vector<string> styleSplitted = split(style, ";");

    for(string line : styleSplitted){
        
        vector<string> lineSplitted = split(line, ":");
        if(lineSplitted[0] == colorSelector && lineSplitted.size() > 0){
            *color = QColor(QString::fromStdString(lineSplitted[1]));
            // cout << colorSelector << " : " <<   lineSplitted[1] << endl;
            return;
        }
        // cout << lineSplitted[0] <<endl;
    }
}

void Button::getColorFromStyleSheet(){
    string style = "";

    pointColor = QColor("black");
    // bgColor = QColor("white");
    bgColor = QColor("red");

    if(isHoldable() && isHeld()){
        style = getStyleFromPseudoState("Held");
    }
    else if(isDown()){
        style = getStyleFromPseudoState("Pressed");
    }else if(isChecked()){
        style = getStyleFromPseudoState("Checked");
    }else{
        style = getStyleFromPseudoState("");
    }
    if(style != ""){

        // cout << "get bg color\n";
        setColorFromStyle(&bgColor, style, "background-color");
        // cout << "get fg color\n";
        setColorFromStyle(&pointColor, style, "color");

        // vector<string> styleSplitted = split(style, ";");
        // // cout << "style is : " << style << endl;
        // for(int i=0; i<styleSplitted.size(); i++){
        //     // cout << "stylesplitted[" << i << "] = " << styleSplitted[i] << endl;
        //     if(styleSplitted[i].find("background") != string::npos){
        //         vector<string> colorsSplitted = split(styleSplitted[i], " ");
        //         // cout << "bgcolor found, bgcolor = " << colorsSplitted[colorsSplitted.size() - 1] << endl;
        //         bgColor = QColor(QString::fromStdString(colorsSplitted[colorsSplitted.size() - 1]));
        //     }
        //     if((styleSplitted[i].find("color") != string::npos) &&
        //         (styleSplitted[i].find("background") == string::npos)){
        //         vector<string> colorsSplitted = split(styleSplitted[i], " ");
        //         // cout << "ptcolor found, ptcolor = " << colorsSplitted[colorsSplitted.size() - 1] << endl;
        //         pointColor = QColor(QString::fromStdString(colorsSplitted[colorsSplitted.size() - 1]));
        //     }
        // }
    }
    
}

void Button::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setBrush(QColor("black"));
    // p.setPen(QPen(QColor("red")));

    p.setBackgroundMode(Qt::OpaqueMode);
    p.setRenderHint(QPainter::Antialiasing);

    // cout << "button paintevent, width = " << this->width() << endl;
    // cout << "button paintevent, parent width = " << QAbstractButton::width() << endl;

    // QColor bgColor = p.background().color();
    // QColor pointColor = QColor(p.pen().color());

    getColorFromStyleSheet();

    // ensurePolished();

    // bgColor = getBackgroundColor();
    // pointColor = getForegroundColor();

    // cout << "bgColor = " << bgColor.rgb() << endl;
    

    p.setBrush(QBrush(bgColor));
    p.setPen(QPen(pointColor));

    float w = this->width();
    float h = this->height();
    // p.setPen(QPen(QColor("red")));
    p.drawRect(0, 0, this->width(), this->height());

    // draw text
    p.setPen(pointColor);
    p.setBackgroundMode(Qt::TransparentMode);
    p.drawText(QRectF(0, 0, w, h), Qt::AlignHCenter| Qt::AlignVCenter,  text_);
}