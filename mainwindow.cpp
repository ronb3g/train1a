#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLCDNumber>
#include <QTime>
#include <QPixmap>
#include <iostream>
#include <QVector>
#include <string>
#include <QList>
#include <limits> // for numeric_limits
#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include <QQueue>
#include <QTimer>
#include <QSettings>
#include <QTextBrowser>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <Q_DebugStream.h>
#include <QTimeEdit>
#include <QVariant>
#include <sstream>




using namespace std;
typedef long vertex_t;
typedef long weight_t;

const weight_t max_weight = INFINITY;

template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}

struct neighbor
{
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};
typedef std::vector<std::vector<neighbor> > adjacency_list_t;

void DijkstraComputePaths(vertex_t source,
    const adjacency_list_t &adjacency_list,
        std::vector<weight_t> &min_distance,
            std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
        vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
        neighbor_iter != neighbors.end();
        neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;

            if (distance_through_u < min_distance[v])
            {
                vertex_queue.erase(std::make_pair(min_distance[v], v));
                min_distance[v] = distance_through_u;
                previous[v] = u;

                vertex_queue.insert(std::make_pair(min_distance[v], v));

            }
        }
    }
}

std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ldb = QSqlDatabase::addDatabase("QSQLITE", "ldb");
    ldb.setDatabaseName( "train.db" );
    if(!ldb.open())
       { qDebug() << ldb.lastError();}

    ui->setupUi(this);

    //Start and Start button graphics
    startIcon = new QIcon("startbutton.png");
    stopIcon = new QIcon("stop.jpg");
    ui->startButton->setIcon(*startIcon);
    ui->startButton->setIconSize(QSize(65,65));
    ui->stopButton->setIcon(*stopIcon);
    ui->stopButton->setIconSize(QSize(35,35));

    //create train image icons
    engine1Icon = new QIcon("engine1.jpg");
    engine2Icon = new QIcon("engine2.jpg");
    engine3Icon = new QIcon("engine3.jpg");
    engine4Icon = new QIcon("engine4.jpg");
    engine5Icon = new QIcon("engine5.jpg");

    //train image and route info hidden until selected
    ui->trainimageButton1->hide();
    ui->routeInfo1->hide();
    ui->trainimageButton2->hide();
    ui->routeInfo2->hide();
    ui->trainimageButton3->hide();
    ui->routeInfo3->hide();
    ui->trainimageButton4->hide();
    ui->routeInfo4->hide();
    ui->trainimageButton5->hide();
    ui->routeInfo5->hide();



    //start button starts trains and periodic route calculations
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(calculateRoute()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(calculateRoute()));

    //once schedule created, select radio button to lock it in
    connect(ui->setButton1, SIGNAL(clicked(bool)), this, SLOT(greyOut1()));
    connect(ui->setButton2, SIGNAL(clicked(bool)), this, SLOT(greyOut2()));
    connect(ui->setButton3, SIGNAL(clicked(bool)), this, SLOT(greyOut3()));
    connect(ui->setButton4, SIGNAL(clicked(bool)), this, SLOT(greyOut4()));
    connect(ui->setButton5, SIGNAL(clicked(bool)), this, SLOT(greyOut5()));

    //feature to include randomly selected node to include in origin box to start train from
    //i.e. dynamically added due to node being occuppied when train placed on it
    connect(ui->occButton, SIGNAL(clicked()), this, SLOT(occupiedNode()));
    //randomly selected destination for testing route algorithm
    connect(ui->destButton, SIGNAL(clicked()), this, SLOT(destNode()));

    //Clock feature added to enable delayed train departures
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();

    //feature to block certain destinations based on origin
    //i.e. if origin Endpoint 1 selected need to block destination box from selecting Endpoint 1
    connect(ui->originBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(blockDest()));

    //feature to save and load configurations
    savefile = new QAction("Save config", this);
    loadfile = new QAction("Load config", this);
    connect(loadfile, SIGNAL(triggered()), this, SLOT(loadText()));
    ui->menuCPE_453_Team_1A->addAction(loadfile);
    ui->menuCPE_453_Team_1A->addAction(savefile);
    connect(savefile, SIGNAL(triggered()), this, SLOT(saveText()));


}

MainWindow::~MainWindow()
{
    ldb.close();
    delete ui;
}


