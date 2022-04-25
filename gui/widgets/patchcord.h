#ifndef WIDGETS_PATCHCORD_H
#define WIDGETS_PATCHCORD_H

#include <QWidget>
#include <QPointF>
#include <QColor>
#include <QSize>

#include <string>
#include <vector>
#include <iostream>

#include <QWidget>

class PatchCord;

#include "patchpoint.h"

using namespace::std;

class PatchCord: public QWidget{

    Q_OBJECT

    public:
        PatchCord(QWidget* parent=nullptr);

        void setPos(Patchpoint*);
        void setPos(QPointF);
        void setStartPp(Patchpoint*);
        void setEndPp(Patchpoint*);

        Patchpoint* getStartPp();
        Patchpoint* getEndPp();

        string endPoint_io = "";

        bool isHovered;

        void connectPc();
        void disconnectPc(Patchpoint*);

        void deleteFromPpLists();

        void resize(QSize);

    private:
        QPointF startPoint, endPoint;

        Patchpoint *startPp = nullptr;
        Patchpoint *endPp = nullptr;
        Patchpoint *inPp = nullptr;
        Patchpoint *outPp = nullptr;

        static vector<string> colors;
        QColor color;

        

        virtual void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent*);

        void get_start_and_end_points();
};

#endif /* WIDGETS_PATCHCORD_H */
