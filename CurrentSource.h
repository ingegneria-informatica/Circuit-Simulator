//
// Created by cipher on 13/07/19.
//

#ifndef FIRSTSIMULATORTEST_CURRENTSOURCE_H
#define FIRSTSIMULATORTEST_CURRENTSOURCE_H


#include "Component.h"

class CurrentSource : public Component{

public:
    CurrentSource(float value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override  final;
    void setCurrent(float v) override final;
};


#endif //FIRSTSIMULATORTEST_CURRENTSOURCE_H
