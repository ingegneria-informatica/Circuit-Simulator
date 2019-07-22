//
// Created by cipher on 18/07/19.
//

#include "MainWindow.h"
#include "CurrentSource.h"
#include "VoltageSource.h"
#include "Resistor.h"

#include <QtWidgets>
#include <iostream>


MainWindow::MainWindow(CircuitScene *scene) {

    createToolBox();
    createActions();
    menuBar()->setNativeMenuBar(false);
    createMenus();
    createToolbars();

    auto *layout= new QHBoxLayout;
    this->scene=scene;
    auto view= new QGraphicsView(scene);
    layout->addWidget(toolBox);
    layout->addWidget(view);

    QWidget *w=new QWidget;
    w->setLayout(layout);

    setCentralWidget(w);

    setWindowTitle(tr("Circuit Simulator"));
    setUnifiedTitleAndToolBarOnMac(true);

}


void MainWindow::createToolBox() {

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    auto *toolboxLayout= new QGridLayout;
    toolboxLayout->addWidget(createCellWidget(tr("Resistor"),":/images/resistor.png",Resistor),0,0);
    toolboxLayout->addWidget(createCellWidget(tr("Voltage Source"),":/images/voltagesource.png",VoltageSource),0,1);
    toolboxLayout->addWidget(createCellWidget(tr("Current Source"),":/images/currentsource.png",CurrentSource),1,0);
    toolboxLayout->addWidget(createCellWidget("Wire",":/images/wire.png",Wire),1,1);
    toolboxLayout->setRowStretch(3,10);
    toolboxLayout->setColumnStretch(3,10);

    auto itemWidget= new QWidget;
    itemWidget->setLayout(toolboxLayout);

    toolBox=new QToolBox;
    toolBox->setMinimumWidth(100);
    toolBox->addItem(itemWidget,tr("Electric Components"));


}

void MainWindow::createActions() {

    QIcon icon= QIcon(":/images/delete.png");
    deleteAction=new QAction(icon,tr("&Delete"),this);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItems);


    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

}

void MainWindow::deleteItems() {
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (auto it : selectedItems)
        delete it;

}

void MainWindow::createToolbars() {

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);

}


QWidget* MainWindow::createCellWidget(const QString &text, const QString &image, int id ) {

    QToolButton *button= new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50,50));
    button->setCheckable(true);
    buttonGroup->addButton(button,Qt::NoModifier);
    buttonGroup->setId(button,id);

    auto *layout= new QGridLayout;
    layout->addWidget(button,0,0,Qt::AlignCenter);
    layout->addWidget(new QLabel(text),1,0,Qt::AlignCenter);

    auto *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

void MainWindow::createMenus() {
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu= menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int id=buttonGroup->checkedId();
    switch (id){
        case CurrentSource:
            std::cout<<"current source"<<std::endl;
            break;
        case Resistor:
            std::cout<<"resistor"<<std::endl;
            break;
        case VoltageSource:
            std::cout<<"voltage source"<<std::endl;
            break;
        default:
            std::cout<<"nothing"<<std::endl;
    }
}

