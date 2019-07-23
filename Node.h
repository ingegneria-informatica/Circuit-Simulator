//
// Created by Sierra on 2019-06-13.
//

#ifndef FIRSTSIMULATORTEST_NODE_H
#define FIRSTSIMULATORTEST_NODE_H

#include <QMouseEvent>
#include <vector>
#include <QGraphicsItem>
#include "NodeObserver.h"
#define NodeSize 10
#define nodeGridSize 20

class Component;

class NodeObserver;

class Node: public QGraphicsItem{
public:
    Node(float x, float y,bool isGround=false);
    ~Node();
    void connect(Component *c);
    void disconnect(Component *c);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem*,QWidget* ) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* ) override;
    void setVoltage(float value);
    float getVoltage();
    static QPointF toGrid(QPointF n);
    void checkLink();

    void setObserver(NodeObserver *o);
    std::list<Component*> getComponents();
    bool isGround();

    bool operator==(Node& a);

private:
    float voltage;
    std::list<Component*> components;
    NodeObserver* observer;
    bool gnd;
};


#endif //FIRSTSIMULATORTEST_NODE_H
