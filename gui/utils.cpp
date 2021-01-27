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
