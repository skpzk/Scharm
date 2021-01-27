#include "defs.h"

string knobColors(knobType type){
    switch (type)
    {
    case general:
        return "#009acd";
        break;
    case seq:
        return "#00b2ee";
        break;
    case filter:
        return "#bf3eff";
        break;
    case rhythm:
        return "#ff0000";
        break;
    case env:
        return "#00cd66";
        break;
    case vco1:
        return "#00ee76";
        break;
    case vco2:
        return "#ff8c00";
        break;
    default:
        return "#000000";
        break;
    }
};

QString defaultKnobStyleSheet(knobType type){
    string style = "qproperty-ringColor: ";
    style.append(knobColors(type));
    style.append(";");
    return QString::fromStdString(style);
};