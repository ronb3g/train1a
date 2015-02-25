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
    void checkSwitches(string cN, string nN);
    void setSwitch(string sN, string sM);

};

#endif // MAINWINDOW_H
