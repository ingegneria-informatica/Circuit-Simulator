//
// Created by Sierra on 2019-07-29.
//

#ifndef FIRSTSIMULATORTEST_VCCS_H
#define FIRSTSIMULATORTEST_VCCS_H

#include "Component.h"

class VCCS : public Component{
public:
    VCCS(float value,Component *d);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final;
};



#endif //FIRSTSIMULATORTEST_VCCS_H
