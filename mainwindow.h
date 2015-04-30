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
#include <QBasicTimer>

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
    QAction* toggleControl;
    QAction* Occtest; //occupancy test
    QAction* clearAll;
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
    QBasicTimer m_timer;


private:
    Ui::MainWindow *ui;
    QSqlDatabase ldb;
    int* Eng1; //these 5 values deal with route planning
    int* Eng2;
    int* Eng3;
    int* Eng4;
    int* Eng5;
    int E1S;
    int E2S; //these five values list the size of Eng1-Eng5
    int E3S;
    int E4S;
    int E5S;
    int occupyitt; //Itterator to check occupancy wait time
    int fakeoccupancy;
    int pathcomplete1;
    int pathcomplete2;
    int pathcomplete3;
    int pathcomplete4;
    int pathcomplete5;
    int ready;
    int Eng1Start;
    int Eng2Start;
    int Eng3Start;
    int Eng4Start;
    int Eng5Start;
    int Eng1Speed; //Engine 1 travels 1 unit in 1 second.
    int Eng2Speed; //Engine 2 travels 1 unit in 2 seconds.
    int Eng3Speed; //Engine 3 travels 1 unit in 3 seconds.
    int Eng4Speed; //Engine 4 travels 1 unit in 2 seconds.
    int Eng5Speed; //Engine 5 travels 1 unit in 1 second.
    int i1; //Itterator for Comparison
    int i2; //Itterator for Comparison
    int i3; //Itterator for Comparison
    int i4; //Itterator for Comparison
    int i5; //Itterator for Comparison
    std::vector<int>  comparisonArray;
    std::vector<int>  comparisonArray2;
    std::vector<int>  comparisonArray3;
    std::vector<int>  comparisonArray4;
    std::vector<int> comparisonArray5;
    std::list<vertex_t> backupPath;
    QString tmpstr;
    int controlBit;

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
    void toggleBit();
   // void occupy();
    void clearChecks();
    void loadText();
    void stopTimer();
    void blockDest();
    void checkSwitches(QString cN, QString nN);
    void setSwitch(QString sN, QString sM);
    void viewTable();
    void tableOperation(std::list<vertex_t> path, int trainsel, int testInt, int attempt, int islong);
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
    int checkPaths(int trainS, int islong);
    int trackLength(int trackNum);
    void Eng1Time(int attempt, int islong);
    void Eng2Time(int attempt, int islong);
    void Eng3Time(int attempt, int islong);
    void Eng4Time(int attempt, int islong);
    void Eng5Time(int attempt, int islong);
    void reroute(int tNum, int tVal);
    int compare2(std::vector<int>  a, std::vector<int>  b, int a1, int b1);
    int compare3(std::vector<int> a, std::vector<int>  b, std::vector<int>  c, int a1, int b1, int c1);
    int longPathroute(int start, int end);
    QString oursTotheirs(int number);//Convert our integer adjacency to their string
    int theirsToours(QString string);//Convert their string adjacency to our int
    void timerEvent(QTimerEvent * ev);
    void createDBtables();
    void initializeSwitchtable();
};

#endif // MAINWINDOW_H
