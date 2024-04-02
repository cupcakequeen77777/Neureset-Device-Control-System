#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_off->hide();
    ui->btn_on->show();
    ui->dateTimeEdit->hide();
    ui->btn_setDate->hide();
    ui->control->hide();
    ui->eegSite->setMaximum(NUM_EEGSITES);


    for (int i=0; i<NUM_EEGSITES; i++) {
        connect(this, &MainWindow::disconnect, controller->getEEGSite(i), &EEGSite::disconnectSite);
    }

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_btn_pauseTreatement_clicked(){
    qDebug ("on_btn_pauseTreatement_clicked");
}


void MainWindow::on_btn_continueTreatment_clicked(){
    qDebug ("continue Treatment");
}


void MainWindow::on_btn_stopTreatement_clicked(){
    qDebug ("stop Treatment");
}

void MainWindow::on_btn_disconnectSite_clicked(){
    int eegId = ui->eegSite->value();
    qDebug () << "disconnect Site" << eegId;
//    connect(this, &MainWindow::disconnect, controller->getEEGSite(eegId), &EEGSite::disconnectSite);
    emit disconnect(eegId);
//    controller->getEEGSite(eegId)->disconnect();
}


void MainWindow::on_widget_menuOpts_itemActivated(QListWidgetItem *item){
    qDebug ()<< "menuOpts_itemActivated" << item->text();
    if(item->text() == "TIME AND DATE"){
        ui->dateTimeEdit->show();
        ui->btn_setDate->show();
    }
}


void MainWindow::on_btn_on_clicked(){
    qDebug() << "You turned on the  machine";
    ui->btn_on->hide();
    ui->btn_off->show();
    ui->control->show();
}


void MainWindow::on_btn_off_clicked(){
    qDebug() << "You turned off the  machine";
    ui->btn_off->hide();
    ui->btn_on->show();
    ui->control->hide();
}


void MainWindow::on_btn_setDate_clicked(){
    qDebug() << "The date is now: " << ui->dateTimeEdit->date();
    qDebug() << "The time is now: " <<ui->dateTimeEdit->time();

    ui->dateTimeEdit->hide();
    ui->btn_setDate->hide();
}

