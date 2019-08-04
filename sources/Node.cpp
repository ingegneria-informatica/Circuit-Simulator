//
// Created by Sierra on 2019-06-13.
//
#include "Node.h"
#include "Component.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <iostream>
#include <cmath>
#define FLT_EPSILON 0.001
#define nodeOnTop 200

Node::Node(float x, float y,bool isGround):observer(nullptr),voltage(0),gnd(isGround){
    setAcceptHoverEvents(true);
    //if(!gnd)
    //setOpacity(0.01);
    setZValue(nodeOnTop);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setX(x);
    setY(y);
}

Node::Node(QPointF point, bool isGround):observer(nullptr),voltage(0),gnd(isGround){
    setAcceptHoverEvents(true);
    //if(!gnd)
    //setOpacity(0.01);
    setZValue(nodeOnTop);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setX(point.x());
    setY(point.y());
}

int Node::type() const {
    return Component::node;
}

Node::~Node(){
    if(observer!= nullptr)
        observer->removeNotify(this);

    std::list<Component*> toDelete=components;
    for(auto &component : toDelete) {
        delete component;
    }
}

void Node::connect(Component *c) {
    components.push_back(c);
    //setParentItem(c);
}

void Node::disconnect(Component *c){
    components.remove(c);
}

bool Node::operator==(Node& n) {
    return qAbs(x() - n.x()) < (NodeSize) && qAbs(y() - n.y()) < (NodeSize);
}

void Node::setObserver(NodeObserver *o){
    observer=o;
}

std::list<Component*> Node::getComponents() {
    return components;
}

QRectF Node::boundingRect() const {
    if (gnd)
        return QRectF( -NodeSize*3-10, -NodeSize*3-10, NodeSize*6+200, NodeSize*6+50); //200 50 for rect
    return QRectF( -NodeSize/2-10, -NodeSize/2-10, NodeSize+200, NodeSize+50);
}

QPainterPath Node::shape() const {
        QPainterPath path;
        QPolygon polygon;
        polygon << QPoint(-20,-20);
        polygon << QPoint(+20,-20);
        polygon << QPoint(+20,+20);
        polygon << QPoint(-20,+20);
        path.addPolygon(polygon);
        return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(gnd) {
        QPixmap pixmap(":/images/ground.png");
        painter->drawPixmap(-15, 0, 30, 20, pixmap);  //image as it is
        painter->setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(-NodeSize / 2, -NodeSize / 2, NodeSize, NodeSize);
    }
    if(hovering){
        //painter->translate(QPointF(this->x(),this->y()));
        QPainterPath path;
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        path.addRoundedRect(QRectF(10,10-17.5,100,30),10,10);
        painter->fillPath(path,QColor(255, 189, 189));
        painter->drawPath(path);
        painter->drawText(QPointF(20,10), "Voltage:"+QString().number(voltage));
    }
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    for (auto component : components) {
        prepareGeometryChange();
        component->update();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    this->setPos(Node::toGrid(this->pos()));
    checkLink();
    for (auto component : components) {
        prepareGeometryChange();
        component->update();
    }
}

void Node::checkLink(){
    observer->update(*this);
}

void Node::setVoltage(float value) {
    if(abs(value)<FLT_EPSILON)
        voltage=0;
    else
        voltage=value;
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    std::cout<<"Node Voltage:"<<voltage<<std::endl;
    QGraphicsItem::mousePressEvent(event);
}

bool Node::isGround(){
    return gnd;
}

float Node::getVoltage() {
    return voltage;
}

QPointF Node::toGrid(QPointF n){
    n.setX(((((int)n.x())+nodeGridSize/2)/nodeGridSize)*nodeGridSize);
    n.setY(((((int)n.y())+nodeGridSize/2)/nodeGridSize)*nodeGridSize);
    return n;
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hovering=true;
    update();
    //setOpacity(1);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    hovering=false;
    update();
    //if(!gnd)
    //setOpacity(0.01);
}

void Node::setGnd(bool value) {
    gnd=value;
}