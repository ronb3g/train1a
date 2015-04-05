#ifndef SQL_TABLES_H
#define SQL_TABLES_H

#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <QMessageBox>

void MainWindow::traininfoTable()
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



void MainWindow::pathinfoTable()
{
    QString vt = "pathinfo";
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

void MainWindow::throttleTable()
{
    QString vt = "throttle";
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

void MainWindow::switchesTable()
{
    QString vt = "switches";
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

void MainWindow::techsupportDoc()
{

    QString ts = "";
    ts.append("Scrum Master:\n     Ronald Burke -- rob0001@uah.edu\n\n");
    ts.append("Test Master:\n     Wesley Stewart -- ws0004@uah.edu\n\n");
    ts.append("ICD:\n     Shannon Washington -- smw0012@uah.edu");

    QMessageBox msgBox;
    msgBox.setWindowTitle("Technical Support");
    msgBox.setText(ts);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){
      // do something
    }else {
      // do something else
    }
}

void MainWindow::createDBtables()
{
    QSqlQuery q(ldb);
    q.prepare("CREATE TABLE switches(switchID TEXT,position TEXT);");
    if ( !q.exec() )
    {
    statusBar()->showMessage(tr("Create table failed"));
    qDebug() << q.lastError();
    qDebug() << "Error on INSERT";
    }
    else
    {
    statusBar()->showMessage(tr("Create table was succesful"));
    }
    //QSqlQuery q(ldb);
    q.prepare("CREATE TABLE throttle(trainID TEXT,direction TEXT,throttle INTEGER);");
    if ( !q.exec() )
    {
    statusBar()->showMessage(tr("Create table failed"));
    qDebug() << q.lastError();
    qDebug() << "Error on INSERT";
    }
    else
    {
    statusBar()->showMessage(tr("Create table was succesful"));
    }
    //QSqlQuery q(ldb);
    q.prepare("CREATE TABLE traininfo(trainID TEXT,current TEXT,destination TEXT,next TEXT,path INTEGER);");
    if ( !q.exec() )
    {
    statusBar()->showMessage(tr("Create table failed"));
    qDebug() << q.lastError();
    qDebug() << "Error on INSERT";
    }
    else
    {
    statusBar()->showMessage(tr("Create table was succesful"));
    }
    //QSqlQuery q(ldb);
    q.prepare("CREATE TABLE pathinfo(pathID INTEGER,nextpath INTEGER,next2 TEXT,next3 TEXT,next4 TEXT,next5 TEXT,next6 TEXT,next7 TEXT,next8 TEXT,next9 TEXT,next10 TEXT);");
    if ( !q.exec() )
    {
    statusBar()->showMessage(tr("Create table failed"));
    qDebug() << q.lastError();
    qDebug() << "Error on INSERT";
    }
    else
    {
    statusBar()->showMessage(tr("Create table was succesful"));
    }

    q.prepare("INSERT INTO pathinfo(pathID,nextpath,next2,next3,next4,next5,next6,next7,next8,next9,next10) VALUES(?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(1); q.addBindValue(2); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO pathinfo(pathID,nextpath,next2,next3,next4,next5,next6,next7,next8,next9,next10) VALUES(?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(2); q.addBindValue(3); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO pathinfo(pathID,nextpath,next2,next3,next4,next5,next6,next7,next8,next9,next10) VALUES(?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(3); q.addBindValue(4); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO pathinfo(pathID,nextpath,next2,next3,next4,next5,next6,next7,next8,next9,next10) VALUES(?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(4); q.addBindValue(5); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}


    q.prepare("INSERT INTO traininfo (trainID,current,destination,next,path) VALUES(?,?,?,?,?)");
    q.addBindValue("Engine 1"); q.addBindValue(""); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO traininfo (trainID,current,destination,next,path) VALUES(?,?,?,?,?)");
    q.addBindValue("Engine 2"); q.addBindValue(""); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO traininfo (trainID,current,destination,next,path) VALUES(?,?,?,?,?)");
    q.addBindValue("Engine 3"); q.addBindValue(""); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO traininfo (trainID,current,destination,next,path) VALUES(?,?,?,?,?)");
    q.addBindValue("Engine 4"); q.addBindValue(""); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO traininfo (trainID,current,destination,next,path) VALUES(?,?,?,?,?)");
    q.addBindValue("Engine 5"); q.addBindValue(""); q.addBindValue(""); q.addBindValue(""); q.addBindValue("");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}

    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("11"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("12"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("13"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("14"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("21"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("22"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("23"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("24"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("31"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("32"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("33"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("34"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("41"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("42"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("43"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("51"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("52"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("61"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("62"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("63"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("64"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("71"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("72"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("73"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("74"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("81"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("82"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("83"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
    q.prepare("INSERT INTO switches(switchID,position) VALUES(?,?)");
    q.addBindValue("84"); q.addBindValue("Pass");
    if ( !q.exec() ){qDebug() << q.lastError(); qDebug() << "Error on INSERT";}
}

#endif // SQL_TABLES_H