//Function to check and toggle switches as needed
void MainWindow::checkSwitches(QString cN, QString nN)
{
    if (cN == "36")
    {
        if(nN == "35")
            setSwitch("11", "Open");
        else if(nN == "68")
           setSwitch("11", "Bypass");
        else
           return;
    }
    else if (cN == "39")
    {
        if(nN == "38")
            setSwitch("21", "Open");
        else if(nN == "37")
           setSwitch("21", "Bypass");
        else
           return;
    }
    else if (cN == "65")
    {
        if(nN == "66")
            setSwitch("43", "Open");
        else if(nN == "67")
           setSwitch("43", "Bypass");
        else
           return;
    }
    else if (cN == "31")
    {
        if(nN == "70")
            setSwitch("42", "Open");
        else if(nN == "30")
           setSwitch("42", "Bypass");
        else
           return;
    }
    else if (cN == "32")
    {
        if(nN == "33")
            setSwitch("13", "Open");
        else if(nN == "34")
          {
           setSwitch("13", "Bypass");
           setSwitch("12", "Bypass");
          }
        else
           return;
    }
    else if (cN == "34")
    {
        if(nN == "50")
            setSwitch("12", "Open");
        else if(nN == "32")
            {
             setSwitch("13", "Bypass");
             setSwitch("12", "Bypass");
            }
        else
           return;
    }
    else if (cN == "0")
    {
        if(nN == "1")
            setSwitch("22", "Open");
        else if(nN == "2")
           {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
           }
        else
           return;
    }
    else if (cN == "2")
    {
        if(nN == "71")
            setSwitch("23", "Open");
        else if(nN == "0")
            {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
            }
        else
           return;
    }
    else if (cN == "29")
    {
        if(nN == "21")
            setSwitch("24", "Open");
        else if(nN == "28")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (cN == "28")
    {
        if(nN == "26")
            setSwitch("34", "Open");
        else if(nN == "27")
           setSwitch("34", "Bypass");
        else if(nN == "29")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (cN == "26")
    {
        if(nN == "51")
            setSwitch("33", "Open");
        else if(nN == "25")
           setSwitch("33", "Bypass");
        else if(nN == "28")
            setSwitch("34", "Open");
        else
           return;
    }
    else if (cN == "51")
    {
        if(nN == "52")
            setSwitch("32", "Open");
        else if(nN == "55")
           setSwitch("32", "Bypass");
        else if(nN == "26")
            setSwitch("33", "Open");
        else
           return;
    }
    else if (cN == "52")
    {
        if(nN == "53")
            setSwitch("31", "Open");
        else if(nN == "54")
           setSwitch("31", "Bypass");
        else if(nN == "51")
            setSwitch("32", "Open");
        else
           return;
    }
    else if (cN == "49")
    {
        if(nN == "47")
            setSwitch("41", "Open");
        else if(nN == "48")
           setSwitch("41", "Bypass");
        else
           return;
    }
    else if (cN == "56")
    {
        if(nN == "57")
            setSwitch("14", "Open");
        else if(nN == "58")
           setSwitch("14", "Bypass");
        else
           return;
    }
    else if (cN == "42")
    {
        if(nN == "43")
            setSwitch("63", "Open");
        else if(nN == "44")
           setSwitch("63", "Bypass");
        else if(nN == "41")
            setSwitch("62", "Open");
        else
           return;
    }
    else if (cN == "41")
    {
        if(nN == "42")
            setSwitch("62", "Open");
        else if(nN == "46")
           setSwitch("62", "Bypass");
        else if(nN == "40")
            setSwitch("61", "Open");
        else
           return;
    }
    else if (cN == "40")
    {
        if(nN == "41")
            setSwitch("61", "Open");
        else if(nN == "45")
           setSwitch("61", "Bypass");
        else
           return;
    }
    else if (cN == "61")
    {
        if(nN == "59")
            setSwitch("84", "Open");
        else if(nN == "60")
           setSwitch("84", "Bypass");
        else
           return;
    }
    else if (cN == "22")
    {
        if(nN == "24")
            setSwitch("64", "Open");
        else if(nN == "23")
           setSwitch("64", "Bypass");
        else if(nN == "20")
            setSwitch("75","Bypass");
        else
           return;
    }
    else if (cN == "20")
    {
        if(nN == "21")
            setSwitch("74", "Open");
        else if(nN == "22")
           setSwitch("74", "Bypass");
        else
           return;
    }
    else if (cN == "4")
    {
        if(nN == "6")
            setSwitch("73", "Open");
        else if(nN == "5")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (cN == "5")
    {
        if(nN == "3")
            setSwitch("72", "Open");
        else if(nN == "4")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (cN == "7")
    {
        if(nN == "9")
            setSwitch("71", "Open");
        else if(nN == "8")
           setSwitch("71", "Bypass");
        else
           return;
    }
    else if (cN == "62")
    {
        if(nN == "63")
            setSwitch("52", "Open");
        else if(nN == "64")
           setSwitch("52", "Bypass");
        else
           return;
    }
    else if (cN == "17")
    {
        if(nN == "19")
            setSwitch("51", "Open");
        else if(nN == "18")
           setSwitch("51", "Bypass");
        else
           return;
    }
    else if (cN == "16")
    {
        if(nN == "12")
            setSwitch("83", "Open");
        else if(nN == "14")
        {
           setSwitch("83", "Bypass");
           setSwitch("82", "Bypass");
        }
        else
           return;
    }
    else if (cN == "14")
    {
        if(nN == "15")
            setSwitch("82", "Open");
        else if(nN == "16")
        {
           setSwitch("82", "Bypass");
           setSwitch("83", "Bypass");
        }
        else
           return;
    }
    else if (cN == "10")
    {
        if(nN == "12")
            setSwitch("81", "Open");
        else if(nN == "11")
           setSwitch("81", "Bypass");
        else
           return;
    }
    else if (cN == "35" && nN == "36")
        setSwitch("11", "Open");
    else if (cN == "38" && nN == "39")
        setSwitch("21", "Open");
    else if (cN == "66" && nN == "65")
        setSwitch("43", "Open");
    else if (cN == "70" && nN == "31")
        setSwitch("42", "Open");
    else if (cN == "33" && nN == "32")
        setSwitch("13", "Open");
    else if (cN == "50" && nN == "34")
        setSwitch("12", "Open");
    else if (cN == "1" && nN == "0")
        setSwitch("22", "Open");
    else if (cN == "71" && nN == "2")
        setSwitch("23", "Open");
    else if (cN == "53" && nN == "52")
        setSwitch("31", "Open");
    else if (cN == "47" && nN == "49")
        setSwitch("41", "Open");
    else if (cN == "57" && nN == "56")
        setSwitch("14", "Open");
    else if (cN == "43" && nN == "42")
        setSwitch("63", "Open");
    else if (cN == "59" && nN == "61")
        setSwitch("84", "Open");
    else if (cN == "24" && nN == "22")
        setSwitch("64", "Open");
    else if (cN == "6" && nN == "4")
        setSwitch("73", "Open");
    else if (cN == "3" && nN == "5")
        setSwitch("72", "Open");
    else if (cN == "9" && nN == "7")
        setSwitch("71", "Open");
    else if (cN == "63" && nN == "62")
        setSwitch("52", "Open");
    else if (cN == "19" && nN == "17")
        setSwitch("51", "Open");
    else if (cN == "15" && nN == "14")
        setSwitch("82", "Open");
    else if(cN == "21")
    {
        if (nN == "20")
            setSwitch("74", "Open");
        else if (nN == "29")
            setSwitch("24", "Open");
        else
            return;
    }
    else if(cN == "12")
    {
        if (nN == "16")
            setSwitch("83", "Open");
        else if (nN == "10")
            setSwitch("81", "Open");
        else
            return;
    }
    else if (cN == "68" && nN == "36")
        setSwitch("11", "Bypass");
    else if (cN == "37" && nN == "39")
        setSwitch("21", "Bypass");
    else if (cN == "67" && nN == "65")
        setSwitch("43", "Bypass");
    else if (cN == "30" && nN == "31")
        setSwitch("42", "Bypass");
    else if (cN == "27" && nN == "28")
        setSwitch("34", "Bypass");
    else if (cN == "25" && nN == "26")
        setSwitch("33", "Bypass");
    else if (cN == "55" && nN == "51")
        setSwitch("32", "Bypass");
    else if (cN == "54" && nN == "52")
        setSwitch("31", "Bypass");
    else if (cN == "48" && nN == "49")
        setSwitch("41", "Bypass");
    else if (cN == "58" && nN == "56")
        setSwitch("14", "Bypass");
    else if (cN == "44" && nN == "42")
        setSwitch("63", "Bypass");
    else if (cN == "46" && nN == "41")
        setSwitch("62", "Bypass");
    else if (cN == "45" && nN == "40")
        setSwitch("61", "Bypass");
    else if (cN == "60" && nN == "61")
        setSwitch("84", "Bypass");
    else if (cN == "23" && nN == "22")
        setSwitch("64", "Bypass");
    else if (cN == "8" && nN == "7")
        setSwitch("71", "Bypass");
    else if (cN == "64" && nN == "62")
        setSwitch("52", "Bypass");
    else if (cN == "18" && nN == "17")
        setSwitch("51", "Bypass");
    else if (cN == "11" && nN == "10")
        setSwitch("81", "Bypass");

    return;
}

void MainWindow::setSwitch(QString sN, QString sM)
{
    QSqlQuery q(ldb);
    q.prepare("UPDATE switches SET position=? WHERE switch=?");
    q.bindValue(0, sM);
    q.bindValue(1, sN);

    if(!q.exec())
    {
    cout << "Failed to update database";
    }
}


//Function to block unavailable destinations
void MainWindow::blockDest()
{
    if (ui->originBox1->currentIndex() == 1 || ui->originBox1->currentIndex() == 2)
    {

    }
}

//Function to save current configuration
void MainWindow::saveText()
{
    QSettings settings(".mytrainsettings.ini",QSettings::IniFormat);
    settings.setValue("def/trainselectBox1", ui->trainselectBox1->currentIndex());
    settings.setValue("def/trainselectBox2", ui->trainselectBox2->currentIndex());
    settings.setValue("def/trainselectBox3", ui->trainselectBox3->currentIndex());
    settings.setValue("def/trainselectBox4", ui->trainselectBox4->currentIndex());
    settings.setValue("def/trainselectBox5", ui->trainselectBox5->currentIndex());
    settings.setValue("def/facingBox1", ui->facingBox1->currentIndex());
    settings.setValue("def/facingBox2", ui->facingBox2->currentIndex());
    settings.setValue("def/facingBox3", ui->facingBox3->currentIndex());
    settings.setValue("def/facingBox4", ui->facingBox4->currentIndex());
    settings.setValue("def/facingBox5", ui->facingBox5->currentIndex());
    settings.setValue("def/originBox1", ui->originBox1->currentIndex());
    settings.setValue("def/originBox2", ui->originBox2->currentIndex());
    settings.setValue("def/originBox3", ui->originBox3->currentIndex());
    settings.setValue("def/originBox4", ui->originBox4->currentIndex());
    settings.setValue("def/originBox5", ui->originBox5->currentIndex());
    settings.setValue("def/headingBox1", ui->headingBox1->currentIndex());
    settings.setValue("def/headingBox2", ui->headingBox2->currentIndex());
    settings.setValue("def/headingBox3", ui->headingBox3->currentIndex());
    settings.setValue("def/headingBox4", ui->headingBox4->currentIndex());
    settings.setValue("def/headingBox5", ui->headingBox5->currentIndex());
    settings.setValue("def/destBox1", ui->destBox1->currentIndex());
    settings.setValue("def/destBox2", ui->destBox2->currentIndex());
    settings.setValue("def/destBox3", ui->destBox3->currentIndex());
    settings.setValue("def/destBox4", ui->destBox4->currentIndex());
    settings.setValue("def/destBox5", ui->destBox5->currentIndex());
    settings.setValue("def/throttleBox1", ui->throttleBox1->currentIndex());
    settings.setValue("def/throttleBox2", ui->throttleBox2->currentIndex());
    settings.setValue("def/throttleBox3", ui->throttleBox3->currentIndex());
    settings.setValue("def/throttleBox4", ui->throttleBox4->currentIndex());
    settings.setValue("def/throttleBox5", ui->throttleBox5->currentIndex());

}

//Function to load previously saved configuration
void MainWindow::loadText()
{
    QSettings settings(".mytrainsettings.ini",QSettings::IniFormat);
    ui->trainselectBox1->setCurrentIndex(settings.value("def/trainselectBox1").toInt());
    ui->trainselectBox2->setCurrentIndex(settings.value("def/trainselectBox2").toInt());
    ui->trainselectBox3->setCurrentIndex(settings.value("def/trainselectBox3").toInt());
    ui->trainselectBox4->setCurrentIndex(settings.value("def/trainselectBox4").toInt());
    ui->trainselectBox5->setCurrentIndex(settings.value("def/trainselectBox5").toInt());
    ui->facingBox1->setCurrentIndex(settings.value("def/facingBox1").toInt());
    ui->facingBox2->setCurrentIndex(settings.value("def/facingBox2").toInt());
    ui->facingBox3->setCurrentIndex(settings.value("def/facingBox3").toInt());
    ui->facingBox4->setCurrentIndex(settings.value("def/facingBox4").toInt());
    ui->facingBox5->setCurrentIndex(settings.value("def/facingBox5").toInt());
    ui->originBox1->setCurrentIndex(settings.value("def/originBox1").toInt());
    ui->originBox2->setCurrentIndex(settings.value("def/originBox2").toInt());
    ui->originBox3->setCurrentIndex(settings.value("def/originBox3").toInt());
    ui->originBox4->setCurrentIndex(settings.value("def/originBox4").toInt());
    ui->originBox5->setCurrentIndex(settings.value("def/originBox5").toInt());
    ui->headingBox1->setCurrentIndex(settings.value("def/headingBox1").toInt());
    ui->headingBox2->setCurrentIndex(settings.value("def/headingBox2").toInt());
    ui->headingBox3->setCurrentIndex(settings.value("def/headingBox3").toInt());
    ui->headingBox4->setCurrentIndex(settings.value("def/headingBox4").toInt());
    ui->headingBox5->setCurrentIndex(settings.value("def/headingBox5").toInt());
    ui->destBox1->setCurrentIndex(settings.value("def/destBox1").toInt());
    ui->destBox2->setCurrentIndex(settings.value("def/destBox2").toInt());
    ui->destBox3->setCurrentIndex(settings.value("def/destBox3").toInt());
    ui->destBox4->setCurrentIndex(settings.value("def/destBox4").toInt());
    ui->destBox5->setCurrentIndex(settings.value("def/destBox5").toInt());
    ui->throttleBox1->setCurrentIndex(settings.value("def/throttleBox1").toInt());
    ui->throttleBox2->setCurrentIndex(settings.value("def/throttleBox2").toInt());
    ui->throttleBox3->setCurrentIndex(settings.value("def/throttleBox3").toInt());
    ui->throttleBox4->setCurrentIndex(settings.value("def/throttleBox4").toInt());
    ui->throttleBox5->setCurrentIndex(settings.value("def/throttleBox5").toInt());
}

//function to add occupied node to origin boxes
void MainWindow::occupiedNode()
{
    ui->originBox1->addItem(ui->addorlineEdit3->text());
    ui->originBox2->addItem(ui->addorlineEdit3->text());
    ui->originBox3->addItem(ui->addorlineEdit3->text());
    ui->originBox4->addItem(ui->addorlineEdit3->text());
    ui->originBox5->addItem(ui->addorlineEdit3->text());
}

//function to add random destination to destination boxes
void MainWindow::destNode()
{
    ui->destBox1->addItem((ui->adddestlineEdit4->text()));
    ui->destBox2->addItem((ui->adddestlineEdit4->text()));
    ui->destBox3->addItem((ui->adddestlineEdit4->text()));
    ui->destBox4->addItem((ui->adddestlineEdit4->text()));
    ui->destBox5->addItem((ui->adddestlineEdit4->text()));
}

//function to stop route calculations
void MainWindow::stopTimer()
{
    //recalculateTimer->stop();
}

//function to start periodic route calculations
void MainWindow::calculateRoute()
{
    QTimer *recalculateTimer = new QTimer(this);

    if(recalculateTimer->isActive() == true)
    {
        recalculateTimer->stop();
        ui->startButton->setDisabled(false);
    }

    else
    {
    ui->startButton->setDisabled(true); //disabled start button to avoid throwing timer off

    if(ui->setButton1->isChecked() == true)
    connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut1()));
    if(ui->setButton2->isChecked() == true)
    connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut2()));
    if(ui->setButton3->isChecked() == true)
    connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut3()));
    if(ui->setButton4->isChecked() == true)
    connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut4()));
    if(ui->setButton5->isChecked() == true)
    connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut5()));

    recalculateTimer->start(5000);

    }
}
//function to display real world time according to current pc
void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ssap");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcdNumber->display(text);
}


