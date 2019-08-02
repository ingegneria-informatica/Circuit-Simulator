//
// Created by cipher on 18/07/19.
//


#include "CircuitScene.h"

#include <QtWidgets>



class MainWindow: public QMainWindow{

Q_OBJECT

public:
    MainWindow(CircuitScene *scene);
public slots:
    void showDialog();
private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();

    void buttonGroupClicked(int type);
    void backgroundButtonGroupClicked(QAbstractButton *button);

    void sceneScaleChanged(const QString &scale);

    void selectItems();
    void deleteItems();
    void runCircuit();
    void loadImages();

    void about();

    QWidget *createCellWidget(const QString &text,const QPixmap &image, int id);
    QWidget *createBackgroundCellWidget(const QString &text,const QString &image);

    CircuitScene *scene;
    QGraphicsView *view;

    QToolBar *editToolBar;
    QToolBar *viewToolBar;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QComboBox *sceneScaleCombo;

    QAction *deleteAction;
    QAction *selectAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *runCircuitAction;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *circuitButtonGroup;
    QGridLayout *toolboxLayout;
};