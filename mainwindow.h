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
    QPushButton * stopButton;
    QMenu* menu;
    QAction* savefile;
    QAction* loadfile;
    QTimer* recalculateTimer;
    QTimer* timer;


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



};

#endif // MAINWINDOW_H
