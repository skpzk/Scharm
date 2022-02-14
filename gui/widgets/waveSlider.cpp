#include "waveSlider.h"

#include <QSizePolicy>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QVector>

#include "../utils.h"
// #include "../utils.h"
#include "../../utils/utils.h"
#include "../../state/state.h"

#include <cmath>

using namespace::std;


QPainterPath compute_tri_path(float x0, float y0, float width, float height){
	float amplitude = height / 2;

	float distance_x = width / 2;

	QPointF points[2];

	QPointF p1 = QPointF(x0, y0 + height);
	points[0] = QPointF(x0 + distance_x, y0);
	points[1] = QPointF(x0 + 2 * distance_x, y0 + height);

	QPainterPath path = QPainterPath(p1);

	for(int i=0; i<2; i++){
		path.lineTo(points[i]);
    }
	return path;
}

QPainterPath compute_saw_path(float x0, float y0, float width, float height){

	QPointF points[2];

	QPointF p1 = QPointF(x0, y0 + height);
	points[0] = QPointF(x0 + width, y0);
	points[1] = QPointF(x0 + width, y0 + height);

	QPainterPath path = QPainterPath(p1);

	for(int i=0; i<2; i++){
		path.lineTo(points[i]);
    }
	return path;
}

QPainterPath compute_sqr_path(float x0, float y0, float width, float height){
    float amplitude = height / 2;

    float y_middle = y0 + amplitude;
	float distance_x = width / 2;

	QPointF points[3];

	QPointF p1 = QPointF(x0, y_middle + amplitude);
	points[0] = QPointF(x0, y0);
	points[1] = QPointF(x0 + 2 * distance_x, y0);
	points[2] = QPointF(x0 + 2 * distance_x, y_middle + amplitude);

	QPainterPath path = QPainterPath(p1);

	for(int i=0; i<3; i++){
		path.lineTo(points[i]);
    }
	return path;
}

QPolygonF compute_arrow(float x0, float y0, float width, float height){

    float c = min(width, height / cos(deg2rad(30)));

    float y_middle = y0 + height / 2;
    float x_middle = x0 + width / 2;

    float distance_y = c * cos(deg2rad(30));

    QVector<QPointF> points;

	QPointF p1 = QPointF(x_middle - c / 2, y_middle - distance_y / 2);
	points.push_back(QPointF(x_middle + c / 2, y_middle - distance_y / 2));
	points.push_back(QPointF(x_middle, y_middle + distance_y / 2));
    points.push_back(p1);

	QPolygonF poly = QPolygonF(points);

	return poly;

}

WaveSlider::WaveSlider(int nb_items, int dir): QSlider(nullptr){
	steps = nb_items;
	setMaximum(nb_items - 1);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	handlePos = QPointF(0, 0);
	barRect = QRectF(0, 0, 0, 0);

	this->direction = dir;

	// stateKey = "";
	// self.valueChanged.connect(self.warnState)

	setValue(0);

    connect(this, &QSlider::valueChanged, this, &WaveSlider::warnState);
}

void WaveSlider::setStateParamText(string text){
    stateKey = text;
    lowerWithoutSpaces(&stateKey);
    checkState();
}

void WaveSlider::warnState(int _){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    // cout << "warning state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) value() / maximum() << endl;
    State::params(stateKey)->setValue((float) value());
}

void WaveSlider::checkState(){
    // # mprint("key =", self._stateKey, "Statevalue = ", State.params[self._stateKey], "value = ", self.value(), "Warning state")
    // # print("value = ", self.value())
    // cout << "checking state :\n";
    // cout << "key = " << stateKey << ", value = " << (float) *State::params(stateKey) << endl;
    // State::params(stateKey)->setValue(value() / maximum());
    setValue(*State::params(stateKey));
}

int WaveSlider::getMousePositionInSteps(QPoint mousePos){
	float dist = abs(barRect.y() + barRect.height() - mousePos.y());
	return floor(dist / barRect.height() * steps);
}

void WaveSlider::mousePressEvent(QMouseEvent *ev){
	// if mouse is on handle : grab and move the handle
	// else : directly move handle to the position of the mouse
	if(barRect.contains(ev->pos())){
		this->setValue(this->getMousePositionInSteps(ev->pos()));
	}
}

