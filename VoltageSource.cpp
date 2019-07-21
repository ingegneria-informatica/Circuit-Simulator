//
// Created by cipher on 13/07/19.
//

#include "VoltageSource.h"

void VoltageSource::paint(QPainter* painter, const QStyleOptionGraphicsItem* qg, QWidget* qw) {

    painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    Component::paint(painter,qg,qw);
}

VoltageSource::VoltageSource(float value):Component(1,0,value) {
    voltage=value;
    QPixmap pic(":/images/voltagesource.png");
    setPixmap(pic);
}