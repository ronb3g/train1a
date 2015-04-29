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






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    
    //initialize all variables that will be used here.

    controlBit = 0;
    ready = 1;
    Eng1 = NULL;
        Eng2 = NULL;
        Eng3 = NULL;
        Eng4 = NULL;
        Eng5 = NULL;
        E1S = 0;
        E2S = 0;
        E3S = 0;
        E4S = 0;
        E5S = 0;
        Eng1Speed = 1; //Engine 1 travels 1 unit in 1 second.
        Eng2Speed = 2; //Engine 2 travels 1 unit in 2 seconds.
        Eng3Speed = 3; //Engine 3 travels 1 unit in 3 seconds.
        Eng4Speed = 2; //Engine 4 travels 1 unit in 2 seconds.
        Eng5Speed = 1; //Engine 5 travels 1 unit in 1 second.
        i1 = 0; //Itterator for Comparison
        i2 = 0; //Itterator for Comparison
        i3 = 0; //Itterator for Comparison
        i4 = 0; //Itterator for Comparison
        i5 = 0; //Itterator for Comparison
        //comparisonArray = NULL;
        //comparisonArray2 = NULL;
       // comparisonArray3 = NULL;
        //comparisonArray4 = NULL;
        //comparisonArray5 = NULL;

        m_timer.start(1000, this);
        fakeoccupancy = 0;
        occupyitt = 0;

    ldb = QSqlDatabase::addDatabase("QSQLITE", "ldb"); //local database initialization
    ldb.setDatabaseName( "train.db" );
    if(!ldb.open())
       { qDebug() << ldb.lastError();}

    ui->setupUi(this);
    
    initializeSwitchtable();

    //Start and Start button graphics
    startIcon = new QIcon(":/startbutton/startbutton.png");
    stopIcon = new QIcon(":/startbutton/stop.jpg");
    ui->startButton->setIcon(*startIcon);
    ui->startButton->setIconSize(QSize(65,65));
    ui->stopButton->setIcon(*stopIcon);
    ui->stopButton->setIconSize(QSize(35,35));



    //create train image icons
    engine1Icon = new QIcon(":/startbutton/engine1.jpg");
    engine2Icon = new QIcon(":/startbutton/engine2.jpg");
    engine3Icon = new QIcon(":/startbutton/engine3.jpg");
    engine4Icon = new QIcon(":/startbutton/engine4.jpg");
    engine5Icon = new QIcon(":/startbutton/engine5.jpg");

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

   
    //feature to include randomly selected node to include in origin box to start train from
    //i.e. dynamically added due to node being occuppied when train placed on it
    connect(ui->occButton, SIGNAL(clicked()), this, SLOT(occupiedNode()));
    //randomly selected destination for testing route algorithm
    connect(ui->destButton, SIGNAL(clicked()), this, SLOT(destNode()));

    connect(ui->queryButton, SIGNAL(clicked()), this, SLOT(createDBtables()));

    //Clock feature added to enable delayed train departures
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();

    //feature to block certain destinations based on origin
    //i.e. if origin Endpoint 1 selected need to block destination box from selecting Endpoint 1
    connect(ui->originBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(blockDest()));

    //feature to save and load configurations, as well as toggle the control bit
    toggleControl = new QAction("Toggle Control", this);
    savefile = new QAction("Save config", this);
    loadfile = new QAction("Load config", this);
    Occtest = new QAction("Occupancy Test", this);
    connect(loadfile, SIGNAL(triggered()), this, SLOT(loadText()));
    ui->menuCPE_453_Team_1A->addAction(loadfile);
    ui->menuCPE_453_Team_1A->addAction(savefile);
    ui->menuCPE_453_Team_1A->addAction(toggleControl);
    connect(savefile, SIGNAL(triggered()), this, SLOT(saveText()));
    connect(toggleControl, SIGNAL(triggered()), this, SLOT(toggleBit()));

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
    clearAll = new QAction("Clear checkboxes", this);
    connect(clearAll, SIGNAL(triggered()), this, SLOT(clearChecks()));
    menu->addAction(clearAll);
    connect (Occtest, SIGNAL(triggered()), this, SLOT(occupy()));
            menu->addAction(Occtest);

    ui->ModeEdit->setText("Scheduler Control");
    ui->stopButton->setDisabled(true);


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
    ui->addorlineEdit3->clear();
}

//function to add random destination to destination boxes
void MainWindow::destNode()
{
    ui->destBox1->addItem((ui->adddestlineEdit4->text()));
    ui->destBox2->addItem((ui->adddestlineEdit4->text()));
    ui->destBox3->addItem((ui->adddestlineEdit4->text()));
    ui->destBox4->addItem((ui->adddestlineEdit4->text()));
    ui->destBox5->addItem((ui->adddestlineEdit4->text()));
    ui->adddestlineEdit4->clear();
}

//function to route calculations
void MainWindow::stopTimer()
{
    //recalculateTimer->stop();
}

void MainWindow::stop1()
{

   // m_timer.stop();

    occupyitt = 0;
    comparisonArray.clear();
    comparisonArray2.clear();
    comparisonArray3.clear();
    comparisonArray4.clear();
    comparisonArray5.clear();
    pathcomplete1 = 0;
    pathcomplete2 = 0;
    pathcomplete3 = 0;
    pathcomplete4 = 0;
    pathcomplete5 = 0;
//    if (Eng1 != NULL)
//    delete Eng1;
//    if (Eng2 != NULL)
//    delete Eng2;
//    if (Eng3 != NULL)
//    delete Eng3;
//    if (Eng4 != NULL)
//    delete Eng4;
//    if (Eng5 != NULL)
//    delete Eng5;
    ui->routeInfo1->clear();
    ui->routeInfo2->clear();
    ui->routeInfo3->clear();
    ui->routeInfo4->clear();
    ui->routeInfo5->clear();

    ui->setButton1->setDisabled(false);
   // if (ui->setButton1->isChecked() == true)
    {
//        if (Eng1 != NULL)
//        {
//        delete Eng1;
//        }
        //ui->setButton1->click();
        ui->trainselectBox1->setDisabled(false);
        ui->originBox1->setDisabled(false);
        ui->destBox1->setDisabled(false);
        ui->throttleBox1->setDisabled(false);
        ui->facingBox1->setDisabled(false);
        //ui->headingBox1->setDisabled(false);
        ui->trainimageButton1->hide();
        ui->routeInfo1->clear();
        ui->routeInfo1->hide();
    }

    ui->setButton2->setDisabled(false);
    //if (ui->setButton2->isChecked() == true)
    {
//        if (Eng2 != NULL)
//        {
//        delete Eng2;
//        }
       // ui->setButton2->click();
        ui->trainselectBox2->setDisabled(false);
        ui->originBox2->setDisabled(false);
        ui->destBox2->setDisabled(false);
        ui->throttleBox2->setDisabled(false);
        ui->facingBox2->setDisabled(false);
        //ui->headingBox2->setDisabled(false);
        ui->trainimageButton2->hide();
        ui->routeInfo2->clear();
        ui->routeInfo2->hide();
    }

    ui->setButton3->setDisabled(false);
    //if (ui->setButton3->isChecked() == true)
    {
//        if (Eng3 != NULL)
//        {
//        delete Eng3;
//        }
       // ui->setButton3->click();
        ui->trainselectBox3->setDisabled(false);
        ui->originBox3->setDisabled(false);
        ui->destBox3->setDisabled(false);
        ui->throttleBox3->setDisabled(false);
        ui->facingBox3->setDisabled(false);
        //ui->headingBox3->setDisabled(false);
        ui->trainimageButton3->hide();
        ui->routeInfo3->clear();
        ui->routeInfo3->hide();
    }

    ui->setButton4->setDisabled(false);
    //if (ui->setButton4->isChecked() == true)
    {
//        if (Eng4 != NULL)
//        {
//        delete Eng4;
//        }
      //  ui->setButton4->click();
        ui->trainselectBox4->setDisabled(false);
        ui->originBox4->setDisabled(false);
        ui->destBox4->setDisabled(false);
        ui->throttleBox4->setDisabled(false);
        ui->facingBox4->setDisabled(false);
        //ui->headingBox4->setDisabled(false);
        ui->trainimageButton4->hide();
        ui->routeInfo4->clear();
        ui->routeInfo4->hide();
    }

    ui->setButton5->setDisabled(false);
    //if (ui->setButton5->isChecked() == true)
    {
//        if (Eng5 != NULL)
//        {
//        delete Eng5;
//        }
        //ui->setButton5->click();
        ui->trainselectBox5->setDisabled(false);
        ui->originBox5->setDisabled(false);
        ui->destBox5->setDisabled(false);
        ui->throttleBox5->setDisabled(false);
        ui->facingBox5->setDisabled(false);
        //ui->headingBox5->setDisabled(false);
        ui->trainimageButton5->hide();
        ui->routeInfo5->clear();
        ui->routeInfo5->hide();
    }

    ui->stopButton->setDisabled(true);
    ui->startButton->setDisabled(false);
}

