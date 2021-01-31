#include "patchpoint.h"

#include <QSize>
#include <QFontMetrics>

#include <cmath>
#include <set>

#include "../sections/patchbay.h"
#include "../utils.h"

std::set<std::string> implementedPps = {};

Patchpoint::Patchpoint(QWidget * parent,
    const QString& text, string ioType): 
QWidget(parent),
  text_(text),
  ioType(ioType)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setSizePolicy(Qtw.QSizePolicy.Ignored, Qtw.QSizePolicy.Ignored);
    setMinimumHeight(10);
    setMinimumSize(QSize(36, 36));

    hasFixedSize = false;
    hasFixedFontSize = false;
	fontsizefactor = .38;
    fixedFontSize = 0;
    mainRadius = 10;
    ppMargin = 3;
	ppColor = QColor("black");

    center = QPointF(0, 0);

}

void Patchpoint::setFixedSize(float size){
	fixedSize = size;
	hasFixedSize = true;

}
float Patchpoint::getFixedSize() const {
	return fixedSize;
}

void Patchpoint::setPpColor(QColor color){
	ppColor = color;
}

QColor Patchpoint::getPpColor() const{
	return ppColor;
}

void Patchpoint::computeCenter(){

	float width = this->width(), height = this->height();

	float center_x = width / 2.;
	float center_y = height / 2.;
	float center_y_pp;



	if(!this->hasFixedSize){
		if(!this->hasFixedFontSize){
			mainRadius = min((float) (width/2. - ppMargin), height/(2 + 2*fontsizefactor) - ppMargin);
			mainRadius = max(mainRadius, (float) 1.);
			center_y_pp = center_y + mainRadius * fontsizefactor;
		}else{
			mainRadius = min((float) (width/2. - ppMargin), (height - 2*fixedFontSize) / 2 - ppMargin);
			mainRadius = max(mainRadius, (float) 1.);
			center_y_pp = center_y + mainRadius * fixedFontSize;
		}
	}else{
		mainRadius = fixedSize / 2;
		mainRadius = max(mainRadius, (float) 1.);
		center_y_pp = center_y + mainRadius * fontsizefactor;
	}

	center = QPointF(center_x, center_y_pp);
}

QPolygonF Patchpoint::createPoly(int n_points, float radius, float center_x, float center_y){
	QPolygonF polygon;
	float w = 360. / ((float) n_points);

	for(int i=0; i<n_points; i++){
		float t = w*i;
		float x = radius * cos(deg2rad(t));
		float y = radius * sin(deg2rad(t));
		polygon.append(QPointF(center_x + x, center_y + y));
	}
	return polygon;
}

void Patchpoint::paintEvent(QPaintEvent* event){
	this->computeCenter();
	float center_x = center.x();
	float center_y_pp = center.y();
	float width = this->width();

	QPainter painter(this);

	// So that we can use the background color
	painter.setBackgroundMode(Qt::OpaqueMode);
	// Smooth out the circle
	painter.setRenderHint(QPainter::Antialiasing);
	// Use background color
	QColor textBgColor = QColor(painter.background().color());
	// print("bgcolor = ", bgColor)
	bgColor = QColor("transparent");
	QColor pointColor = QColor(painter.pen().color());

	// self.pointColor = pointColor
	// self.bgColor = textBgColor

	int alpha = 150;

	Patchbay *parent = (Patchbay*) this->parent();

	if((parent->displayPp == "all") || (parent->displayPp == this->ioType)){
		pointColor.setAlpha(255);
	}else{
		pointColor.setAlpha(alpha);
	}

	// draw text
	if(!hasFixedFontSize){
		fontsize = mainRadius * fontsizefactor;
	}else{
		fontsize = fixedFontSize;
	}
	
	QRectF textRect = QRectF(0, center_y_pp - mainRadius - 2 * fontsize, width, 2 * fontsize);
	QFont f = painter.font();
	f.setPointSizeF(fontsize);

	// self._io = 'in'
	if(ioType == "out"){
		QRectF fm = QFontMetrics(f).boundingRect(text_);
		// print("fm = ", fm)
		painter.setBrush(pointColor);
		painter.setPen(QPen(pointColor));
		painter.drawRect(QRectF(center_x - fm.width() / 2, center_y_pp - mainRadius - 2 * fontsize,
										fm.width(), fm.height()));
		painter.setPen(QPen(textBgColor));
	}

	painter.setFont(f);
	painter.setBackgroundMode(Qt::TransparentMode);
	painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, text_);

	// draw hexagon
	painter.setBrush(bgColor);
	painter.setPen(pointColor);
	painter.drawPolygon(this->createPoly(6, mainRadius, center_x, center_y_pp));


	// draw outer circle
	float radius_outer = mainRadius * .8;
	if(implementedPps.find(this->text_.toStdString()) == implementedPps.end()){
		painter.setBrush(QBrush(QColor("#999999")));
	}
	painter.drawEllipse(QPointF(center_x, center_y_pp), radius_outer, radius_outer);

	// draw inner circle
	float radius_inner = mainRadius * .5;
	// painter.setBrush(QBrush(pointColor))
	// painter.setBrush(QColor(ppColor));
	painter.drawEllipse(QPointF(center_x, center_y_pp), radius_inner, radius_inner);

	// print("self.center from paintEvent :", self.center)


}