#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

void widgetConf(QWidget *);
void layoutConf(QHBoxLayout *);
void layoutConf(QVBoxLayout *);

float rad2deg(float);
float deg2rad(float);
float distance(QPointF, QPointF);

#endif // GUIUTILS_H