//function to start periodic route calculations
void MainWindow::calculateRoute()
{

    if (controlBit == 1)
    {
        return;
    }

    ui->stopButton->setDisabled(false);
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
    //m_timer.start(250, this);
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

int MainWindow::longPathroute(int start, int end)
{
    if ((start == 43 || start == 44 || start == 13) &&
        (end == 43 || end == 44 || end == 13))
        return 1;
    else if ((start == 53 || start == 54 || start == 55 || start == 69) &&
             (end == 53 || end == 54 || end == 55 || end == 69))
             return 2;
    else if ((start == 95 || start == 94))
        return 3;
    else
        return 0;
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
}

void MainWindow::setgreyOut2()
{
    ui->setButton2->click();
    greyOut2();
}

void MainWindow::setgreyOut3()
{
    ui->setButton3->click();
    greyOut3();
}

void MainWindow::setgreyOut4()
{
    ui->setButton4->click();
    greyOut4();
}

void MainWindow::setgreyOut5()
{
    ui->setButton5->click();
    greyOut5();
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
    int oldEnd;
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
    else if (ui->originBox1->currentIndex() == 8)
        start = 95;
    else if (ui->originBox1->currentIndex() == 9)
        start = 94;
    else
        start = theirsToours(ui->originBox1->currentText());
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
    else if (ui->destBox1->currentIndex() == 8)
        end = 57;
    else if (ui->destBox1->currentIndex() == 9)
        end = 60;
    else
        end = theirsToours(ui->destBox1->currentText());
    //grey out line 1 if radio button selected
    if (ui->setButton1->isChecked()== true)
    {
        ui->trainselectBox1->setDisabled(true);
        ui->originBox1->setDisabled(true);
        ui->destBox1->setDisabled(true);
        ui->throttleBox1->setDisabled(true);
        ui->facingBox1->setDisabled(true);
        //ui->headingBox1->setDisabled(true);
        ui->setButton1->setDisabled(true);
        ui->trainimageButton1->show();
        ui->routeInfo1->show();



        if (ui->originBox1->currentIndex() != 0)
        {

            if ((ui->originBox1->currentIndex() == ui->originBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox1->currentIndex() == ui->originBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox1->currentIndex() == ui->originBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox1->currentIndex() == ui->originBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
        }

        if (ui->destBox1->currentIndex() != 0)
        {

            if ((ui->destBox1->currentIndex() == ui->destBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox1->currentIndex() == ui->destBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox1->currentIndex() == ui->destBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox1->currentIndex() == ui->destBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
        }
        
        if (ui->trainselectBox1->currentIndex() == 0 || ui->originBox1->currentIndex() == 0 || ui->destBox1->currentIndex() == 0 || ui->facingBox1->currentIndex() == 0 || ui->throttleBox1->currentIndex() == 0)
{
    cout << "Route info incomplete. Can not compute route with information given. Please fill out all values before attempting to start planning a route." << endl;
    ui->buttonGroup->setExclusive(false);
    ui->delayButton1->setChecked(false);
    ui->setButton1->setChecked(false);
    ui->buttonGroup->setExclusive(true);
    return;
    }

        if (start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)
        {
            pathcomplete1 = 1;
        }

        if((!(start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)) != (!(end== 95 || end == 59 || end == 61 || end == 93 || end == 62 || end == 99 || end == 100 || end == 60 || end == 94 || end == 58 || end == 56 || end == 96 || end == 65 || end == 101 || end == 102 || end == 57)))
        {
            cout << "Invalid path. Attempting to move between two non-connected tracks. " << endl;
            ui->buttonGroup->setExclusive(false);
            ui->delayButton1->setChecked(false);
            ui->setButton1->setChecked(false);
            ui->buttonGroup->setExclusive(true);
            return;
        }
        if(start == -1)
        {
            cout << "Start node is invalid." << endl;
            ui->buttonGroup->setExclusive(false);
            ui->delayButton1->setChecked(false);
            ui->setButton1->setChecked(false);
            ui->buttonGroup->setExclusive(true);
                    return;
        }
        if (end == -1)
        {
            cout << "Destination node is invalid" << endl;
            ui->buttonGroup->setExclusive(false);
            ui->delayButton1->setChecked(false);
            ui->setButton1->setChecked(false);
            ui->buttonGroup->setExclusive(true);
            return;
        }




        int shortorLong =  longPathroute(start, end);

                if (shortorLong == 0)
                {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));


 std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                        tableOperation(path, 1, 0, 0, 0);
                        cout << "Estimated time: " << to_string(comparisonArray.size()) << "seconds." << endl;
                        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";

                        cout << oursTotheirs(comparisonArray[0]).toStdString() << " ";
                        int l = 1;
                        
                        //This bit of code checks steps through the vector, finding where the train changes segments, then outputs these segments.
                        for (; l<comparisonArray.size(); l++)
                        {
                            while (comparisonArray[l] == comparisonArray[l-1] && (l < comparisonArray.size()))
                            {
                                l++;
                            }


                                if(l < comparisonArray.size())
                         cout << oursTotheirs(comparisonArray[l]).toStdString() << " ";
                         }
                         cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 1)
                {
                    oldEnd = end;
                    end = 1;

                        for(int i=3; i!=0; i--)
                        {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                //calculate and output information
                                       std::vector<weight_t> min_distance;
                std::vector<vertex_t> previous;
                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                if (i == 3)
                std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                tableOperation(path, 1, 0, i, 1);




                if(i <= 1)
                {
                    cout << "Estimated time: " << to_string(comparisonArray.size()) << "seconds." << endl;
                    std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";


                    cout << oursTotheirs(comparisonArray[0]).toStdString() << " ";
                    int l = 1;
                    for (; l<comparisonArray.size(); l++)
                    {
                        while (comparisonArray[l] == comparisonArray[l-1] && (l < comparisonArray.size()))
                        {
                            l++;
                        }
                         if(l < comparisonArray.size())
                     cout << oursTotheirs(comparisonArray[l]).toStdString() << " ";
                     }
                     cout << endl;
                }

                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;




                        if (i==3){start = 1; end = 2;}
                        else if (i==2) {start = 2; end = oldEnd;}
            }
        }
                else if (shortorLong == 3)
                {
                  adjacency_list_t adjacency_list(150);
                adjacency_list[62].push_back(neighbor(63, 2));
//                adjacency_list[63].push_back(neighbor(62, 2));
                adjacency_list[63].push_back(neighbor(92, 2));
//                adjacency_list[92].push_back(neighbor(63, 2));
                adjacency_list[92].push_back(neighbor(64, 2));
                adjacency_list[62].push_back(neighbor(99, 2));
                adjacency_list[99].push_back(neighbor(100, 2));
                adjacency_list[100].push_back(neighbor(60, 2));
//                adjacency_list[64].push_back(neighbor(92, 2));
                adjacency_list[64].push_back(neighbor(62, 2));
//                adjacency_list[62].push_back(neighbor(64, 2));
//                adjacency_list[62].push_back(neighbor(93, 2));
                adjacency_list[93].push_back(neighbor(62, 2));
                adjacency_list[61].push_back(neighbor(93, 2));
//                adjacency_list[93].push_back(neighbor(61, 2));
//                adjacency_list[60].push_back(neighbor(61, 2));
//                adjacency_list[61].push_back(neighbor(60, 2));
//                adjacency_list[61].push_back(neighbor(59, 2));
                adjacency_list[59].push_back(neighbor(61, 2));
//                adjacency_list[59].push_back(neighbor(95, 2));
                adjacency_list[95].push_back(neighbor(59, 2));
//                adjacency_list[94].push_back(neighbor(60, 2));
                adjacency_list[60].push_back(neighbor(94, 2));
                adjacency_list[94].push_back(neighbor(58, 2));
//                adjacency_list[58].push_back(neighbor(94, 2));
                adjacency_list[57].push_back(neighbor(95, 2));
//                adjacency_list[95].push_back(neighbor(57, 2));
                adjacency_list[102].push_back(neighbor(57, 2));
//                adjacency_list[57].push_back(neighbor(56, 2));
//                adjacency_list[56].push_back(neighbor(58, 2));
                adjacency_list[58].push_back(neighbor(56, 2));
                adjacency_list[56].push_back(neighbor(96, 2));
//                adjacency_list[96].push_back(neighbor(56, 2));
//                adjacency_list[65].push_back(neighbor(96, 2));
                adjacency_list[96].push_back(neighbor(65, 2));
                adjacency_list[65].push_back(neighbor(66, 2));
                adjacency_list[65].push_back(neighbor(101, 2));
                adjacency_list[101].push_back(neighbor(102, 2));
//                adjacency_list[66].push_back(neighbor(65, 2));
//                adjacency_list[65].push_back(neighbor(67, 2));
                adjacency_list[67].push_back(neighbor(65, 2));
//                adjacency_list[67].push_back(neighbor(97, 2));
                adjacency_list[97].push_back(neighbor(67, 2));
//                adjacency_list[97].push_back(neighbor(66, 2));
                adjacency_list[66].push_back(neighbor(97, 2));

 std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                        tableOperation(path, 1, 0, 0, 0);
                        cout << "Estimated time: " << to_string(comparisonArray.size()) << "seconds." << endl;
                        std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";

                        cout << oursTotheirs(comparisonArray[0]).toStdString() << " ";
                        int l = 1;
                        for (; l<comparisonArray.size(); l++)
                        {
                            while (comparisonArray[l] == comparisonArray[l-1] && (l < comparisonArray.size()))
                            {
                                l++;
                            }
                            if(l < comparisonArray.size())
                         cout << oursTotheirs(comparisonArray[l]).toStdString() << " ";
                         }
                         cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 2)
                        {
                            oldEnd = end;
                            end = 16;

                                for(int i=3; i!=0; i--){
                        adjacency_list_t adjacency_list(100);
                        adjacency_list[43].push_back(neighbor(75, 2));
                        adjacency_list[75].push_back(neighbor(43, 2));
                        adjacency_list[75].push_back(neighbor(42, 2));
                        adjacency_list[42].push_back(neighbor(75, 2));
                        adjacency_list[42].push_back(neighbor(76, 2));
                        adjacency_list[76].push_back(neighbor(42, 2));
                        adjacency_list[76].push_back(neighbor(44, 2));
                        adjacency_list[44].push_back(neighbor(76, 2));
                        adjacency_list[42].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(42, 2));
                        adjacency_list[46].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(46, 2));
                        adjacency_list[46].push_back(neighbor(77, 2));
                        adjacency_list[77].push_back(neighbor(46, 2));
                        adjacency_list[77].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(77, 2));
                        adjacency_list[41].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(41, 2));
                        adjacency_list[45].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(45, 2));
                        adjacency_list[45].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(45, 2));
                        adjacency_list[47].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(47, 2));
                        adjacency_list[49].push_back(neighbor(47, 2));
                        adjacency_list[47].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(49, 2));
                        adjacency_list[40].push_back(neighbor(15, 2));
                        adjacency_list[15].push_back(neighbor(40, 2));
                        adjacency_list[15].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(15, 2));
                        adjacency_list[14].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(79, 2));
                        adjacency_list[79].push_back(neighbor(14, 2));
                        adjacency_list[79].push_back(neighbor(80, 2));
                        adjacency_list[80].push_back(neighbor(79, 2));
                        adjacency_list[80].push_back(neighbor(8, 2));
                        adjacency_list[8].push_back(neighbor(80, 2));
                        adjacency_list[8].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(8, 2));
                        adjacency_list[7].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(5, 2));
                        adjacency_list[5].push_back(neighbor(7, 2));
                        adjacency_list[5].push_back(neighbor(3, 2));
                        adjacency_list[3].push_back(neighbor(5, 2));
                        //adjacency_list[5].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(5, 2));
                        adjacency_list[3].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(3, 2));
                        adjacency_list[1].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(1, 2));
                        adjacency_list[1].push_back(neighbor(0, 2));
                        adjacency_list[0].push_back(neighbor(1, 2));
                        adjacency_list[0].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(0, 2));
                        adjacency_list[2].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(2, 2));
                        adjacency_list[4].push_back(neighbor(6, 2));
                        adjacency_list[6].push_back(neighbor(4, 2));
                        //adjacency_list[6].push_back(neighbor(19, 2));
                        adjacency_list[19].push_back(neighbor(6, 2));
                        adjacency_list[19].push_back(neighbor(17, 2));
                        adjacency_list[17].push_back(neighbor(19, 2));
                        adjacency_list[17].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(17, 2));
                        adjacency_list[16].push_back(neighbor(82, 2));
                        adjacency_list[82].push_back(neighbor(16, 2));
                        adjacency_list[82].push_back(neighbor(12, 2));
                        adjacency_list[12].push_back(neighbor(82, 2));
                        adjacency_list[12].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(12, 2));
                        adjacency_list[11].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(11, 2));
                        adjacency_list[11].push_back(neighbor(13, 2));
                        adjacency_list[13].push_back(neighbor(11, 2));
                        adjacency_list[10].push_back(neighbor(98, 2));
                        adjacency_list[98].push_back(neighbor(10, 2));
                        adjacency_list[98].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(98, 2));
                        adjacency_list[17].push_back(neighbor(18, 2));
                        adjacency_list[18].push_back(neighbor(17, 2));
                        adjacency_list[18].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(18, 2));
                        adjacency_list[20].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(21, 2));
                        adjacency_list[21].push_back(neighbor(20, 2));
                        adjacency_list[21].push_back(neighbor(83, 2));
                        adjacency_list[83].push_back(neighbor(21, 2));
                        adjacency_list[83].push_back(neighbor(84, 2));
                        adjacency_list[84].push_back(neighbor(83, 2));
                        adjacency_list[84].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(84, 2));
                        adjacency_list[29].push_back(neighbor(28, 2));
                        adjacency_list[28].push_back(neighbor(29, 2));
                        adjacency_list[28].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(28, 2));
                        adjacency_list[26].push_back(neighbor(51, 2));
                        adjacency_list[51].push_back(neighbor(26, 2));
                        adjacency_list[51].push_back(neighbor(52, 2));
                        adjacency_list[52].push_back(neighbor(51, 2));
                        adjacency_list[52].push_back(neighbor(85, 2));
                        adjacency_list[85].push_back(neighbor(52, 2));
                        adjacency_list[85].push_back(neighbor(53, 2));
                        adjacency_list[53].push_back(neighbor(85, 2));
                        adjacency_list[52].push_back(neighbor(86, 2));
                        adjacency_list[86].push_back(neighbor(52, 2));
                        adjacency_list[86].push_back(neighbor(54, 2));
                        adjacency_list[54].push_back(neighbor(86, 2));
                        adjacency_list[51].push_back(neighbor(87, 2));
                        adjacency_list[87].push_back(neighbor(51, 2));
                        adjacency_list[87].push_back(neighbor(55, 2));
                        adjacency_list[55].push_back(neighbor(87, 2));
                        adjacency_list[22].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(24, 2));
                        adjacency_list[24].push_back(neighbor(22, 2));
                        adjacency_list[24].push_back(neighbor(25, 2));
                        adjacency_list[25].push_back(neighbor(24, 2));
                        adjacency_list[25].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(25, 2));
                        adjacency_list[27].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(27, 2));
                        adjacency_list[28].push_back(neighbor(27, 2));
                        adjacency_list[27].push_back(neighbor(28, 2));
                        adjacency_list[0].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(0, 2));
                        adjacency_list[39].push_back(neighbor(38, 2));
                        adjacency_list[38].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(39, 2));
                        adjacency_list[88].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(88, 2));
                        adjacency_list[88].push_back(neighbor(89, 2));
                        adjacency_list[89].push_back(neighbor(88, 2));
                        adjacency_list[89].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(89, 2));
                        adjacency_list[34].push_back(neighbor(50, 2));
                        adjacency_list[50].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(32, 2));
                        adjacency_list[32].push_back(neighbor(34, 2));
                        adjacency_list[32].push_back(neighbor(33, 2));
                        adjacency_list[33].push_back(neighbor(32, 2));
                        adjacency_list[33].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(33, 2));
                        adjacency_list[35].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(35, 2));
                        adjacency_list[35].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(35, 2));
                        adjacency_list[36].push_back(neighbor(68, 2));
                        adjacency_list[68].push_back(neighbor(36, 2));
                        adjacency_list[68].push_back(neighbor(69, 2));
                        adjacency_list[69].push_back(neighbor(68, 2));
                        adjacency_list[38].push_back(neighbor(91, 2));
                        adjacency_list[91].push_back(neighbor(38, 2));
                        adjacency_list[91].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(91, 2));
                        adjacency_list[32].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(32, 2));
                        adjacency_list[50].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(50, 2));
                        adjacency_list[31].push_back(neighbor(30, 2));
                        adjacency_list[30].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(70, 2));
                        adjacency_list[70].push_back(neighbor(31, 2));
                        adjacency_list[70].push_back(neighbor(71, 2));
                        adjacency_list[71].push_back(neighbor(70, 2));
                        adjacency_list[71].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(71, 2));
                        adjacency_list[30].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(30, 2));

                        //calculate and output information
 std::vector<weight_t> min_distance;
                                std::vector<vertex_t> previous;
                                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                                if (i == 3)
                                std::cout << "Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);


                                tableOperation(path, 1, 0, i, 1);



                                if(i <= 1)
                                {

                                    cout << "Estimated time: " << to_string(comparisonArray.size()) << "seconds." << endl;
                                    std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
                                    cout << oursTotheirs(comparisonArray[0]).toStdString() << " ";
                                    int l = 1;
                                    for (; l<comparisonArray.size(); l++)
                                    {
                                        while (comparisonArray[l] == comparisonArray[l-1] && (l < comparisonArray.size()))
                                        {
                                            l++;
                                        }
                                         if(l < comparisonArray.size())
                                     cout << oursTotheirs(comparisonArray[l]).toStdString() << " ";
                                     }
                                     cout << endl;
                                }


                                //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                                std::cout << std::endl;
                                if (i==3){start = 16; end = 0;}
                                else if (i==2) {start = 0; end = oldEnd;}
                        }


        }}
    //if radio button un-checked then un-greyout line 1
    else
    {
        ui->trainselectBox1->setDisabled(false);
        ui->originBox1->setDisabled(false);
        ui->destBox1->setDisabled(false);
        ui->throttleBox1->setDisabled(false);
        ui->facingBox1->setDisabled(false);
        //ui->headingBox1->setDisabled(false);
        ui->trainimageButton1->hide();
        ui->routeInfo1->clear();
        ui->routeInfo1->hide();
    }
}

