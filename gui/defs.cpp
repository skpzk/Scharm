#include "defs.h"
#include "../utils/utils.h"


Theme::Theme(){
    darkTheme = true;
    // darkTheme = false;
    if(darkTheme){
        backgroundColor = "#111111";
        buttonBackground = "#111111";
        // buttonBackground = "red";
        pointColor = "white";
				activeColor = pointColor;
				activeColorTamed = "#aaaaaa";
				// pointColor = "red",
				inactiveColor = "black";
        barsColor = "#aaaaaa";
        sectionLabelColor = backgroundColor;
        ppColor = backgroundColor;
    }else{
        backgroundColor = "white";
        buttonBackground = "white";
        pointColor = "black";
        barsColor = "black";
        sectionLabelColor = "black";
        ppColor = "black";
				activeColorTamed = "black";
				activeColor = "black";
    }
}

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


QString buttonDefaultStyleSheet(){
    Theme theme;
	string styleSheet = 
		"Button{"
			"background-color: buttonBackground;"
			// "qproperty-backgroundColor: buttonBackground;"
			"border: 1px solid #aaa;"
			"color: buttonForeground;"
			// "color: red;"
			// "qproperty-foregroundColor: buttonForeground;"
		"}"
		"Button:Checked{"
			"background-color: #b4b4b4;"
			// "background-color: red;"
			// "qproperty-backgroundColor: orange;"
			"border: 0px;"
			"color: buttonForegroundChecked;"
			// "color: red;"
			// "qproperty-foregroundColor: buttonForegroundChecked;"
		"}"
		"Button:Pressed{"
			"background-color: #343434;"
			// "background-color: orange;"
			"border: 0px;"
		"}"
		"Button:Held{"
			"background-color: orange;"
		"}"
		"";

	styleSheet = replaceAll(styleSheet, "buttonBackground", theme.buttonBackground);
	styleSheet = replaceAll(styleSheet, "buttonForegroundChecked", theme.inactiveColor);
	styleSheet = replaceAll(styleSheet, "buttonForeground", theme.activeColorTamed);
	return QString::fromStdString(styleSheet);
}

QString mainWindowStyleSheet(){
    Theme theme;
	string styleSheet = 
	"QMainWindow{"
		"background-color: backgroundColor;"
	"}"
	"QGroupBox{"
		"border: 0px solid #414141;"
		"border-top: 1px solid pointColor;"
		"margin: 0px;"
		"margin-top: 10px;"
		"padding: 0px;"
		"padding-top: 0px;}"
	"QGroupBox::title{"
		"subcontrol-origin: margin;"
		"subcontrol-position: top;"
		"padding: 0 10px 0 10px;"
		"padding: 0 0px 0 0px;"
		"background-color: pointColor;"
		"font-size: 500pt;"
		"color: backgroundColor"
	"}"
	"Radio{"
		"color: pointColor;"
	"}"
	"QLabel{"
		"color: pointColor;"
	"}"
	"SectionLabel{"
		"color: sectionLabelColor;"
		"qproperty-barsColor: barsColorName;"
	"}"
	"WaveSlider{"
		"color: pointColor;"
	"}"
	"Patchpoint{"
		"qproperty-fixedSize: 36;"
		"color: pointColor;"
		"background-color: backgroundColor;"
		"qproperty-ppColor: ppColorName;"
	"}"
	"Knob{"
		"color: pointColor;"
		"background-color: backgroundColor;"
	"}";

    styleSheet = replaceAll(styleSheet, "backgroundColor", theme.backgroundColor);
    styleSheet = replaceAll(styleSheet, "pointColor", theme.pointColor);
    styleSheet = replaceAll(styleSheet, "barsColorName", theme.barsColor);
    styleSheet = replaceAll(styleSheet, "sectionLabelColor", theme.sectionLabelColor);
    styleSheet = replaceAll(styleSheet, "ppColorName", theme.ppColor);
    
	return QString::fromStdString(styleSheet);
}