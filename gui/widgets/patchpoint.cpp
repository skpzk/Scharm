#include "patchpoint.h"

#include <QSize>
#include <QFontMetrics>
#include <QMouseEvent>

#include <cmath>
#include <set>

#include "../sections/patchbay.h"
#include "../utils.h"

#include "../../utils/utils.h"

std::set<std::string> implementedPps = {
	"VCA EG", "VCA",
	"VCO 1", "VCO 1 SUB 1", "VCO 1 SUB 2", 
	"VCO 2", "VCO 2 SUB 1", "VCO 2 SUB 2",
	"NOISE", "VCO 1 SUB", "VCO 2 SUB", 
	"VCO 1 PWM", "VCO 2 PWM", "CUTOFF", 
	"VCF EG", "CLOCK"};

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

	pcs = new PatchCordList;

	hasPcGrabbed = false;

	setMouseTracking(true);

}

string Patchpoint::getName(){
	string name = text_.toStdString();
	lowerWithoutSpaces(&name);
	return name;
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

QPointF Patchpoint::getCenter(){
	computeCenter();
	return this->mapToParent(center.toPoint());
}
void Patchpoint::setPcToHoveredIfCursorOnPp(QMouseEvent * event){
	if(distance(event->pos(), center) < mainRadius){
		for(int i=0; i<pcs->pcs.size(); i++){
			// cout << "is hovered" << endl;
			pcs->pcs[i]->isHovered = true;
			pcs->pcs[i]->repaint();
		}
	}else{
		for(int i=0; i<pcs->pcs.size(); i++){
			// cout << "is not" << endl;
			// print("is not")
			pcs->pcs[i]->isHovered = false;
			pcs->pcs[i]->repaint();
		}
	}
}

void Patchpoint::enterEvent(QEvent *ev){
	// cout << "Enter event\n";
	setPcToHoveredIfCursorOnPp((QMouseEvent*) ev);
}

void Patchpoint::leaveEvent(QEvent *ev){
	// cout << "Leave event\n";
	for(int i=0; i<pcs->pcs.size(); i++){
		// cout << "is not" << endl;
		// print("is not")
		pcs->pcs[i]->isHovered = false;
		pcs->pcs[i]->repaint();
	}
}

void Patchpoint::mousePressEvent(QMouseEvent *ev){
// def mousePressEvent(self, event: QtGui.QMouseEvent) -> None:
	// cout << "mouse press   detected" << text_.toStdString() << endl;

	if((this->pcs->is_empty()) || (ev->modifiers() == Qt::ControlModifier)){
		// cout << "Pb crate pc\n";

		GuiPatchbay *parent = (GuiPatchbay*) this->parent();
		parent->createPC(this);

		hasPcGrabbed = true;
	}else if(!(this->pcs->is_empty())){
		// elif self has a pc, disconnect it from self

		// cout << "removing pc\n";

		PatchCord *pc = pcs->last();
		pcs->remove(pc);

		GuiPatchbay *parent = (GuiPatchbay*) this->parent();
		parent->movePC(pc, this);

		hasPcGrabbed = true;

	}

/*
	else if(!self.pcs.is_empty()){
		// elif self has a pc, disconnect it from self
		pc = self.pcs.last()
		self.pcs.remove(pc)
		self.parent().movePC(pc, self)
		self.hasPcGrabbed = True
	}
	// else : ignore
*/
}
void Patchpoint::mouseMoveEvent(QMouseEvent *ev){
	// cout << "mouse move detected :" << text_.toStdString() << endl;
	if(ev->buttons() == Qt::LeftButton && hasPcGrabbed){
		// send mouse position to pb
		GuiPatchbay *parent = (GuiPatchbay*) this->parent();
		parent->moveLastPC(this->mapToParent(ev->pos()));
	}else{
		// set pc to hovered if the mouse in on the pp
		// cout << "is Pc hovered ?\n"; 
		setPcToHoveredIfCursorOnPp(ev);
	}
}
void Patchpoint::mouseReleaseEvent(QMouseEvent *ev){
	if(hasPcGrabbed){
		hasPcGrabbed = false;
		GuiPatchbay *parent = (GuiPatchbay*) this->parent();
		// parent->moveLastPC(this->mapToParent(ev->pos()));
		parent->findReleasePp(this->mapToParent(ev->pos()));
	}
}
/*
# detect pp of release
# if it exist and is of the good io type, connect pc to pp
# if not, delete pc
*/
tuple<bool, Patchpoint*> Patchpoint::isMouseReleaseOnPp(QPointF p){
	if(distance(getCenter(), p) < mainRadius){
		return make_tuple(true, this);
	}
	return make_tuple(false, this);
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

bool Patchpoint::operator==(const Patchpoint& rhs){
	return (text_ == rhs.text_) && (ioType == rhs.ioType);
	// return true;
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

	int alpha = 100;

	GuiPatchbay *parent = (GuiPatchbay*) this->parent();

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
	painter.setPen(QPen(pointColor));

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
	// std::cout << this->text_.toStdString() << endl;
	// std::cout << (implementedPps.find(this->text_.toStdString()) == implementedPps.end()) << std::endl;
	if(implementedPps.find(this->text_.toStdString()) == implementedPps.end()){
		painter.setBrush(QBrush(QColor("#999999")));
	}
	painter.drawEllipse(QPointF(center_x, center_y_pp), radius_outer, radius_outer);

	// draw inner circle
	
	if(implementedPps.find(this->text_.toStdString()) == implementedPps.end()){
		painter.setBrush(QBrush(QColor("#111111")));
	}
	
	float radius_inner = mainRadius * .5;
	// painter.setBrush(QBrush(pointColor))
	// painter.setBrush(QColor(ppColor));
	painter.drawEllipse(QPointF(center_x, center_y_pp), radius_inner, radius_inner);

	// print("self.center from paintEvent :", self.center)


}