//lock in and calculate route for line 2
void MainWindow::greyOut2()
{
    new Q_DebugStream(std::cout, ui->routeInfo2); //Redirect Console output to textBrowser2
    //Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox2->currentIndex() == 1)
    {
        ui->trainimageButton2->setIcon(*engine2Icon);
        ui->trainimageButton2->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox2->currentIndex() == 2)
    {
        ui->trainimageButton2->setIcon(*engine1Icon);
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
    int oldEnd;
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
    else if (ui->originBox2->currentIndex() == 8)
        start = 95;
    else if (ui->originBox2->currentIndex() == 9)
        start = 94;
    else
        start = theirsToours(ui->originBox2->currentText());
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
    else if (ui->destBox2->currentIndex() == 8)
        end = 57;
    else if (ui->destBox2->currentIndex() == 9)
        end = 60;
    else
        end = theirsToours(ui->destBox2->currentText());
    //grey out line 2 if radio button selected
    if (ui->setButton2->isChecked()== true)
    {
        ui->trainselectBox2->setDisabled(true);
        ui->originBox2->setDisabled(true);
        ui->destBox2->setDisabled(true);
        ui->throttleBox2->setDisabled(true);
        ui->facingBox2->setDisabled(true);
        //ui->headingBox2->setDisabled(true);
        ui->setButton2->setDisabled(true);
        ui->trainimageButton2->show();
        ui->routeInfo2->show();


        if (ui->originBox2->currentIndex() != 0)
        {

            if ((ui->originBox2->currentIndex() == ui->originBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox2->currentIndex() == ui->originBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox2->currentIndex() == ui->originBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox2->currentIndex() == ui->originBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
        }

        if (ui->destBox2->currentIndex() != 0)
        {

            if ((ui->destBox2->currentIndex() == ui->destBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox2->currentIndex() == ui->destBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox2->currentIndex() == ui->destBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox2->currentIndex() == ui->destBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
        }

        if (ui->trainselectBox2->currentIndex() == 0 || ui->originBox2->currentIndex() == 0 || ui->destBox2->currentIndex() == 0 || ui->facingBox2->currentIndex() == 0 || ui->throttleBox2->currentIndex() == 0)
        {
            cout << "Route info incomplete. Can not compute route with information given. Please fill out all values before attempting to start planning a route." << endl;
            ui->buttonGroup_2->setExclusive(false);
            ui->delayButton2->setChecked(false);
            ui->setButton2->setChecked(false);
            ui->buttonGroup_2->setExclusive(true);
            return;
            }
        if((!(start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)) != (!(end== 95 || end == 59 || end == 61 || end == 93 || end == 62 || end == 99 || end == 100 || end == 60 || end == 94 || end == 58 || end == 56 || end == 96 || end == 65 || end == 101 || end == 102 || end == 57)))
        {
            cout << "Invalid path. Attempting to move between two non-connected tracks. " << endl;
            ui->buttonGroup_2->setExclusive(false);
            ui->delayButton2->setChecked(false);
            ui->setButton2->setChecked(false);
            ui->buttonGroup_2->setExclusive(true);
            return;
        }

        if (start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)
        {
            pathcomplete2 = 1;
        }
        if(start == -1)
        {
            cout << "Start node is invalid." << endl;
            ui->buttonGroup_2->setExclusive(false);
            ui->delayButton2->setChecked(false);
            ui->setButton2->setChecked(false);
            ui->buttonGroup_2->setExclusive(true);
                    return;
        }
        if (end == -1)
        {
            cout << "Destination node is invalid" << endl;
            ui->buttonGroup_2->setExclusive(false);
            ui->delayButton2->setChecked(false);
            ui->setButton2->setChecked(false);
            ui->buttonGroup_2->setExclusive(true);
            return;
        }

        int shortorLong =  longPathroute(start, end);

                if (shortorLong == 0)
                {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                 std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                        tableOperation(path, 2, 0, 0, 0);
                        cout << "Estimated time: " << to_string(comparisonArray2.size()) << "seconds." << endl;
                        std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";

                        cout << oursTotheirs(comparisonArray2[0]).toStdString() << " ";
                        int l = 1;
                        for (; l<comparisonArray2.size(); l++)
                        {
                            while (comparisonArray2[l] == comparisonArray2[l-1] && (l < comparisonArray2.size()))
                            {
                                l++;
                            }
                            if(l < comparisonArray2.size())
                         cout << oursTotheirs(comparisonArray2[l]).toStdString() << " ";
                         }
                         cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 3)
                {
                    adjacency_list_t adjacency_list(150);
                  adjacency_list[62].push_back(neighbor(63, 2));
  //                adjacency_list[63].push_back(neighbor(62, 2));
                  adjacency_list[63].push_back(neighbor(92, 2));
  //                adjacency_list[92].push_back(neighbor(63, 2));
                  adjacency_list[92].push_back(neighbor(64, 2));
                  adjacency_list[62].push_back(neighbor(99, 2));
                  adjacency_list[99].push_back(neighbor(100, 2));
                  adjacency_list[100].push_back(neighbor(60, 2));
  //                adjacency_list[64].push_back(neighbor(92, 2));
                  adjacency_list[64].push_back(neighbor(62, 2));
  //                adjacency_list[62].push_back(neighbor(64, 2));
  //                adjacency_list[62].push_back(neighbor(93, 2));
                  adjacency_list[93].push_back(neighbor(62, 2));
                  adjacency_list[61].push_back(neighbor(93, 2));
  //                adjacency_list[93].push_back(neighbor(61, 2));
  //                adjacency_list[60].push_back(neighbor(61, 2));
  //                adjacency_list[61].push_back(neighbor(60, 2));
  //                adjacency_list[61].push_back(neighbor(59, 2));
                  adjacency_list[59].push_back(neighbor(61, 2));
  //                adjacency_list[59].push_back(neighbor(95, 2));
                  adjacency_list[95].push_back(neighbor(59, 2));
  //                adjacency_list[94].push_back(neighbor(60, 2));
                  adjacency_list[60].push_back(neighbor(94, 2));
                  adjacency_list[94].push_back(neighbor(58, 2));
  //                adjacency_list[58].push_back(neighbor(94, 2));
                  adjacency_list[57].push_back(neighbor(95, 2));
  //                adjacency_list[95].push_back(neighbor(57, 2));
                  adjacency_list[102].push_back(neighbor(57, 2));
  //                adjacency_list[57].push_back(neighbor(56, 2));
  //                adjacency_list[56].push_back(neighbor(58, 2));
                  adjacency_list[58].push_back(neighbor(56, 2));
                  adjacency_list[56].push_back(neighbor(96, 2));
  //                adjacency_list[96].push_back(neighbor(56, 2));
  //                adjacency_list[65].push_back(neighbor(96, 2));
                  adjacency_list[96].push_back(neighbor(65, 2));
                  adjacency_list[65].push_back(neighbor(66, 2));
                  adjacency_list[65].push_back(neighbor(101, 2));
                  adjacency_list[101].push_back(neighbor(102, 2));
  //                adjacency_list[66].push_back(neighbor(65, 2));
  //                adjacency_list[65].push_back(neighbor(67, 2));
                  adjacency_list[67].push_back(neighbor(65, 2));
  //                adjacency_list[67].push_back(neighbor(97, 2));
                  adjacency_list[97].push_back(neighbor(67, 2));
  //                adjacency_list[97].push_back(neighbor(66, 2));
                  adjacency_list[66].push_back(neighbor(97, 2));

                 std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                        tableOperation(path, 2, 0, 0, 0);
                        cout << "Estimated time: " << to_string(comparisonArray2.size()) << "seconds." << endl;
                        std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";

                        cout << oursTotheirs(comparisonArray2[0]).toStdString() << " ";
                        int l = 1;
                        for (; l<comparisonArray2.size(); l++)
                        {
                            while (comparisonArray2[l] == comparisonArray2[l-1] && (l < comparisonArray2.size()))
                            {
                                l++;
                            }
                            if(l < comparisonArray2.size())
                         cout << oursTotheirs(comparisonArray2[l]).toStdString() << " ";
                         }
                         cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 1)
                {
                    oldEnd = end;
                    end = 1;

                        for(int i=3; i!=0; i--)
                        {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                //calculate and output information
                                       std::vector<weight_t> min_distance;
                std::vector<vertex_t> previous;
                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                if (i == 3)
                std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);


                tableOperation(path, 2, 0, i, 1);



                if(i <= 1)
                {
                    cout << "Estimated time: " << to_string(comparisonArray2.size()) << "seconds." << endl;
                    std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
                    cout << oursTotheirs(comparisonArray2[0]).toStdString() << " ";
                    int l = 1;
                    for (; l<comparisonArray2.size(); l++)
                    {
                        while (comparisonArray2[l] == comparisonArray2[l-1] && (l < comparisonArray2.size()))
                        {
                            l++;
                        }
                         if(l < comparisonArray2.size())
                     cout << oursTotheirs(comparisonArray2[l]).toStdString() << " ";
                     }
                     cout << endl;
                }


                //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                std::cout << std::endl;
                if (i==3){start = 16; end = 0;}
                else if (i==2) {start = 0; end = oldEnd;}
            }
        }
                    else if (shortorLong == 2)
                        {
                            oldEnd = end;
                            end = 16;

                                for(int i=3; i!=0; i--){
                        adjacency_list_t adjacency_list(100);
                        adjacency_list[43].push_back(neighbor(75, 2));
                        adjacency_list[75].push_back(neighbor(43, 2));
                        adjacency_list[75].push_back(neighbor(42, 2));
                        adjacency_list[42].push_back(neighbor(75, 2));
                        adjacency_list[42].push_back(neighbor(76, 2));
                        adjacency_list[76].push_back(neighbor(42, 2));
                        adjacency_list[76].push_back(neighbor(44, 2));
                        adjacency_list[44].push_back(neighbor(76, 2));
                        adjacency_list[42].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(42, 2));
                        adjacency_list[46].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(46, 2));
                        adjacency_list[46].push_back(neighbor(77, 2));
                        adjacency_list[77].push_back(neighbor(46, 2));
                        adjacency_list[77].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(77, 2));
                        adjacency_list[41].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(41, 2));
                        adjacency_list[45].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(45, 2));
                        adjacency_list[45].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(45, 2));
                        adjacency_list[47].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(47, 2));
                        adjacency_list[49].push_back(neighbor(47, 2));
                        adjacency_list[47].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(49, 2));
                        adjacency_list[40].push_back(neighbor(15, 2));
                        adjacency_list[15].push_back(neighbor(40, 2));
                        adjacency_list[15].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(15, 2));
                        adjacency_list[14].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(79, 2));
                        adjacency_list[79].push_back(neighbor(14, 2));
                        adjacency_list[79].push_back(neighbor(80, 2));
                        adjacency_list[80].push_back(neighbor(79, 2));
                        adjacency_list[80].push_back(neighbor(8, 2));
                        adjacency_list[8].push_back(neighbor(80, 2));
                        adjacency_list[8].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(8, 2));
                        adjacency_list[7].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(5, 2));
                        adjacency_list[5].push_back(neighbor(7, 2));
                        adjacency_list[5].push_back(neighbor(3, 2));
                        adjacency_list[3].push_back(neighbor(5, 2));
                        //adjacency_list[5].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(5, 2));
                        adjacency_list[3].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(3, 2));
                        adjacency_list[1].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(1, 2));
                        adjacency_list[1].push_back(neighbor(0, 2));
                        adjacency_list[0].push_back(neighbor(1, 2));
                        adjacency_list[0].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(0, 2));
                        adjacency_list[2].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(2, 2));
                        adjacency_list[4].push_back(neighbor(6, 2));
                        adjacency_list[6].push_back(neighbor(4, 2));
                        //adjacency_list[6].push_back(neighbor(19, 2));
                        adjacency_list[19].push_back(neighbor(6, 2));
                        adjacency_list[19].push_back(neighbor(17, 2));
                        adjacency_list[17].push_back(neighbor(19, 2));
                        adjacency_list[17].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(17, 2));
                        adjacency_list[16].push_back(neighbor(82, 2));
                        adjacency_list[82].push_back(neighbor(16, 2));
                        adjacency_list[82].push_back(neighbor(12, 2));
                        adjacency_list[12].push_back(neighbor(82, 2));
                        adjacency_list[12].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(12, 2));
                        adjacency_list[11].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(11, 2));
                        adjacency_list[11].push_back(neighbor(13, 2));
                        adjacency_list[13].push_back(neighbor(11, 2));
                        adjacency_list[10].push_back(neighbor(98, 2));
                        adjacency_list[98].push_back(neighbor(10, 2));
                        adjacency_list[98].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(98, 2));
                        adjacency_list[17].push_back(neighbor(18, 2));
                        adjacency_list[18].push_back(neighbor(17, 2));
                        adjacency_list[18].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(18, 2));
                        adjacency_list[20].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(21, 2));
                        adjacency_list[21].push_back(neighbor(20, 2));
                        adjacency_list[21].push_back(neighbor(83, 2));
                        adjacency_list[83].push_back(neighbor(21, 2));
                        adjacency_list[83].push_back(neighbor(84, 2));
                        adjacency_list[84].push_back(neighbor(83, 2));
                        adjacency_list[84].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(84, 2));
                        adjacency_list[29].push_back(neighbor(28, 2));
                        adjacency_list[28].push_back(neighbor(29, 2));
                        adjacency_list[28].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(28, 2));
                        adjacency_list[26].push_back(neighbor(51, 2));
                        adjacency_list[51].push_back(neighbor(26, 2));
                        adjacency_list[51].push_back(neighbor(52, 2));
                        adjacency_list[52].push_back(neighbor(51, 2));
                        adjacency_list[52].push_back(neighbor(85, 2));
                        adjacency_list[85].push_back(neighbor(52, 2));
                        adjacency_list[85].push_back(neighbor(53, 2));
                        adjacency_list[53].push_back(neighbor(85, 2));
                        adjacency_list[52].push_back(neighbor(86, 2));
                        adjacency_list[86].push_back(neighbor(52, 2));
                        adjacency_list[86].push_back(neighbor(54, 2));
                        adjacency_list[54].push_back(neighbor(86, 2));
                        adjacency_list[51].push_back(neighbor(87, 2));
                        adjacency_list[87].push_back(neighbor(51, 2));
                        adjacency_list[87].push_back(neighbor(55, 2));
                        adjacency_list[55].push_back(neighbor(87, 2));
                        adjacency_list[22].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(24, 2));
                        adjacency_list[24].push_back(neighbor(22, 2));
                        adjacency_list[24].push_back(neighbor(25, 2));
                        adjacency_list[25].push_back(neighbor(24, 2));
                        adjacency_list[25].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(25, 2));
                        adjacency_list[27].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(27, 2));
                        adjacency_list[28].push_back(neighbor(27, 2));
                        adjacency_list[27].push_back(neighbor(28, 2));
                        adjacency_list[0].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(0, 2));
                        adjacency_list[39].push_back(neighbor(38, 2));
                        adjacency_list[38].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(39, 2));
                        adjacency_list[88].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(88, 2));
                        adjacency_list[88].push_back(neighbor(89, 2));
                        adjacency_list[89].push_back(neighbor(88, 2));
                        adjacency_list[89].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(89, 2));
                        adjacency_list[34].push_back(neighbor(50, 2));
                        adjacency_list[50].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(32, 2));
                        adjacency_list[32].push_back(neighbor(34, 2));
                        adjacency_list[32].push_back(neighbor(33, 2));
                        adjacency_list[33].push_back(neighbor(32, 2));
                        adjacency_list[33].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(33, 2));
                        adjacency_list[35].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(35, 2));
                        adjacency_list[35].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(35, 2));
                        adjacency_list[36].push_back(neighbor(68, 2));
                        adjacency_list[68].push_back(neighbor(36, 2));
                        adjacency_list[68].push_back(neighbor(69, 2));
                        adjacency_list[69].push_back(neighbor(68, 2));
                        adjacency_list[38].push_back(neighbor(91, 2));
                        adjacency_list[91].push_back(neighbor(38, 2));
                        adjacency_list[91].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(91, 2));
                        adjacency_list[32].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(32, 2));
                        adjacency_list[50].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(50, 2));
                        adjacency_list[31].push_back(neighbor(30, 2));
                        adjacency_list[30].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(70, 2));
                        adjacency_list[70].push_back(neighbor(31, 2));
                        adjacency_list[70].push_back(neighbor(71, 2));
                        adjacency_list[71].push_back(neighbor(70, 2));
                        adjacency_list[71].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(71, 2));
                        adjacency_list[30].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(30, 2));

                        //calculate and output information
                                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        if (i == 3)
                        std::cout << "Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);


                        tableOperation(path, 2, 0, i, 1);



                        if(i <= 1)
                        {

                            cout << "Estimated time: " << to_string(comparisonArray2.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray2[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray2.size(); l++)
                            {
                                while (comparisonArray2[l] == comparisonArray2[l-1] && (l < comparisonArray2.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray2.size())
                             cout << oursTotheirs(comparisonArray2[l]).toStdString() << " ";
                             }
                             cout << endl;
                        }


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
                        if (i==3){start = 16; end = 0;}
                        else if (i==2) {start = 0; end = oldEnd;}
                        }


        }}
    //if radio button un-checked then un-greyout line 2
    else
    {
        ui->trainselectBox2->setDisabled(false);
        ui->originBox2->setDisabled(false);
        ui->destBox2->setDisabled(false);
        ui->throttleBox2->setDisabled(false);
        ui->facingBox2->setDisabled(false);
        //ui->headingBox2->setDisabled(false);
        ui->trainimageButton2->hide();
        ui->routeInfo2->clear();
        ui->routeInfo2->hide();
    }
}

