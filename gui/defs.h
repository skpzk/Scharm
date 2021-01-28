#ifndef GUIDEFS_H
#define GUIDEFS_H

#include <QColor>
#include <string>
#include <QString>

using namespace::std;

enum knobType { general, filter, seq, rhythm, vco1, vco2, env};

string knobColors(knobType type);

QString defaultKnobStyleSheet(knobType type);
QString mainWindowStyleSheet();
QString buttonDefaultStyleSheet();

string replace(string base, string oldValue, string newValue);
string replaceAll(string base, string oldValue, string newValue);
vector<string> split(string stringToBeSplitted, string delimeter);

class Theme{
    public:
        Theme();
        bool darkTheme;
        string backgroundColor, 
            buttonBackground, 
            pointColor, 
            barsColor, 
	        sectionLabelColor,
	        ppColor;
};

#endif // GUIDEFS_H