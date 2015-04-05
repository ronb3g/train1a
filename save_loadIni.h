#ifndef SAVE_LOADINI_H
#define SAVE_LOADINI_H
#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <QFileDialog>

//Function to save current configuration
void MainWindow::saveText()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".ini", tr("Config (*.ini)"));

    QSettings settings(fileName,QSettings::IniFormat);

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
    //settings.setValue("def/headingBox1", ui->headingBox1->currentIndex());
    //settings.setValue("def/headingBox2", ui->headingBox2->currentIndex());
    //settings.setValue("def/headingBox3", ui->headingBox3->currentIndex());
    //settings.setValue("def/headingBox4", ui->headingBox4->currentIndex());
    //settings.setValue("def/headingBox5", ui->headingBox5->currentIndex());
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
    settings.setValue("def/spinBox1_1", ui->spinBox1_1->value());
    settings.setValue("def/spinBox1_2", ui->spinBox1_2->value());
    settings.setValue("def/spinBox2_1", ui->spinBox2_1->value());
    settings.setValue("def/spinBox2_2", ui->spinBox2_2->value());
    settings.setValue("def/spinBox3_1", ui->spinBox3_1->value());
    settings.setValue("def/spinBox3_2", ui->spinBox3_2->value());
    settings.setValue("def/spinBox4_1", ui->spinBox4_1->value());
    settings.setValue("def/spinBox4_2", ui->spinBox4_2->value());
    settings.setValue("def/spinBox5_1", ui->spinBox5_1->value());
    settings.setValue("def/spinBox5_2", ui->spinBox5_2->value());


}

void MainWindow::loadText()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("Config (*.ini)"));
       QFile someFile(filename);
       if( !someFile.exists())
       {
           qDebug() << "Error -- File does not exist";
       }
       else if (!someFile.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           qDebug() << "Error -- unable to open file for input";
       }
       else
       {
            QSettings settings(filename,QSettings::IniFormat);
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
            //ui->headingBox1->setCurrentIndex(settings.value("def/headingBox1").toInt());
            //ui->headingBox2->setCurrentIndex(settings.value("def/headingBox2").toInt());
            //ui->headingBox3->setCurrentIndex(settings.value("def/headingBox3").toInt());
            //ui->headingBox4->setCurrentIndex(settings.value("def/headingBox4").toInt());
            //ui->headingBox5->setCurrentIndex(settings.value("def/headingBox5").toInt());
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
            ui->spinBox1_1->setValue(settings.value("def/spinBox1_1").toInt());
            ui->spinBox1_2->setValue(settings.value("def/spinBox1_2").toInt());
            ui->spinBox2_1->setValue(settings.value("def/spinBox2_1").toInt());
            ui->spinBox2_2->setValue(settings.value("def/spinBox2_2").toInt());
            ui->spinBox3_1->setValue(settings.value("def/spinBox3_1").toInt());
            ui->spinBox3_2->setValue(settings.value("def/spinBox3_2").toInt());
            ui->spinBox4_1->setValue(settings.value("def/spinBox4_1").toInt());
            ui->spinBox4_2->setValue(settings.value("def/spinBox4_2").toInt());
            ui->spinBox5_1->setValue(settings.value("def/spinBox5_1").toInt());
            ui->spinBox5_2->setValue(settings.value("def/spinBox5_2").toInt());
       }
}



#endif // SAVE_LOADINI_H
