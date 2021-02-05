#include "patchcord.h"

#include <stdlib.h>     /* srand, rand */
#include <cmath>

#include <tuple>

#include <QPen>
#include <QPainterPath>

#include "../utils.h"

vector<string> pcColors = {"#009acd",
                     "#cd0000",
                     "#ee2c2c",
                     "#eeb422",
                     "#ffd700",
                     "#00b2ee",
                     "#68228b",
                     "#ff8c00",
                     "#228b22",
                     "#00ee76",
                     "#ff69b4"};

vector<string> PatchCord::colors;

QPointF compute_point_on_segment(QPointF A, QPointF B, float ratio){
	/*
	returns point at distance ratio * distance(AB) from point A in the direction of point B
	*/
	float cos1 = B.x() - A.x();
	float sin1 = B.y() - A.y();

	float theta1 = atan2(sin1, cos1);
	float d = distance(A, B);
    // cout << "theta = " << theta1 << ", sin(theta) = " << sin(theta1) << endl;
    // cout << "distance = " << d << endl;
    // cout << "ratio    = " << ratio << endl;

    // cout << "y offset = " << ratio * d * sin(theta1) << endl;
	return QPointF(A.x() + ratio * d * cos(theta1), A.y() + ratio * d * sin(theta1));
}

tuple<QPointF, QPointF, QPointF> compute_control_points(QPointF A, QPointF B){
/*
def compute_control_points(A, B) -> (QtCore.QPoint, QtCore.QPoint, QtCore.QPoint):
	/*
	computes the coordinates of the control points to create a bezier curve
	the curve mimics a quadratic curve
	*/
	QPointF middlePoint = QPointF(((float) A.x() + B.x())/2., max(A.y(), B.y()) + 50);
	// print("M :", middlePoint)

	// the distance of 2/3 creates a quadratic curve

	QPointF c1 = compute_point_on_segment(A, middlePoint, 2./3);
	QPointF c2 = compute_point_on_segment(B, middlePoint, 2./3);
	// print("c1 :", c1)
	// print("c2 :", c2)

	return make_tuple(c1, c2, middlePoint);

}

PatchCord::PatchCord(QWidget* parent):
QWidget(parent){
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    startPoint = QPointF(0, 0);
    endPoint = QPointF(0, 0);
    isHovered = true;

    float testConstructor = QWidget::height();

    cout << "colors length = " << PatchCord::colors.size() << endl;

    if(PatchCord::colors.size() == 0){
        // PatchCord::colors = pcColors.copy();
        copy(pcColors.begin(), pcColors.end(), back_inserter(PatchCord::colors)); 
    }
    int colorNum = rand() % PatchCord::colors.size();
    string test = PatchCord::colors[colorNum];
    this->color = QColor(test.c_str());
    PatchCord::colors.erase(PatchCord::colors.begin() + colorNum);

    this->raise();
    this->show();

    cout << "pc created\n";
}

void PatchCord::setPos(Patchpoint* pp){
    endPoint = pp->getCenter();
    repaint();
}
void PatchCord::setPos(QPointF pp){
    endPoint = pp;
    repaint();
}
void PatchCord::setStartPp(Patchpoint* pp){
    startPoint = pp->getCenter();
    startPp = pp;
    if(pp->ioType == "in"){
        inPp = pp;
        endPoint_io = "out";
    }else{
        outPp = pp;
        endPoint_io = "in";
    }
    repaint();
}
void PatchCord::setEndPp(Patchpoint* pp){
    endPoint = pp->getCenter();
    endPp = pp;
    if(pp->ioType == "in"){
        inPp = pp;
    }else{
        outPp = pp;
    }

}
void PatchCord::connectPc(){
    inPp->pcs->add(this);
    outPp->pcs->add(this);
    // Warn state !!
}
void PatchCord::disconnectPc(Patchpoint* pp){
    // Warn state !!
    if(pp->ioType == "out"){
        endPp = outPp;
        endPoint = outPp->getCenter();

        startPp = inPp;
        startPoint = inPp->getCenter();

        outPp = nullptr;
    }else{
        endPp = inPp;
        endPoint = inPp->getCenter();

        startPp = outPp;
        startPoint = outPp->getCenter();

        inPp = nullptr;
    }
    endPoint_io = pp->ioType;
    repaint();
}

