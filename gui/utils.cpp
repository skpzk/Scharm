#define _USE_MATH_DEFINES
#include <cmath>

#include "utils.h"

void widgetConf(QWidget *widget){
	widget->setContentsMargins(0, 0, 0, 0);
}

void layoutConf(QHBoxLayout *layout){
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
}

void layoutConf(QVBoxLayout *layout){
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
}

float rad2deg(float rad){
	return rad * 180. / M_PI;
}
