#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
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
#include <QPushButton>
#include <QQueue>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>
#include <QTextBrowser>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QtSql>
#define infinity 9999999

using namespace std;
typedef long vertex_t;
typedef long double weight_t;

const weight_t max_weight = infinity;

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
    ui->setupUi(this);

/*
    ldb = QSqlDatabase::addDatabase("QSQLITE", "ldb");
    ldb.setDatabaseName( "train.db" );
    if(!db.isOpen())
    qDebug() << ldb.lastError;

*/
    //Start and Start button graphics
    startIcon = new QIcon("startbutton.png");
    stopIcon = new QIcon("stop.jpg");
    ui->startButton->setIcon(*startIcon);
    ui->startButton->setIconSize(QSize(65,65));
    ui->stopButton->setIcon(*stopIcon);
    ui->stopButton->setIconSize(QSize(35,35));

    //start button starts trains and periodic route calculations
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(calculateRoute()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopTimer()));

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
    delete ui;
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
    QSettings settings("mytrainsettings.ini",QSettings::IniFormat);
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
    ui->startButton->setDisabled(true); //disabled start button to avoid throwing timer off

    QTimer *recalculateTimer = new QTimer(this);

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
        //adjacency list selected based on heading
        if (ui->headingBox1->currentIndex() == 1)
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
        std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << min_distance[end] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
    
    
    
        
        
        // test code for reading the vector to a database
        int pathSize = path.size();
        cout << "The total number of hops is " << pathSize << endl;
        //convert the list to an array
        int *copyarray = new int[pathSize];
        for (int i=0; i<pathSize; i++)
        {
            copyarray[i] = path.front();
            path.pop_front();
        }
        for (int i=0; i<pathSize; i++)
            cout << copyarray[i] << " ";

        //Use array to fill out traininfo and pathinfo table. Values needed: values in the array, value of the "trainselectBox1" box,
        //final node, initial node, number of rows on the path info table
/*
        QSqlQuery q(ldb);
        int numRows = q.exec("SELECT COUNT(*) FROM pathinfo;");
        numRows++;
        //q.prepare("UPDATE pathInfo SET ")

        
        q.prepare("UPDATE traininfo SET current= ':cL', destination= ':dL', next= ':nL' WHERE trainID=':tid';");
        q.bindvalue(":tid", ui->trainselectBox1->currentText());
        q.bindvalue(":cL", copyarray[1] );
        q.bindvalue(":dL", copyarray[pathSize]);
        q.bindvalue(":nL", copyarray[2]);

        if(!q.exec())
        {
            cout << "Failed to update database";
        }

*/


        //
        
        
        
        
        
        
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
    }
}

//lock in and calculate route for line 2
void MainWindow::greyOut2()
{
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
    }
}

//lock in and calculate route for line 3
void MainWindow::greyOut3()
{
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
    }
}

//lock in and calculate route for line 4
void MainWindow::greyOut4()
{
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
    }
}

//lock in and calculate route for line 5
void MainWindow::greyOut5()
{
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
    }
}