//lock in and calculate route for line 1
void MainWindow::greyOut1()
{

    new Q_DebugStream(std::cout, ui->routeInfo1); //Redirect Console output to textBrowser1
    Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser1

    //Determine which image to display
    if(ui->trainselectBox1->currentIndex() == 1)
    {
        ui->trainimageButton1->setIcon(*engine1Icon);
        ui->trainimageButton1->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox1->currentIndex() == 2)
    {
        ui->trainimageButton1->setIcon(*engine2Icon);
        ui->trainimageButton1->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox1->currentIndex() == 3)
    {
        ui->trainimageButton1->setIcon(*engine3Icon);
        ui->trainimageButton1->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox1->currentIndex() == 4)
    {
        ui->trainimageButton1->setIcon(*engine4Icon);
        ui->trainimageButton1->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox1->currentIndex() == 5)
    {
        ui->trainimageButton1->setIcon(*engine5Icon);
        ui->trainimageButton1->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton1->hide();

    int start;
    int end;
    //translate origin box
    if (ui->originBox1->currentIndex() == 1)
        start = 43;
    else if (ui->originBox1->currentIndex() == 2)
        start = 44;
    else if (ui->originBox1->currentIndex() == 3)
        start = 55;
    else if (ui->originBox1->currentIndex() == 4)
        start = 54;
    else if (ui->originBox1->currentIndex() == 5)
        start = 53;
    else if (ui->originBox1->currentIndex() == 6)
        start = 13;
    else if (ui->originBox1->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox1->currentIndex() == 1)
        end = 43;
    else if (ui->destBox1->currentIndex() == 2)
        end = 44;
    else if (ui->destBox1->currentIndex() == 3)
        end = 55;
    else if (ui->destBox1->currentIndex() == 4)
        end = 54;
    else if (ui->destBox1->currentIndex() == 5)
        end = 53;
    else if (ui->destBox1->currentIndex() == 6)
        end = 13;
    else if (ui->destBox1->currentIndex() == 7)
        end = 69;
    //grey out line 1 if radio button selected
    if (ui->setButton1->isChecked()== true)
    {
        ui->trainselectBox1->setDisabled(true);
        ui->originBox1->setDisabled(true);
        ui->destBox1->setDisabled(true);
        ui->throttleBox1->setDisabled(true);
        ui->facingBox1->setDisabled(true);
        ui->headingBox1->setDisabled(true);
        ui->trainimageButton1->show();
        ui->routeInfo1->show();
        //adjacency list selected based on heading
        if (ui->headingBox1->currentIndex() == 1)
        {
        //adjacency list east
        adjacency_list_t adjacency_list(100);

        adjacency_list[0].push_back(neighbor(39, 3));
        adjacency_list[1].push_back(neighbor(0, 2));
        adjacency_list[2].push_back(neighbor(4, 9));
        adjacency_list[3].push_back(neighbor(1, 1));
        adjacency_list[4].push_back(neighbor(72, 9));
        adjacency_list[5].push_back(neighbor(3, 9));
        adjacency_list[7].push_back(neighbor(5, 8));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[9].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(11, 9));
        adjacency_list[11].push_back(neighbor(13, 9));
        adjacency_list[14].push_back(neighbor(8, 6));
        adjacency_list[15].push_back(neighbor(14, 5));
        adjacency_list[26].push_back(neighbor(51, 4));
        adjacency_list[28].push_back(neighbor(26, 3));
        adjacency_list[29].push_back(neighbor(28, 2));
        adjacency_list[30].push_back(neighbor(29, 1));
        adjacency_list[31].push_back(neighbor(70, 9));
        adjacency_list[31].push_back(neighbor(30, 8));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[33].push_back(neighbor(32, 6));
        adjacency_list[35].push_back(neighbor(33, 5));
        adjacency_list[36].push_back(neighbor(35, 4));
        adjacency_list[36].push_back(neighbor(68, 9));
        adjacency_list[38].push_back(neighbor(36, 5));
        adjacency_list[39].push_back(neighbor(38, 4));
        adjacency_list[40].push_back(neighbor(15, 4));
        adjacency_list[41].push_back(neighbor(40, 3));
        adjacency_list[42].push_back(neighbor(41, 2));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[44].push_back(neighbor(42, 9));
        adjacency_list[51].push_back(neighbor(52, 3));
        adjacency_list[52].push_back(neighbor(53, 4));
        adjacency_list[52].push_back(neighbor(54, 9));
        adjacency_list[51].push_back(neighbor(55, 9));
        adjacency_list[68].push_back(neighbor(69, 9));
        adjacency_list[70].push_back(neighbor(71, 9));
        adjacency_list[71].push_back(neighbor(2, 9));
        adjacency_list[72].push_back(neighbor(73, 9));
        adjacency_list[73].push_back(neighbor(9, 9));

        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";



// test code for reading the vector to a database
        int pathSize = path.size();
        int numOfRows = pathSize/10; //number of rows on the path table needed
        if (pathSize%10 !=0)
            numOfRows++;
        cout << "The total number of hops is " << to_string(pathSize) << endl;
        //convert the list to an array
        int *copyarray = new int[pathSize];
        for (int i=0; i<pathSize; i++)
        {
            copyarray[i] = path.front();
            path.pop_front();
        }
        for (int i=0; i<pathSize; i++)
        {
            cout << to_string(copyarray[i]) << " ";

        }
        cout << endl;

        //Use array to fill out traininfo and pathinfo table. Values needed: values in the array, value of the "trainselectBox1" box,
        //final node, initial node, number of rows on the path info table
        QSqlQuery q(ldb);

        q.exec("SELECT COUNT(*) FROM pathinfo;");
        int numRows; //Current number of rows in the path table
        while(q.next())
        {
            numRows = q.value(0).toInt();
        }
        numRows++; //increment this by one to get the first free row.
        int numRows2 = numRows; //duplicate the value for later use
        int itt = 1; //itterator for the loop

        for( int iR = 1; iR<=numOfRows; iR++)
        {
            q.prepare("INSERT INTO pathinfo VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
            q.bindValue(0,numRows); //bind first value to the row on the path table.

            if(numOfRows == (iR))
                q.bindValue(1, QVariant(QVariant::Int));
            else
                q.bindValue(1,(numRows+1));

            numRows++;

            if(itt <= pathSize){
            q.bindValue(2, copyarray[itt]);
            itt++;}
            else
                q.bindValue(2, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(3, copyarray[itt]);
            itt++;}
            else
                q.bindValue(3, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(4, copyarray[itt]);
            itt++;}
            else
                q.bindValue(4, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(5, copyarray[itt]);
            itt++;}
            else
                q.bindValue(5, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(6, copyarray[itt]);
            itt++;}
            else
                q.bindValue(6, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(7, copyarray[itt]);
            itt++;}
            else
                q.bindValue(7, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(8, copyarray[itt]);
            itt++;}
            else
                q.bindValue(8, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(9, copyarray[itt]);
            itt++;}
            else
                q.bindValue(9, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(10, copyarray[itt]);
            itt++;}
            else
                q.bindValue(10, QVariant(QVariant::String));

            if(itt <= pathSize){
            q.bindValue(11, copyarray[itt]);
            itt++;}
            else
                q.bindValue(11, QVariant(QVariant::String));


            if(!q.exec())
            {
                cout << "Failed to update database";
            }
         }


    q.prepare("UPDATE traininfo SET current=?,destination=?,next=?,path=? WHERE trainID=?");
    q.bindValue(0, copyarray[0] );
    q.bindValue(1, copyarray[pathSize-1]);
    q.bindValue(2, copyarray[1]);
//<<<<<<< HEAD
    //q.bindValue(4, numRows2);
    //q.bindValue(3, );
//=======
    q.bindValue(3, numRows2);
    QString tmpstr = ui->trainselectBox1->currentText();
    q.bindValue(4, tmpstr);
//>>>>>>> refs/remotes/train1a/master

    if(!q.exec())
    {
        cout << "Failed to update database";
    }


q.prepare("SELECT * FROM traininfo WHERE trainID=?");
q.bindValue(0, tmpstr);
if(!q.exec())
{
    cout << "Failed to update database";
}
QString currentLoc;
QString nextLoc;
QString endLoc;
int nextTab = NULL;
QString path2;
QString path3;
QString path4;
QString path5;
QString path6;
QString path7;
QString path8;
QString path9;
QString path10;
QString path11;
int pathTab;
int currenttab;
int itt2 = 0;



while(q.next())
{
    currentLoc = q.value(1).toString();
    nextLoc = q.value(2).toString();
    endLoc = q.value(3).toString();
    pathTab = q.value(4).toInt();
}

q.prepare("SELECT * FROM pathinfo WHERE pathID=?");
q.bindValue(0,pathTab);
if(!q.exec())
{
    cout << "Failed to update database";
}
while(q.next())
{
    nextTab = q.value(1).toInt();
    path2 = q.value(2).toString();
    path3 = q.value(3).toString();
    path4 = q.value(4).toString();
    path5 = q.value(5).toString();
    path6 = q.value(6).toString();
    path7 = q.value(7).toString();
    path8 = q.value(8).toString();
    path9 = q.value(9).toString();
    path10 = q.value(10).toString();
    path11 = q.value(11).toString();
}



for(int i=0; i<3; i++)
{


    //checkSwitches(currentLoc, nextLoc);
    //cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << endLoc.toStdString() << endl;
    //currentLoc = nextLoc;

if(path2 != NULL){
    nextLoc = path2;
    checkSwitches(currentLoc, path2);
    cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path2.toStdString() << endl;
    currentLoc = nextLoc;
}

if(path3 != NULL){
    nextLoc = path3;
    checkSwitches(currentLoc, path3);
    cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path3.toStdString() << endl;
    currentLoc = nextLoc;
}

 if(path4 != NULL){
    nextLoc = path4;
    checkSwitches(currentLoc, path4);
    cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path4.toStdString() << endl;
    currentLoc = nextLoc;
 }

  if(path5 != NULL){
    nextLoc = path5;
    checkSwitches(currentLoc, path5);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path5.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path6 != NULL){
    nextLoc = path6;
    checkSwitches(currentLoc, path6);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path6.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path7 != NULL){
    nextLoc = path7;
    checkSwitches(currentLoc, path7);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path7.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path8 != NULL){
    nextLoc = path8;
    checkSwitches(currentLoc, path8);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path8.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path9 != NULL){
    nextLoc = path9;
    checkSwitches(currentLoc, path9);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path9.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path10 != NULL){
    nextLoc = path10;
    checkSwitches(currentLoc, path10);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path10.toStdString() << endl;
    currentLoc = nextLoc;
  }

  if(path11 != NULL){
    nextLoc = path11;
    checkSwitches(currentLoc, path11);
    cout << tmpstr.toStdString() << "Moved from " << currentLoc.toStdString() << " to " << path11.toStdString() << endl;
}

  if(nextTab == NULL){

      q.prepare("SELECT * FROM pathinfo WHERE pathID=?");
      q.bindValue(0,nextTab);
      if(!q.exec())
      {
          cout << "Failed to update database 123";
      }

      //int nextTab2 = nextTab;

      while(q.next())
      {
          currenttab = q.value(0).toInt();
          nextTab = q.value(1).toInt();
          path2 = q.value(2).toString();
          path3 = q.value(3).toString();
          path4 = q.value(4).toString();
          path5 = q.value(5).toString();
          path6 = q.value(6).toString();
          path7 = q.value(7).toString();
          path8 = q.value(8).toString();
          path9 = q.value(9).toString();
          path10 = q.value(10).toString();
          path11 = q.value(11).toString();
      }






      q.prepare("UPDATE traininfo SET current=?,next=?,path=? WHERE trainID =?");
      q.bindValue(0,currentLoc);
      q.bindValue(1,path2);
      q.bindValue(2,currenttab);
      q.bindValue(3,tmpstr);
      if(!q.exec())
      {
          cout << "Failed to update database 456";
      }

}
}




        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }
        //adjacency list selected based on heading
        else if (ui->headingBox1->currentIndex() == 2)
        {
            //adjacency list west
            adjacency_list_t adjacency_list(100);

            adjacency_list[53].push_back(neighbor(52, 9));
            adjacency_list[54].push_back(neighbor(52, 9));
            adjacency_list[55].push_back(neighbor(51, 9));
            adjacency_list[52].push_back(neighbor(51, 9));
            adjacency_list[51].push_back(neighbor(26, 9));
            adjacency_list[26].push_back(neighbor(28, 9));
            adjacency_list[28].push_back(neighbor(29, 9));
            adjacency_list[29].push_back(neighbor(30, 9));
            adjacency_list[30].push_back(neighbor(31, 9));
            adjacency_list[31].push_back(neighbor(32, 9));
            adjacency_list[32].push_back(neighbor(34, 9));
            adjacency_list[34].push_back(neighbor(37, 9));
            adjacency_list[37].push_back(neighbor(39, 9));
            adjacency_list[39].push_back(neighbor(0, 9));
            adjacency_list[0].push_back(neighbor(1, 9));
            adjacency_list[1].push_back(neighbor(3, 9));
            adjacency_list[3].push_back(neighbor(5, 9));
            adjacency_list[5].push_back(neighbor(7, 9));
            adjacency_list[7].push_back(neighbor(9, 9));
            adjacency_list[9].push_back(neighbor(10, 9));
            adjacency_list[10].push_back(neighbor(11, 9));
            adjacency_list[11].push_back(neighbor(13, 9));
            adjacency_list[7].push_back(neighbor(8, 9));
            adjacency_list[8].push_back(neighbor(14, 9));
            adjacency_list[14].push_back(neighbor(15, 9));
            adjacency_list[15].push_back(neighbor(40, 9));
            adjacency_list[40].push_back(neighbor(41, 9));
            adjacency_list[41].push_back(neighbor(42, 9));
            adjacency_list[42].push_back(neighbor(43, 9));
            adjacency_list[42].push_back(neighbor(44, 9));
            adjacency_list[10].push_back(neighbor(12, 9));
            adjacency_list[12].push_back(neighbor(16, 9));
            adjacency_list[16].push_back(neighbor(17, 9));
            adjacency_list[17].push_back(neighbor(19, 9));
            adjacency_list[19].push_back(neighbor(6, 9));
            adjacency_list[6].push_back(neighbor(4, 9));
            adjacency_list[4].push_back(neighbor(2, 9));
            adjacency_list[2].push_back(neighbor(75, 9));
            adjacency_list[75].push_back(neighbor(74, 9));
            adjacency_list[74].push_back(neighbor(38, 9));
            adjacency_list[38].push_back(neighbor(36, 9));
            adjacency_list[36].push_back(neighbor(68, 9));
            adjacency_list[68].push_back(neighbor(69, 9));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
            std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
            std::cout << std::endl;
        }

    }
    //if radio button un-checked then un-greyout line 1
    else
    {
        ui->trainselectBox1->setDisabled(false);
        ui->originBox1->setDisabled(false);
        ui->destBox1->setDisabled(false);
        ui->throttleBox1->setDisabled(false);
        ui->facingBox1->setDisabled(false);
        ui->headingBox1->setDisabled(false);
        ui->trainimageButton1->hide();
        ui->routeInfo1->clear();
        ui->routeInfo1->hide();
    }
}

//lock in and calculate route for line 2
void MainWindow::greyOut2()
{
    new Q_DebugStream(std::cout, ui->routeInfo2); //Redirect Console output to textBrowser2
    Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox2->currentIndex() == 1)
    {
        ui->trainimageButton2->setIcon(*engine1Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox2->currentIndex() == 2)
    {
        ui->trainimageButton2->setIcon(*engine2Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox2->currentIndex() == 3)
    {
        ui->trainimageButton2->setIcon(*engine3Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox2->currentIndex() == 4)
    {
        ui->trainimageButton2->setIcon(*engine4Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox2->currentIndex() == 5)
    {
        ui->trainimageButton2->setIcon(*engine5Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton2->hide();

    int start;
    int end;
    //translate origin box
    if (ui->originBox2->currentIndex() == 1)
        start = 43;
    else if (ui->originBox2->currentIndex() == 2)
        start = 44;
    else if (ui->originBox2->currentIndex() == 3)
        start = 55;
    else if (ui->originBox2->currentIndex() == 4)
        start = 54;
    else if (ui->originBox2->currentIndex() == 5)
        start = 53;
    else if (ui->originBox2->currentIndex() == 6)
        start = 13;
    else if (ui->originBox2->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox2->currentIndex() == 1)
        end = 43;
    else if (ui->destBox2->currentIndex() == 2)
        end = 44;
    else if (ui->destBox2->currentIndex() == 3)
        end = 55;
    else if (ui->destBox2->currentIndex() == 4)
        end = 54;
    else if (ui->destBox2->currentIndex() == 5)
        end = 53;
    else if (ui->destBox2->currentIndex() == 6)
        end = 13;
    else if (ui->destBox2->currentIndex() == 7)
        end = 69;
    //grey out line 2 if radio button selected
    if (ui->setButton2->isChecked()== true)
    {
        ui->trainselectBox2->setDisabled(true);
        ui->originBox2->setDisabled(true);
        ui->destBox2->setDisabled(true);
        ui->throttleBox2->setDisabled(true);
        ui->facingBox2->setDisabled(true);
        ui->headingBox2->setDisabled(true);
        ui->trainimageButton2->show();
        ui->routeInfo2->show();
        //adjacency list selected based on heading
        if (ui->headingBox2->currentIndex() == 1)
        {
        //adjacency list east
        adjacency_list_t adjacency_list(100);

        adjacency_list[0].push_back(neighbor(39, 9));
        adjacency_list[1].push_back(neighbor(0, 9));
        adjacency_list[2].push_back(neighbor(4, 9));
        adjacency_list[3].push_back(neighbor(1, 9));
        adjacency_list[4].push_back(neighbor(72, 9));
        adjacency_list[5].push_back(neighbor(3, 9));
        adjacency_list[7].push_back(neighbor(5, 9));
        adjacency_list[8].push_back(neighbor(7, 9));
        adjacency_list[9].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(11, 9));
        adjacency_list[11].push_back(neighbor(13, 9));
        adjacency_list[14].push_back(neighbor(8, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[26].push_back(neighbor(51, 9));
        adjacency_list[28].push_back(neighbor(26, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[31].push_back(neighbor(70, 9));
        adjacency_list[31].push_back(neighbor(30, 9));
        adjacency_list[32].push_back(neighbor(31, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[35].push_back(neighbor(33, 9));
        adjacency_list[36].push_back(neighbor(35, 9));
        adjacency_list[36].push_back(neighbor(68, 9));
        adjacency_list[38].push_back(neighbor(36, 9));
        adjacency_list[39].push_back(neighbor(38, 9));
        adjacency_list[40].push_back(neighbor(15, 9));
        adjacency_list[41].push_back(neighbor(40, 9));
        adjacency_list[42].push_back(neighbor(41, 9));
        adjacency_list[43].push_back(neighbor(42, 9));
        adjacency_list[44].push_back(neighbor(42, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(53, 9));
        adjacency_list[52].push_back(neighbor(54, 9));
        adjacency_list[51].push_back(neighbor(55, 9));
        adjacency_list[68].push_back(neighbor(69, 9));
        adjacency_list[70].push_back(neighbor(71, 9));
        adjacency_list[71].push_back(neighbor(2, 9));
        adjacency_list[72].push_back(neighbor(73, 9));
        adjacency_list[73].push_back(neighbor(9, 9));

        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }
        //adjacency list selected based on heading
        else if (ui->headingBox2->currentIndex() == 2)
        {
            //adjacency list west
            adjacency_list_t adjacency_list(100);

            adjacency_list[53].push_back(neighbor(52, 9));
            adjacency_list[54].push_back(neighbor(52, 9));
            adjacency_list[55].push_back(neighbor(51, 9));
            adjacency_list[52].push_back(neighbor(51, 9));
            adjacency_list[51].push_back(neighbor(26, 9));
            adjacency_list[26].push_back(neighbor(28, 9));
            adjacency_list[28].push_back(neighbor(29, 9));
            adjacency_list[29].push_back(neighbor(30, 9));
            adjacency_list[30].push_back(neighbor(31, 9));
            adjacency_list[31].push_back(neighbor(32, 9));
            adjacency_list[32].push_back(neighbor(34, 9));
            adjacency_list[34].push_back(neighbor(37, 9));
            adjacency_list[37].push_back(neighbor(39, 9));
            adjacency_list[39].push_back(neighbor(0, 9));
            adjacency_list[0].push_back(neighbor(1, 9));
            adjacency_list[1].push_back(neighbor(3, 9));
            adjacency_list[3].push_back(neighbor(5, 9));
            adjacency_list[5].push_back(neighbor(7, 9));
            adjacency_list[7].push_back(neighbor(9, 9));
            adjacency_list[9].push_back(neighbor(10, 9));
            adjacency_list[10].push_back(neighbor(11, 9));
            adjacency_list[11].push_back(neighbor(13, 9));
            adjacency_list[7].push_back(neighbor(8, 9));
            adjacency_list[8].push_back(neighbor(14, 9));
            adjacency_list[14].push_back(neighbor(15, 9));
            adjacency_list[15].push_back(neighbor(40, 9));
            adjacency_list[40].push_back(neighbor(41, 9));
            adjacency_list[41].push_back(neighbor(42, 9));
            adjacency_list[42].push_back(neighbor(43, 9));
            adjacency_list[42].push_back(neighbor(44, 9));
            adjacency_list[10].push_back(neighbor(12, 9));
            adjacency_list[12].push_back(neighbor(16, 9));
            adjacency_list[16].push_back(neighbor(17, 9));
            adjacency_list[17].push_back(neighbor(19, 9));
            adjacency_list[19].push_back(neighbor(6, 9));
            adjacency_list[6].push_back(neighbor(4, 9));
            adjacency_list[4].push_back(neighbor(2, 9));
            adjacency_list[2].push_back(neighbor(75, 9));
            adjacency_list[75].push_back(neighbor(74, 9));
            adjacency_list[74].push_back(neighbor(38, 9));
            adjacency_list[38].push_back(neighbor(36, 9));
            adjacency_list[36].push_back(neighbor(68, 9));
            adjacency_list[68].push_back(neighbor(69, 9));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
            std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
            std::cout << std::endl;
        }

    }
    //if radio button un-checked then un-greyout line 2
    else
    {
        ui->trainselectBox2->setDisabled(false);
        ui->originBox2->setDisabled(false);
        ui->destBox2->setDisabled(false);
        ui->throttleBox2->setDisabled(false);
        ui->facingBox2->setDisabled(false);
        ui->headingBox2->setDisabled(false);
        ui->trainimageButton2->hide();
        ui->routeInfo2->clear();
        ui->routeInfo2->hide();
    }
}

//lock in and calculate route for line 3
void MainWindow::greyOut3()
{
    new Q_DebugStream(std::cout, ui->routeInfo3); //Redirect Console output to textBrowser2
    Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox3->currentIndex() == 1)
    {
        ui->trainimageButton3->setIcon(*engine1Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 2)
    {
        ui->trainimageButton3->setIcon(*engine2Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 3)
    {
        ui->trainimageButton3->setIcon(*engine3Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 4)
    {
        ui->trainimageButton3->setIcon(*engine4Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 5)
    {
        ui->trainimageButton3->setIcon(*engine5Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton3->hide();

    int start;
    int end;
    //translate origin box
    if (ui->originBox3->currentIndex() == 1)
        start = 43;
    else if (ui->originBox3->currentIndex() == 2)
        start = 44;
    else if (ui->originBox3->currentIndex() == 3)
        start = 55;
    else if (ui->originBox3->currentIndex() == 4)
        start = 54;
    else if (ui->originBox3->currentIndex() == 5)
        start = 53;
    else if (ui->originBox3->currentIndex() == 6)
        start = 13;
    else if (ui->originBox3->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox3->currentIndex() == 1)
        end = 43;
    else if (ui->destBox3->currentIndex() == 2)
        end = 44;
    else if (ui->destBox3->currentIndex() == 3)
        end = 55;
    else if (ui->destBox3->currentIndex() == 4)
        end = 54;
    else if (ui->destBox3->currentIndex() == 5)
        end = 53;
    else if (ui->destBox3->currentIndex() == 6)
        end = 13;
    else if (ui->destBox3->currentIndex() == 7)
        end = 69;
    //grey out line 3 if radio button selected
    if (ui->setButton3->isChecked()== true)
    {
        ui->trainselectBox3->setDisabled(true);
        ui->originBox3->setDisabled(true);
        ui->destBox3->setDisabled(true);
        ui->throttleBox3->setDisabled(true);
        ui->facingBox3->setDisabled(true);
        ui->headingBox3->setDisabled(true);
        ui->trainimageButton3->show();
        ui->routeInfo3->show();

        //adjacency list selected based on heading
        if (ui->headingBox3->currentIndex() == 1)
        {
        //adjacency list east
        adjacency_list_t adjacency_list(100);

        adjacency_list[0].push_back(neighbor(39, 9));
        adjacency_list[1].push_back(neighbor(0, 9));
        adjacency_list[2].push_back(neighbor(4, 9));
        adjacency_list[3].push_back(neighbor(1, 9));
        adjacency_list[4].push_back(neighbor(72, 9));
        adjacency_list[5].push_back(neighbor(3, 9));
        adjacency_list[7].push_back(neighbor(5, 9));
        adjacency_list[8].push_back(neighbor(7, 9));
        adjacency_list[9].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(11, 9));
        adjacency_list[11].push_back(neighbor(13, 9));
        adjacency_list[14].push_back(neighbor(8, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[26].push_back(neighbor(51, 9));
        adjacency_list[28].push_back(neighbor(26, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[31].push_back(neighbor(70, 9));
        adjacency_list[31].push_back(neighbor(30, 9));
        adjacency_list[32].push_back(neighbor(31, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[35].push_back(neighbor(33, 9));
        adjacency_list[36].push_back(neighbor(35, 9));
        adjacency_list[36].push_back(neighbor(68, 9));
        adjacency_list[38].push_back(neighbor(36, 9));
        adjacency_list[39].push_back(neighbor(38, 9));
        adjacency_list[40].push_back(neighbor(15, 9));
        adjacency_list[41].push_back(neighbor(40, 9));
        adjacency_list[42].push_back(neighbor(41, 9));
        adjacency_list[43].push_back(neighbor(42, 9));
        adjacency_list[44].push_back(neighbor(42, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(53, 9));
        adjacency_list[52].push_back(neighbor(54, 9));
        adjacency_list[51].push_back(neighbor(55, 9));
        adjacency_list[68].push_back(neighbor(69, 9));
        adjacency_list[70].push_back(neighbor(71, 9));
        adjacency_list[71].push_back(neighbor(2, 9));
        adjacency_list[72].push_back(neighbor(73, 9));
        adjacency_list[73].push_back(neighbor(9, 9));

        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }
        //adjacency list selected based on heading
        else if (ui->headingBox3->currentIndex() == 2)
        {
            //adjacency list west
            adjacency_list_t adjacency_list(100);

            adjacency_list[53].push_back(neighbor(52, 9));
            adjacency_list[54].push_back(neighbor(52, 9));
            adjacency_list[55].push_back(neighbor(51, 9));
            adjacency_list[52].push_back(neighbor(51, 9));
            adjacency_list[51].push_back(neighbor(26, 9));
            adjacency_list[26].push_back(neighbor(28, 9));
            adjacency_list[28].push_back(neighbor(29, 9));
            adjacency_list[29].push_back(neighbor(30, 9));
            adjacency_list[30].push_back(neighbor(31, 9));
            adjacency_list[31].push_back(neighbor(32, 9));
            adjacency_list[32].push_back(neighbor(34, 9));
            adjacency_list[34].push_back(neighbor(37, 9));
            adjacency_list[37].push_back(neighbor(39, 9));
            adjacency_list[39].push_back(neighbor(0, 9));
            adjacency_list[0].push_back(neighbor(1, 9));
            adjacency_list[1].push_back(neighbor(3, 9));
            adjacency_list[3].push_back(neighbor(5, 9));
            adjacency_list[5].push_back(neighbor(7, 9));
            adjacency_list[7].push_back(neighbor(9, 9));
            adjacency_list[9].push_back(neighbor(10, 9));
            adjacency_list[10].push_back(neighbor(11, 9));
            adjacency_list[11].push_back(neighbor(13, 9));
            adjacency_list[7].push_back(neighbor(8, 9));
            adjacency_list[8].push_back(neighbor(14, 9));
            adjacency_list[14].push_back(neighbor(15, 9));
            adjacency_list[15].push_back(neighbor(40, 9));
            adjacency_list[40].push_back(neighbor(41, 9));
            adjacency_list[41].push_back(neighbor(42, 9));
            adjacency_list[42].push_back(neighbor(43, 9));
            adjacency_list[42].push_back(neighbor(44, 9));
            adjacency_list[10].push_back(neighbor(12, 9));
            adjacency_list[12].push_back(neighbor(16, 9));
            adjacency_list[16].push_back(neighbor(17, 9));
            adjacency_list[17].push_back(neighbor(19, 9));
            adjacency_list[19].push_back(neighbor(6, 9));
            adjacency_list[6].push_back(neighbor(4, 9));
            adjacency_list[4].push_back(neighbor(2, 9));
            adjacency_list[2].push_back(neighbor(75, 9));
            adjacency_list[75].push_back(neighbor(74, 9));
            adjacency_list[74].push_back(neighbor(38, 9));
            adjacency_list[38].push_back(neighbor(36, 9));
            adjacency_list[36].push_back(neighbor(68, 9));
            adjacency_list[68].push_back(neighbor(69, 9));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
            std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
            std::cout << std::endl;
        }

    }
    //if radio button un-checked then un-greyout line 3
    else
    {
        ui->trainselectBox3->setDisabled(false);
        ui->originBox3->setDisabled(false);
        ui->destBox3->setDisabled(false);
        ui->throttleBox3->setDisabled(false);
        ui->facingBox3->setDisabled(false);
        ui->headingBox3->setDisabled(false);
        ui->trainimageButton3->hide();
        ui->routeInfo3->clear();
        ui->routeInfo3->hide();
    }
}

//lock in and calculate route for line 4
void MainWindow::greyOut4()
{
    new Q_DebugStream(std::cout, ui->routeInfo4); //Redirect Console output to textBrowser2
    Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox4->currentIndex() == 1)
    {
        ui->trainimageButton4->setIcon(*engine1Icon);
        ui->trainimageButton4->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox4->currentIndex() == 2)
    {
        ui->trainimageButton4->setIcon(*engine2Icon);
        ui->trainimageButton4->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox4->currentIndex() == 3)
    {
        ui->trainimageButton4->setIcon(*engine3Icon);
        ui->trainimageButton4->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox4->currentIndex() == 4)
    {
        ui->trainimageButton4->setIcon(*engine4Icon);
        ui->trainimageButton4->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox4->currentIndex() == 5)
    {
        ui->trainimageButton4->setIcon(*engine5Icon);
        ui->trainimageButton4->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton4->hide();

    int start;
    int end;
    //translate origin box
    if (ui->originBox4->currentIndex() == 1)
        start = 43;
    else if (ui->originBox4->currentIndex() == 2)
        start = 44;
    else if (ui->originBox4->currentIndex() == 3)
        start = 55;
    else if (ui->originBox4->currentIndex() == 4)
        start = 54;
    else if (ui->originBox4->currentIndex() == 5)
        start = 53;
    else if (ui->originBox4->currentIndex() == 6)
        start = 13;
    else if (ui->originBox4->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox4->currentIndex() == 1)
        end = 43;
    else if (ui->destBox4->currentIndex() == 2)
        end = 44;
    else if (ui->destBox4->currentIndex() == 3)
        end = 55;
    else if (ui->destBox4->currentIndex() == 4)
        end = 54;
    else if (ui->destBox4->currentIndex() == 5)
        end = 53;
    else if (ui->destBox4->currentIndex() == 6)
        end = 13;
    else if (ui->destBox4->currentIndex() == 7)
        end = 69;
    //grey out line 4 if radio button selected
    if (ui->setButton4->isChecked()== true)
    {
        ui->trainselectBox4->setDisabled(true);
        ui->originBox4->setDisabled(true);
        ui->destBox4->setDisabled(true);
        ui->throttleBox4->setDisabled(true);
        ui->facingBox4->setDisabled(true);
        ui->headingBox4->setDisabled(true);
        ui->trainimageButton4->show();
        ui->routeInfo4->show();
        //adjacency list selected based on heading
        if (ui->headingBox4->currentIndex() == 1)
        {
        //adjacency list east
        adjacency_list_t adjacency_list(100);

        adjacency_list[0].push_back(neighbor(39, 9));
        adjacency_list[1].push_back(neighbor(0, 9));
        adjacency_list[2].push_back(neighbor(4, 9));
        adjacency_list[3].push_back(neighbor(1, 9));
        adjacency_list[4].push_back(neighbor(72, 9));
        adjacency_list[5].push_back(neighbor(3, 9));
        adjacency_list[7].push_back(neighbor(5, 9));
        adjacency_list[8].push_back(neighbor(7, 9));
        adjacency_list[9].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(11, 9));
        adjacency_list[11].push_back(neighbor(13, 9));
        adjacency_list[14].push_back(neighbor(8, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[26].push_back(neighbor(51, 9));
        adjacency_list[28].push_back(neighbor(26, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[31].push_back(neighbor(70, 9));
        adjacency_list[31].push_back(neighbor(30, 9));
        adjacency_list[32].push_back(neighbor(31, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[35].push_back(neighbor(33, 9));
        adjacency_list[36].push_back(neighbor(35, 9));
        adjacency_list[36].push_back(neighbor(68, 9));
        adjacency_list[38].push_back(neighbor(36, 9));
        adjacency_list[39].push_back(neighbor(38, 9));
        adjacency_list[40].push_back(neighbor(15, 9));
        adjacency_list[41].push_back(neighbor(40, 9));
        adjacency_list[42].push_back(neighbor(41, 9));
        adjacency_list[43].push_back(neighbor(42, 9));
        adjacency_list[44].push_back(neighbor(42, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(53, 9));
        adjacency_list[52].push_back(neighbor(54, 9));
        adjacency_list[51].push_back(neighbor(55, 9));
        adjacency_list[68].push_back(neighbor(69, 9));
        adjacency_list[70].push_back(neighbor(71, 9));
        adjacency_list[71].push_back(neighbor(2, 9));
        adjacency_list[72].push_back(neighbor(73, 9));
        adjacency_list[73].push_back(neighbor(9, 9));

        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }
        //adjacency list selected based on heading
        else if (ui->headingBox4->currentIndex() == 2)
        {
            //adjacency list west
            adjacency_list_t adjacency_list(100);

            adjacency_list[53].push_back(neighbor(52, 9));
            adjacency_list[54].push_back(neighbor(52, 9));
            adjacency_list[55].push_back(neighbor(51, 9));
            adjacency_list[52].push_back(neighbor(51, 9));
            adjacency_list[51].push_back(neighbor(26, 9));
            adjacency_list[26].push_back(neighbor(28, 9));
            adjacency_list[28].push_back(neighbor(29, 9));
            adjacency_list[29].push_back(neighbor(30, 9));
            adjacency_list[30].push_back(neighbor(31, 9));
            adjacency_list[31].push_back(neighbor(32, 9));
            adjacency_list[32].push_back(neighbor(34, 9));
            adjacency_list[34].push_back(neighbor(37, 9));
            adjacency_list[37].push_back(neighbor(39, 9));
            adjacency_list[39].push_back(neighbor(0, 9));
            adjacency_list[0].push_back(neighbor(1, 9));
            adjacency_list[1].push_back(neighbor(3, 9));
            adjacency_list[3].push_back(neighbor(5, 9));
            adjacency_list[5].push_back(neighbor(7, 9));
            adjacency_list[7].push_back(neighbor(9, 9));
            adjacency_list[9].push_back(neighbor(10, 9));
            adjacency_list[10].push_back(neighbor(11, 9));
            adjacency_list[11].push_back(neighbor(13, 9));
            adjacency_list[7].push_back(neighbor(8, 9));
            adjacency_list[8].push_back(neighbor(14, 9));
            adjacency_list[14].push_back(neighbor(15, 9));
            adjacency_list[15].push_back(neighbor(40, 9));
            adjacency_list[40].push_back(neighbor(41, 9));
            adjacency_list[41].push_back(neighbor(42, 9));
            adjacency_list[42].push_back(neighbor(43, 9));
            adjacency_list[42].push_back(neighbor(44, 9));
            adjacency_list[10].push_back(neighbor(12, 9));
            adjacency_list[12].push_back(neighbor(16, 9));
            adjacency_list[16].push_back(neighbor(17, 9));
            adjacency_list[17].push_back(neighbor(19, 9));
            adjacency_list[19].push_back(neighbor(6, 9));
            adjacency_list[6].push_back(neighbor(4, 9));
            adjacency_list[4].push_back(neighbor(2, 9));
            adjacency_list[2].push_back(neighbor(75, 9));
            adjacency_list[75].push_back(neighbor(74, 9));
            adjacency_list[74].push_back(neighbor(38, 9));
            adjacency_list[38].push_back(neighbor(36, 9));
            adjacency_list[36].push_back(neighbor(68, 9));
            adjacency_list[68].push_back(neighbor(69, 9));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
            std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
            std::cout << std::endl;
        }

    }
    //if radio button un-checked then un-greyout line 4
    else
    {
        ui->trainselectBox4->setDisabled(false);
        ui->originBox4->setDisabled(false);
        ui->destBox4->setDisabled(false);
        ui->throttleBox4->setDisabled(false);
        ui->facingBox4->setDisabled(false);
        ui->headingBox4->setDisabled(false);
        ui->trainimageButton4->hide();
        ui->routeInfo4->clear();
        ui->routeInfo4->hide();
    }
}

//lock in and calculate route for line 5
void MainWindow::greyOut5()
{
    new Q_DebugStream(std::cout, ui->routeInfo5); //Redirect Console output to textBrowser2
    Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox5->currentIndex() == 1)
    {
        ui->trainimageButton5->setIcon(*engine1Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox5->currentIndex() == 2)
    {
        ui->trainimageButton5->setIcon(*engine2Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox5->currentIndex() == 3)
    {
        ui->trainimageButton5->setIcon(*engine3Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox5->currentIndex() == 4)
    {
        ui->trainimageButton5->setIcon(*engine4Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox5->currentIndex() == 5)
    {
        ui->trainimageButton5->setIcon(*engine5Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton5->hide();

    int start;
    int end;
    //translate origin box
    if (ui->originBox5->currentIndex() == 1)
        start = 43;
    else if (ui->originBox5->currentIndex() == 2)
        start = 44;
    else if (ui->originBox5->currentIndex() == 3)
        start = 55;
    else if (ui->originBox5->currentIndex() == 4)
        start = 54;
    else if (ui->originBox5->currentIndex() == 5)
        start = 53;
    else if (ui->originBox5->currentIndex() == 6)
        start = 13;
    else if (ui->originBox5->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox5->currentIndex() == 1)
        end = 43;
    else if (ui->destBox5->currentIndex() == 2)
        end = 44;
    else if (ui->destBox5->currentIndex() == 3)
        end = 55;
    else if (ui->destBox5->currentIndex() == 4)
        end = 54;
    else if (ui->destBox5->currentIndex() == 5)
        end = 53;
    else if (ui->destBox5->currentIndex() == 6)
        end = 13;
    else if (ui->destBox5->currentIndex() == 7)
        end = 69;
    //grey out line 5 if radio button selected
    if (ui->setButton5->isChecked()== true)
    {
        ui->trainselectBox5->setDisabled(true);
        ui->originBox5->setDisabled(true);
        ui->destBox5->setDisabled(true);
        ui->throttleBox5->setDisabled(true);
        ui->facingBox5->setDisabled(true);
        ui->headingBox5->setDisabled(true);
        ui->trainimageButton5->show();
        ui->routeInfo5->show();
        //adjacency list selected based on heading
        if (ui->headingBox5->currentIndex() == 1)
        {
        //adjacency list east
        adjacency_list_t adjacency_list(100);

        adjacency_list[0].push_back(neighbor(39, 9));
        adjacency_list[1].push_back(neighbor(0, 9));
        adjacency_list[2].push_back(neighbor(4, 9));
        adjacency_list[3].push_back(neighbor(1, 9));
        adjacency_list[4].push_back(neighbor(72, 9));
        adjacency_list[5].push_back(neighbor(3, 9));
        adjacency_list[7].push_back(neighbor(5, 9));
        adjacency_list[8].push_back(neighbor(7, 9));
        adjacency_list[9].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(11, 9));
        adjacency_list[11].push_back(neighbor(13, 9));
        adjacency_list[14].push_back(neighbor(8, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[26].push_back(neighbor(51, 9));
        adjacency_list[28].push_back(neighbor(26, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[31].push_back(neighbor(70, 9));
        adjacency_list[31].push_back(neighbor(30, 9));
        adjacency_list[32].push_back(neighbor(31, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[35].push_back(neighbor(33, 9));
        adjacency_list[36].push_back(neighbor(35, 9));
        adjacency_list[36].push_back(neighbor(68, 9));
        adjacency_list[38].push_back(neighbor(36, 9));
        adjacency_list[39].push_back(neighbor(38, 9));
        adjacency_list[40].push_back(neighbor(15, 9));
        adjacency_list[41].push_back(neighbor(40, 9));
        adjacency_list[42].push_back(neighbor(41, 9));
        adjacency_list[43].push_back(neighbor(42, 9));
        adjacency_list[44].push_back(neighbor(42, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(53, 9));
        adjacency_list[52].push_back(neighbor(54, 9));
        adjacency_list[51].push_back(neighbor(55, 9));
        adjacency_list[68].push_back(neighbor(69, 9));
        adjacency_list[70].push_back(neighbor(71, 9));
        adjacency_list[71].push_back(neighbor(2, 9));
        adjacency_list[72].push_back(neighbor(73, 9));
        adjacency_list[73].push_back(neighbor(9, 9));

        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }
        //adjacency list selected based on heading
        else if (ui->headingBox5->currentIndex() == 2)
        {
            //adjacency list west
            adjacency_list_t adjacency_list(100);

            adjacency_list[53].push_back(neighbor(52, 9));
            adjacency_list[54].push_back(neighbor(52, 9));
            adjacency_list[55].push_back(neighbor(51, 9));
            adjacency_list[52].push_back(neighbor(51, 9));
            adjacency_list[51].push_back(neighbor(26, 9));
            adjacency_list[26].push_back(neighbor(28, 9));
            adjacency_list[28].push_back(neighbor(29, 9));
            adjacency_list[29].push_back(neighbor(30, 9));
            adjacency_list[30].push_back(neighbor(31, 9));
            adjacency_list[31].push_back(neighbor(32, 9));
            adjacency_list[32].push_back(neighbor(34, 9));
            adjacency_list[34].push_back(neighbor(37, 9));
            adjacency_list[37].push_back(neighbor(39, 9));
            adjacency_list[39].push_back(neighbor(0, 9));
            adjacency_list[0].push_back(neighbor(1, 9));
            adjacency_list[1].push_back(neighbor(3, 9));
            adjacency_list[3].push_back(neighbor(5, 9));
            adjacency_list[5].push_back(neighbor(7, 9));
            adjacency_list[7].push_back(neighbor(9, 9));
            adjacency_list[9].push_back(neighbor(10, 9));
            adjacency_list[10].push_back(neighbor(11, 9));
            adjacency_list[11].push_back(neighbor(13, 9));
            adjacency_list[7].push_back(neighbor(8, 9));
            adjacency_list[8].push_back(neighbor(14, 9));
            adjacency_list[14].push_back(neighbor(15, 9));
            adjacency_list[15].push_back(neighbor(40, 9));
            adjacency_list[40].push_back(neighbor(41, 9));
            adjacency_list[41].push_back(neighbor(42, 9));
            adjacency_list[42].push_back(neighbor(43, 9));
            adjacency_list[42].push_back(neighbor(44, 9));
            adjacency_list[10].push_back(neighbor(12, 9));
            adjacency_list[12].push_back(neighbor(16, 9));
            adjacency_list[16].push_back(neighbor(17, 9));
            adjacency_list[17].push_back(neighbor(19, 9));
            adjacency_list[19].push_back(neighbor(6, 9));
            adjacency_list[6].push_back(neighbor(4, 9));
            adjacency_list[4].push_back(neighbor(2, 9));
            adjacency_list[2].push_back(neighbor(75, 9));
            adjacency_list[75].push_back(neighbor(74, 9));
            adjacency_list[74].push_back(neighbor(38, 9));
            adjacency_list[38].push_back(neighbor(36, 9));
            adjacency_list[36].push_back(neighbor(68, 9));
            adjacency_list[68].push_back(neighbor(69, 9));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
            std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
            std::cout << std::endl;
        }

    }
    //if radio button un-checked then un-greyout line 5
    else
    {
        ui->trainselectBox5->setDisabled(false);
        ui->originBox5->setDisabled(false);
        ui->destBox5->setDisabled(false);
        ui->throttleBox5->setDisabled(false);
        ui->facingBox5->setDisabled(false);
        ui->headingBox5->setDisabled(false);
        ui->trainimageButton5->hide();
        ui->routeInfo5->clear();
        ui->routeInfo5->hide();
    }
}

