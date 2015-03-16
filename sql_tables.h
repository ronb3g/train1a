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


#endif // SQL_TABLES_H
