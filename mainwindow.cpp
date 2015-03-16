#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QLCDNumber>
#include <QTime>
#include <QPixmap>
#include <iostream>
#include <QVector>
#include <string>
#include <QList>
#include <set>
#include <QQueue>
#include <QTimer>
#include <QTextBrowser>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <Q_DebugStream.h>
#include <QTimeEdit>
#include <QVariant>
#include <sstream>
#include <Dijkstra.h>
#include <save_loadIni.h>
#include <QTableView>
#include <switchFunctions.h>
#include <tableOperations.h>
#include <sql_tables.h>
#include <QMessageBox>
#include <QFileDialog>
#include <timedelay.h>
#include <fstream>
#include <Qsound>




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

    /*
    mapIcon = new QIcon("MasterTracknumbered1.jpg");
    //ui->graphicsView->setScene(scene);
    QGraphicsView* w = new QGraphicsView();
    QGraphicsScene *scn = new QGraphicsScene( w );
     scn->setSceneRect( w->rect() );
     w->setScene( scn );
     w->scale(.1, .1);
     //w->setAlignment(Qt::AlignCenter);
     //w->setDragMode(QGraphicsView::ScrollHandDrag);
     w->viewport();
     //w->addScrollBarWidget(w, Qt::AlignCenter);
     //w->setFixedSize( 400, 400 );
    QPixmap pix( "MasterTracknumbered1.jpg" );
    scn->addPixmap(pix);
    w->show();
*/


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
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop1()));

    connect(ui->delaypushButton, SIGNAL(clicked()), this, SLOT(timeDelay1()));
    //connect(ui->timeEdit1, SIGNAL(timeChanged(QTime)), this, SLOT(timedDelay()));

    //once schedule created, select radio button to lock it in
    //connect(ui->setButton1, SIGNAL(clicked(bool)), this, SLOT(greyOut1()));
    //connect(ui->setButton2, SIGNAL(clicked(bool)), this, SLOT(greyOut2()));
    //connect(ui->setButton3, SIGNAL(clicked(bool)), this, SLOT(greyOut3()));
    //connect(ui->setButton4, SIGNAL(clicked(bool)), this, SLOT(greyOut4()));
    //connect(ui->setButton5, SIGNAL(clicked(bool)), this, SLOT(greyOut5()));

    //feature to include randomly selected node to include in origin box to start train from
    //i.e. dynamically added due to node being occuppied when train placed on it
    connect(ui->occButton, SIGNAL(clicked()), this, SLOT(occupiedNode()));
    //randomly selected destination for testing route algorithm
    connect(ui->destButton, SIGNAL(clicked()), this, SLOT(destNode()));

    connect(ui->queryButton, SIGNAL(clicked()), this, SLOT(viewTable()));

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

    //SQL Database query drop down menu selection
    traininfo = new QAction("Train Info table", this);
    throttle = new QAction("Throttle table", this);
    pathinfo = new QAction("Path Info table", this);
    switches = new QAction("Switches table", this);
    connect(traininfo, SIGNAL(triggered()), this, SLOT(traininfoTable()));
    connect(throttle, SIGNAL(triggered()), this, SLOT(throttleTable()));
    connect(pathinfo, SIGNAL(triggered()), this, SLOT(pathinfoTable()));
    connect(switches, SIGNAL(triggered()), this, SLOT(switchesTable()));

    menu = menuBar()->addMenu("Query");
    menu->addAction(traininfo);
    menu->addAction(throttle);
    menu->addAction(pathinfo);
    menu->addAction(switches);

    techsupport = new QAction("Technical Support", this);
    connect(techsupport, SIGNAL(triggered()), this, SLOT(techsupportDoc()));
    menu = menuBar()->addMenu("Help");
    menu->addAction(techsupport);

}

MainWindow::~MainWindow()
{
    ldb.close();
    delete ui;
}

void MainWindow::viewTable()
{
    QString vt = "traininfo";
    //vt = ui->sqlQueryEdit->text();

    QSqlTableModel* model = new QSqlTableModel(0, ldb);
    model->setTable(vt);
    model->select();
    QTableView* view = new QTableView;
    view = new QTableView;
    view->setModel(model);
    view->show();


    if ( model->lastError().isValid() )
    {
        statusBar()->showMessage(tr(" table not opened successfully"));
    }
    else
    {
        statusBar()->showMessage(tr("table opened successfully"));
    }
}


