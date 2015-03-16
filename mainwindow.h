#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QIcon>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QMessageBox>
#include <QGraphicsScene>

typedef long vertex_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QIcon *startIcon;
    QIcon *stopIcon;
    QIcon *engine1Icon;
    QIcon *engine2Icon;
    QIcon *engine3Icon;
    QIcon *engine4Icon;
    QIcon *engine5Icon;
    QIcon *mapIcon;
    QPushButton * stopButton;
    QMenu* menu;
    QAction* savefile;
    QAction* loadfile;
    QAction* traininfo;
    QAction* throttle;
    QAction* pathinfo;
    QAction* switches;
    QTimer* recalculateTimer;
    QTimer* timer;
    QTimer* a;
    QTimer* b;
    QTimer* c;
    QTimer* d;
    QTimer* e;
    QAction* techsupport;
    QGraphicsScene* scene;


private:
    Ui::MainWindow *ui;
    QSqlDatabase ldb;

private slots:

    void greyOut1();
    void greyOut2();
    void greyOut3();
    void greyOut4();
    void greyOut5();
    void showTime();
    void calculateRoute();
    void occupiedNode();
    void destNode();
    void saveText();
    void loadText();
    void stopTimer();
    void blockDest();
    void checkSwitches(QString cN, QString nN);
    void setSwitch(QString sN, QString sM);
    void viewTable();
    void tableOperation(std::list<vertex_t> somePath);
    void traininfoTable();
    void throttleTable();
    void pathinfoTable();
    void switchesTable();
    void techsupportDoc();
    void timeDelay1();
    void timeDelay2();
    void timeDelay3();
    void timeDelay4();
    void timeDelay5();
    void setgreyOut1();
    void setgreyOut2();
    void setgreyOut3();
    void setgreyOut4();
    void setgreyOut5();
    void stop1();



};

#endif // MAINWINDOW_H