//lock in and calculate route for line 3
void MainWindow::greyOut3()
{
    new Q_DebugStream(std::cout, ui->routeInfo3); //Redirect Console output to textBrowser2
    //Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox3->currentIndex() == 1)
    {
        ui->trainimageButton3->setIcon(*engine3Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 2)
    {
        ui->trainimageButton3->setIcon(*engine2Icon);
        ui->trainimageButton3->setIconSize(QSize(85,85));
    }
    else if(ui->trainselectBox3->currentIndex() == 3)
    {
        ui->trainimageButton3->setIcon(*engine1Icon);
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
    int oldEnd;
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
    else if (ui->originBox3->currentIndex() == 8)
        start = 95;
    else if (ui->originBox3->currentIndex() == 9)
        start = 94;
    else
        start = theirsToours(ui->originBox3->currentText());
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
    else if (ui->destBox3->currentIndex() == 8)
        end = 57;
    else if (ui->destBox3->currentIndex() == 9)
        end = 60;
    else
        end = theirsToours(ui->destBox3->currentText());
    //grey out line 3 if radio button selected
    if (ui->setButton3->isChecked()== true)
    {
        ui->trainselectBox3->setDisabled(true);
        ui->originBox3->setDisabled(true);
        ui->destBox3->setDisabled(true);
        ui->throttleBox3->setDisabled(true);
        ui->facingBox3->setDisabled(true);
        //ui->headingBox3->setDisabled(true);
        ui->setButton3->setDisabled(true);
        ui->trainimageButton3->show();
        ui->routeInfo3->show();


        if (ui->originBox3->currentIndex() != 0)
           {

               if ((ui->originBox3->currentIndex() == ui->originBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
               {
                   cout << "Trains can not begin concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->originBox3->currentIndex() == ui->originBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
               {
                   cout << "Trains can not begin concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->originBox3->currentIndex() == ui->originBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
               {
                   cout << "Trains can not begin concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->originBox3->currentIndex() == ui->originBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
               {
                   cout << "Trains can not begin concurent paths at the same node." << endl;
                   return;
               }
           }

           if (ui->destBox3->currentIndex() != 0)
           {

               if ((ui->destBox3->currentIndex() == ui->destBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
               {
                   cout << "Trains can not end concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->destBox3->currentIndex() == ui->destBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
               {
                   cout << "Trains can not end concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->destBox3->currentIndex() == ui->destBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
               {
                   cout << "Trains can not end concurent paths at the same node." << endl;
                   return;
               }
               else if ((ui->destBox3->currentIndex() == ui->destBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
               {
                   cout << "Trains can not end concurent paths at the same node." << endl;
                   return;
               }
           }

                if (ui->trainselectBox3->currentIndex() == 0 || ui->originBox3->currentIndex() == 0 || ui->destBox3->currentIndex() == 0 || ui->facingBox3->currentIndex() == 0 || ui->throttleBox3->currentIndex() == 0)
        {
            cout << "Route info incomplete. Can not compute route with information given. Please fill out all values before attempting to start planning a route." << endl;
            ui->buttonGroup_3->setExclusive(false);
            ui->delayButton3->setChecked(false);
            ui->setButton3->setChecked(false);
            ui->buttonGroup_3->setExclusive(true);
            return;
            }
                if((!(start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)) != (!(end== 95 || end == 59 || end == 61 || end == 93 || end == 62 || end == 99 || end == 100 || end == 60 || end == 94 || end == 58 || end == 56 || end == 96 || end == 65 || end == 101 || end == 102 || end == 57)))
                {
                    cout << "Invalid path. Attempting to move between two non-connected tracks. " << endl;
                    ui->buttonGroup_3->setExclusive(false);
                    ui->delayButton3->setChecked(false);
                    ui->setButton3->setChecked(false);
                    ui->buttonGroup_3->setExclusive(true);
                    return;
                }

                if (start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)
                {
                    pathcomplete3 = 1;
                }
                if(start == -1)
                {
                    cout << "Start node is invalid." << endl;\
                    ui->buttonGroup_3->setExclusive(false);
                    ui->delayButton3->setChecked(false);
                    ui->setButton3->setChecked(false);
                    ui->buttonGroup_3->setExclusive(true);
                            return;
                }
                if (end == -1)
                {
                    cout << "Destination node is invalid" << endl;
                    ui->buttonGroup_3->setExclusive(false);
                    ui->delayButton3->setChecked(false);
                    ui->setButton3->setChecked(false);
                    ui->buttonGroup_3->setExclusive(true);
                    return;
                }


        int shortorLong =  longPathroute(start, end);

                if (shortorLong == 0)
                {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);

                        tableOperation(path, 3, 0, 0, 0);



                            cout << "Estimated time: " << to_string(comparisonArray3.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray3[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray3.size(); l++)
                            {
                                while (comparisonArray3[l] == comparisonArray3[l-1] && (l < comparisonArray3.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray3.size())
                             cout << oursTotheirs(comparisonArray3[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 3)
                {
                    adjacency_list_t adjacency_list(150);
                  adjacency_list[62].push_back(neighbor(63, 2));
  //                adjacency_list[63].push_back(neighbor(62, 2));
                  adjacency_list[63].push_back(neighbor(92, 2));
  //                adjacency_list[92].push_back(neighbor(63, 2));
                  adjacency_list[92].push_back(neighbor(64, 2));
                  adjacency_list[62].push_back(neighbor(99, 2));
                  adjacency_list[99].push_back(neighbor(100, 2));
                  adjacency_list[100].push_back(neighbor(60, 2));
  //                adjacency_list[64].push_back(neighbor(92, 2));
                  adjacency_list[64].push_back(neighbor(62, 2));
  //                adjacency_list[62].push_back(neighbor(64, 2));
  //                adjacency_list[62].push_back(neighbor(93, 2));
                  adjacency_list[93].push_back(neighbor(62, 2));
                  adjacency_list[61].push_back(neighbor(93, 2));
  //                adjacency_list[93].push_back(neighbor(61, 2));
  //                adjacency_list[60].push_back(neighbor(61, 2));
  //                adjacency_list[61].push_back(neighbor(60, 2));
  //                adjacency_list[61].push_back(neighbor(59, 2));
                  adjacency_list[59].push_back(neighbor(61, 2));
  //                adjacency_list[59].push_back(neighbor(95, 2));
                  adjacency_list[95].push_back(neighbor(59, 2));
  //                adjacency_list[94].push_back(neighbor(60, 2));
                  adjacency_list[60].push_back(neighbor(94, 2));
                  adjacency_list[94].push_back(neighbor(58, 2));
  //                adjacency_list[58].push_back(neighbor(94, 2));
                  adjacency_list[57].push_back(neighbor(95, 2));
  //                adjacency_list[95].push_back(neighbor(57, 2));
                  adjacency_list[102].push_back(neighbor(57, 2));
  //                adjacency_list[57].push_back(neighbor(56, 2));
  //                adjacency_list[56].push_back(neighbor(58, 2));
                  adjacency_list[58].push_back(neighbor(56, 2));
                  adjacency_list[56].push_back(neighbor(96, 2));
  //                adjacency_list[96].push_back(neighbor(56, 2));
  //                adjacency_list[65].push_back(neighbor(96, 2));
                  adjacency_list[96].push_back(neighbor(65, 2));
                  adjacency_list[65].push_back(neighbor(66, 2));
                  adjacency_list[65].push_back(neighbor(101, 2));
                  adjacency_list[101].push_back(neighbor(102, 2));
  //                adjacency_list[66].push_back(neighbor(65, 2));
  //                adjacency_list[65].push_back(neighbor(67, 2));
                  adjacency_list[67].push_back(neighbor(65, 2));
  //                adjacency_list[67].push_back(neighbor(97, 2));
                  adjacency_list[97].push_back(neighbor(67, 2));
  //                adjacency_list[97].push_back(neighbor(66, 2));
                  adjacency_list[66].push_back(neighbor(97, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        //std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 3, 0, 0, 0);



                            cout << "Estimated time: " << to_string(comparisonArray3.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray3[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray3.size(); l++)
                            {
                                while (comparisonArray3[l] == comparisonArray3[l-1] && (l < comparisonArray3.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray3.size())
                             cout << oursTotheirs(comparisonArray3[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 1)
                {
                    oldEnd = end;
                    end = 1;

                        for(int i=3; i!=0; i--)
                        {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                //calculate and output information
                        std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 3, 0, i, 1);

                        if(i <= 1)
                        {

                            cout << "Estimated time: " << to_string(comparisonArray3.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray3[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray3.size(); l++)
                            {
                                while (comparisonArray3[l] == comparisonArray3[l-1] && (l < comparisonArray3.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray3.size())
                             cout << oursTotheirs(comparisonArray3[l]).toStdString() << " ";
                             }
                             cout << endl;
                        }

                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;


                        if (i==3){start = 1; end = 2;}
                        else if (i==2) {start = 2; end = oldEnd;}
            }
        }
                    else if (shortorLong == 2)
                        {
                            oldEnd = end;
                            end = 16;

                                for(int i=3; i!=0; i--){
                        adjacency_list_t adjacency_list(100);
                        adjacency_list[43].push_back(neighbor(75, 2));
                        adjacency_list[75].push_back(neighbor(43, 2));
                        adjacency_list[75].push_back(neighbor(42, 2));
                        adjacency_list[42].push_back(neighbor(75, 2));
                        adjacency_list[42].push_back(neighbor(76, 2));
                        adjacency_list[76].push_back(neighbor(42, 2));
                        adjacency_list[76].push_back(neighbor(44, 2));
                        adjacency_list[44].push_back(neighbor(76, 2));
                        adjacency_list[42].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(42, 2));
                        adjacency_list[46].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(46, 2));
                        adjacency_list[46].push_back(neighbor(77, 2));
                        adjacency_list[77].push_back(neighbor(46, 2));
                        adjacency_list[77].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(77, 2));
                        adjacency_list[41].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(41, 2));
                        adjacency_list[45].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(45, 2));
                        adjacency_list[45].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(45, 2));
                        adjacency_list[47].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(47, 2));
                        adjacency_list[49].push_back(neighbor(47, 2));
                        adjacency_list[47].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(49, 2));
                        adjacency_list[40].push_back(neighbor(15, 2));
                        adjacency_list[15].push_back(neighbor(40, 2));
                        adjacency_list[15].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(15, 2));
                        adjacency_list[14].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(79, 2));
                        adjacency_list[79].push_back(neighbor(14, 2));
                        adjacency_list[79].push_back(neighbor(80, 2));
                        adjacency_list[80].push_back(neighbor(79, 2));
                        adjacency_list[80].push_back(neighbor(8, 2));
                        adjacency_list[8].push_back(neighbor(80, 2));
                        adjacency_list[8].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(8, 2));
                        adjacency_list[7].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(5, 2));
                        adjacency_list[5].push_back(neighbor(7, 2));
                        adjacency_list[5].push_back(neighbor(3, 2));
                        adjacency_list[3].push_back(neighbor(5, 2));
                        //adjacency_list[5].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(5, 2));
                        adjacency_list[3].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(3, 2));
                        adjacency_list[1].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(1, 2));
                        adjacency_list[1].push_back(neighbor(0, 2));
                        adjacency_list[0].push_back(neighbor(1, 2));
                        adjacency_list[0].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(0, 2));
                        adjacency_list[2].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(2, 2));
                        adjacency_list[4].push_back(neighbor(6, 2));
                        adjacency_list[6].push_back(neighbor(4, 2));
                        //adjacency_list[6].push_back(neighbor(19, 2));
                        adjacency_list[19].push_back(neighbor(6, 2));
                        adjacency_list[19].push_back(neighbor(17, 2));
                        adjacency_list[17].push_back(neighbor(19, 2));
                        adjacency_list[17].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(17, 2));
                        adjacency_list[16].push_back(neighbor(82, 2));
                        adjacency_list[82].push_back(neighbor(16, 2));
                        adjacency_list[82].push_back(neighbor(12, 2));
                        adjacency_list[12].push_back(neighbor(82, 2));
                        adjacency_list[12].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(12, 2));
                        adjacency_list[11].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(11, 2));
                        adjacency_list[11].push_back(neighbor(13, 2));
                        adjacency_list[13].push_back(neighbor(11, 2));
                        adjacency_list[10].push_back(neighbor(98, 2));
                        adjacency_list[98].push_back(neighbor(10, 2));
                        adjacency_list[98].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(98, 2));
                        adjacency_list[17].push_back(neighbor(18, 2));
                        adjacency_list[18].push_back(neighbor(17, 2));
                        adjacency_list[18].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(18, 2));
                        adjacency_list[20].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(21, 2));
                        adjacency_list[21].push_back(neighbor(20, 2));
                        adjacency_list[21].push_back(neighbor(83, 2));
                        adjacency_list[83].push_back(neighbor(21, 2));
                        adjacency_list[83].push_back(neighbor(84, 2));
                        adjacency_list[84].push_back(neighbor(83, 2));
                        adjacency_list[84].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(84, 2));
                        adjacency_list[29].push_back(neighbor(28, 2));
                        adjacency_list[28].push_back(neighbor(29, 2));
                        adjacency_list[28].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(28, 2));
                        adjacency_list[26].push_back(neighbor(51, 2));
                        adjacency_list[51].push_back(neighbor(26, 2));
                        adjacency_list[51].push_back(neighbor(52, 2));
                        adjacency_list[52].push_back(neighbor(51, 2));
                        adjacency_list[52].push_back(neighbor(85, 2));
                        adjacency_list[85].push_back(neighbor(52, 2));
                        adjacency_list[85].push_back(neighbor(53, 2));
                        adjacency_list[53].push_back(neighbor(85, 2));
                        adjacency_list[52].push_back(neighbor(86, 2));
                        adjacency_list[86].push_back(neighbor(52, 2));
                        adjacency_list[86].push_back(neighbor(54, 2));
                        adjacency_list[54].push_back(neighbor(86, 2));
                        adjacency_list[51].push_back(neighbor(87, 2));
                        adjacency_list[87].push_back(neighbor(51, 2));
                        adjacency_list[87].push_back(neighbor(55, 2));
                        adjacency_list[55].push_back(neighbor(87, 2));
                        adjacency_list[22].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(24, 2));
                        adjacency_list[24].push_back(neighbor(22, 2));
                        adjacency_list[24].push_back(neighbor(25, 2));
                        adjacency_list[25].push_back(neighbor(24, 2));
                        adjacency_list[25].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(25, 2));
                        adjacency_list[27].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(27, 2));
                        adjacency_list[28].push_back(neighbor(27, 2));
                        adjacency_list[27].push_back(neighbor(28, 2));
                        adjacency_list[0].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(0, 2));
                        adjacency_list[39].push_back(neighbor(38, 2));
                        adjacency_list[38].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(39, 2));
                        adjacency_list[88].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(88, 2));
                        adjacency_list[88].push_back(neighbor(89, 2));
                        adjacency_list[89].push_back(neighbor(88, 2));
                        adjacency_list[89].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(89, 2));
                        adjacency_list[34].push_back(neighbor(50, 2));
                        adjacency_list[50].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(32, 2));
                        adjacency_list[32].push_back(neighbor(34, 2));
                        adjacency_list[32].push_back(neighbor(33, 2));
                        adjacency_list[33].push_back(neighbor(32, 2));
                        adjacency_list[33].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(33, 2));
                        adjacency_list[35].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(35, 2));
                        adjacency_list[35].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(35, 2));
                        adjacency_list[36].push_back(neighbor(68, 2));
                        adjacency_list[68].push_back(neighbor(36, 2));
                        adjacency_list[68].push_back(neighbor(69, 2));
                        adjacency_list[69].push_back(neighbor(68, 2));
                        adjacency_list[38].push_back(neighbor(91, 2));
                        adjacency_list[91].push_back(neighbor(38, 2));
                        adjacency_list[91].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(91, 2));
                        adjacency_list[32].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(32, 2));
                        adjacency_list[50].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(50, 2));
                        adjacency_list[31].push_back(neighbor(30, 2));
                        adjacency_list[30].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(70, 2));
                        adjacency_list[70].push_back(neighbor(31, 2));
                        adjacency_list[70].push_back(neighbor(71, 2));
                        adjacency_list[71].push_back(neighbor(70, 2));
                        adjacency_list[71].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(71, 2));
                        adjacency_list[30].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(30, 2));

                        //calculate and output information
                                std::vector<weight_t> min_distance;
                                std::vector<vertex_t> previous;
                                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                                std::cout << "Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                                std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                                tableOperation(path, 3, 0, i, 1);

                                if(i <= 1)
                                {

                                    cout << "Estimated time: " << to_string(comparisonArray3.size()) << "seconds." << endl;
                                    std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
                                    cout << oursTotheirs(comparisonArray3[0]).toStdString() << " ";
                                    int l = 1;
                                    for (; l<comparisonArray3.size(); l++)
                                    {
                                        while (comparisonArray3[l] == comparisonArray3[l-1] && (l < comparisonArray3.size()))
                                        {
                                            l++;
                                        }
                                         if(l < comparisonArray3.size())
                                     cout << oursTotheirs(comparisonArray3[l]).toStdString() << " ";
                                     }
                                     cout << endl;
                                }

                                //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                                std::cout << std::endl;
                                if (i==3){start = 16; end = 0;}
                                else if (i==2) {start = 0; end = oldEnd;}
                        }


        }}
    //if radio button un-checked then un-greyout line 3
    else
    {
        ui->trainselectBox3->setDisabled(false);
        ui->originBox3->setDisabled(false);
        ui->destBox3->setDisabled(false);
        ui->throttleBox3->setDisabled(false);
        ui->facingBox3->setDisabled(false);
        //ui->headingBox3->setDisabled(false);
        ui->trainimageButton3->hide();
        ui->routeInfo3->clear();
        ui->routeInfo3->hide();
    }
}

