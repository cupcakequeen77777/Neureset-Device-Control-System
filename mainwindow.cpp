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

    connect(controller, &NeuresetController::lostContact, this, &MainWindow::contactLost);

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
    controller->disconnectSite(eegId);
}


void MainWindow::on_btn_connectSites_clicked(){
    qDebug () << "reconnect Sites";
    controller->reconnectSites();
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


void MainWindow::contactLost(bool x){
    if(x){
        qDebug() << "MainWindow recieves contactLost from controller";
        ui->contactLostSignal->setStyleSheet("background-color: red ");
        ui->btn_connectSites->setEnabled(true);
    }else{
        qDebug() << "MainWindow recieves not contactLost from controller";
        ui->contactLostSignal->setStyleSheet("background-color: pink ");
        ui->btn_connectSites->setEnabled(false);
    }

}



