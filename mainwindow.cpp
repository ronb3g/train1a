#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QLCDNumber>
#include <QTime>
#include <QPixmap>
//#include <stdafx>
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
#include <stdafx.h>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>

using namespace std;
typedef long vertex_t;
    typedef double weight_t;

    const weight_t max_weight = std::numeric_limits<double>::infinity();

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

                    int za = distance_through_u;
                    int zb = neighbor_iter->target;
                    int zc = weight;
                    int zd = min_distance[v];


                    if (distance_through_u < min_distance[v]) {
                        vertex_queue.erase(std::make_pair(min_distance[v], v));

                        min_distance[v] = distance_through_u;
                        previous[v] = u;
                        vertex_queue.insert(std::make_pair(min_distance[v], v));

                    }

                }
            }
        }
    std::list<vertex_t> DijkstraGetShortestPathTo(
            vertex_t vertex, const std::vector<vertex_t> &previous)
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

    startIcon = new QIcon("startbutton.png");
    stopIcon = new QIcon("stop.jpg");

    startIcon = new QIcon("startbutton.png");
    stopIcon = new QIcon("stop.jpg");
    ui->startButton->setIcon(*startIcon);
    ui->startButton->setIconSize(QSize(65,65));
    ui->stopButton->setIcon(*stopIcon);
    ui->stopButton->setIconSize(QSize(35,35));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(calculateRoute()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopTimer()));




    connect(ui->setButton1, SIGNAL(clicked(bool)), this, SLOT(greyOut1()));
    connect(ui->setButton2, SIGNAL(clicked(bool)), this, SLOT(greyOut2()));
    connect(ui->setButton3, SIGNAL(clicked(bool)), this, SLOT(greyOut3()));
    connect(ui->setButton4, SIGNAL(clicked(bool)), this, SLOT(greyOut4()));
    connect(ui->setButton5, SIGNAL(clicked(bool)), this, SLOT(greyOut5()));

    connect(ui->occButton, SIGNAL(clicked()), this, SLOT(occupiedNode()));
    connect(ui->destButton, SIGNAL(clicked()), this, SLOT(destNode()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
       timer->start(1000);
       showTime();

       savefile = new QAction("Save config", this);
       loadfile = new QAction("Load config", this);
       connect(loadfile, SIGNAL(triggered()), this, SLOT(loadText()));
       //menu = menuBar()->addMenu("Config");
       ui->menuCPE_453_Team_1A->addAction(loadfile);
       ui->menuCPE_453_Team_1A->addAction(savefile);
       connect(savefile, SIGNAL(triggered()), this, SLOT(saveText()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveText()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save config"), "/", tr("Text (*.txt)"));
    QFile outputFile(fileName);
    //QString settingsName = QSettings::fileName()

    if (fileName.isEmpty())
    {
        return;
    }

    else if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString ba = ui->trainselectBox1->currentText();
       // outputFile.write(ba);
        outputFile.close();
    }

}
void MainWindow::loadText()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Text (*.txt)"));
    QFile inputFile(fileName);

    if (fileName.isEmpty())
    {
        return;
    }

    else if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString ba = inputFile.readAll();
        //ui->trainselectBox1->
        //ui->trainselectBox1->set
        inputFile.close();
    }
}


void MainWindow::occupiedNode()
{
    ui->originBox1->addItem(ui->addorlineEdit3->text());
    ui->originBox2->addItem(ui->addorlineEdit3->text());
    ui->originBox3->addItem(ui->addorlineEdit3->text());
    ui->originBox4->addItem(ui->addorlineEdit3->text());
    ui->originBox5->addItem(ui->addorlineEdit3->text());
}

void MainWindow::destNode()
{
    ui->destBox1->addItem((ui->adddestlineEdit4->text()));
    ui->destBox2->addItem((ui->adddestlineEdit4->text()));
    ui->destBox3->addItem((ui->adddestlineEdit4->text()));
    ui->destBox4->addItem((ui->adddestlineEdit4->text()));
    ui->destBox5->addItem((ui->adddestlineEdit4->text()));
}

void MainWindow::stopTimer()
{
    //recalculateTimer->stop();
}