//lock in and calculate route for line 4
void MainWindow::greyOut4()
{
    new Q_DebugStream(std::cout, ui->routeInfo4); //Redirect Console output to textBrowser2
   // Q_DebugStream::registerQDebugMessageHandler(); //Redirect qDebug() output to textBrowser2

    //Determine which image to display
    if(ui->trainselectBox4->currentIndex() == 1)
    {
        ui->trainimageButton4->setIcon(*engine4Icon);
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
        ui->trainimageButton4->setIcon(*engine1Icon);
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
    int oldEnd;
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
    else if (ui->originBox4->currentIndex() == 8)
        start = 95;
    else if (ui->originBox4->currentIndex() == 9)
        start = 94;
    else
        start = theirsToours(ui->originBox4->currentText());
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
    else if (ui->destBox4->currentIndex() == 8)
        end = 57;
    else if (ui->destBox4->currentIndex() == 9)
        end = 60;
    else
        end = theirsToours(ui->destBox4->currentText());
    //grey out line 4 if radio button selected
    if (ui->setButton4->isChecked()== true)
    {
        ui->trainselectBox4->setDisabled(true);
        ui->originBox4->setDisabled(true);
        ui->destBox4->setDisabled(true);
        ui->throttleBox4->setDisabled(true);
        ui->facingBox4->setDisabled(true);
        //ui->headingBox4->setDisabled(true);
        ui->setButton4->setDisabled(true);
        ui->trainimageButton4->show();
        ui->routeInfo4->show();

        if (ui->originBox4->currentIndex() != 0)
         {

             if ((ui->originBox4->currentIndex() == ui->originBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
             {
                 cout << "Trains can not begin concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->originBox4->currentIndex() == ui->originBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
             {
                 cout << "Trains can not begin concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->originBox4->currentIndex() == ui->originBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
             {
                 cout << "Trains can not begin concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->originBox4->currentIndex() == ui->originBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
             {
                 cout << "Trains can not begin concurent paths at the same node." << endl;
                 return;
             }
         }

         if (ui->destBox4->currentIndex() != 0)
         {

             if ((ui->destBox4->currentIndex() == ui->destBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
             {
                 cout << "Trains can not end concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->destBox4->currentIndex() == ui->destBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
             {
                 cout << "Trains can not end concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->destBox4->currentIndex() == ui->destBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
             {
                 cout << "Trains can not end concurent paths at the same node." << endl;
                 return;
             }
             else if ((ui->destBox4->currentIndex() == ui->destBox5->currentIndex()) && (ui->buttonGroup_5->checkedButton() != 0))
             {
                 cout << "Trains can not end concurent paths at the same node." << endl;
                 return;
             }
         }

                if (ui->trainselectBox4->currentIndex() == 0 || ui->originBox4->currentIndex() == 0 || ui->destBox4->currentIndex() == 0 || ui->facingBox4->currentIndex() == 0 || ui->throttleBox4->currentIndex() == 0)
        {
            cout << "Route info incomplete. Can not compute route with information given. Please fill out all values before attempting to start planning a route." << endl;
            ui->buttonGroup_4->setExclusive(false);
            ui->delayButton4->setChecked(false);
            ui->setButton4->setChecked(false);
            ui->buttonGroup_4->setExclusive(true);
            return;
            }

                if((!(start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)) != (!(end== 95 || end == 59 || end == 61 || end == 93 || end == 62 || end == 99 || end == 100 || end == 60 || end == 94 || end == 58 || end == 56 || end == 96 || end == 65 || end == 101 || end == 102 || end == 57)))
                {
                    cout << "Invalid path. Attempting to move between two non-connected tracks. " << endl;
                    ui->buttonGroup_4->setExclusive(false);
                    ui->delayButton4->setChecked(false);
                    ui->setButton4->setChecked(false);
                    ui->buttonGroup_4->setExclusive(true);
                    return;
                }

                if (start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)
                {
                    pathcomplete4 = 1;
                }
                if(start == -1)
                {
                    cout << "Start node is invalid." << endl;
                    ui->buttonGroup_4->setExclusive(false);
                    ui->delayButton4->setChecked(false);
                    ui->setButton4->setChecked(false);
                    ui->buttonGroup_4->setExclusive(true);
                            return;
                }
                if (end == -1)
                {
                    cout << "Destination node is invalid" << endl;
                    ui->buttonGroup_4->setExclusive(false);
                    ui->delayButton4->setChecked(false);
                    ui->setButton4->setChecked(false);
                    ui->buttonGroup_4->setExclusive(true);
                    return;
                }
        
        int shortorLong =  longPathroute(start, end);

                if (shortorLong == 0)
                {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 4, 0, 0, 0);



                            cout << "Estimated time: " << to_string(comparisonArray4.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray4[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray4.size(); l++)
                            {
                                while (comparisonArray4[l] == comparisonArray4[l-1] && (l < comparisonArray4.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray4.size())
                             cout << oursTotheirs(comparisonArray4[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 3)
                {
                    adjacency_list_t adjacency_list(150);
                  adjacency_list[62].push_back(neighbor(63, 2));
  //                adjacency_list[63].push_back(neighbor(62, 2));
                  adjacency_list[63].push_back(neighbor(92, 2));
  //                adjacency_list[92].push_back(neighbor(63, 2));
                  adjacency_list[92].push_back(neighbor(64, 2));
                  adjacency_list[62].push_back(neighbor(99, 2));
                  adjacency_list[99].push_back(neighbor(100, 2));
                  adjacency_list[100].push_back(neighbor(60, 2));
  //                adjacency_list[64].push_back(neighbor(92, 2));
                  adjacency_list[64].push_back(neighbor(62, 2));
  //                adjacency_list[62].push_back(neighbor(64, 2));
  //                adjacency_list[62].push_back(neighbor(93, 2));
                  adjacency_list[93].push_back(neighbor(62, 2));
                  adjacency_list[61].push_back(neighbor(93, 2));
  //                adjacency_list[93].push_back(neighbor(61, 2));
  //                adjacency_list[60].push_back(neighbor(61, 2));
  //                adjacency_list[61].push_back(neighbor(60, 2));
  //                adjacency_list[61].push_back(neighbor(59, 2));
                  adjacency_list[59].push_back(neighbor(61, 2));
  //                adjacency_list[59].push_back(neighbor(95, 2));
                  adjacency_list[95].push_back(neighbor(59, 2));
  //                adjacency_list[94].push_back(neighbor(60, 2));
                  adjacency_list[60].push_back(neighbor(94, 2));
                  adjacency_list[94].push_back(neighbor(58, 2));
  //                adjacency_list[58].push_back(neighbor(94, 2));
                  adjacency_list[57].push_back(neighbor(95, 2));
  //                adjacency_list[95].push_back(neighbor(57, 2));
                  adjacency_list[102].push_back(neighbor(57, 2));
  //                adjacency_list[57].push_back(neighbor(56, 2));
  //                adjacency_list[56].push_back(neighbor(58, 2));
                  adjacency_list[58].push_back(neighbor(56, 2));
                  adjacency_list[56].push_back(neighbor(96, 2));
  //                adjacency_list[96].push_back(neighbor(56, 2));
  //                adjacency_list[65].push_back(neighbor(96, 2));
                  adjacency_list[96].push_back(neighbor(65, 2));
                  adjacency_list[65].push_back(neighbor(66, 2));
                  adjacency_list[65].push_back(neighbor(101, 2));
                  adjacency_list[101].push_back(neighbor(102, 2));
  //                adjacency_list[66].push_back(neighbor(65, 2));
  //                adjacency_list[65].push_back(neighbor(67, 2));
                  adjacency_list[67].push_back(neighbor(65, 2));
  //                adjacency_list[67].push_back(neighbor(97, 2));
                  adjacency_list[97].push_back(neighbor(67, 2));
  //                adjacency_list[97].push_back(neighbor(66, 2));
                  adjacency_list[66].push_back(neighbor(97, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        //std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 4, 0, 0, 0);



                            cout << "Estimated time: " << to_string(comparisonArray4.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray4[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray4.size(); l++)
                            {
                                while (comparisonArray4[l] == comparisonArray4[l-1] && (l < comparisonArray4.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray4.size())
                             cout << oursTotheirs(comparisonArray4[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 1)
                {
                    oldEnd = end;
                    end = 1;

                        for(int i=3; i!=0; i--)
                        {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                //calculate and output information
                        std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        //std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 4, 0, i, 1);


                        if(i <= 1)
                        {

                            cout << "Estimated time: " << to_string(comparisonArray4.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray4[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray4.size(); l++)
                            {
                                while (comparisonArray4[l] == comparisonArray4[l-1] && (l < comparisonArray4.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray4.size())
                             cout << oursTotheirs(comparisonArray4[l]).toStdString() << " ";
                             }
                             cout << endl;
                        }

                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;


                        if (i==3){start = 1; end = 2;}
                        else if (i==2) {start = 2; end = oldEnd;}
            }
        }
                    else if (shortorLong == 2)
                        {
                            oldEnd = end;
                            end = 16;

                                for(int i=3; i!=0; i--){
                        adjacency_list_t adjacency_list(100);
                        adjacency_list[43].push_back(neighbor(75, 2));
                        adjacency_list[75].push_back(neighbor(43, 2));
                        adjacency_list[75].push_back(neighbor(42, 2));
                        adjacency_list[42].push_back(neighbor(75, 2));
                        adjacency_list[42].push_back(neighbor(76, 2));
                        adjacency_list[76].push_back(neighbor(42, 2));
                        adjacency_list[76].push_back(neighbor(44, 2));
                        adjacency_list[44].push_back(neighbor(76, 2));
                        adjacency_list[42].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(42, 2));
                        adjacency_list[46].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(46, 2));
                        adjacency_list[46].push_back(neighbor(77, 2));
                        adjacency_list[77].push_back(neighbor(46, 2));
                        adjacency_list[77].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(77, 2));
                        adjacency_list[41].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(41, 2));
                        adjacency_list[45].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(45, 2));
                        adjacency_list[45].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(45, 2));
                        adjacency_list[47].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(47, 2));
                        adjacency_list[49].push_back(neighbor(47, 2));
                        adjacency_list[47].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(49, 2));
                        adjacency_list[40].push_back(neighbor(15, 2));
                        adjacency_list[15].push_back(neighbor(40, 2));
                        adjacency_list[15].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(15, 2));
                        adjacency_list[14].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(79, 2));
                        adjacency_list[79].push_back(neighbor(14, 2));
                        adjacency_list[79].push_back(neighbor(80, 2));
                        adjacency_list[80].push_back(neighbor(79, 2));
                        adjacency_list[80].push_back(neighbor(8, 2));
                        adjacency_list[8].push_back(neighbor(80, 2));
                        adjacency_list[8].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(8, 2));
                        adjacency_list[7].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(5, 2));
                        adjacency_list[5].push_back(neighbor(7, 2));
                        adjacency_list[5].push_back(neighbor(3, 2));
                        adjacency_list[3].push_back(neighbor(5, 2));
                        //adjacency_list[5].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(5, 2));
                        adjacency_list[3].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(3, 2));
                        adjacency_list[1].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(1, 2));
                        adjacency_list[1].push_back(neighbor(0, 2));
                        adjacency_list[0].push_back(neighbor(1, 2));
                        adjacency_list[0].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(0, 2));
                        adjacency_list[2].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(2, 2));
                        adjacency_list[4].push_back(neighbor(6, 2));
                        adjacency_list[6].push_back(neighbor(4, 2));
                        //adjacency_list[6].push_back(neighbor(19, 2));
                        adjacency_list[19].push_back(neighbor(6, 2));
                        adjacency_list[19].push_back(neighbor(17, 2));
                        adjacency_list[17].push_back(neighbor(19, 2));
                        adjacency_list[17].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(17, 2));
                        adjacency_list[16].push_back(neighbor(82, 2));
                        adjacency_list[82].push_back(neighbor(16, 2));
                        adjacency_list[82].push_back(neighbor(12, 2));
                        adjacency_list[12].push_back(neighbor(82, 2));
                        adjacency_list[12].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(12, 2));
                        adjacency_list[11].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(11, 2));
                        adjacency_list[11].push_back(neighbor(13, 2));
                        adjacency_list[13].push_back(neighbor(11, 2));
                        adjacency_list[10].push_back(neighbor(98, 2));
                        adjacency_list[98].push_back(neighbor(10, 2));
                        adjacency_list[98].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(98, 2));
                        adjacency_list[17].push_back(neighbor(18, 2));
                        adjacency_list[18].push_back(neighbor(17, 2));
                        adjacency_list[18].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(18, 2));
                        adjacency_list[20].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(21, 2));
                        adjacency_list[21].push_back(neighbor(20, 2));
                        adjacency_list[21].push_back(neighbor(83, 2));
                        adjacency_list[83].push_back(neighbor(21, 2));
                        adjacency_list[83].push_back(neighbor(84, 2));
                        adjacency_list[84].push_back(neighbor(83, 2));
                        adjacency_list[84].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(84, 2));
                        adjacency_list[29].push_back(neighbor(28, 2));
                        adjacency_list[28].push_back(neighbor(29, 2));
                        adjacency_list[28].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(28, 2));
                        adjacency_list[26].push_back(neighbor(51, 2));
                        adjacency_list[51].push_back(neighbor(26, 2));
                        adjacency_list[51].push_back(neighbor(52, 2));
                        adjacency_list[52].push_back(neighbor(51, 2));
                        adjacency_list[52].push_back(neighbor(85, 2));
                        adjacency_list[85].push_back(neighbor(52, 2));
                        adjacency_list[85].push_back(neighbor(53, 2));
                        adjacency_list[53].push_back(neighbor(85, 2));
                        adjacency_list[52].push_back(neighbor(86, 2));
                        adjacency_list[86].push_back(neighbor(52, 2));
                        adjacency_list[86].push_back(neighbor(54, 2));
                        adjacency_list[54].push_back(neighbor(86, 2));
                        adjacency_list[51].push_back(neighbor(87, 2));
                        adjacency_list[87].push_back(neighbor(51, 2));
                        adjacency_list[87].push_back(neighbor(55, 2));
                        adjacency_list[55].push_back(neighbor(87, 2));
                        adjacency_list[22].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(24, 2));
                        adjacency_list[24].push_back(neighbor(22, 2));
                        adjacency_list[24].push_back(neighbor(25, 2));
                        adjacency_list[25].push_back(neighbor(24, 2));
                        adjacency_list[25].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(25, 2));
                        adjacency_list[27].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(27, 2));
                        adjacency_list[28].push_back(neighbor(27, 2));
                        adjacency_list[27].push_back(neighbor(28, 2));
                        adjacency_list[0].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(0, 2));
                        adjacency_list[39].push_back(neighbor(38, 2));
                        adjacency_list[38].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(39, 2));
                        adjacency_list[88].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(88, 2));
                        adjacency_list[88].push_back(neighbor(89, 2));
                        adjacency_list[89].push_back(neighbor(88, 2));
                        adjacency_list[89].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(89, 2));
                        adjacency_list[34].push_back(neighbor(50, 2));
                        adjacency_list[50].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(32, 2));
                        adjacency_list[32].push_back(neighbor(34, 2));
                        adjacency_list[32].push_back(neighbor(33, 2));
                        adjacency_list[33].push_back(neighbor(32, 2));
                        adjacency_list[33].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(33, 2));
                        adjacency_list[35].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(35, 2));
                        adjacency_list[35].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(35, 2));
                        adjacency_list[36].push_back(neighbor(68, 2));
                        adjacency_list[68].push_back(neighbor(36, 2));
                        adjacency_list[68].push_back(neighbor(69, 2));
                        adjacency_list[69].push_back(neighbor(68, 2));
                        adjacency_list[38].push_back(neighbor(91, 2));
                        adjacency_list[91].push_back(neighbor(38, 2));
                        adjacency_list[91].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(91, 2));
                        adjacency_list[32].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(32, 2));
                        adjacency_list[50].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(50, 2));
                        adjacency_list[31].push_back(neighbor(30, 2));
                        adjacency_list[30].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(70, 2));
                        adjacency_list[70].push_back(neighbor(31, 2));
                        adjacency_list[70].push_back(neighbor(71, 2));
                        adjacency_list[71].push_back(neighbor(70, 2));
                        adjacency_list[71].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(71, 2));
                        adjacency_list[30].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(30, 2));

                        //calculate and output information
                                std::vector<weight_t> min_distance;
                                std::vector<vertex_t> previous;
                                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                                std::cout << "Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                                //std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                                tableOperation(path, 4, 0, i, 1);

                                if(i <= 1)
                                {

                                    cout << "Estimated time: " << to_string(comparisonArray4.size()) << "seconds." << endl;
                                    std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
                                    cout << oursTotheirs(comparisonArray4[0]).toStdString() << " ";
                                    int l = 1;
                                    for (; l<comparisonArray4.size(); l++)
                                    {
                                        while (comparisonArray4[l] == comparisonArray4[l-1] && (l < comparisonArray4.size()))
                                        {
                                            l++;
                                        }
                                         if(l < comparisonArray4.size())
                                     cout << oursTotheirs(comparisonArray4[l]).toStdString() << " ";
                                     }
                                     cout << endl;
                                }

                                //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                                std::cout << std::endl;
                                if (i==3){start = 16; end = 0;}
                                else if (i==2) {start = 0; end = oldEnd;}
                        }


        }}
    //if radio button un-checked then un-greyout line 4
    else
    {
        ui->trainselectBox4->setDisabled(false);
        ui->originBox4->setDisabled(false);
        ui->destBox4->setDisabled(false);
        ui->throttleBox4->setDisabled(false);
        ui->facingBox4->setDisabled(false);
        //ui->headingBox4->setDisabled(false);
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
        ui->trainimageButton5->setIcon(*engine5Icon);
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
        ui->trainimageButton5->setIcon(*engine1Icon);
        ui->trainimageButton5->setIconSize(QSize(85,85));
    }
    else
        ui->trainimageButton5->hide();

    int start;
    int end;
    int oldEnd;
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
    else if (ui->originBox5->currentIndex() == 8)
        start = 95;
    else if (ui->originBox5->currentIndex() == 9)
        start = 94;
    else
        start = theirsToours(ui->originBox5->currentText());
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
    else if (ui->destBox5->currentIndex() == 8)
        end = 57;
    else if (ui->destBox5->currentIndex() == 9)
        end = 60;
    else
        end = theirsToours(ui->destBox5->currentText());
    //grey out line 5 if radio button selected
    if (ui->setButton5->isChecked()== true)
    {
        ui->trainselectBox5->setDisabled(true);
        ui->originBox5->setDisabled(true);
        ui->destBox5->setDisabled(true);
        ui->throttleBox5->setDisabled(true);
        ui->facingBox5->setDisabled(true);
        //ui->headingBox5->setDisabled(true);
        ui->setButton5->setDisabled(true);
        ui->trainimageButton5->show();
        ui->routeInfo5->show();

        if (ui->originBox5->currentIndex() != 0)
        {

            if ((ui->originBox5->currentIndex() == ui->originBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox5->currentIndex() == ui->originBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox5->currentIndex() == ui->originBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->originBox5->currentIndex() == ui->originBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not begin concurent paths at the same node." << endl;
                return;
            }
        }

        if (ui->destBox5->currentIndex() != 0)
        {

            if ((ui->destBox5->currentIndex() == ui->destBox1->currentIndex()) && (ui->buttonGroup->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox5->currentIndex() == ui->destBox2->currentIndex()) && (ui->buttonGroup_2->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox5->currentIndex() == ui->destBox3->currentIndex()) && (ui->buttonGroup_3->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
            else if ((ui->destBox5->currentIndex() == ui->destBox4->currentIndex()) && (ui->buttonGroup_4->checkedButton() != 0))
            {
                cout << "Trains can not end concurent paths at the same node." << endl;
                return;
            }
        }
        
                if (ui->trainselectBox5->currentIndex() == 0 || ui->originBox5->currentIndex() == 0 || ui->destBox5->currentIndex() == 0 || ui->facingBox5->currentIndex() == 0 || ui->throttleBox5->currentIndex() == 0)
        {
            cout << "Route info incomplete. Cannot compute route with information given. Please fill out all values before attempting to start planning a route." << endl;
            ui->buttonGroup_5->setExclusive(false);
            ui->delayButton5->setChecked(false);
            ui->setButton5->setChecked(false);
            ui->buttonGroup_5->setExclusive(true);
            return;
            }

                if((!(start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)) != (!(end== 95 || end == 59 || end == 61 || end == 93 || end == 62 || end == 99 || end == 100 || end == 60 || end == 94 || end == 58 || end == 56 || end == 96 || end == 65 || end == 101 || end == 102 || end == 57)))
                {
                    cout << "Invalid path. Attempting to move between two non-connected tracks. " << endl;
                    ui->buttonGroup_5->setExclusive(false);
                    ui->delayButton5->setChecked(false);
                    ui->setButton5->setChecked(false);
                    ui->buttonGroup_5->setExclusive(true);
                    return;
                }

                if (start == 95 || start == 59 || start == 61 || start == 93 || start == 62 || start == 99 || start == 100 || start == 60 || start == 94 || start == 58 || start == 56 || start == 96 || start == 65 || start == 101 || start == 102 || start == 57)
                {
                    pathcomplete5 = 1;
                }
                if(start == -1)
                {
                    cout << "Start node is invalid." << endl;
                    ui->buttonGroup_5->setExclusive(false);
                    ui->delayButton5->setChecked(false);
                    ui->setButton5->setChecked(false);
                    ui->buttonGroup_5->setExclusive(true);
                            return;
                }
                if (end == -1)
                {
                    cout << "Destination node is invalid" << endl;
                    ui->buttonGroup_5->setExclusive(false);
                    ui->delayButton5->setChecked(false);
                    ui->setButton5->setChecked(false);
                    ui->buttonGroup_5->setExclusive(true);
                    return;
                }


        int shortorLong =  longPathroute(start, end);

                if (shortorLong == 0)
                {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                       // std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 5, 0, 0, 0);




                            cout << "Estimated time: " << to_string(comparisonArray5.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray5[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray5.size(); l++)
                            {
                                while (comparisonArray5[l] == comparisonArray5[l-1] && (l < comparisonArray5.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray5.size())
                             cout << oursTotheirs(comparisonArray5[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 3)
                {
                    adjacency_list_t adjacency_list(150);
                  adjacency_list[62].push_back(neighbor(63, 2));
  //                adjacency_list[63].push_back(neighbor(62, 2));
                  adjacency_list[63].push_back(neighbor(92, 2));
  //                adjacency_list[92].push_back(neighbor(63, 2));
                  adjacency_list[92].push_back(neighbor(64, 2));
                  adjacency_list[62].push_back(neighbor(99, 2));
                  adjacency_list[99].push_back(neighbor(100, 2));
                  adjacency_list[100].push_back(neighbor(60, 2));
  //                adjacency_list[64].push_back(neighbor(92, 2));
                  adjacency_list[64].push_back(neighbor(62, 2));
  //                adjacency_list[62].push_back(neighbor(64, 2));
  //                adjacency_list[62].push_back(neighbor(93, 2));
                  adjacency_list[93].push_back(neighbor(62, 2));
                  adjacency_list[61].push_back(neighbor(93, 2));
  //                adjacency_list[93].push_back(neighbor(61, 2));
  //                adjacency_list[60].push_back(neighbor(61, 2));
  //                adjacency_list[61].push_back(neighbor(60, 2));
  //                adjacency_list[61].push_back(neighbor(59, 2));
                  adjacency_list[59].push_back(neighbor(61, 2));
  //                adjacency_list[59].push_back(neighbor(95, 2));
                  adjacency_list[95].push_back(neighbor(59, 2));
  //                adjacency_list[94].push_back(neighbor(60, 2));
                  adjacency_list[60].push_back(neighbor(94, 2));
                  adjacency_list[94].push_back(neighbor(58, 2));
  //                adjacency_list[58].push_back(neighbor(94, 2));
                  adjacency_list[57].push_back(neighbor(95, 2));
  //                adjacency_list[95].push_back(neighbor(57, 2));
                  adjacency_list[102].push_back(neighbor(57, 2));
  //                adjacency_list[57].push_back(neighbor(56, 2));
  //                adjacency_list[56].push_back(neighbor(58, 2));
                  adjacency_list[58].push_back(neighbor(56, 2));
                  adjacency_list[56].push_back(neighbor(96, 2));
  //                adjacency_list[96].push_back(neighbor(56, 2));
  //                adjacency_list[65].push_back(neighbor(96, 2));
                  adjacency_list[96].push_back(neighbor(65, 2));
                  adjacency_list[65].push_back(neighbor(66, 2));
                  adjacency_list[65].push_back(neighbor(101, 2));
                  adjacency_list[101].push_back(neighbor(102, 2));
  //                adjacency_list[66].push_back(neighbor(65, 2));
  //                adjacency_list[65].push_back(neighbor(67, 2));
                  adjacency_list[67].push_back(neighbor(65, 2));
  //                adjacency_list[67].push_back(neighbor(97, 2));
                  adjacency_list[97].push_back(neighbor(67, 2));
  //                adjacency_list[97].push_back(neighbor(66, 2));
                  adjacency_list[66].push_back(neighbor(97, 2));

                std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                       // std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 5, 0, 0, 0);



                            cout << "Estimated time: " << to_string(comparisonArray5.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray5[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray5.size(); l++)
                            {
                                while (comparisonArray5[l] == comparisonArray5[l-1] && (l < comparisonArray5.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray5.size())
                             cout << oursTotheirs(comparisonArray5[l]).toStdString() << " ";
                             }
                             cout << endl;


                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;
            }
                else if (shortorLong == 1)
                {
                    oldEnd = end;
                    end = 1;

                        for(int i=3; i!=0; i--)
                        {
                adjacency_list_t adjacency_list(100);
                adjacency_list[43].push_back(neighbor(75, 2));
                adjacency_list[75].push_back(neighbor(43, 2));
                adjacency_list[75].push_back(neighbor(42, 2));
                adjacency_list[42].push_back(neighbor(75, 2));
                adjacency_list[42].push_back(neighbor(76, 2));
                adjacency_list[76].push_back(neighbor(42, 2));
                adjacency_list[76].push_back(neighbor(44, 2));
                adjacency_list[44].push_back(neighbor(76, 2));
                adjacency_list[42].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(42, 2));
                adjacency_list[46].push_back(neighbor(41, 2));
                adjacency_list[41].push_back(neighbor(46, 2));
                adjacency_list[46].push_back(neighbor(77, 2));
                adjacency_list[77].push_back(neighbor(46, 2));
                adjacency_list[77].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(77, 2));
                adjacency_list[41].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(41, 2));
                adjacency_list[45].push_back(neighbor(40, 2));
                adjacency_list[40].push_back(neighbor(45, 2));
                adjacency_list[45].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(45, 2));
                adjacency_list[47].push_back(neighbor(78, 2));
                adjacency_list[78].push_back(neighbor(47, 2));
                adjacency_list[49].push_back(neighbor(47, 2));
                adjacency_list[47].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(48, 2));
                adjacency_list[48].push_back(neighbor(49, 2));
                adjacency_list[40].push_back(neighbor(15, 2));
                adjacency_list[15].push_back(neighbor(40, 2));
                adjacency_list[15].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(15, 2));
                adjacency_list[14].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(14, 2));
                adjacency_list[14].push_back(neighbor(79, 2));
                adjacency_list[79].push_back(neighbor(14, 2));
                adjacency_list[79].push_back(neighbor(80, 2));
                adjacency_list[80].push_back(neighbor(79, 2));
                adjacency_list[80].push_back(neighbor(8, 2));
                adjacency_list[8].push_back(neighbor(80, 2));
                adjacency_list[8].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(8, 2));
                adjacency_list[7].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(7, 2));
                adjacency_list[7].push_back(neighbor(5, 2));
                adjacency_list[5].push_back(neighbor(7, 2));
                adjacency_list[5].push_back(neighbor(3, 2));
                adjacency_list[3].push_back(neighbor(5, 2));
                //adjacency_list[5].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(5, 2));
                adjacency_list[3].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(3, 2));
                adjacency_list[1].push_back(neighbor(81, 2));
                adjacency_list[81].push_back(neighbor(1, 2));
                adjacency_list[1].push_back(neighbor(0, 2));
                adjacency_list[0].push_back(neighbor(1, 2));
                adjacency_list[0].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(0, 2));
                adjacency_list[2].push_back(neighbor(4, 2));
                adjacency_list[4].push_back(neighbor(2, 2));
                adjacency_list[4].push_back(neighbor(6, 2));
                adjacency_list[6].push_back(neighbor(4, 2));
                //adjacency_list[6].push_back(neighbor(19, 2));
                adjacency_list[19].push_back(neighbor(6, 2));
                adjacency_list[19].push_back(neighbor(17, 2));
                adjacency_list[17].push_back(neighbor(19, 2));
                adjacency_list[17].push_back(neighbor(16, 2));
                adjacency_list[16].push_back(neighbor(17, 2));
                adjacency_list[16].push_back(neighbor(82, 2));
                adjacency_list[82].push_back(neighbor(16, 2));
                adjacency_list[82].push_back(neighbor(12, 2));
                adjacency_list[12].push_back(neighbor(82, 2));
                adjacency_list[12].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(12, 2));
                adjacency_list[11].push_back(neighbor(10, 2));
                adjacency_list[10].push_back(neighbor(11, 2));
                adjacency_list[11].push_back(neighbor(13, 2));
                adjacency_list[13].push_back(neighbor(11, 2));
                adjacency_list[10].push_back(neighbor(98, 2));
                adjacency_list[98].push_back(neighbor(10, 2));
                adjacency_list[98].push_back(neighbor(9, 2));
                adjacency_list[9].push_back(neighbor(98, 2));
                adjacency_list[17].push_back(neighbor(18, 2));
                adjacency_list[18].push_back(neighbor(17, 2));
                adjacency_list[18].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(18, 2));
                adjacency_list[20].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(20, 2));
                adjacency_list[20].push_back(neighbor(21, 2));
                adjacency_list[21].push_back(neighbor(20, 2));
                adjacency_list[21].push_back(neighbor(83, 2));
                adjacency_list[83].push_back(neighbor(21, 2));
                adjacency_list[83].push_back(neighbor(84, 2));
                adjacency_list[84].push_back(neighbor(83, 2));
                adjacency_list[84].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(84, 2));
                adjacency_list[29].push_back(neighbor(28, 2));
                adjacency_list[28].push_back(neighbor(29, 2));
                adjacency_list[28].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(28, 2));
                adjacency_list[26].push_back(neighbor(51, 2));
                adjacency_list[51].push_back(neighbor(26, 2));
                adjacency_list[51].push_back(neighbor(52, 2));
                adjacency_list[52].push_back(neighbor(51, 2));
                adjacency_list[52].push_back(neighbor(85, 2));
                adjacency_list[85].push_back(neighbor(52, 2));
                adjacency_list[85].push_back(neighbor(53, 2));
                adjacency_list[53].push_back(neighbor(85, 2));
                adjacency_list[52].push_back(neighbor(86, 2));
                adjacency_list[86].push_back(neighbor(52, 2));
                adjacency_list[86].push_back(neighbor(54, 2));
                adjacency_list[54].push_back(neighbor(86, 2));
                adjacency_list[51].push_back(neighbor(87, 2));
                adjacency_list[87].push_back(neighbor(51, 2));
                adjacency_list[87].push_back(neighbor(55, 2));
                adjacency_list[55].push_back(neighbor(87, 2));
                adjacency_list[22].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(22, 2));
                adjacency_list[22].push_back(neighbor(24, 2));
                adjacency_list[24].push_back(neighbor(22, 2));
                adjacency_list[24].push_back(neighbor(25, 2));
                adjacency_list[25].push_back(neighbor(24, 2));
                adjacency_list[25].push_back(neighbor(26, 2));
                adjacency_list[26].push_back(neighbor(25, 2));
                adjacency_list[27].push_back(neighbor(23, 2));
                adjacency_list[23].push_back(neighbor(27, 2));
                adjacency_list[28].push_back(neighbor(27, 2));
                adjacency_list[27].push_back(neighbor(28, 2));
                adjacency_list[0].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(0, 2));
                adjacency_list[39].push_back(neighbor(38, 2));
                adjacency_list[38].push_back(neighbor(39, 2));
                adjacency_list[39].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(39, 2));
                adjacency_list[88].push_back(neighbor(37, 2));
                adjacency_list[37].push_back(neighbor(88, 2));
                adjacency_list[88].push_back(neighbor(89, 2));
                adjacency_list[89].push_back(neighbor(88, 2));
                adjacency_list[89].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(89, 2));
                adjacency_list[34].push_back(neighbor(50, 2));
                adjacency_list[50].push_back(neighbor(34, 2));
                adjacency_list[34].push_back(neighbor(32, 2));
                adjacency_list[32].push_back(neighbor(34, 2));
                adjacency_list[32].push_back(neighbor(33, 2));
                adjacency_list[33].push_back(neighbor(32, 2));
                adjacency_list[33].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(33, 2));
                adjacency_list[35].push_back(neighbor(90, 2));
                adjacency_list[90].push_back(neighbor(35, 2));
                adjacency_list[35].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(35, 2));
                adjacency_list[36].push_back(neighbor(68, 2));
                adjacency_list[68].push_back(neighbor(36, 2));
                adjacency_list[68].push_back(neighbor(69, 2));
                adjacency_list[69].push_back(neighbor(68, 2));
                adjacency_list[38].push_back(neighbor(91, 2));
                adjacency_list[91].push_back(neighbor(38, 2));
                adjacency_list[91].push_back(neighbor(36, 2));
                adjacency_list[36].push_back(neighbor(91, 2));
                adjacency_list[32].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(32, 2));
                adjacency_list[50].push_back(neighbor(49, 2));
                adjacency_list[49].push_back(neighbor(50, 2));
                adjacency_list[31].push_back(neighbor(30, 2));
                adjacency_list[30].push_back(neighbor(31, 2));
                adjacency_list[31].push_back(neighbor(70, 2));
                adjacency_list[70].push_back(neighbor(31, 2));
                adjacency_list[70].push_back(neighbor(71, 2));
                adjacency_list[71].push_back(neighbor(70, 2));
                adjacency_list[71].push_back(neighbor(2, 2));
                adjacency_list[2].push_back(neighbor(71, 2));
                adjacency_list[30].push_back(neighbor(29, 2));
                adjacency_list[29].push_back(neighbor(30, 2));

                //calculate and output information
                        std::vector<weight_t> min_distance;
                        std::vector<vertex_t> previous;
                        DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                        std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                        std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                        //std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                        tableOperation(path, 5, 0, i, 1);


                        if(i <= 1)
                        {

                            cout << "Estimated time: " << to_string(comparisonArray5.size()) << "seconds." << endl;
                            std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                            cout << oursTotheirs(comparisonArray5[0]).toStdString() << " ";
                            int l = 1;
                            for (; l<comparisonArray5.size(); l++)
                            {
                                while (comparisonArray5[l] == comparisonArray5[l-1] && (l < comparisonArray5.size()))
                                {
                                    l++;
                                }
                                 if(l < comparisonArray5.size())
                             cout << oursTotheirs(comparisonArray5[l]).toStdString() << " ";
                             }
                             cout << endl;
                        }

                        //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                        std::cout << std::endl;


                        if (i==3){start = 1; end = 2;}
                        else if (i==2) {start = 2; end = oldEnd;}
            }
        }
                    else if (shortorLong == 2)
                        {
                            oldEnd = end;
                            end = 16;

                                for(int i=3; i!=0; i--){
                        adjacency_list_t adjacency_list(100);
                        adjacency_list[43].push_back(neighbor(75, 2));
                        adjacency_list[75].push_back(neighbor(43, 2));
                        adjacency_list[75].push_back(neighbor(42, 2));
                        adjacency_list[42].push_back(neighbor(75, 2));
                        adjacency_list[42].push_back(neighbor(76, 2));
                        adjacency_list[76].push_back(neighbor(42, 2));
                        adjacency_list[76].push_back(neighbor(44, 2));
                        adjacency_list[44].push_back(neighbor(76, 2));
                        adjacency_list[42].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(42, 2));
                        adjacency_list[46].push_back(neighbor(41, 2));
                        adjacency_list[41].push_back(neighbor(46, 2));
                        adjacency_list[46].push_back(neighbor(77, 2));
                        adjacency_list[77].push_back(neighbor(46, 2));
                        adjacency_list[77].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(77, 2));
                        adjacency_list[41].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(41, 2));
                        adjacency_list[45].push_back(neighbor(40, 2));
                        adjacency_list[40].push_back(neighbor(45, 2));
                        adjacency_list[45].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(45, 2));
                        adjacency_list[47].push_back(neighbor(78, 2));
                        adjacency_list[78].push_back(neighbor(47, 2));
                        adjacency_list[49].push_back(neighbor(47, 2));
                        adjacency_list[47].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(48, 2));
                        adjacency_list[48].push_back(neighbor(49, 2));
                        adjacency_list[40].push_back(neighbor(15, 2));
                        adjacency_list[15].push_back(neighbor(40, 2));
                        adjacency_list[15].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(15, 2));
                        adjacency_list[14].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(14, 2));
                        adjacency_list[14].push_back(neighbor(79, 2));
                        adjacency_list[79].push_back(neighbor(14, 2));
                        adjacency_list[79].push_back(neighbor(80, 2));
                        adjacency_list[80].push_back(neighbor(79, 2));
                        adjacency_list[80].push_back(neighbor(8, 2));
                        adjacency_list[8].push_back(neighbor(80, 2));
                        adjacency_list[8].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(8, 2));
                        adjacency_list[7].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(7, 2));
                        adjacency_list[7].push_back(neighbor(5, 2));
                        adjacency_list[5].push_back(neighbor(7, 2));
                        adjacency_list[5].push_back(neighbor(3, 2));
                        adjacency_list[3].push_back(neighbor(5, 2));
                        //adjacency_list[5].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(5, 2));
                        adjacency_list[3].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(3, 2));
                        adjacency_list[1].push_back(neighbor(81, 2));
                        adjacency_list[81].push_back(neighbor(1, 2));
                        adjacency_list[1].push_back(neighbor(0, 2));
                        adjacency_list[0].push_back(neighbor(1, 2));
                        adjacency_list[0].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(0, 2));
                        adjacency_list[2].push_back(neighbor(4, 2));
                        adjacency_list[4].push_back(neighbor(2, 2));
                        adjacency_list[4].push_back(neighbor(6, 2));
                        adjacency_list[6].push_back(neighbor(4, 2));
                        //adjacency_list[6].push_back(neighbor(19, 2));
                        adjacency_list[19].push_back(neighbor(6, 2));
                        adjacency_list[19].push_back(neighbor(17, 2));
                        adjacency_list[17].push_back(neighbor(19, 2));
                        adjacency_list[17].push_back(neighbor(16, 2));
                        adjacency_list[16].push_back(neighbor(17, 2));
                        adjacency_list[16].push_back(neighbor(82, 2));
                        adjacency_list[82].push_back(neighbor(16, 2));
                        adjacency_list[82].push_back(neighbor(12, 2));
                        adjacency_list[12].push_back(neighbor(82, 2));
                        adjacency_list[12].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(12, 2));
                        adjacency_list[11].push_back(neighbor(10, 2));
                        adjacency_list[10].push_back(neighbor(11, 2));
                        adjacency_list[11].push_back(neighbor(13, 2));
                        adjacency_list[13].push_back(neighbor(11, 2));
                        adjacency_list[10].push_back(neighbor(98, 2));
                        adjacency_list[98].push_back(neighbor(10, 2));
                        adjacency_list[98].push_back(neighbor(9, 2));
                        adjacency_list[9].push_back(neighbor(98, 2));
                        adjacency_list[17].push_back(neighbor(18, 2));
                        adjacency_list[18].push_back(neighbor(17, 2));
                        adjacency_list[18].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(18, 2));
                        adjacency_list[20].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(20, 2));
                        adjacency_list[20].push_back(neighbor(21, 2));
                        adjacency_list[21].push_back(neighbor(20, 2));
                        adjacency_list[21].push_back(neighbor(83, 2));
                        adjacency_list[83].push_back(neighbor(21, 2));
                        adjacency_list[83].push_back(neighbor(84, 2));
                        adjacency_list[84].push_back(neighbor(83, 2));
                        adjacency_list[84].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(84, 2));
                        adjacency_list[29].push_back(neighbor(28, 2));
                        adjacency_list[28].push_back(neighbor(29, 2));
                        adjacency_list[28].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(28, 2));
                        adjacency_list[26].push_back(neighbor(51, 2));
                        adjacency_list[51].push_back(neighbor(26, 2));
                        adjacency_list[51].push_back(neighbor(52, 2));
                        adjacency_list[52].push_back(neighbor(51, 2));
                        adjacency_list[52].push_back(neighbor(85, 2));
                        adjacency_list[85].push_back(neighbor(52, 2));
                        adjacency_list[85].push_back(neighbor(53, 2));
                        adjacency_list[53].push_back(neighbor(85, 2));
                        adjacency_list[52].push_back(neighbor(86, 2));
                        adjacency_list[86].push_back(neighbor(52, 2));
                        adjacency_list[86].push_back(neighbor(54, 2));
                        adjacency_list[54].push_back(neighbor(86, 2));
                        adjacency_list[51].push_back(neighbor(87, 2));
                        adjacency_list[87].push_back(neighbor(51, 2));
                        adjacency_list[87].push_back(neighbor(55, 2));
                        adjacency_list[55].push_back(neighbor(87, 2));
                        adjacency_list[22].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(22, 2));
                        adjacency_list[22].push_back(neighbor(24, 2));
                        adjacency_list[24].push_back(neighbor(22, 2));
                        adjacency_list[24].push_back(neighbor(25, 2));
                        adjacency_list[25].push_back(neighbor(24, 2));
                        adjacency_list[25].push_back(neighbor(26, 2));
                        adjacency_list[26].push_back(neighbor(25, 2));
                        adjacency_list[27].push_back(neighbor(23, 2));
                        adjacency_list[23].push_back(neighbor(27, 2));
                        adjacency_list[28].push_back(neighbor(27, 2));
                        adjacency_list[27].push_back(neighbor(28, 2));
                        adjacency_list[0].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(0, 2));
                        adjacency_list[39].push_back(neighbor(38, 2));
                        adjacency_list[38].push_back(neighbor(39, 2));
                        adjacency_list[39].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(39, 2));
                        adjacency_list[88].push_back(neighbor(37, 2));
                        adjacency_list[37].push_back(neighbor(88, 2));
                        adjacency_list[88].push_back(neighbor(89, 2));
                        adjacency_list[89].push_back(neighbor(88, 2));
                        adjacency_list[89].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(89, 2));
                        adjacency_list[34].push_back(neighbor(50, 2));
                        adjacency_list[50].push_back(neighbor(34, 2));
                        adjacency_list[34].push_back(neighbor(32, 2));
                        adjacency_list[32].push_back(neighbor(34, 2));
                        adjacency_list[32].push_back(neighbor(33, 2));
                        adjacency_list[33].push_back(neighbor(32, 2));
                        adjacency_list[33].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(33, 2));
                        adjacency_list[35].push_back(neighbor(90, 2));
                        adjacency_list[90].push_back(neighbor(35, 2));
                        adjacency_list[35].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(35, 2));
                        adjacency_list[36].push_back(neighbor(68, 2));
                        adjacency_list[68].push_back(neighbor(36, 2));
                        adjacency_list[68].push_back(neighbor(69, 2));
                        adjacency_list[69].push_back(neighbor(68, 2));
                        adjacency_list[38].push_back(neighbor(91, 2));
                        adjacency_list[91].push_back(neighbor(38, 2));
                        adjacency_list[91].push_back(neighbor(36, 2));
                        adjacency_list[36].push_back(neighbor(91, 2));
                        adjacency_list[32].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(32, 2));
                        adjacency_list[50].push_back(neighbor(49, 2));
                        adjacency_list[49].push_back(neighbor(50, 2));
                        adjacency_list[31].push_back(neighbor(30, 2));
                        adjacency_list[30].push_back(neighbor(31, 2));
                        adjacency_list[31].push_back(neighbor(70, 2));
                        adjacency_list[70].push_back(neighbor(31, 2));
                        adjacency_list[70].push_back(neighbor(71, 2));
                        adjacency_list[71].push_back(neighbor(70, 2));
                        adjacency_list[71].push_back(neighbor(2, 2));
                        adjacency_list[2].push_back(neighbor(71, 2));
                        adjacency_list[30].push_back(neighbor(29, 2));
                        adjacency_list[29].push_back(neighbor(30, 2));

                        //calculate and output information
                                std::vector<weight_t> min_distance;
                                std::vector<vertex_t> previous;
                                DijkstraComputePaths(start, adjacency_list, min_distance, previous);
                                std::cout << "Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
                                std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
                               // std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                                tableOperation(path, 5, 0, i, 1);


                                if(i <= 1)
                                {

                                    cout << "Estimated time: " << to_string(comparisonArray5.size()) << "seconds." << endl;
                                    std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
                                    cout << oursTotheirs(comparisonArray5[0]).toStdString() << " ";
                                    int l = 1;
                                    for (; l<comparisonArray5.size(); l++)
                                    {
                                        while (comparisonArray5[l] == comparisonArray5[l-1] && (l < comparisonArray5.size()))
                                        {
                                            l++;
                                        }
                                         if(l < comparisonArray5.size())
                                     cout << oursTotheirs(comparisonArray5[l]).toStdString() << " ";
                                     }
                                     cout << endl;
                                }

                                //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
                                std::cout << std::endl;
                                if (i==3){start = 16; end = 0;}
                                else if (i==2) {start = 0; end = oldEnd;}
                        }


        }}
    //if radio button un-checked then un-greyout line 5
    else
    {
        ui->trainselectBox5->setDisabled(false);
        ui->originBox5->setDisabled(false);
        ui->destBox5->setDisabled(false);
        ui->throttleBox5->setDisabled(false);
        ui->facingBox5->setDisabled(false);
        //ui->headingBox5->setDisabled(false);
        ui->trainimageButton5->hide();
        ui->routeInfo5->clear();
        ui->routeInfo5->hide();
    }
}

