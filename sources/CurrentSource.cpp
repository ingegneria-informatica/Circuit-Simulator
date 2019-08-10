//
// Created by cipher on 13/07/19.
//

#include "CurrentSource.h"
#include "ResourceManager.h"

void CurrentSource::paint(QPainter *painter, const QStyleOptionGraphicsItem *qg, QWidget *qw) {

    painter->setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    Component::paint(painter,qg,qw);
}

CurrentSource::CurrentSource(float value):ActiveComponent(0,1,value) {
    pixmap=ResourceManager::getImage(Component::types::currentSource);
    current=value;
    this->value=value;
    unit="A";
}

void CurrentSource::setCurrent(float value) {
    current=current;
    //FIXME should be zero when it returns the half bug
}