void WaveSlider::mouseMoveEvent(QMouseEvent *ev){
	if(barRect.contains(ev->pos())){
		this->setValue(this->getMousePositionInSteps(ev->pos()));
	}
}


void WaveSlider::mouseReleaseEvent(QMouseEvent *ev){
	;
}

void WaveSlider::paintEvent(QPaintEvent*){

    QPainter p(this);

    QColor pointColor = QColor(p.pen().color());
    QColor bgColor = QColor(p.background().color());
    bgColor.setNamedColor("transparent");

    p.setBrush(QBrush(bgColor));

    // compute size, depending on available space and nb of items

    // width represents the width of the rectangle containing the handle
    // height = steps * width
    // width = min(self.width / 2, self.height/steps)

    float pwidth = min(this->width()/2, this->height() / steps);

    float height = pwidth * steps;
    // draw outer rect

    float outer_rect_x = 0;
    float outer_rect_y = 0;

    if(direction == 1){
        // cout << "Direction = 1\n";
        outer_rect_x = this->width() - pwidth;
        outer_rect_y = 0;
    }
    QRectF rect = QRectF(outer_rect_x, outer_rect_y, pwidth-1, height);

    p.drawRect(rect);
    // p.drawText(QtCore.QPointF(0, 70), str(self.value()));

    p.setRenderHint(QPainter::Antialiasing);
    // draw handle
    p.setBrush(QBrush(pointColor));

    float margin = pwidth * 1 / 9;

    float x0_handle = 2 * margin + outer_rect_x;
    float y0_handle = 2 * margin + outer_rect_y;

    // cout << "Value = " << this->value() << endl;
    // cout << "max - Value = " << this->maximum() - this->value() << endl;
    // cout << "posy = " << ((float) (this->maximum() - this->value())) / (this->maximum() + 1) << endl;

    float y_handle = ((float) (this->maximum() - this->value())) / (this->maximum() + 1) * height;
    float w_handle = pwidth - 4 * margin;
    float h_handle = pwidth - 4 * margin;

    QRectF handleRect = QRectF(x0_handle, y0_handle + y_handle, w_handle,
                                h_handle);
    p.drawRect(handleRect);
    
    QPointF handlePos = QPointF(x0_handle, y0_handle);
    
    barRect = QRectF(outer_rect_x, outer_rect_y, 2 * pwidth, height);
    if(direction == 1){
        barRect = QRectF(outer_rect_x - pwidth, outer_rect_y, 2 * pwidth, height);
    }
    // draw waves indicators :
    // sqr, saw, between sqr and saw, tri, sin

    p.setBrush(QBrush(bgColor));
    margin = pwidth * 2 / 9;
    // print("margin = ", margin)
    float width_w_indic = pwidth - 2 * margin;
    float height_w_indic = pwidth - 2 * margin;

    float x0_w = pwidth + margin;
    // cout << "Direction = " << direction << endl;
    if(direction == 1){
        x0_w = outer_rect_x - pwidth + margin;
    }
    float y0_w = 0 + margin;

    // sin wave :
    float radius_sine_wave_indic = width_w_indic / 2;
    p.drawEllipse(QPointF(x0_w + radius_sine_wave_indic, y0_w + radius_sine_wave_indic), radius_sine_wave_indic,
                    radius_sine_wave_indic);

    // tri wave :
    QPainterPath path = compute_tri_path(x0_w, y0_w + pwidth, width_w_indic, height_w_indic);
    p.drawPath(path);

    // saw wave :
    path = compute_saw_path(x0_w, y0_w + 2 * pwidth, width_w_indic, height_w_indic);
    p.drawPath(path);

    // btw sqr and saw wave :
    QPolygonF poly = compute_arrow(x0_w, y0_w + 3 * pwidth, width_w_indic, height_w_indic);
    p.drawPolygon(poly);

    // square wave :
    p.setBrush(QBrush(bgColor));
    path = compute_sqr_path(x0_w, y0_w + 4 * pwidth, width_w_indic, height_w_indic);
    p.drawPath(path);

    // p.drawLine(QtCore.QPointF(self.width() / 2, 0), QtCore.QPointF(self.width() / 2, self.height()))
}