void MainWindow::timerEvent(QTimerEvent * ev)
{

    if (controlBit == 0) //If we are in control of the train.
    {
        if(ev->timerId() != m_timer.timerId())
        {
            QMainWindow::timerEvent(ev);
            return;
        }

        if(comparisonArray.size() != 0)
        {
           // pathcomplete1 = 1;
              int pause1 = 0;

            new Q_DebugStream(std::cout, ui->routeInfo1); //Redirect Console output to textBrowser2
            if(fakeoccupancy == 1) //this is where the occupancy data check code is in the final version. Currently simulated by a global variable. This call would be asking "Is the appropriate segment occupied?"
            {
                if (occupyitt < 3)
                {
                    cout << "Engine 1 one track segment behind. Pausing route until it catches up." << endl;
                occupyitt++;
                pause1 = 1;
                }
                else if (occupyitt >= 3 && ready != 0)
                {
                    cout << "Engine 1 stalled for over 3 seconds. Temporarily stopping all other active trains until issue is resolved." << endl;
                    pause1 = 1;
                    ready = 0;
                    occupyitt++;

                }
                else
                    pause1 = 1;




            }
            if(fakeoccupancy == 0 && ready == 0)
            {
                cout << "Engine 1 caught up. Resuming all routes." << endl;
                ready = 1;
            }

            if(pause1 != 1)
           {
                if(comparisonArray[0] == comparisonArray[1]) //if the train has moved, but is still at the same segment
            {
                cout << "Engine 1 still at track segment " << oursTotheirs(comparisonArray[0]).toStdString() << endl;
            }
            else
            {
                QString str = QString::fromStdString(to_string(comparisonArray[0]));
                QString str2 = QString::fromStdString(to_string(comparisonArray[1]));
                checkSwitches(str, str2);
                cout << "Engine 1 moved to track segment " << oursTotheirs(comparisonArray[1]).toStdString() << endl;
            }
            comparisonArray.erase(comparisonArray.begin()); }
            qApp->processEvents();

        }
        if(comparisonArray2.size() != 0)
        {



            //pathcomplete2 = 1;
            new Q_DebugStream(std::cout, ui->routeInfo2);
            if (ready == 0)
                cout << "All engines stopped to avoid collisions" << endl;
            else
            {
            if(comparisonArray2[0] == comparisonArray2[1])
            {
                cout << "Engine 2 still at track segment " << oursTotheirs(comparisonArray2[0]).toStdString() << endl;
            }
            else
            {
                QString str = QString::fromStdString(to_string(comparisonArray2[0]));
                QString str2 = QString::fromStdString(to_string(comparisonArray2[1]));
                checkSwitches(str, str2);
                cout << "Engine 2 moved to track segment " << oursTotheirs(comparisonArray2[1]).toStdString() << endl;
            }
            comparisonArray2.erase(comparisonArray2.begin());
            qApp->processEvents();
        }}
        if(comparisonArray3.size() != 0)
        {
            //pathcomplete3 = 1;
            new Q_DebugStream(std::cout, ui->routeInfo3);
            if (ready == 0)
                cout << "All engines stopped to avoid collisions" << endl;
            else
            {
            if(comparisonArray3[0] == comparisonArray3[1])
            {
                cout << "Engine 3 still at track segment " << oursTotheirs(comparisonArray3[0]).toStdString() << endl;
            }
            else
            {
                QString str = QString::fromStdString(to_string(comparisonArray3[0]));
                QString str2 = QString::fromStdString(to_string(comparisonArray3[1]));
                checkSwitches(str, str2);
                cout << "Engine 3 moved to track segment " << oursTotheirs(comparisonArray3[1]).toStdString() << endl;
            }
            comparisonArray3.erase(comparisonArray3.begin());
            qApp->processEvents();
        }}
        if(comparisonArray4.size() != 0)
        {
           // pathcomplete4 = 1;
            new Q_DebugStream(std::cout, ui->routeInfo4);
            if (ready == 0)
                cout << "All engines stopped to avoid collisions" << endl;
            else
            {
            if(comparisonArray4[0] == comparisonArray4[1])
            {
                cout << "Engine 4 still at track segment " << oursTotheirs(comparisonArray4[0]).toStdString() << endl;
            }
            else
            {
                QString str = QString::fromStdString(to_string(comparisonArray4[0]));
                QString str2 = QString::fromStdString(to_string(comparisonArray4[1]));
                checkSwitches(str, str2);
                cout << " Engine 4 moved to track segment " << oursTotheirs(comparisonArray4[1]).toStdString() << endl;
            }
            comparisonArray4.erase(comparisonArray4.begin());
            qApp->processEvents();
        }}
        if(comparisonArray5.size() != 0)
        {
            //pathcomplete5 = 1;
            new Q_DebugStream(std::cout, ui->routeInfo5);
            if (ready == 0)
                cout << "All engines stopped to avoid collisions" << endl;
            else
            {
            if(comparisonArray5[0] == comparisonArray5[1])
            {
                cout << "Engine 5 still at track segment " << oursTotheirs(comparisonArray5[0]).toStdString() << endl;
            }
            else
            {
                QString str = QString::fromStdString(to_string(comparisonArray5[0]));
                QString str2 = QString::fromStdString(to_string(comparisonArray5[1]));
                checkSwitches(str, str2);
                cout << "Engine 5 moved to track segment " << oursTotheirs(comparisonArray5[1]).toStdString() << endl;
            }
            comparisonArray5.erase(comparisonArray5.begin());
            qApp->processEvents();
        }}






    }
    else
    {
    //cout << "Tablet controls enabled. Scheduler disabled " << controlBit << endl;
    ui->stopButton->click();
    }


}

