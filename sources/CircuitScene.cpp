//
// Created by Sierra on 2019-06-23.
//

#include "CircuitScene.h"

#define sceneSize 1100

QColor CircuitScene::gridColor = QColor(237,237,237,125);

CircuitScene::CircuitScene(Circuit* c):circuit(c){

    myMode=moveItem;

    setSceneRect(0, 0, sceneSize, sceneSize);
    circuit->setObserver(this);

    for(int x=0; x<=sceneSize; x+=nodeGridSize)
        addLine(x,0,x,1000,QPen(gridColor));

    for(int y=0; y<=sceneSize; y+=nodeGridSize)
        addLine(0,y,sceneSize,y, QPen(gridColor));

    sceneMenu=new QMenu();
    createItemMenus();

}

void CircuitScene::addNotify(QGraphicsItem *item) {
    if(item->scene()!=this) {
        addItem(item);
    }
}

void CircuitScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    mousePressPoint=Node::toGrid(event->scenePos());
    QGraphicsItem *clicked=itemAt(mousePressPoint,QTransform());
    if(clicked->type()<Component::itemType::node) //FIXME togliere
    selecting=true;
    if(myMode==moveItem) {
        if(clicked->type()==Component::node) {
            clearSelection();
            clicked->setSelected(true);
        }
        QGraphicsScene::mousePressEvent(event);
    }
}

void CircuitScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    mousePressPoint=Node::toGrid(event->scenePos());
    exSel= itemAt(mousePressPoint,QTransform());
    if (exSel->type()>=Component::itemType::component)
        ((Component*)exSel)->contextMenu->exec(event->screenPos());
    else
        sceneMenu->exec(event->screenPos());
}

void CircuitScene::drawForeground(QPainter *painter, const QRectF &rect) {
    if (selecting) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(QRectF(mousePressPoint, mouseReleasePoint));
    }
    QGraphicsScene::drawForeground(painter,rect);
}

void CircuitScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    mouseReleasePoint = Node::toGrid(event->scenePos());
    if (event->button() == Qt::LeftButton) {
        if (myMode == insertItem) {
            if ((mousePressPoint - mouseReleasePoint).manhattanLength() < NodeSize) {
                QPoint defaultPos = QPoint(0, 100);
                mousePressPoint = mousePressPoint - defaultPos;
                mouseReleasePoint = mousePressPoint + defaultPos;
            }
            createComponent();
        }
        if (myMode == selectDependent) {
            QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
            if (clicked != nullptr)
                if (clicked->type() >= Component::component) {
                    selectedDependent = (Component *) clicked;
                    selectedDependent->update();
                    selectedDependent->setControlled();
                    myMode = insertItem;
                }
        }
        if (myMode == moveItem) {
            for(auto &item : selectedItems())
                if(item->type()==Component::node) {
                    ((Node*) item)->setPos(Node::toGrid(((Node*) item)->pos()));
                    ((Node*)item)->checkLink();
                }
            for(auto &item : selectedItems())
                //if selected with CMD
                if(item->type()>=Component::component) {
                    ((Component*)item)->getNodes().first->setSelected(true);
                    ((Component*)item)->getNodes().second->setSelected(true);
                }
            //shouldn't run together
            if(selecting) {
                QPainterPath path;
                path.addRect(QRectF(mousePressPoint, mouseReleasePoint));
                setSelectionArea(path);
                update();
                selecting=false;
            }
        }
    }
}

void CircuitScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(myMode==moveItem) {
        mouseReleasePoint = Node::toGrid(event->scenePos());
        update();//FIXME togliere
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void CircuitScene::keyPressEvent(QKeyEvent *event) {
    if(event->key()==Qt::Key::Key_C)
        circuit->print();
}

void CircuitScene::setType(Component::types type) {
    myType=type;
}

void CircuitScene::setMode(CircuitScene::modes mode) {
    myMode=mode;
}

void CircuitScene::setcValue(float v) {
    cValue=v;
}

Circuit* CircuitScene::getCircuit() {
    return circuit;
}

void CircuitScene::deleteItem() {
    if(exSel!=nullptr)
        delete exSel;
}

void CircuitScene::changeValue() {
    if (exSel->type()==Component::activeComponent) {
        emit insertValue();
        circuit->update((ActiveComponent*)exSel,cValue);
    }
}

void CircuitScene::selectAll() {
    for(auto &item : items())
        if (item->type()>=Component::component)
            item->setSelected(true);
}


void CircuitScene::createComponent() {
    Component *c;

    bool gnd=false;
    switch(myType) {
        case Component::resistor:
            c = new Resistor(cValue);
            break;
        case Component::voltmeter:
            c = new Voltmeter;
            break;
        case Component::amperometer:
            c=new Amperometer;
            break;
        case Component::voltageSource:
            c = new VoltageSource(cValue);
            break;
        case Component::currentSource:
            c = new CurrentSource(cValue);
            break;
        case Component::wire:
            c = new Wire;
            break;
        case Component::vcvs:
            c = new VCVS(cValue,selectedDependent);
            break;
        case Component::vccs:
            c = new VCCS(cValue,selectedDependent);
            break;
        case Component::ccvs:
            c = new CCVS(cValue,selectedDependent);
            break;
        case Component::cccs:
            c = new CCCS(cValue,selectedDependent);
            break;
        case Component::ground:
            c = new Wire;
            gnd=true;
            break;
        default:
            c = nullptr;
    }

    if(c!= nullptr) {
        auto *p = new Node(mousePressPoint);
        auto *n = new Node(mouseReleasePoint,gnd);
        if (c->type()==Component::component)
            c->setMenu(itemMenu);
        if (c->type()==Component::activeComponent)
            c->setMenu(richItemMenu);
        circuit->add(c, p, n);
        c->update();
        setMode(CircuitScene::modes(CircuitScene::moveItem));
    }
}

void CircuitScene::createItemMenus(){
    richItemMenu=new QMenu();
    itemMenu=new QMenu();
    QAction* deleteAction=new QAction(tr("&Delete"),this);
    connect(deleteAction, &QAction::triggered, this, &CircuitScene::deleteItem);
    richItemMenu->addAction(deleteAction);
    itemMenu->addAction(deleteAction);

    QAction* changeAction=new QAction(tr("&Change"),this);
    connect(changeAction, &QAction::triggered, this, &CircuitScene::changeValue);
    richItemMenu->addAction(changeAction);

    QAction* selectAllAction=new QAction(tr("&Select All"),this);
    connect(selectAllAction, &QAction::triggered, this, &CircuitScene::selectAll);
    sceneMenu->addAction(selectAllAction);
}