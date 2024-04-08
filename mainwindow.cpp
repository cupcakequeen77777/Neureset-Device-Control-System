#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defs.h"

#include <QtDebug>
#include <QApplication>


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
    ui->eegSiteWave->setMaximum(NUM_EEGSITES);
    ui->theGraph->hide();

    connect(controller, &NeuresetController::lostContact, this, &MainWindow::contactLost);
    connect(controller, &NeuresetController::treatmentDelivered, this, &MainWindow::treatmentDelivered);
    connect(controller, &NeuresetController::timeUpdated, this, &MainWindow::updateTreatmentTime);

    connect(controller, &NeuresetController::updatedProgressBar, this, &MainWindow::updateProgressBar);

    initializeBatteryStuff();
    createChart();

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initializeBatteryStuff() {
    batteryInstance = new Battery(this); // Battery instance is a child of MainWindow
    connect(batteryInstance, &Battery::batteryLevelChanged, this, &MainWindow::updateBatteryLevel);
    connect(batteryInstance, &Battery::batteryDepleted, this, &MainWindow::handleBatteryDepleted);

    ui->battery->setMaximum(100);
    ui->battery->setValue(batteryInstance->getBatteryLevel());
}

//create a graphical representation of the waveform and add it to the GUI
void MainWindow::createChart(){
    QChart *c = controller->generateChart(-1); // series = controller->generateSeries(series);
    controller->setBaseline(); // sets baseline for all the EEG censor
    // TEST
    //qDebug()<<"The Baseline value of EEG 10 is"<< controller->getEEGSite(10)->getBaselineFrequency();

    QChartView *chartView = new QChartView(c);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(ui->theGraph->size());
    chartView->setParent(ui->theGraph);
}

void MainWindow::on_btn_pauseTreatement_clicked(){
    qDebug ("on_btn_pauseTreatement_clicked");
    controller->pauseTimer();
}


void MainWindow::on_btn_continueTreatment_clicked(){
    qDebug ("continue Treatment");
    controller->resumeTimer();
}


void MainWindow::on_btn_stopTreatement_clicked(){
    qDebug ("stop Treatment");
    controller->stopTimer();
}


void MainWindow::on_btn_disconnectSite_clicked(){
    int eegId = ui->eegSite->value();
    qDebug () << "disconnect Site" << eegId;
    controller->disconnectSite(eegId);

    //also pause the timer after disconnected.
    controller->pauseTimer();
}

void MainWindow::on_btn_connectSites_clicked(){
    qDebug () << "reconnect Sites";
    controller->reconnectSites();

    controller->resumeTimer();
}


void MainWindow::on_widget_menuOpts_itemActivated(QListWidgetItem *item){
    qDebug ()<< "menuOpts_itemActivated" << item->text();
    if(item->text() == "TIME AND DATE"){
        ui->dateTimeEdit->show();
        ui->btn_setDate->show();
    }
    else{
        ui->dateTimeEdit->hide();
        ui->btn_setDate->hide();
    }
    if(item->text() == "NEW SESSION"){
        controller->startTimer();
        // Enable the admin box
        ui->eegSite->setEnabled(true);
        ui->btn_disconnectSite->setEnabled(true);

        //switch to session info tab
        ui->tabWidget->setCurrentIndex(0);
        ui->contactSignal->setStyleSheet("background-color: blue");
        controller->startNewSession();
        controller->history();
    }
    if(item->text() == "SESSION LOG"){
        qInfo() << "SESSION LOG:";
        QString history = controller->history();
        QString filename = "Data.txt";
        QFile file(filename);


        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << history; // Write the session logs here
            file.close(); // Close the file when done
        } else {
            qDebug() << "Error opening the file.";
        }
    }
}


void MainWindow::on_btn_on_clicked(){
    qDebug() << "You turned on the  machine";
    ui->btn_on->hide();
    ui->btn_off->show();
    ui->control->show();
    ui->theGraph->show();
    ui->AdminView->show();
    ui->btn_seeEEGWave->setEnabled(true);
    ui->eegSiteWave->setEnabled(true);



    // start the timer for the battery consumption
    batteryInstance->startBatteryConsumption();
}


void MainWindow::on_btn_off_clicked(){
    qDebug() << "You turned off the  machine";
    ui->btn_off->hide();
    ui->btn_on->show();
    ui->control->hide();
    ui->theGraph->hide();

    // Enable the admin box
    ui->btn_connectSites->setEnabled(false);
    ui->eegSite->setEnabled(false);
    ui->btn_disconnectSite->setEnabled(false);
    ui->btn_seeEEGWave->setEnabled(false);
    ui->eegSiteWave->setEnabled(false);


    ui->contactSignal->setStyleSheet("background-color: #B8D6F5");
    ui->contactLostSignal->setStyleSheet("background-color: pink");
    ui->treatementSignal->setStyleSheet("background-color: #A9E6B3");

    //stoped the timer when turning off the machine
    controller->stopTimer();
    
    // stop the battery consumption timer
    batteryInstance->stopBatteryConsumption();

}


void MainWindow::on_btn_setDate_clicked(){
    qDebug() << "The date is now: " << ui->dateTimeEdit->date();
    qDebug() << "The time is now: " <<ui->dateTimeEdit->time();

    qDebug() << "The date time is now: " <<ui->dateTimeEdit->dateTime();

    ui->dateTimeEdit->hide();
    ui->btn_setDate->hide();
}

void MainWindow::updateTreatmentTime(const QString& time) {
    ui->treamentTime->setText(time);
}


void MainWindow::updateBatteryLevel(int level) {
    ui->battery->setValue(level);

    if (level < 20) {
        // Change the batteryLabel's appearance to indicate low battery
        ui->batteryLabel->setStyleSheet("QLabel { color: white; background-color: red; }");
        ui->batteryLabel->setText("Low Battery!");
    } else {
        // reset the batteryLabel appearance when the battery is not low
        ui->batteryLabel->setStyleSheet("");
        ui->batteryLabel->setText("");
    }
}

void MainWindow::handleBatteryDepleted() {
    qDebug() << "Battery depleted. Application will now close.";
    on_btn_off_clicked();
}


void MainWindow::contactLost(bool x){
    if(x){
        qDebug() << "MainWindow recieves contactLost from controller";
        ui->contactLostSignal->setStyleSheet("background-color: red");
        ui->btn_connectSites->setEnabled(true);
        ui->contactSignal->setStyleSheet("background-color: #B8D6F5");
    }else{
        qDebug() << "MainWindow recieves not contactLost from controller";
        ui->contactLostSignal->setStyleSheet("background-color: pink");
        ui->btn_connectSites->setEnabled(false);
        ui->contactSignal->setStyleSheet("background-color: blue");
    }
}

void MainWindow::treatmentDelivered(bool lightOn){
    if(lightOn){
        ui->treatementSignal->setStyleSheet("background-color: green");
    }else{
        ui->treatementSignal->setStyleSheet("background-color: #A9E6B3");
    }
}

void MainWindow::updateProgressBar(int progress) {
    ui->treatementProgress->setValue(progress);
}


void MainWindow::on_btn_seeEEGWave_clicked(){
    QChart *c = controller->generateChart(ui->eegSiteWave->value());

    QChartView *chartView = new QChartView(c);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(QSize(400, 300));
    //opens chart in a new window
    chartView->show();
}