void MainWindow::toggleBit() //If tablet control is enabled, take control. If scheduler is active, give control to tablet.
{
    if (controlBit == 0)
    {
        //cout << "Tablet controls enabled" << endl;
        controlBit = 1;
        ui->ModeEdit->setText("Tablet Control");
        ui->startButton->setDisabled(true);
    }
    else
    {
        controlBit = 0;
        ui->ModeEdit->setText("Scheduler Control");
        ui->startButton->setEnabled(true);
        //cout << "automatic controls enabled" << endl;
    }
}



void MainWindow::clearChecks()
{
    //This function clears all the radio buttons.
    

ui->buttonGroup->setExclusive(false);
ui->buttonGroup_2->setExclusive(false);
ui->buttonGroup_3->setExclusive(false);
ui->buttonGroup_4->setExclusive(false);
ui->buttonGroup_5->setExclusive(false);

ui->delayButton1->setChecked(false);
ui->delayButton2->setChecked(false);
ui->delayButton3->setChecked(false);
ui->delayButton4->setChecked(false);
ui->delayButton5->setChecked(false);

ui->setButton1->setChecked(false);
ui->setButton2->setChecked(false);
ui->setButton3->setChecked(false);
ui->setButton4->setChecked(false);
ui->setButton5->setChecked(false);

ui->buttonGroup->setExclusive(true);
ui->buttonGroup_2->setExclusive(true);
ui->buttonGroup_3->setExclusive(true);
ui->buttonGroup_4->setExclusive(true);
ui->buttonGroup_5->setExclusive(true);

}