//Function to block unavailable destinations
void MainWindow::blockDest()
{
    if (ui->originBox1->currentIndex() == 1 || ui->originBox1->currentIndex() == 2)
    {

    }
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

void MainWindow::stop1()
{
    ui->startButton->setDisabled(false);

    ui->setButton1->setDisabled(false);
    if (ui->setButton1->isChecked() == true)
    {
        ui->setButton1->click();
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

    ui->setButton2->setDisabled(false);
    if (ui->setButton2->isChecked() == true)
    {
        ui->setButton2->click();
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

    ui->setButton3->setDisabled(false);
    if (ui->setButton3->isChecked() == true)
    {
        ui->setButton3->click();
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

    ui->setButton4->setDisabled(false);
    if (ui->setButton4->isChecked() == true)
    {
        ui->setButton4->click();
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

    ui->setButton5->setDisabled(false);
    if (ui->setButton5->isChecked() == true)
    {
        ui->setButton5->click();
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

//function to start periodic route calculations
void MainWindow::calculateRoute()
{

    ui->startButton->setDisabled(true); //disabled start button to avoid throwing timer off

    if(ui->setButton1->isChecked() == true)
        greyOut1();
    else if(ui->delayButton1->isChecked() == true)
        timeDelay1();
    if(ui->setButton2->isChecked() == true)
        greyOut2();
    else if(ui->delayButton2->isChecked() == true)
        timeDelay2();
    if(ui->setButton3->isChecked() == true)
        greyOut3();
    else if(ui->delayButton3->isChecked() == true)
        timeDelay3();
    if(ui->setButton4->isChecked() == true)
        greyOut4();
    else if(ui->delayButton4->isChecked() == true)
        timeDelay4();
    if(ui->setButton5->isChecked() == true)
        greyOut5();
    else if(ui->delayButton5->isChecked() == true)
        timeDelay5();
    //connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut1()));
    //if(ui->setButton2->isChecked() == true)
    //connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut2()));
    //if(ui->setButton3->isChecked() == true)
    //connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut3()));
    //if(ui->setButton4->isChecked() == true)
    //connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut4()));
    //if(ui->setButton5->isChecked() == true)
    //connect(recalculateTimer, SIGNAL(timeout()), this, SLOT(greyOut5()));

    //recalculateTimer->start(5000);


}
//function to display real world time according to current pc
void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcdNumber->display(text);
}

void MainWindow::setgreyOut1()
{
    ui->setButton1->click();
    greyOut1();
    QSound::play("american_train.wav");
}

void MainWindow::setgreyOut2()
{
    ui->setButton2->click();
    greyOut2();
    QSound::play("american_train_whistle_blow.wav");
}

void MainWindow::setgreyOut3()
{
    ui->setButton3->click();
    greyOut3();
    QSound::play("amtrak_passenger_train_whistle_blow_exterior_long.wav");
}

void MainWindow::setgreyOut4()
{
    ui->setButton4->click();
    greyOut4();
    QSound::play("vintage_steam_train_double_whistle.wav");
}

void MainWindow::setgreyOut5()
{
    ui->setButton5->click();
    greyOut5();
    QSound::play("vintage_steam_train_single_whistle.wav");
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
        ui->setButton1->setDisabled(true);
        ui->trainimageButton1->show();
        ui->routeInfo1->show();


        if((ui->originBox1->currentIndex() == 1 ||
            ui->originBox1->currentIndex() == 2) &&
           (ui->destBox1->currentIndex() == 3 ||
            ui->destBox1->currentIndex() == 4 ||
            ui->destBox1->currentIndex() == 5 ||
            ui->destBox1->currentIndex() == 6 ||
            ui->destBox1->currentIndex() == 7))
        {
        //adjacency list from 1 & 2 to everywhere else

            adjacency_list_t adjacency_listeast(100);

            adjacency_listeast[0].push_back(neighbor(39, 3));
            adjacency_listeast[1].push_back(neighbor(0, 2));
            adjacency_listeast[2].push_back(neighbor(4, 9));
            adjacency_listeast[3].push_back(neighbor(1, 1));
            adjacency_listeast[4].push_back(neighbor(72, 9));
            adjacency_listeast[5].push_back(neighbor(3, 9));
            adjacency_listeast[7].push_back(neighbor(5, 8));
            adjacency_listeast[8].push_back(neighbor(7, 7));
            adjacency_listeast[9].push_back(neighbor(10, 9));
            adjacency_listeast[10].push_back(neighbor(11, 9));
            adjacency_listeast[11].push_back(neighbor(13, 9));
            adjacency_listeast[14].push_back(neighbor(8, 6));
            adjacency_listeast[15].push_back(neighbor(14, 5));
            adjacency_listeast[26].push_back(neighbor(51, 4));
            adjacency_listeast[28].push_back(neighbor(26, 3));
            adjacency_listeast[29].push_back(neighbor(28, 2));
            adjacency_listeast[30].push_back(neighbor(29, 1));
            adjacency_listeast[31].push_back(neighbor(70, 9));
            adjacency_listeast[31].push_back(neighbor(30, 8));
            adjacency_listeast[32].push_back(neighbor(31, 7));
            adjacency_listeast[33].push_back(neighbor(32, 6));
            adjacency_listeast[35].push_back(neighbor(33, 5));
            adjacency_listeast[36].push_back(neighbor(35, 4));
            adjacency_listeast[36].push_back(neighbor(68, 9));
            adjacency_listeast[38].push_back(neighbor(36, 5));
            adjacency_listeast[39].push_back(neighbor(38, 4));
            adjacency_listeast[40].push_back(neighbor(15, 4));
            adjacency_listeast[41].push_back(neighbor(40, 3));
            adjacency_listeast[42].push_back(neighbor(41, 2));
            adjacency_listeast[43].push_back(neighbor(42, 1));
            adjacency_listeast[44].push_back(neighbor(42, 9));
            adjacency_listeast[51].push_back(neighbor(52, 3));
            adjacency_listeast[52].push_back(neighbor(53, 4));
            adjacency_listeast[52].push_back(neighbor(54, 9));
            adjacency_listeast[51].push_back(neighbor(55, 9));
            adjacency_listeast[68].push_back(neighbor(69, 9));
            adjacency_listeast[70].push_back(neighbor(71, 9));
            adjacency_listeast[71].push_back(neighbor(2, 9));
            adjacency_listeast[72].push_back(neighbor(73, 9));
            adjacency_listeast[73].push_back(neighbor(9, 9));


        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_listeast, min_distance, previous);
        std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
        tableOperation(path);

        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }

        //adjacency list selected based on heading
        else if ((ui->originBox1->currentIndex() == 3 ||
                  ui->originBox1->currentIndex() == 4 ||
                  ui->originBox1->currentIndex() == 5) &&
                 (ui->destBox1->currentIndex() == 1 ||
                  ui->destBox1->currentIndex() == 2))
        {
            //adjacency list 3, 4 & 5 to 1 & 2
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
            std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }
        else if((ui->originBox1->currentIndex() == 5 ||
                ui->originBox1->currentIndex() == 4 ||
                ui->originBox1->currentIndex() == 3) &&
                (ui->destBox1->currentIndex() == 6 ||
                ui->destBox1->currentIndex() == 7))
        {
            //adjacency list 3, 4 & 5 to 6 & 7
            adjacency_list_t adjacency_lists(150);

            adjacency_lists[53].push_back(neighbor(52, 2));
            adjacency_lists[54].push_back(neighbor(52, 2));
            adjacency_lists[55].push_back(neighbor(51, 2));
            adjacency_lists[52].push_back(neighbor(51, 2));
            adjacency_lists[51].push_back(neighbor(26, 2));
            adjacency_lists[26].push_back(neighbor(28, 2));
            adjacency_lists[28].push_back(neighbor(29, 2));
            adjacency_lists[29].push_back(neighbor(30, 2));
            adjacency_lists[30].push_back(neighbor(31, 2));
            adjacency_lists[31].push_back(neighbor(32, 2));
            adjacency_lists[32].push_back(neighbor(34, 2));
            adjacency_lists[34].push_back(neighbor(37, 2));
            adjacency_lists[37].push_back(neighbor(39, 2));
            adjacency_lists[39].push_back(neighbor(0, 2));
            adjacency_lists[0].push_back(neighbor(1, 2));
            adjacency_lists[1].push_back(neighbor(3, 2));
            adjacency_lists[3].push_back(neighbor(72, 2));
            adjacency_lists[72].push_back(neighbor(73, 2));
            adjacency_lists[73].push_back(neighbor(9, 2));
            adjacency_lists[9].push_back(neighbor(10, 2));
            adjacency_lists[10].push_back(neighbor(11, 2));
            adjacency_lists[11].push_back(neighbor(13, 2));
            adjacency_lists[10].push_back(neighbor(12, 2));
            adjacency_lists[12].push_back(neighbor(16, 2));
            adjacency_lists[16].push_back(neighbor(17, 2));
            adjacency_lists[17].push_back(neighbor(19, 2));
            adjacency_lists[19].push_back(neighbor(6, 2));
            adjacency_lists[6].push_back(neighbor(4, 2));
            adjacency_lists[4].push_back(neighbor(2, 2));
            adjacency_lists[2].push_back(neighbor(75, 2));
            adjacency_lists[75].push_back(neighbor(74, 2));
            adjacency_lists[74].push_back(neighbor(38, 2));
            adjacency_lists[38].push_back(neighbor(36, 2));
            adjacency_lists[36].push_back(neighbor(68, 2));
            adjacency_lists[68].push_back(neighbor(69, 2));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_lists, min_distance, previous);
            std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox1->currentIndex() == 6)
        {
            //adjacency list 6 to everywhere
            adjacency_list_t adjacency_list(150);

                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(73, 2));
                adjacency_list[73].push_back(neighbor(72, 2));
                adjacency_list[72].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[35].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(55, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(54, 2));
                adjacency_list[52].push_back(neighbor(53, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(44, 2));
                adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox1->currentIndex() == 7)
        {
            //adjacency list 7 to everywhere
            adjacency_list_t adjacency_list(150);

            adjacency_list[69].push_back(neighbor(68, 2));
            adjacency_list[68].push_back(neighbor(36, 2));
            adjacency_list[36].push_back(neighbor(38, 2));
            adjacency_list[38].push_back(neighbor(74, 2));
            adjacency_list[74].push_back(neighbor(75, 2));
            adjacency_list[75].push_back(neighbor(1, 2));
            adjacency_list[1].push_back(neighbor(3, 2));
            adjacency_list[3].push_back(neighbor(72, 2));
            adjacency_list[72].push_back(neighbor(73, 2));
            adjacency_list[73].push_back(neighbor(9, 2));
            adjacency_list[9].push_back(neighbor(10, 2));
            adjacency_list[10].push_back(neighbor(11, 2));
            adjacency_list[10].push_back(neighbor(12, 2));
            adjacency_list[11].push_back(neighbor(13, 2));
            adjacency_list[12].push_back(neighbor(16, 2));
            adjacency_list[16].push_back(neighbor(17, 2));
            adjacency_list[17].push_back(neighbor(19, 2));
            adjacency_list[19].push_back(neighbor(6, 2));
            adjacency_list[6].push_back(neighbor(4, 2));
            adjacency_list[4].push_back(neighbor(2, 2));
            adjacency_list[2].push_back(neighbor(0, 2));
            adjacency_list[0].push_back(neighbor(39, 2));
            adjacency_list[39].push_back(neighbor(37, 2));
            adjacency_list[37].push_back(neighbor(34, 2));
            adjacency_list[34].push_back(neighbor(32, 2));
            adjacency_list[32].push_back(neighbor(31, 2));
            adjacency_list[31].push_back(neighbor(30, 2));
            adjacency_list[30].push_back(neighbor(29, 2));
            adjacency_list[29].push_back(neighbor(28, 2));
            adjacency_list[28].push_back(neighbor(26, 2));
            adjacency_list[26].push_back(neighbor(51, 2));
            adjacency_list[51].push_back(neighbor(55, 2));
            adjacency_list[51].push_back(neighbor(52, 2));
            adjacency_list[52].push_back(neighbor(54, 2));
            adjacency_list[52].push_back(neighbor(53, 2));
            adjacency_list[3].push_back(neighbor(5, 2));
            adjacency_list[5].push_back(neighbor(7, 2));
            adjacency_list[7].push_back(neighbor(8, 2));
            adjacency_list[8].push_back(neighbor(14, 2));
            adjacency_list[14].push_back(neighbor(15, 2));
            adjacency_list[15].push_back(neighbor(40, 2));
            adjacency_list[40].push_back(neighbor(41, 2));
            adjacency_list[41].push_back(neighbor(42, 2));
            adjacency_list[42].push_back(neighbor(44, 2));
            adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
            tableOperation(path);
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
        ui->setButton2->setDisabled(true);
        ui->trainimageButton2->show();
        ui->routeInfo2->show();

        if((ui->originBox2->currentIndex() == 1 ||
            ui->originBox2->currentIndex() == 2) &&
           (ui->destBox2->currentIndex() == 3 ||
            ui->destBox2->currentIndex() == 4 ||
            ui->destBox2->currentIndex() == 5 ||
            ui->destBox2->currentIndex() == 6 ||
            ui->destBox2->currentIndex() == 7))
        {
        //adjacency list from 1 & 2 to everywhere else

            adjacency_list_t adjacency_listeast(100);

            adjacency_listeast[0].push_back(neighbor(39, 3));
            adjacency_listeast[1].push_back(neighbor(0, 2));
            adjacency_listeast[2].push_back(neighbor(4, 9));
            adjacency_listeast[3].push_back(neighbor(1, 1));
            adjacency_listeast[4].push_back(neighbor(72, 9));
            adjacency_listeast[5].push_back(neighbor(3, 9));
            adjacency_listeast[7].push_back(neighbor(5, 8));
            adjacency_listeast[8].push_back(neighbor(7, 7));
            adjacency_listeast[9].push_back(neighbor(10, 9));
            adjacency_listeast[10].push_back(neighbor(11, 9));
            adjacency_listeast[11].push_back(neighbor(13, 9));
            adjacency_listeast[14].push_back(neighbor(8, 6));
            adjacency_listeast[15].push_back(neighbor(14, 5));
            adjacency_listeast[26].push_back(neighbor(51, 4));
            adjacency_listeast[28].push_back(neighbor(26, 3));
            adjacency_listeast[29].push_back(neighbor(28, 2));
            adjacency_listeast[30].push_back(neighbor(29, 1));
            adjacency_listeast[31].push_back(neighbor(70, 9));
            adjacency_listeast[31].push_back(neighbor(30, 8));
            adjacency_listeast[32].push_back(neighbor(31, 7));
            adjacency_listeast[33].push_back(neighbor(32, 6));
            adjacency_listeast[35].push_back(neighbor(33, 5));
            adjacency_listeast[36].push_back(neighbor(35, 4));
            adjacency_listeast[36].push_back(neighbor(68, 9));
            adjacency_listeast[38].push_back(neighbor(36, 5));
            adjacency_listeast[39].push_back(neighbor(38, 4));
            adjacency_listeast[40].push_back(neighbor(15, 4));
            adjacency_listeast[41].push_back(neighbor(40, 3));
            adjacency_listeast[42].push_back(neighbor(41, 2));
            adjacency_listeast[43].push_back(neighbor(42, 1));
            adjacency_listeast[44].push_back(neighbor(42, 9));
            adjacency_listeast[51].push_back(neighbor(52, 3));
            adjacency_listeast[52].push_back(neighbor(53, 4));
            adjacency_listeast[52].push_back(neighbor(54, 9));
            adjacency_listeast[51].push_back(neighbor(55, 9));
            adjacency_listeast[68].push_back(neighbor(69, 9));
            adjacency_listeast[70].push_back(neighbor(71, 9));
            adjacency_listeast[71].push_back(neighbor(2, 9));
            adjacency_listeast[72].push_back(neighbor(73, 9));
            adjacency_listeast[73].push_back(neighbor(9, 9));


        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_listeast, min_distance, previous);
        std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
        tableOperation(path);

        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }

        //adjacency list selected based on heading
        else if ((ui->originBox2->currentIndex() == 3 ||
                  ui->originBox2->currentIndex() == 4 ||
                  ui->originBox2->currentIndex() == 5) &&
                 (ui->destBox2->currentIndex() == 1 ||
                  ui->destBox2->currentIndex() == 2))
        {
            //adjacency list 3, 4 & 5 to 1 & 2
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
            std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }
        else if((ui->originBox2->currentIndex() == 5 ||
                ui->originBox2->currentIndex() == 4 ||
                ui->originBox2->currentIndex() == 3) &&
                (ui->destBox2->currentIndex() == 6 ||
                ui->destBox2->currentIndex() == 7))
        {
            //adjacency list 3, 4 & 5 to 6 & 7
            adjacency_list_t adjacency_lists(150);

            adjacency_lists[53].push_back(neighbor(52, 2));
            adjacency_lists[54].push_back(neighbor(52, 2));
            adjacency_lists[55].push_back(neighbor(51, 2));
            adjacency_lists[52].push_back(neighbor(51, 2));
            adjacency_lists[51].push_back(neighbor(26, 2));
            adjacency_lists[26].push_back(neighbor(28, 2));
            adjacency_lists[28].push_back(neighbor(29, 2));
            adjacency_lists[29].push_back(neighbor(30, 2));
            adjacency_lists[30].push_back(neighbor(31, 2));
            adjacency_lists[31].push_back(neighbor(32, 2));
            adjacency_lists[32].push_back(neighbor(34, 2));
            adjacency_lists[34].push_back(neighbor(37, 2));
            adjacency_lists[37].push_back(neighbor(39, 2));
            adjacency_lists[39].push_back(neighbor(0, 2));
            adjacency_lists[0].push_back(neighbor(1, 2));
            adjacency_lists[1].push_back(neighbor(3, 2));
            adjacency_lists[3].push_back(neighbor(72, 2));
            adjacency_lists[72].push_back(neighbor(73, 2));
            adjacency_lists[73].push_back(neighbor(9, 2));
            adjacency_lists[9].push_back(neighbor(10, 2));
            adjacency_lists[10].push_back(neighbor(11, 2));
            adjacency_lists[11].push_back(neighbor(13, 2));
            adjacency_lists[10].push_back(neighbor(12, 2));
            adjacency_lists[12].push_back(neighbor(16, 2));
            adjacency_lists[16].push_back(neighbor(17, 2));
            adjacency_lists[17].push_back(neighbor(19, 2));
            adjacency_lists[19].push_back(neighbor(6, 2));
            adjacency_lists[6].push_back(neighbor(4, 2));
            adjacency_lists[4].push_back(neighbor(2, 2));
            adjacency_lists[2].push_back(neighbor(75, 2));
            adjacency_lists[75].push_back(neighbor(74, 2));
            adjacency_lists[74].push_back(neighbor(38, 2));
            adjacency_lists[38].push_back(neighbor(36, 2));
            adjacency_lists[36].push_back(neighbor(68, 2));
            adjacency_lists[68].push_back(neighbor(69, 2));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_lists, min_distance, previous);
            std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox2->currentIndex() == 6)
        {
            //adjacency list 6 to everywhere
            adjacency_list_t adjacency_list(150);

                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(73, 2));
                adjacency_list[73].push_back(neighbor(72, 2));
                adjacency_list[72].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[35].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(55, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(54, 2));
                adjacency_list[52].push_back(neighbor(53, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(44, 2));
                adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox2->currentIndex() == 7)
        {
            //adjacency list 7 to everywhere
            adjacency_list_t adjacency_list(150);

            adjacency_list[69].push_back(neighbor(68, 2));
            adjacency_list[68].push_back(neighbor(36, 2));
            adjacency_list[36].push_back(neighbor(38, 2));
            adjacency_list[38].push_back(neighbor(74, 2));
            adjacency_list[74].push_back(neighbor(75, 2));
            adjacency_list[75].push_back(neighbor(1, 2));
            adjacency_list[1].push_back(neighbor(3, 2));
            adjacency_list[3].push_back(neighbor(72, 2));
            adjacency_list[72].push_back(neighbor(73, 2));
            adjacency_list[73].push_back(neighbor(9, 2));
            adjacency_list[9].push_back(neighbor(10, 2));
            adjacency_list[10].push_back(neighbor(11, 2));
            adjacency_list[10].push_back(neighbor(12, 2));
            adjacency_list[11].push_back(neighbor(13, 2));
            adjacency_list[12].push_back(neighbor(16, 2));
            adjacency_list[16].push_back(neighbor(17, 2));
            adjacency_list[17].push_back(neighbor(19, 2));
            adjacency_list[19].push_back(neighbor(6, 2));
            adjacency_list[6].push_back(neighbor(4, 2));
            adjacency_list[4].push_back(neighbor(2, 2));
            adjacency_list[2].push_back(neighbor(0, 2));
            adjacency_list[0].push_back(neighbor(39, 2));
            adjacency_list[39].push_back(neighbor(37, 2));
            adjacency_list[37].push_back(neighbor(34, 2));
            adjacency_list[34].push_back(neighbor(32, 2));
            adjacency_list[32].push_back(neighbor(31, 2));
            adjacency_list[31].push_back(neighbor(30, 2));
            adjacency_list[30].push_back(neighbor(29, 2));
            adjacency_list[29].push_back(neighbor(28, 2));
            adjacency_list[28].push_back(neighbor(26, 2));
            adjacency_list[26].push_back(neighbor(51, 2));
            adjacency_list[51].push_back(neighbor(55, 2));
            adjacency_list[51].push_back(neighbor(52, 2));
            adjacency_list[52].push_back(neighbor(54, 2));
            adjacency_list[52].push_back(neighbor(53, 2));
            adjacency_list[3].push_back(neighbor(5, 2));
            adjacency_list[5].push_back(neighbor(7, 2));
            adjacency_list[7].push_back(neighbor(8, 2));
            adjacency_list[8].push_back(neighbor(14, 2));
            adjacency_list[14].push_back(neighbor(15, 2));
            adjacency_list[15].push_back(neighbor(40, 2));
            adjacency_list[40].push_back(neighbor(41, 2));
            adjacency_list[41].push_back(neighbor(42, 2));
            adjacency_list[42].push_back(neighbor(44, 2));
            adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
            tableOperation(path);
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
        ui->setButton3->setDisabled(true);
        ui->trainimageButton3->show();
        ui->routeInfo3->show();

        if((ui->originBox3->currentIndex() == 1 ||
            ui->originBox3->currentIndex() == 2) &&
           (ui->destBox3->currentIndex() == 3 ||
            ui->destBox3->currentIndex() == 4 ||
            ui->destBox3->currentIndex() == 5 ||
            ui->destBox3->currentIndex() == 6 ||
            ui->destBox3->currentIndex() == 7))
        {
        //adjacency list from 1 & 2 to everywhere else

            adjacency_list_t adjacency_listeast(100);

            adjacency_listeast[0].push_back(neighbor(39, 3));
            adjacency_listeast[1].push_back(neighbor(0, 2));
            adjacency_listeast[2].push_back(neighbor(4, 9));
            adjacency_listeast[3].push_back(neighbor(1, 1));
            adjacency_listeast[4].push_back(neighbor(72, 9));
            adjacency_listeast[5].push_back(neighbor(3, 9));
            adjacency_listeast[7].push_back(neighbor(5, 8));
            adjacency_listeast[8].push_back(neighbor(7, 7));
            adjacency_listeast[9].push_back(neighbor(10, 9));
            adjacency_listeast[10].push_back(neighbor(11, 9));
            adjacency_listeast[11].push_back(neighbor(13, 9));
            adjacency_listeast[14].push_back(neighbor(8, 6));
            adjacency_listeast[15].push_back(neighbor(14, 5));
            adjacency_listeast[26].push_back(neighbor(51, 4));
            adjacency_listeast[28].push_back(neighbor(26, 3));
            adjacency_listeast[29].push_back(neighbor(28, 2));
            adjacency_listeast[30].push_back(neighbor(29, 1));
            adjacency_listeast[31].push_back(neighbor(70, 9));
            adjacency_listeast[31].push_back(neighbor(30, 8));
            adjacency_listeast[32].push_back(neighbor(31, 7));
            adjacency_listeast[33].push_back(neighbor(32, 6));
            adjacency_listeast[35].push_back(neighbor(33, 5));
            adjacency_listeast[36].push_back(neighbor(35, 4));
            adjacency_listeast[36].push_back(neighbor(68, 9));
            adjacency_listeast[38].push_back(neighbor(36, 5));
            adjacency_listeast[39].push_back(neighbor(38, 4));
            adjacency_listeast[40].push_back(neighbor(15, 4));
            adjacency_listeast[41].push_back(neighbor(40, 3));
            adjacency_listeast[42].push_back(neighbor(41, 2));
            adjacency_listeast[43].push_back(neighbor(42, 1));
            adjacency_listeast[44].push_back(neighbor(42, 9));
            adjacency_listeast[51].push_back(neighbor(52, 3));
            adjacency_listeast[52].push_back(neighbor(53, 4));
            adjacency_listeast[52].push_back(neighbor(54, 9));
            adjacency_listeast[51].push_back(neighbor(55, 9));
            adjacency_listeast[68].push_back(neighbor(69, 9));
            adjacency_listeast[70].push_back(neighbor(71, 9));
            adjacency_listeast[71].push_back(neighbor(2, 9));
            adjacency_listeast[72].push_back(neighbor(73, 9));
            adjacency_listeast[73].push_back(neighbor(9, 9));


        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_listeast, min_distance, previous);
        std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
        tableOperation(path);

        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }

        //adjacency list selected based on heading
        else if ((ui->originBox3->currentIndex() == 3 ||
                  ui->originBox3->currentIndex() == 4 ||
                  ui->originBox3->currentIndex() == 5) &&
                 (ui->destBox3->currentIndex() == 1 ||
                  ui->destBox3->currentIndex() == 2))
        {
            //adjacency list 3, 4 & 5 to 1 & 2
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
            std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }
        else if((ui->originBox3->currentIndex() == 5 ||
                ui->originBox3->currentIndex() == 4 ||
                ui->originBox3->currentIndex() == 3) &&
                (ui->destBox3->currentIndex() == 6 ||
                ui->destBox3->currentIndex() == 7))
        {
            //adjacency list 3, 4 & 5 to 6 & 7
            adjacency_list_t adjacency_lists(150);

            adjacency_lists[53].push_back(neighbor(52, 2));
            adjacency_lists[54].push_back(neighbor(52, 2));
            adjacency_lists[55].push_back(neighbor(51, 2));
            adjacency_lists[52].push_back(neighbor(51, 2));
            adjacency_lists[51].push_back(neighbor(26, 2));
            adjacency_lists[26].push_back(neighbor(28, 2));
            adjacency_lists[28].push_back(neighbor(29, 2));
            adjacency_lists[29].push_back(neighbor(30, 2));
            adjacency_lists[30].push_back(neighbor(31, 2));
            adjacency_lists[31].push_back(neighbor(32, 2));
            adjacency_lists[32].push_back(neighbor(34, 2));
            adjacency_lists[34].push_back(neighbor(37, 2));
            adjacency_lists[37].push_back(neighbor(39, 2));
            adjacency_lists[39].push_back(neighbor(0, 2));
            adjacency_lists[0].push_back(neighbor(1, 2));
            adjacency_lists[1].push_back(neighbor(3, 2));
            adjacency_lists[3].push_back(neighbor(72, 2));
            adjacency_lists[72].push_back(neighbor(73, 2));
            adjacency_lists[73].push_back(neighbor(9, 2));
            adjacency_lists[9].push_back(neighbor(10, 2));
            adjacency_lists[10].push_back(neighbor(11, 2));
            adjacency_lists[11].push_back(neighbor(13, 2));
            adjacency_lists[10].push_back(neighbor(12, 2));
            adjacency_lists[12].push_back(neighbor(16, 2));
            adjacency_lists[16].push_back(neighbor(17, 2));
            adjacency_lists[17].push_back(neighbor(19, 2));
            adjacency_lists[19].push_back(neighbor(6, 2));
            adjacency_lists[6].push_back(neighbor(4, 2));
            adjacency_lists[4].push_back(neighbor(2, 2));
            adjacency_lists[2].push_back(neighbor(75, 2));
            adjacency_lists[75].push_back(neighbor(74, 2));
            adjacency_lists[74].push_back(neighbor(38, 2));
            adjacency_lists[38].push_back(neighbor(36, 2));
            adjacency_lists[36].push_back(neighbor(68, 2));
            adjacency_lists[68].push_back(neighbor(69, 2));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_lists, min_distance, previous);
            std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox3->currentIndex() == 6)
        {
            //adjacency list 6 to everywhere
            adjacency_list_t adjacency_list(150);

                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(73, 2));
                adjacency_list[73].push_back(neighbor(72, 2));
                adjacency_list[72].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[35].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(55, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(54, 2));
                adjacency_list[52].push_back(neighbor(53, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(44, 2));
                adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox3->currentIndex() == 7)
        {
            //adjacency list 7 to everywhere
            adjacency_list_t adjacency_list(150);

            adjacency_list[69].push_back(neighbor(68, 2));
            adjacency_list[68].push_back(neighbor(36, 2));
            adjacency_list[36].push_back(neighbor(38, 2));
            adjacency_list[38].push_back(neighbor(74, 2));
            adjacency_list[74].push_back(neighbor(75, 2));
            adjacency_list[75].push_back(neighbor(1, 2));
            adjacency_list[1].push_back(neighbor(3, 2));
            adjacency_list[3].push_back(neighbor(72, 2));
            adjacency_list[72].push_back(neighbor(73, 2));
            adjacency_list[73].push_back(neighbor(9, 2));
            adjacency_list[9].push_back(neighbor(10, 2));
            adjacency_list[10].push_back(neighbor(11, 2));
            adjacency_list[10].push_back(neighbor(12, 2));
            adjacency_list[11].push_back(neighbor(13, 2));
            adjacency_list[12].push_back(neighbor(16, 2));
            adjacency_list[16].push_back(neighbor(17, 2));
            adjacency_list[17].push_back(neighbor(19, 2));
            adjacency_list[19].push_back(neighbor(6, 2));
            adjacency_list[6].push_back(neighbor(4, 2));
            adjacency_list[4].push_back(neighbor(2, 2));
            adjacency_list[2].push_back(neighbor(0, 2));
            adjacency_list[0].push_back(neighbor(39, 2));
            adjacency_list[39].push_back(neighbor(37, 2));
            adjacency_list[37].push_back(neighbor(34, 2));
            adjacency_list[34].push_back(neighbor(32, 2));
            adjacency_list[32].push_back(neighbor(31, 2));
            adjacency_list[31].push_back(neighbor(30, 2));
            adjacency_list[30].push_back(neighbor(29, 2));
            adjacency_list[29].push_back(neighbor(28, 2));
            adjacency_list[28].push_back(neighbor(26, 2));
            adjacency_list[26].push_back(neighbor(51, 2));
            adjacency_list[51].push_back(neighbor(55, 2));
            adjacency_list[51].push_back(neighbor(52, 2));
            adjacency_list[52].push_back(neighbor(54, 2));
            adjacency_list[52].push_back(neighbor(53, 2));
            adjacency_list[3].push_back(neighbor(5, 2));
            adjacency_list[5].push_back(neighbor(7, 2));
            adjacency_list[7].push_back(neighbor(8, 2));
            adjacency_list[8].push_back(neighbor(14, 2));
            adjacency_list[14].push_back(neighbor(15, 2));
            adjacency_list[15].push_back(neighbor(40, 2));
            adjacency_list[40].push_back(neighbor(41, 2));
            adjacency_list[41].push_back(neighbor(42, 2));
            adjacency_list[42].push_back(neighbor(44, 2));
            adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
            tableOperation(path);
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
        ui->setButton4->setDisabled(true);
        ui->trainimageButton4->show();
        ui->routeInfo4->show();

        if((ui->originBox4->currentIndex() == 1 ||
            ui->originBox4->currentIndex() == 2) &&
           (ui->destBox4->currentIndex() == 3 ||
            ui->destBox4->currentIndex() == 4 ||
            ui->destBox4->currentIndex() == 5 ||
            ui->destBox4->currentIndex() == 6 ||
            ui->destBox4->currentIndex() == 7))
        {
        //adjacency list from 1 & 2 to everywhere else

            adjacency_list_t adjacency_listeast(100);

            adjacency_listeast[0].push_back(neighbor(39, 3));
            adjacency_listeast[1].push_back(neighbor(0, 2));
            adjacency_listeast[2].push_back(neighbor(4, 9));
            adjacency_listeast[3].push_back(neighbor(1, 1));
            adjacency_listeast[4].push_back(neighbor(72, 9));
            adjacency_listeast[5].push_back(neighbor(3, 9));
            adjacency_listeast[7].push_back(neighbor(5, 8));
            adjacency_listeast[8].push_back(neighbor(7, 7));
            adjacency_listeast[9].push_back(neighbor(10, 9));
            adjacency_listeast[10].push_back(neighbor(11, 9));
            adjacency_listeast[11].push_back(neighbor(13, 9));
            adjacency_listeast[14].push_back(neighbor(8, 6));
            adjacency_listeast[15].push_back(neighbor(14, 5));
            adjacency_listeast[26].push_back(neighbor(51, 4));
            adjacency_listeast[28].push_back(neighbor(26, 3));
            adjacency_listeast[29].push_back(neighbor(28, 2));
            adjacency_listeast[30].push_back(neighbor(29, 1));
            adjacency_listeast[31].push_back(neighbor(70, 9));
            adjacency_listeast[31].push_back(neighbor(30, 8));
            adjacency_listeast[32].push_back(neighbor(31, 7));
            adjacency_listeast[33].push_back(neighbor(32, 6));
            adjacency_listeast[35].push_back(neighbor(33, 5));
            adjacency_listeast[36].push_back(neighbor(35, 4));
            adjacency_listeast[36].push_back(neighbor(68, 9));
            adjacency_listeast[38].push_back(neighbor(36, 5));
            adjacency_listeast[39].push_back(neighbor(38, 4));
            adjacency_listeast[40].push_back(neighbor(15, 4));
            adjacency_listeast[41].push_back(neighbor(40, 3));
            adjacency_listeast[42].push_back(neighbor(41, 2));
            adjacency_listeast[43].push_back(neighbor(42, 1));
            adjacency_listeast[44].push_back(neighbor(42, 9));
            adjacency_listeast[51].push_back(neighbor(52, 3));
            adjacency_listeast[52].push_back(neighbor(53, 4));
            adjacency_listeast[52].push_back(neighbor(54, 9));
            adjacency_listeast[51].push_back(neighbor(55, 9));
            adjacency_listeast[68].push_back(neighbor(69, 9));
            adjacency_listeast[70].push_back(neighbor(71, 9));
            adjacency_listeast[71].push_back(neighbor(2, 9));
            adjacency_listeast[72].push_back(neighbor(73, 9));
            adjacency_listeast[73].push_back(neighbor(9, 9));


        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_listeast, min_distance, previous);
        std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
        tableOperation(path);

        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }

        //adjacency list selected based on heading
        else if ((ui->originBox4->currentIndex() == 3 ||
                  ui->originBox4->currentIndex() == 4 ||
                  ui->originBox4->currentIndex() == 5) &&
                 (ui->destBox4->currentIndex() == 1 ||
                  ui->destBox4->currentIndex() == 2))
        {
            //adjacency list 3, 4 & 5 to 1 & 2
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
            std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }
        else if((ui->originBox4->currentIndex() == 5 ||
                ui->originBox4->currentIndex() == 4 ||
                ui->originBox4->currentIndex() == 3) &&
                (ui->destBox4->currentIndex() == 6 ||
                ui->destBox4->currentIndex() == 7))
        {
            //adjacency list 3, 4 & 5 to 6 & 7
            adjacency_list_t adjacency_lists(150);

            adjacency_lists[53].push_back(neighbor(52, 2));
            adjacency_lists[54].push_back(neighbor(52, 2));
            adjacency_lists[55].push_back(neighbor(51, 2));
            adjacency_lists[52].push_back(neighbor(51, 2));
            adjacency_lists[51].push_back(neighbor(26, 2));
            adjacency_lists[26].push_back(neighbor(28, 2));
            adjacency_lists[28].push_back(neighbor(29, 2));
            adjacency_lists[29].push_back(neighbor(30, 2));
            adjacency_lists[30].push_back(neighbor(31, 2));
            adjacency_lists[31].push_back(neighbor(32, 2));
            adjacency_lists[32].push_back(neighbor(34, 2));
            adjacency_lists[34].push_back(neighbor(37, 2));
            adjacency_lists[37].push_back(neighbor(39, 2));
            adjacency_lists[39].push_back(neighbor(0, 2));
            adjacency_lists[0].push_back(neighbor(1, 2));
            adjacency_lists[1].push_back(neighbor(3, 2));
            adjacency_lists[3].push_back(neighbor(72, 2));
            adjacency_lists[72].push_back(neighbor(73, 2));
            adjacency_lists[73].push_back(neighbor(9, 2));
            adjacency_lists[9].push_back(neighbor(10, 2));
            adjacency_lists[10].push_back(neighbor(11, 2));
            adjacency_lists[11].push_back(neighbor(13, 2));
            adjacency_lists[10].push_back(neighbor(12, 2));
            adjacency_lists[12].push_back(neighbor(16, 2));
            adjacency_lists[16].push_back(neighbor(17, 2));
            adjacency_lists[17].push_back(neighbor(19, 2));
            adjacency_lists[19].push_back(neighbor(6, 2));
            adjacency_lists[6].push_back(neighbor(4, 2));
            adjacency_lists[4].push_back(neighbor(2, 2));
            adjacency_lists[2].push_back(neighbor(75, 2));
            adjacency_lists[75].push_back(neighbor(74, 2));
            adjacency_lists[74].push_back(neighbor(38, 2));
            adjacency_lists[38].push_back(neighbor(36, 2));
            adjacency_lists[36].push_back(neighbor(68, 2));
            adjacency_lists[68].push_back(neighbor(69, 2));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_lists, min_distance, previous);
            std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox4->currentIndex() == 6)
        {
            //adjacency list 6 to everywhere
            adjacency_list_t adjacency_list(150);

                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(73, 2));
                adjacency_list[73].push_back(neighbor(72, 2));
                adjacency_list[72].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[35].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(55, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(54, 2));
                adjacency_list[52].push_back(neighbor(53, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(44, 2));
                adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox4->currentIndex() == 7)
        {
            //adjacency list 7 to everywhere
            adjacency_list_t adjacency_list(150);

            adjacency_list[69].push_back(neighbor(68, 2));
            adjacency_list[68].push_back(neighbor(36, 2));
            adjacency_list[36].push_back(neighbor(38, 2));
            adjacency_list[38].push_back(neighbor(74, 2));
            adjacency_list[74].push_back(neighbor(75, 2));
            adjacency_list[75].push_back(neighbor(1, 2));
            adjacency_list[1].push_back(neighbor(3, 2));
            adjacency_list[3].push_back(neighbor(72, 2));
            adjacency_list[72].push_back(neighbor(73, 2));
            adjacency_list[73].push_back(neighbor(9, 2));
            adjacency_list[9].push_back(neighbor(10, 2));
            adjacency_list[10].push_back(neighbor(11, 2));
            adjacency_list[10].push_back(neighbor(12, 2));
            adjacency_list[11].push_back(neighbor(13, 2));
            adjacency_list[12].push_back(neighbor(16, 2));
            adjacency_list[16].push_back(neighbor(17, 2));
            adjacency_list[17].push_back(neighbor(19, 2));
            adjacency_list[19].push_back(neighbor(6, 2));
            adjacency_list[6].push_back(neighbor(4, 2));
            adjacency_list[4].push_back(neighbor(2, 2));
            adjacency_list[2].push_back(neighbor(0, 2));
            adjacency_list[0].push_back(neighbor(39, 2));
            adjacency_list[39].push_back(neighbor(37, 2));
            adjacency_list[37].push_back(neighbor(34, 2));
            adjacency_list[34].push_back(neighbor(32, 2));
            adjacency_list[32].push_back(neighbor(31, 2));
            adjacency_list[31].push_back(neighbor(30, 2));
            adjacency_list[30].push_back(neighbor(29, 2));
            adjacency_list[29].push_back(neighbor(28, 2));
            adjacency_list[28].push_back(neighbor(26, 2));
            adjacency_list[26].push_back(neighbor(51, 2));
            adjacency_list[51].push_back(neighbor(55, 2));
            adjacency_list[51].push_back(neighbor(52, 2));
            adjacency_list[52].push_back(neighbor(54, 2));
            adjacency_list[52].push_back(neighbor(53, 2));
            adjacency_list[3].push_back(neighbor(5, 2));
            adjacency_list[5].push_back(neighbor(7, 2));
            adjacency_list[7].push_back(neighbor(8, 2));
            adjacency_list[8].push_back(neighbor(14, 2));
            adjacency_list[14].push_back(neighbor(15, 2));
            adjacency_list[15].push_back(neighbor(40, 2));
            adjacency_list[40].push_back(neighbor(41, 2));
            adjacency_list[41].push_back(neighbor(42, 2));
            adjacency_list[42].push_back(neighbor(44, 2));
            adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
            tableOperation(path);
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
        ui->setButton5->setDisabled(true);
        ui->trainimageButton5->show();
        ui->routeInfo5->show();

        if((ui->originBox5->currentIndex() == 1 ||
            ui->originBox5->currentIndex() == 2) &&
           (ui->destBox5->currentIndex() == 3 ||
            ui->destBox5->currentIndex() == 4 ||
            ui->destBox5->currentIndex() == 5 ||
            ui->destBox5->currentIndex() == 6 ||
            ui->destBox5->currentIndex() == 7))
        {
        //adjacency list from 1 & 2 to everywhere else

            adjacency_list_t adjacency_listeast(100);

            adjacency_listeast[0].push_back(neighbor(39, 3));
            adjacency_listeast[1].push_back(neighbor(0, 2));
            adjacency_listeast[2].push_back(neighbor(4, 9));
            adjacency_listeast[3].push_back(neighbor(1, 1));
            adjacency_listeast[4].push_back(neighbor(72, 9));
            adjacency_listeast[5].push_back(neighbor(3, 9));
            adjacency_listeast[7].push_back(neighbor(5, 8));
            adjacency_listeast[8].push_back(neighbor(7, 7));
            adjacency_listeast[9].push_back(neighbor(10, 9));
            adjacency_listeast[10].push_back(neighbor(11, 9));
            adjacency_listeast[11].push_back(neighbor(13, 9));
            adjacency_listeast[14].push_back(neighbor(8, 6));
            adjacency_listeast[15].push_back(neighbor(14, 5));
            adjacency_listeast[26].push_back(neighbor(51, 4));
            adjacency_listeast[28].push_back(neighbor(26, 3));
            adjacency_listeast[29].push_back(neighbor(28, 2));
            adjacency_listeast[30].push_back(neighbor(29, 1));
            adjacency_listeast[31].push_back(neighbor(70, 9));
            adjacency_listeast[31].push_back(neighbor(30, 8));
            adjacency_listeast[32].push_back(neighbor(31, 7));
            adjacency_listeast[33].push_back(neighbor(32, 6));
            adjacency_listeast[35].push_back(neighbor(33, 5));
            adjacency_listeast[36].push_back(neighbor(35, 4));
            adjacency_listeast[36].push_back(neighbor(68, 9));
            adjacency_listeast[38].push_back(neighbor(36, 5));
            adjacency_listeast[39].push_back(neighbor(38, 4));
            adjacency_listeast[40].push_back(neighbor(15, 4));
            adjacency_listeast[41].push_back(neighbor(40, 3));
            adjacency_listeast[42].push_back(neighbor(41, 2));
            adjacency_listeast[43].push_back(neighbor(42, 1));
            adjacency_listeast[44].push_back(neighbor(42, 9));
            adjacency_listeast[51].push_back(neighbor(52, 3));
            adjacency_listeast[52].push_back(neighbor(53, 4));
            adjacency_listeast[52].push_back(neighbor(54, 9));
            adjacency_listeast[51].push_back(neighbor(55, 9));
            adjacency_listeast[68].push_back(neighbor(69, 9));
            adjacency_listeast[70].push_back(neighbor(71, 9));
            adjacency_listeast[71].push_back(neighbor(2, 9));
            adjacency_listeast[72].push_back(neighbor(73, 9));
            adjacency_listeast[73].push_back(neighbor(9, 9));


        //calculate and output information
        std::vector<weight_t> min_distance;
        std::vector<vertex_t> previous;
        DijkstraComputePaths(start, adjacency_listeast, min_distance, previous);
        std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
        tableOperation(path);

        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
        std::cout << std::endl;
        }

        //adjacency list selected based on heading
        else if ((ui->originBox5->currentIndex() == 3 ||
                  ui->originBox5->currentIndex() == 4 ||
                  ui->originBox5->currentIndex() == 5) &&
                 (ui->destBox5->currentIndex() == 1 ||
                  ui->destBox5->currentIndex() == 2))
        {
            //adjacency list 3, 4 & 5 to 1 & 2
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
            std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }
        else if((ui->originBox5->currentIndex() == 5 ||
                ui->originBox5->currentIndex() == 4 ||
                ui->originBox5->currentIndex() == 3) &&
                (ui->destBox5->currentIndex() == 6 ||
                ui->destBox5->currentIndex() == 7))
        {
            //adjacency list 3, 4 & 5 to 6 & 7
            adjacency_list_t adjacency_lists(150);

            adjacency_lists[53].push_back(neighbor(52, 2));
            adjacency_lists[54].push_back(neighbor(52, 2));
            adjacency_lists[55].push_back(neighbor(51, 2));
            adjacency_lists[52].push_back(neighbor(51, 2));
            adjacency_lists[51].push_back(neighbor(26, 2));
            adjacency_lists[26].push_back(neighbor(28, 2));
            adjacency_lists[28].push_back(neighbor(29, 2));
            adjacency_lists[29].push_back(neighbor(30, 2));
            adjacency_lists[30].push_back(neighbor(31, 2));
            adjacency_lists[31].push_back(neighbor(32, 2));
            adjacency_lists[32].push_back(neighbor(34, 2));
            adjacency_lists[34].push_back(neighbor(37, 2));
            adjacency_lists[37].push_back(neighbor(39, 2));
            adjacency_lists[39].push_back(neighbor(0, 2));
            adjacency_lists[0].push_back(neighbor(1, 2));
            adjacency_lists[1].push_back(neighbor(3, 2));
            adjacency_lists[3].push_back(neighbor(72, 2));
            adjacency_lists[72].push_back(neighbor(73, 2));
            adjacency_lists[73].push_back(neighbor(9, 2));
            adjacency_lists[9].push_back(neighbor(10, 2));
            adjacency_lists[10].push_back(neighbor(11, 2));
            adjacency_lists[11].push_back(neighbor(13, 2));
            adjacency_lists[10].push_back(neighbor(12, 2));
            adjacency_lists[12].push_back(neighbor(16, 2));
            adjacency_lists[16].push_back(neighbor(17, 2));
            adjacency_lists[17].push_back(neighbor(19, 2));
            adjacency_lists[19].push_back(neighbor(6, 2));
            adjacency_lists[6].push_back(neighbor(4, 2));
            adjacency_lists[4].push_back(neighbor(2, 2));
            adjacency_lists[2].push_back(neighbor(75, 2));
            adjacency_lists[75].push_back(neighbor(74, 2));
            adjacency_lists[74].push_back(neighbor(38, 2));
            adjacency_lists[38].push_back(neighbor(36, 2));
            adjacency_lists[36].push_back(neighbor(68, 2));
            adjacency_lists[68].push_back(neighbor(69, 2));

            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_lists, min_distance, previous);
            std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox5->currentIndex() == 6)
        {
            //adjacency list 6 to everywhere
            adjacency_list_t adjacency_list(150);

                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(73, 2));
                adjacency_list[73].push_back(neighbor(72, 2));
                adjacency_list[72].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[35].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(55, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(54, 2));
                adjacency_list[52].push_back(neighbor(53, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(44, 2));
                adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
            tableOperation(path);
        }

        else if(ui->originBox5->currentIndex() == 7)
        {
            //adjacency list 7 to everywhere
            adjacency_list_t adjacency_list(150);

            adjacency_list[69].push_back(neighbor(68, 2));
            adjacency_list[68].push_back(neighbor(36, 2));
            adjacency_list[36].push_back(neighbor(38, 2));
            adjacency_list[38].push_back(neighbor(74, 2));
            adjacency_list[74].push_back(neighbor(75, 2));
            adjacency_list[75].push_back(neighbor(1, 2));
            adjacency_list[1].push_back(neighbor(3, 2));
            adjacency_list[3].push_back(neighbor(72, 2));
            adjacency_list[72].push_back(neighbor(73, 2));
            adjacency_list[73].push_back(neighbor(9, 2));
            adjacency_list[9].push_back(neighbor(10, 2));
            adjacency_list[10].push_back(neighbor(11, 2));
            adjacency_list[10].push_back(neighbor(12, 2));
            adjacency_list[11].push_back(neighbor(13, 2));
            adjacency_list[12].push_back(neighbor(16, 2));
            adjacency_list[16].push_back(neighbor(17, 2));
            adjacency_list[17].push_back(neighbor(19, 2));
            adjacency_list[19].push_back(neighbor(6, 2));
            adjacency_list[6].push_back(neighbor(4, 2));
            adjacency_list[4].push_back(neighbor(2, 2));
            adjacency_list[2].push_back(neighbor(0, 2));
            adjacency_list[0].push_back(neighbor(39, 2));
            adjacency_list[39].push_back(neighbor(37, 2));
            adjacency_list[37].push_back(neighbor(34, 2));
            adjacency_list[34].push_back(neighbor(32, 2));
            adjacency_list[32].push_back(neighbor(31, 2));
            adjacency_list[31].push_back(neighbor(30, 2));
            adjacency_list[30].push_back(neighbor(29, 2));
            adjacency_list[29].push_back(neighbor(28, 2));
            adjacency_list[28].push_back(neighbor(26, 2));
            adjacency_list[26].push_back(neighbor(51, 2));
            adjacency_list[51].push_back(neighbor(55, 2));
            adjacency_list[51].push_back(neighbor(52, 2));
            adjacency_list[52].push_back(neighbor(54, 2));
            adjacency_list[52].push_back(neighbor(53, 2));
            adjacency_list[3].push_back(neighbor(5, 2));
            adjacency_list[5].push_back(neighbor(7, 2));
            adjacency_list[7].push_back(neighbor(8, 2));
            adjacency_list[8].push_back(neighbor(14, 2));
            adjacency_list[14].push_back(neighbor(15, 2));
            adjacency_list[15].push_back(neighbor(40, 2));
            adjacency_list[40].push_back(neighbor(41, 2));
            adjacency_list[41].push_back(neighbor(42, 2));
            adjacency_list[42].push_back(neighbor(44, 2));
            adjacency_list[42].push_back(neighbor(43, 2));


            //calculate and output information
            std::vector<weight_t> min_distance;
            std::vector<vertex_t> previous;
            DijkstraComputePaths(start, adjacency_list, min_distance, previous);
            std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
            std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
            //std::list<vertex_t> path = DijkstraGetShortestPathTo(ui->stoplineEdit2->text().toInt(), previous);
            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
            tableOperation(path);
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

