#include "section.h"
#include "../utils.h"

Section::Section(QWidget *parent, const QString& text):
QWidget(parent)
{
	cout << "creating section class\n";
	// QWidget *box = new QWidget;
	// widgetConf(this);
	QWidget::setContentsMargins(0, 0, 0, 0);

	SectionLabel *label = new SectionLabel(this, text);

	label->setAlignment(Qt::AlignCenter);
	widgetConf(label);

	vbox = new QVBoxLayout;
	layoutConf(vbox);
	this->setLayout(vbox);
	vbox->addWidget(label);
}