/*
def setPoints(self, **kwargs):
    for key, value in kwargs.items():

        if key == 'startpp' and isinstance(value, Patchpoint):
            # cprint("set start pp")
            self.startPoint = value.getCenter()
            self.startPp = value
            if value.io == 'in':
                self.inpp = value
                self.endPointIo = 'out'
            else:
                self.outpp = value
                self.endPointIo = 'in'

        if key == 'pos' and isinstance(value, Patchpoint):
            # print("set loose end pos")
            self.endPoint = value.getCenter()

        if key == 'pos' and (isinstance(value, QtCore.QPointF) or isinstance(value, QtCore.QPoint)):
            # print("set loose end pos")
            self.endPoint = value

        if key == 'endpp' and isinstance(value, Patchpoint):
            # cprint("set end pp")
            self.endPoint = value.getCenter()
            self.endPp = value
            if value.io == 'in':
                self.inpp = value
            else:
                self.outpp = value
*/

// void PatchCord::resize(QSize qs){
//     cout << "Pc::resize called !\n";
//     // this->setFixedSize(qs);
//     // size = qs;

//     QSize s = QSize(100, 100);

//     float test = QWidget::height();

//     QWidget::resize(s);
//     // get_start_and_end_points();
// }

void PatchCord::resizeEvent(QPaintEvent* ev){
    cout << "Pc::resizeEvent called !\n";
    get_start_and_end_points();
}

void PatchCord::get_start_and_end_points(){
    if(startPp != nullptr){
        startPoint = startPp->getCenter();
    }
    if(endPp != nullptr){
        endPoint = endPp->getCenter();
    }
}

void PatchCord::deleteFromPpLists(){
    if(inPp != nullptr){
        inPp->pcs->remove(this);
    }
    if(outPp != nullptr){
        outPp->pcs->remove(this);  
    }
}

void PatchCord::paintEvent(QPaintEvent*){

    // cout << "Pc paint event\n";

    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(QColor("black"), 1);
    pen.setCapStyle(Qt::RoundCap);
    
    p.setPen(pen);

    QPainterPath path = QPainterPath(startPoint);

    QPointF c1, c2, m;
    
    tie(c1, c2, m) = compute_control_points(startPoint, endPoint);

    if(startPoint != endPoint){
        // cout << "end != start\n";
        path.cubicTo(c1, c2, endPoint);
        // cout << "startpoint = (" << startPoint.x() << ", " << startPoint.y() << ")" << endl;
        // cout << "m          = (" << m.x() << ", " << m.y() << ")" << endl;
        // cout << "c1         = (" << c1.x() << ", " << c1.y() << ")" << endl;
        // cout << "c2         = (" << c2.x() << ", " << c2.y() << ")" << endl;

    }else{
        // cout << "end = start\n";
        // cout << "startpoint = (" << startPoint.x() << ", " << startPoint.y() << ")" << endl;
        // cout << "m          = (" << m.x() << ", " << m.y() << ")" << endl;
        path.lineTo(m);
    }
    float alpha;
    if (isHovered){
        alpha = 255;
    }else{
        alpha = 100;
    }
    QColor bgColor = QColor(0, 0, 0, alpha);
    // cout << "Alpha = " << alpha << endl;
    // QColor bgColor = QColor(0, 0, 0);

    pen = QPen(bgColor, 11);
    pen.setCapStyle(Qt::RoundCap);

    p.setPen(pen);
    // p.setPen(QPen(QColor("red"), 11));
    p.drawPath(path);

//    p.drawRect(QRectF(startPoint.x(), startPoint.y(), 100, 100));



    color.setAlpha(alpha);

    pen.setColor(color);
    pen.setWidth(10);
    p.setPen(pen);
    p.drawPath(path);

    // p.setBrush(QColor("red"));
    // p.drawRect(0, 0, width(), height());

}