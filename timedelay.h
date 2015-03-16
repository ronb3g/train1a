#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <QTimer>


void MainWindow::timeDelay5()
{

    int ch,cm,cs,sh,sm;
    QTime time = QTime::currentTime();

    ch = time.hour();
    cm = time.minute();
    cs = time.second();

    sh = ui->spinBox5_1->value();
    sm = ui->spinBox5_2->value();

    int cinSecs = cs + (cm*60) + (ch*60*60);
    int sinSecs = (sm*60) + (sh*60*60);
    int diff = (sinSecs - cinSecs)*1000;

    QTimer::singleShot(diff, this, SLOT(setgreyOut5()));
}

void MainWindow::timeDelay1()
{
    //ui->setButton1->set
    int ch,cm,cs,sh,sm;
    QTime time = QTime::currentTime();

    ch = time.hour();
    cm = time.minute();
    cs = time.second();

    sh = ui->spinBox1_1->value();
    sm = ui->spinBox1_2->value();

    int cinSecs = cs + (cm*60) + (ch*60*60);
    int sinSecs = (sm*60) + (sh*60*60);
    int diff = (sinSecs - cinSecs)*1000;

    QTimer::singleShot(diff, this, SLOT(setgreyOut1()));
}

void MainWindow::timeDelay2()
{
    int ch,cm,cs,sh,sm;
    QTime time = QTime::currentTime();

    ch = time.hour();
    cm = time.minute();
    cs = time.second();

    sh = ui->spinBox2_1->value();
    sm = ui->spinBox2_2->value();

    int cinSecs = cs + (cm*60) + (ch*60*60);
    int sinSecs = (sm*60) + (sh*60*60);
    int diff = (sinSecs - cinSecs)*1000;

    QTimer::singleShot(diff, this, SLOT(setgreyOut2()));
}

void MainWindow::timeDelay3()
{
    int ch,cm,cs,sh,sm;
    QTime time = QTime::currentTime();

    ch = time.hour();
    cm = time.minute();
    cs = time.second();

    sh = ui->spinBox3_1->value();
    sm = ui->spinBox3_2->value();

    int cinSecs = cs + (cm*60) + (ch*60*60);
    int sinSecs = (sm*60) + (sh*60*60);
    int diff = (sinSecs - cinSecs)*1000;

    QTimer::singleShot(diff, this, SLOT(setgreyOut3()));
}

void MainWindow::timeDelay4()
{
    int ch,cm,cs,sh,sm;
    QTime time = QTime::currentTime();

    ch = time.hour();
    cm = time.minute();
    cs = time.second();

    sh = ui->spinBox4_1->value();
    sm = ui->spinBox4_2->value();

    int cinSecs = cs + (cm*60) + (ch*60*60);
    int sinSecs = (sm*60) + (sh*60*60);
    int diff = (sinSecs - cinSecs)*1000;

    QTimer::singleShot(diff, this, SLOT(setgreyOut4()));
}





#endif // TIMEDELAY_H