void MainWindow::calculateRoute()
{   ui->startButton->setDisabled(true);

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

void MainWindow::showTime()
{

    QTime time = QTime::currentTime();

    QString text = time.toString("hh:mm:ssap");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcdNumber->display(text);
}

void MainWindow::greyOut1()
{
    if (ui->setButton1->isChecked()== true)
    {
        ui->trainselectBox1->setDisabled(true);
        ui->originBox1->setDisabled(true);
        ui->destBox1->setDisabled(true);
        ui->throttleBox1->setDisabled(true);
        ui->facingBox1->setDisabled(true);
        ui->headingBox1->setDisabled(true);
        //calculateRoute();
        adjacency_list_t adjacency_list(69);
        adjacency_list[1].push_back(neighbor(2, 7));
        adjacency_list[1].push_back(neighbor(36, 7));
        // 1 = b
        adjacency_list[2].push_back(neighbor(3, 7));

        // 2 = c
        adjacency_list[3].push_back(neighbor(4, 9));

        // 3 = d
        adjacency_list[4].push_back(neighbor(5, 15));

        // 4 = e
        adjacency_list[5].push_back(neighbor(6, 6));

        // 5 = f

        adjacency_list[6].push_back(neighbor(7, 9));

        adjacency_list[7].push_back(neighbor(8, 9));
        adjacency_list[7].push_back(neighbor(37, 9));

        adjacency_list[37].push_back(neighbor(38, 1));
        adjacency_list[38].push_back(neighbor(39, 9));
        adjacency_list[39].push_back(neighbor(40, 9));
        adjacency_list[40].push_back(neighbor(41, 9));

        adjacency_list[37].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(45, 9));

        adjacency_list[45].push_back(neighbor(46, 1));
        adjacency_list[46].push_back(neighbor(45, 9));
        adjacency_list[45].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(67, 9));
        adjacency_list[67].push_back(neighbor(68, 9));
        adjacency_list[68].push_back(neighbor(6, 9));



        adjacency_list[8].push_back(neighbor(9, 9));

        adjacency_list[9].push_back(neighbor(10, 9));

        adjacency_list[10].push_back(neighbor(11, 1));
        adjacency_list[11].push_back(neighbor(12, 9));
        adjacency_list[12].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(14, 9));

        adjacency_list[14].push_back(neighbor(15, 1));
        adjacency_list[15].push_back(neighbor(16, 9));
        adjacency_list[16].push_back(neighbor(17, 9));
        adjacency_list[17].push_back(neighbor(18, 9));

        adjacency_list[18].push_back(neighbor(19, 1));
        adjacency_list[19].push_back(neighbor(20, 9));
        adjacency_list[20].push_back(neighbor(21, 9));
        adjacency_list[21].push_back(neighbor(22, 9));

        adjacency_list[22].push_back(neighbor(23, 1));
        adjacency_list[23].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(25, 9));
        adjacency_list[25].push_back(neighbor(26, 9));

        adjacency_list[26].push_back(neighbor(27, 1));
        adjacency_list[27].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(30, 9));

        adjacency_list[30].push_back(neighbor(31, 1));
        adjacency_list[31].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(34, 9));

        adjacency_list[34].push_back(neighbor(35, 1));
        //adjacency_list[35].push_back(neighbor(34, 9));
        adjacency_list[36].push_back(neighbor(1, 9));
        adjacency_list[36].push_back(neighbor(64, 7));

        adjacency_list[35].push_back(neighbor(36, 9));
        adjacency_list[58].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(60, 9));
        adjacency_list[60].push_back(neighbor(61, 9));
        adjacency_list[61].push_back(neighbor(62, 7));
        adjacency_list[62].push_back(neighbor(66, 7));

        adjacency_list[66].push_back(neighbor(54, 9));
        adjacency_list[54].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(63, 7));
        adjacency_list[63].push_back(neighbor(19, 9));

        adjacency_list[64].push_back(neighbor(65, 1));
        adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[65].push_back(neighbor(47, 9));
        adjacency_list[47].push_back(neighbor(48, 7));
        adjacency_list[48].push_back(neighbor(49, 7));
        adjacency_list[49].push_back(neighbor(50, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(66, 7));


        //adjacency_list[34].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));


        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(27, 9));
        adjacency_list[27].push_back(neighbor(26, 7));
        adjacency_list[26].push_back(neighbor(25, 7));


        adjacency_list[25].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(23, 9));
        adjacency_list[23].push_back(neighbor(22, 9));
        adjacency_list[22].push_back(neighbor(21, 7));
        adjacency_list[21].push_back(neighbor(20, 7));


        adjacency_list[20].push_back(neighbor(19, 9));
        adjacency_list[19].push_back(neighbor(63, 9));
        adjacency_list[63].push_back(neighbor(17, 9));
        adjacency_list[63].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(54, 7));
        adjacency_list[54].push_back(neighbor(53, 7));
        adjacency_list[53].push_back(neighbor(52, 9));
        adjacency_list[53].push_back(neighbor(62, 9));
        adjacency_list[52].push_back(neighbor(51, 7));
        adjacency_list[51].push_back(neighbor(50, 7));
        adjacency_list[50].push_back(neighbor(49, 9));
        adjacency_list[49].push_back(neighbor(48, 9));
        adjacency_list[48].push_back(neighbor(47, 7));
        adjacency_list[47].push_back(neighbor(34, 7));
        //adjacency_list[65].push_back(neighbor(64, 9));
        //adjacency_list[64].push_back(neighbor(36, 9));
        adjacency_list[62].push_back(neighbor(61, 7));
        adjacency_list[61].push_back(neighbor(60, 7));
        adjacency_list[60].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(58, 9));
        adjacency_list[58].push_back(neighbor(35, 7));

        adjacency_list[17].push_back(neighbor(16, 7));


        adjacency_list[16].push_back(neighbor(15, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[14].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(12, 7));
        adjacency_list[12].push_back(neighbor(11, 7));

        adjacency_list[11].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(9, 9));
        adjacency_list[9].push_back(neighbor(8, 9));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[7].push_back(neighbor(6, 7));


        adjacency_list[6].push_back(neighbor(5, 9));
        adjacency_list[5].push_back(neighbor(4, 9));
        adjacency_list[4].push_back(neighbor(3, 9));
        adjacency_list[3].push_back(neighbor(2, 7));
        adjacency_list[2].push_back(neighbor(1, 7));


        //adjacency_list[1].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(ui->originBox1->currentIndex(), adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox1->currentIndex() << " to " << ui->destBox1->currentIndex() <<  ": " << min_distance[4] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->destBox1->currentIndex(), previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;

    }
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
void MainWindow::greyOut2()
{
    if (ui->setButton2->isChecked()== true)
    {
        ui->trainselectBox2->setDisabled(true);
        ui->originBox2->setDisabled(true);
        ui->destBox2->setDisabled(true);
        ui->throttleBox2->setDisabled(true);
        ui->facingBox2->setDisabled(true);
        ui->headingBox2->setDisabled(true);
        adjacency_list_t adjacency_list(69);
        adjacency_list[1].push_back(neighbor(2, 7));
        adjacency_list[1].push_back(neighbor(36, 7));
        // 1 = b
        adjacency_list[2].push_back(neighbor(3, 7));

        // 2 = c
        adjacency_list[3].push_back(neighbor(4, 9));

        // 3 = d
        adjacency_list[4].push_back(neighbor(5, 15));

        // 4 = e
        adjacency_list[5].push_back(neighbor(6, 6));

        // 5 = f

        adjacency_list[6].push_back(neighbor(7, 9));

        adjacency_list[7].push_back(neighbor(8, 9));
        adjacency_list[7].push_back(neighbor(37, 9));

        adjacency_list[37].push_back(neighbor(38, 1));
        adjacency_list[38].push_back(neighbor(39, 9));
        adjacency_list[39].push_back(neighbor(40, 9));
        adjacency_list[40].push_back(neighbor(41, 9));

        adjacency_list[37].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(45, 9));

        adjacency_list[45].push_back(neighbor(46, 1));
        adjacency_list[46].push_back(neighbor(45, 9));
        adjacency_list[45].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(67, 9));
        adjacency_list[67].push_back(neighbor(68, 9));
        adjacency_list[68].push_back(neighbor(6, 9));



        adjacency_list[8].push_back(neighbor(9, 9));

        adjacency_list[9].push_back(neighbor(10, 9));

        adjacency_list[10].push_back(neighbor(11, 1));
        adjacency_list[11].push_back(neighbor(12, 9));
        adjacency_list[12].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(14, 9));

        adjacency_list[14].push_back(neighbor(15, 1));
        adjacency_list[15].push_back(neighbor(16, 9));
        adjacency_list[16].push_back(neighbor(17, 9));
        adjacency_list[17].push_back(neighbor(18, 9));

        adjacency_list[18].push_back(neighbor(19, 1));
        adjacency_list[19].push_back(neighbor(20, 9));
        adjacency_list[20].push_back(neighbor(21, 9));
        adjacency_list[21].push_back(neighbor(22, 9));

        adjacency_list[22].push_back(neighbor(23, 1));
        adjacency_list[23].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(25, 9));
        adjacency_list[25].push_back(neighbor(26, 9));

        adjacency_list[26].push_back(neighbor(27, 1));
        adjacency_list[27].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(30, 9));

        adjacency_list[30].push_back(neighbor(31, 1));
        adjacency_list[31].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(34, 9));

        adjacency_list[34].push_back(neighbor(35, 1));
        //adjacency_list[35].push_back(neighbor(34, 9));
        adjacency_list[36].push_back(neighbor(1, 9));
        adjacency_list[36].push_back(neighbor(64, 7));

        adjacency_list[35].push_back(neighbor(36, 9));
        adjacency_list[58].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(60, 9));
        adjacency_list[60].push_back(neighbor(61, 9));
        adjacency_list[61].push_back(neighbor(62, 7));
        adjacency_list[62].push_back(neighbor(66, 7));

        adjacency_list[66].push_back(neighbor(54, 9));
        adjacency_list[54].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(63, 7));
        adjacency_list[63].push_back(neighbor(19, 9));

        adjacency_list[64].push_back(neighbor(65, 1));
        adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[65].push_back(neighbor(47, 9));
        adjacency_list[47].push_back(neighbor(48, 7));
        adjacency_list[48].push_back(neighbor(49, 7));
        adjacency_list[49].push_back(neighbor(50, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(66, 7));


        //adjacency_list[34].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));


        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(27, 9));
        adjacency_list[27].push_back(neighbor(26, 7));
        adjacency_list[26].push_back(neighbor(25, 7));


        adjacency_list[25].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(23, 9));
        adjacency_list[23].push_back(neighbor(22, 9));
        adjacency_list[22].push_back(neighbor(21, 7));
        adjacency_list[21].push_back(neighbor(20, 7));


        adjacency_list[20].push_back(neighbor(19, 9));
        adjacency_list[19].push_back(neighbor(63, 9));
        adjacency_list[63].push_back(neighbor(17, 9));
        adjacency_list[63].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(54, 7));
        adjacency_list[54].push_back(neighbor(53, 7));
        adjacency_list[53].push_back(neighbor(52, 9));
        adjacency_list[53].push_back(neighbor(62, 9));
        adjacency_list[52].push_back(neighbor(51, 7));
        adjacency_list[51].push_back(neighbor(50, 7));
        adjacency_list[50].push_back(neighbor(49, 9));
        adjacency_list[49].push_back(neighbor(48, 9));
        adjacency_list[48].push_back(neighbor(47, 7));
        adjacency_list[47].push_back(neighbor(34, 7));
        //adjacency_list[65].push_back(neighbor(64, 9));
        //adjacency_list[64].push_back(neighbor(36, 9));
        adjacency_list[62].push_back(neighbor(61, 7));
        adjacency_list[61].push_back(neighbor(60, 7));
        adjacency_list[60].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(58, 9));
        adjacency_list[58].push_back(neighbor(35, 7));

        adjacency_list[17].push_back(neighbor(16, 7));


        adjacency_list[16].push_back(neighbor(15, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[14].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(12, 7));
        adjacency_list[12].push_back(neighbor(11, 7));

        adjacency_list[11].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(9, 9));
        adjacency_list[9].push_back(neighbor(8, 9));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[7].push_back(neighbor(6, 7));


        adjacency_list[6].push_back(neighbor(5, 9));
        adjacency_list[5].push_back(neighbor(4, 9));
        adjacency_list[4].push_back(neighbor(3, 9));
        adjacency_list[3].push_back(neighbor(2, 7));
        adjacency_list[2].push_back(neighbor(1, 7));


        //adjacency_list[1].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(ui->originBox2->currentIndex(), adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox2->currentIndex() << " to " << ui->destBox2->currentIndex() <<  ": " << min_distance[4] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->destBox2->currentIndex(), previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
    }
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
void MainWindow::greyOut3()
{
    if (ui->setButton3->isChecked()== true)
    {
        ui->trainselectBox3->setDisabled(true);
        ui->originBox3->setDisabled(true);
        ui->destBox3->setDisabled(true);
        ui->throttleBox3->setDisabled(true);
        ui->facingBox3->setDisabled(true);
        ui->headingBox3->setDisabled(true);
        adjacency_list_t adjacency_list(69);
        adjacency_list[1].push_back(neighbor(2, 7));
        adjacency_list[1].push_back(neighbor(36, 7));
        // 1 = b
        adjacency_list[2].push_back(neighbor(3, 7));

        // 2 = c
        adjacency_list[3].push_back(neighbor(4, 9));

        // 3 = d
        adjacency_list[4].push_back(neighbor(5, 15));

        // 4 = e
        adjacency_list[5].push_back(neighbor(6, 6));

        // 5 = f

        adjacency_list[6].push_back(neighbor(7, 9));

        adjacency_list[7].push_back(neighbor(8, 9));
        adjacency_list[7].push_back(neighbor(37, 9));

        adjacency_list[37].push_back(neighbor(38, 1));
        adjacency_list[38].push_back(neighbor(39, 9));
        adjacency_list[39].push_back(neighbor(40, 9));
        adjacency_list[40].push_back(neighbor(41, 9));

        adjacency_list[37].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(45, 9));

        adjacency_list[45].push_back(neighbor(46, 1));
        adjacency_list[46].push_back(neighbor(45, 9));
        adjacency_list[45].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(67, 9));
        adjacency_list[67].push_back(neighbor(68, 9));
        adjacency_list[68].push_back(neighbor(6, 9));



        adjacency_list[8].push_back(neighbor(9, 9));

        adjacency_list[9].push_back(neighbor(10, 9));

        adjacency_list[10].push_back(neighbor(11, 1));
        adjacency_list[11].push_back(neighbor(12, 9));
        adjacency_list[12].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(14, 9));

        adjacency_list[14].push_back(neighbor(15, 1));
        adjacency_list[15].push_back(neighbor(16, 9));
        adjacency_list[16].push_back(neighbor(17, 9));
        adjacency_list[17].push_back(neighbor(18, 9));

        adjacency_list[18].push_back(neighbor(19, 1));
        adjacency_list[19].push_back(neighbor(20, 9));
        adjacency_list[20].push_back(neighbor(21, 9));
        adjacency_list[21].push_back(neighbor(22, 9));

        adjacency_list[22].push_back(neighbor(23, 1));
        adjacency_list[23].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(25, 9));
        adjacency_list[25].push_back(neighbor(26, 9));

        adjacency_list[26].push_back(neighbor(27, 1));
        adjacency_list[27].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(30, 9));

        adjacency_list[30].push_back(neighbor(31, 1));
        adjacency_list[31].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(34, 9));

        adjacency_list[34].push_back(neighbor(35, 1));
        //adjacency_list[35].push_back(neighbor(34, 9));
        adjacency_list[36].push_back(neighbor(1, 9));
        adjacency_list[36].push_back(neighbor(64, 7));

        adjacency_list[35].push_back(neighbor(36, 9));
        adjacency_list[58].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(60, 9));
        adjacency_list[60].push_back(neighbor(61, 9));
        adjacency_list[61].push_back(neighbor(62, 7));
        adjacency_list[62].push_back(neighbor(66, 7));

        adjacency_list[66].push_back(neighbor(54, 9));
        adjacency_list[54].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(63, 7));
        adjacency_list[63].push_back(neighbor(19, 9));

        adjacency_list[64].push_back(neighbor(65, 1));
        adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[65].push_back(neighbor(47, 9));
        adjacency_list[47].push_back(neighbor(48, 7));
        adjacency_list[48].push_back(neighbor(49, 7));
        adjacency_list[49].push_back(neighbor(50, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(66, 7));


        //adjacency_list[34].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));


        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(27, 9));
        adjacency_list[27].push_back(neighbor(26, 7));
        adjacency_list[26].push_back(neighbor(25, 7));


        adjacency_list[25].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(23, 9));
        adjacency_list[23].push_back(neighbor(22, 9));
        adjacency_list[22].push_back(neighbor(21, 7));
        adjacency_list[21].push_back(neighbor(20, 7));


        adjacency_list[20].push_back(neighbor(19, 9));
        adjacency_list[19].push_back(neighbor(63, 9));
        adjacency_list[63].push_back(neighbor(17, 9));
        adjacency_list[63].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(54, 7));
        adjacency_list[54].push_back(neighbor(53, 7));
        adjacency_list[53].push_back(neighbor(52, 9));
        adjacency_list[53].push_back(neighbor(62, 9));
        adjacency_list[52].push_back(neighbor(51, 7));
        adjacency_list[51].push_back(neighbor(50, 7));
        adjacency_list[50].push_back(neighbor(49, 9));
        adjacency_list[49].push_back(neighbor(48, 9));
        adjacency_list[48].push_back(neighbor(47, 7));
        adjacency_list[47].push_back(neighbor(34, 7));
        //adjacency_list[65].push_back(neighbor(64, 9));
        //adjacency_list[64].push_back(neighbor(36, 9));
        adjacency_list[62].push_back(neighbor(61, 7));
        adjacency_list[61].push_back(neighbor(60, 7));
        adjacency_list[60].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(58, 9));
        adjacency_list[58].push_back(neighbor(35, 7));

        adjacency_list[17].push_back(neighbor(16, 7));


        adjacency_list[16].push_back(neighbor(15, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[14].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(12, 7));
        adjacency_list[12].push_back(neighbor(11, 7));

        adjacency_list[11].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(9, 9));
        adjacency_list[9].push_back(neighbor(8, 9));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[7].push_back(neighbor(6, 7));


        adjacency_list[6].push_back(neighbor(5, 9));
        adjacency_list[5].push_back(neighbor(4, 9));
        adjacency_list[4].push_back(neighbor(3, 9));
        adjacency_list[3].push_back(neighbor(2, 7));
        adjacency_list[2].push_back(neighbor(1, 7));


        //adjacency_list[1].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(ui->originBox3->currentIndex(), adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox3->currentIndex() << " to " << ui->destBox3->currentIndex() <<  ": " << min_distance[4] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->destBox3->currentIndex(), previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
    }
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
void MainWindow::greyOut4()
{
    if (ui->setButton4->isChecked()== true)
    {
        ui->trainselectBox4->setDisabled(true);
        ui->originBox4->setDisabled(true);
        ui->destBox4->setDisabled(true);
        ui->throttleBox4->setDisabled(true);
        ui->facingBox4->setDisabled(true);
        ui->headingBox4->setDisabled(true);
        adjacency_list_t adjacency_list(69);
        adjacency_list[1].push_back(neighbor(2, 7));
        adjacency_list[1].push_back(neighbor(36, 7));
        // 1 = b
        adjacency_list[2].push_back(neighbor(3, 7));

        // 2 = c
        adjacency_list[3].push_back(neighbor(4, 9));

        // 3 = d
        adjacency_list[4].push_back(neighbor(5, 15));

        // 4 = e
        adjacency_list[5].push_back(neighbor(6, 6));

        // 5 = f

        adjacency_list[6].push_back(neighbor(7, 9));

        adjacency_list[7].push_back(neighbor(8, 9));
        adjacency_list[7].push_back(neighbor(37, 9));

        adjacency_list[37].push_back(neighbor(38, 1));
        adjacency_list[38].push_back(neighbor(39, 9));
        adjacency_list[39].push_back(neighbor(40, 9));
        adjacency_list[40].push_back(neighbor(41, 9));

        adjacency_list[37].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(45, 9));

        adjacency_list[45].push_back(neighbor(46, 1));
        adjacency_list[46].push_back(neighbor(45, 9));
        adjacency_list[45].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(67, 9));
        adjacency_list[67].push_back(neighbor(68, 9));
        adjacency_list[68].push_back(neighbor(6, 9));



        adjacency_list[8].push_back(neighbor(9, 9));

        adjacency_list[9].push_back(neighbor(10, 9));

        adjacency_list[10].push_back(neighbor(11, 1));
        adjacency_list[11].push_back(neighbor(12, 9));
        adjacency_list[12].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(14, 9));

        adjacency_list[14].push_back(neighbor(15, 1));
        adjacency_list[15].push_back(neighbor(16, 9));
        adjacency_list[16].push_back(neighbor(17, 9));
        adjacency_list[17].push_back(neighbor(18, 9));

        adjacency_list[18].push_back(neighbor(19, 1));
        adjacency_list[19].push_back(neighbor(20, 9));
        adjacency_list[20].push_back(neighbor(21, 9));
        adjacency_list[21].push_back(neighbor(22, 9));

        adjacency_list[22].push_back(neighbor(23, 1));
        adjacency_list[23].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(25, 9));
        adjacency_list[25].push_back(neighbor(26, 9));

        adjacency_list[26].push_back(neighbor(27, 1));
        adjacency_list[27].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(30, 9));

        adjacency_list[30].push_back(neighbor(31, 1));
        adjacency_list[31].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(34, 9));

        adjacency_list[34].push_back(neighbor(35, 1));
        //adjacency_list[35].push_back(neighbor(34, 9));
        adjacency_list[36].push_back(neighbor(1, 9));
        adjacency_list[36].push_back(neighbor(64, 7));

        adjacency_list[35].push_back(neighbor(36, 9));
        adjacency_list[58].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(60, 9));
        adjacency_list[60].push_back(neighbor(61, 9));
        adjacency_list[61].push_back(neighbor(62, 7));
        adjacency_list[62].push_back(neighbor(66, 7));

        adjacency_list[66].push_back(neighbor(54, 9));
        adjacency_list[54].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(63, 7));
        adjacency_list[63].push_back(neighbor(19, 9));

        adjacency_list[64].push_back(neighbor(65, 1));
        adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[65].push_back(neighbor(47, 9));
        adjacency_list[47].push_back(neighbor(48, 7));
        adjacency_list[48].push_back(neighbor(49, 7));
        adjacency_list[49].push_back(neighbor(50, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(66, 7));


        //adjacency_list[34].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));


        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(27, 9));
        adjacency_list[27].push_back(neighbor(26, 7));
        adjacency_list[26].push_back(neighbor(25, 7));


        adjacency_list[25].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(23, 9));
        adjacency_list[23].push_back(neighbor(22, 9));
        adjacency_list[22].push_back(neighbor(21, 7));
        adjacency_list[21].push_back(neighbor(20, 7));


        adjacency_list[20].push_back(neighbor(19, 9));
        adjacency_list[19].push_back(neighbor(63, 9));
        adjacency_list[63].push_back(neighbor(17, 9));
        adjacency_list[63].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(54, 7));
        adjacency_list[54].push_back(neighbor(53, 7));
        adjacency_list[53].push_back(neighbor(52, 9));
        adjacency_list[53].push_back(neighbor(62, 9));
        adjacency_list[52].push_back(neighbor(51, 7));
        adjacency_list[51].push_back(neighbor(50, 7));
        adjacency_list[50].push_back(neighbor(49, 9));
        adjacency_list[49].push_back(neighbor(48, 9));
        adjacency_list[48].push_back(neighbor(47, 7));
        adjacency_list[47].push_back(neighbor(34, 7));
        //adjacency_list[65].push_back(neighbor(64, 9));
        //adjacency_list[64].push_back(neighbor(36, 9));
        adjacency_list[62].push_back(neighbor(61, 7));
        adjacency_list[61].push_back(neighbor(60, 7));
        adjacency_list[60].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(58, 9));
        adjacency_list[58].push_back(neighbor(35, 7));

        adjacency_list[17].push_back(neighbor(16, 7));


        adjacency_list[16].push_back(neighbor(15, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[14].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(12, 7));
        adjacency_list[12].push_back(neighbor(11, 7));

        adjacency_list[11].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(9, 9));
        adjacency_list[9].push_back(neighbor(8, 9));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[7].push_back(neighbor(6, 7));


        adjacency_list[6].push_back(neighbor(5, 9));
        adjacency_list[5].push_back(neighbor(4, 9));
        adjacency_list[4].push_back(neighbor(3, 9));
        adjacency_list[3].push_back(neighbor(2, 7));
        adjacency_list[2].push_back(neighbor(1, 7));


        //adjacency_list[1].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(ui->originBox4->currentIndex(), adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox4->currentIndex() << " to " << ui->destBox4->currentIndex() <<  ": " << min_distance[4] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->destBox4->currentIndex(), previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
    }
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
void MainWindow::greyOut5()
{
    if (ui->setButton5->isChecked()== true)
    {
        ui->trainselectBox5->setDisabled(true);
        ui->originBox5->setDisabled(true);
        ui->destBox5->setDisabled(true);
        ui->throttleBox5->setDisabled(true);
        ui->facingBox5->setDisabled(true);
        ui->headingBox5->setDisabled(true);
        adjacency_list_t adjacency_list(69);
        adjacency_list[1].push_back(neighbor(2, 7));
        adjacency_list[1].push_back(neighbor(36, 7));
        // 1 = b
        adjacency_list[2].push_back(neighbor(3, 7));

        // 2 = c
        adjacency_list[3].push_back(neighbor(4, 9));

        // 3 = d
        adjacency_list[4].push_back(neighbor(5, 15));

        // 4 = e
        adjacency_list[5].push_back(neighbor(6, 6));

        // 5 = f

        adjacency_list[6].push_back(neighbor(7, 9));

        adjacency_list[7].push_back(neighbor(8, 9));
        adjacency_list[7].push_back(neighbor(37, 9));

        adjacency_list[37].push_back(neighbor(38, 1));
        adjacency_list[38].push_back(neighbor(39, 9));
        adjacency_list[39].push_back(neighbor(40, 9));
        adjacency_list[40].push_back(neighbor(41, 9));

        adjacency_list[37].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(45, 9));

        adjacency_list[45].push_back(neighbor(46, 1));
        adjacency_list[46].push_back(neighbor(45, 9));
        adjacency_list[45].push_back(neighbor(44, 9));
        adjacency_list[44].push_back(neighbor(43, 9));
        adjacency_list[43].push_back(neighbor(42, 1));
        adjacency_list[42].push_back(neighbor(67, 9));
        adjacency_list[67].push_back(neighbor(68, 9));
        adjacency_list[68].push_back(neighbor(6, 9));



        adjacency_list[8].push_back(neighbor(9, 9));

        adjacency_list[9].push_back(neighbor(10, 9));

        adjacency_list[10].push_back(neighbor(11, 1));
        adjacency_list[11].push_back(neighbor(12, 9));
        adjacency_list[12].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(14, 9));

        adjacency_list[14].push_back(neighbor(15, 1));
        adjacency_list[15].push_back(neighbor(16, 9));
        adjacency_list[16].push_back(neighbor(17, 9));
        adjacency_list[17].push_back(neighbor(18, 9));

        adjacency_list[18].push_back(neighbor(19, 1));
        adjacency_list[19].push_back(neighbor(20, 9));
        adjacency_list[20].push_back(neighbor(21, 9));
        adjacency_list[21].push_back(neighbor(22, 9));

        adjacency_list[22].push_back(neighbor(23, 1));
        adjacency_list[23].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(25, 9));
        adjacency_list[25].push_back(neighbor(26, 9));

        adjacency_list[26].push_back(neighbor(27, 1));
        adjacency_list[27].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(30, 9));

        adjacency_list[30].push_back(neighbor(31, 1));
        adjacency_list[31].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(34, 9));

        adjacency_list[34].push_back(neighbor(35, 1));
        //adjacency_list[35].push_back(neighbor(34, 9));
        adjacency_list[36].push_back(neighbor(1, 9));
        adjacency_list[36].push_back(neighbor(64, 7));

        adjacency_list[35].push_back(neighbor(36, 9));
        adjacency_list[58].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(60, 9));
        adjacency_list[60].push_back(neighbor(61, 9));
        adjacency_list[61].push_back(neighbor(62, 7));
        adjacency_list[62].push_back(neighbor(66, 7));

        adjacency_list[66].push_back(neighbor(54, 9));
        adjacency_list[54].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(63, 7));
        adjacency_list[63].push_back(neighbor(19, 9));

        adjacency_list[64].push_back(neighbor(65, 1));
        adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[65].push_back(neighbor(47, 9));
        adjacency_list[47].push_back(neighbor(48, 7));
        adjacency_list[48].push_back(neighbor(49, 7));
        adjacency_list[49].push_back(neighbor(50, 9));
        adjacency_list[51].push_back(neighbor(52, 9));
        adjacency_list[52].push_back(neighbor(66, 7));


        //adjacency_list[34].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));


        adjacency_list[30].push_back(neighbor(29, 9));
        adjacency_list[29].push_back(neighbor(28, 9));
        adjacency_list[28].push_back(neighbor(27, 9));
        adjacency_list[27].push_back(neighbor(26, 7));
        adjacency_list[26].push_back(neighbor(25, 7));


        adjacency_list[25].push_back(neighbor(24, 9));
        adjacency_list[24].push_back(neighbor(23, 9));
        adjacency_list[23].push_back(neighbor(22, 9));
        adjacency_list[22].push_back(neighbor(21, 7));
        adjacency_list[21].push_back(neighbor(20, 7));


        adjacency_list[20].push_back(neighbor(19, 9));
        adjacency_list[19].push_back(neighbor(63, 9));
        adjacency_list[63].push_back(neighbor(17, 9));
        adjacency_list[63].push_back(neighbor(57, 7));
        adjacency_list[57].push_back(neighbor(56, 9));
        adjacency_list[56].push_back(neighbor(55, 9));
        adjacency_list[55].push_back(neighbor(54, 7));
        adjacency_list[54].push_back(neighbor(53, 7));
        adjacency_list[53].push_back(neighbor(52, 9));
        adjacency_list[53].push_back(neighbor(62, 9));
        adjacency_list[52].push_back(neighbor(51, 7));
        adjacency_list[51].push_back(neighbor(50, 7));
        adjacency_list[50].push_back(neighbor(49, 9));
        adjacency_list[49].push_back(neighbor(48, 9));
        adjacency_list[48].push_back(neighbor(47, 7));
        adjacency_list[47].push_back(neighbor(34, 7));
        //adjacency_list[65].push_back(neighbor(64, 9));
        //adjacency_list[64].push_back(neighbor(36, 9));
        adjacency_list[62].push_back(neighbor(61, 7));
        adjacency_list[61].push_back(neighbor(60, 7));
        adjacency_list[60].push_back(neighbor(59, 9));
        adjacency_list[59].push_back(neighbor(58, 9));
        adjacency_list[58].push_back(neighbor(35, 7));

        adjacency_list[17].push_back(neighbor(16, 7));


        adjacency_list[16].push_back(neighbor(15, 9));
        adjacency_list[15].push_back(neighbor(14, 9));
        adjacency_list[14].push_back(neighbor(13, 9));
        adjacency_list[13].push_back(neighbor(12, 7));
        adjacency_list[12].push_back(neighbor(11, 7));

        adjacency_list[11].push_back(neighbor(10, 9));
        adjacency_list[10].push_back(neighbor(9, 9));
        adjacency_list[9].push_back(neighbor(8, 9));
        adjacency_list[8].push_back(neighbor(7, 7));
        adjacency_list[7].push_back(neighbor(6, 7));


        adjacency_list[6].push_back(neighbor(5, 9));
        adjacency_list[5].push_back(neighbor(4, 9));
        adjacency_list[4].push_back(neighbor(3, 9));
        adjacency_list[3].push_back(neighbor(2, 7));
        adjacency_list[2].push_back(neighbor(1, 7));


        //adjacency_list[1].push_back(neighbor(47, 9));
        //adjacency_list[65].push_back(neighbor(33, 9));
        //adjacency_list[33].push_back(neighbor(32, 9));
        adjacency_list[32].push_back(neighbor(31, 7));
        adjacency_list[31].push_back(neighbor(30, 7));
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(ui->originBox5->currentIndex(), adjacency_list, min_distance, previous);
        //DijkstraComputePaths(ui->startlineEdit1->text().toInt(), adjacency_list, min_distance, previous);
        std::cout << "Distance from " << ui->originBox5->currentIndex() << " to " << ui->destBox5->currentIndex() <<  ": " << min_distance[4] << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->destBox5->currentIndex(), previous);
        //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
        std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
        std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
    }
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

