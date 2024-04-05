#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defs.h"

#include <QtDebug>

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
    connect(controller, &NeuresetController::timeUpdated, this, &MainWindow::updateTreatmentTime);

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
    QLineSeries *series = new QLineSeries();
    for (int i=0; i<60; ++i){
        //random number between 1 and 30
        int randNum = rand() % 30;
        series->append(i, randNum);
        waveformData[i] = randNum;
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("EEG Waveform");
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 60);
    axisX->setTickCount(4);
    axisX->setTitleText("time");
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 30);
    axisY->setTitleText("frequency");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    QChartView *chartView = new QChartView(chart);
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
    if(item->text() == "NEW SESSION"){
        controller->startTimer();
        //switch to session info tab
        ui->tabWidget->setCurrentIndex(0);
        controller->startNewSession();
    }
    if(item->text() == "SESSION LOG"){
        qInfo() << "SESSION LOG:";
        QString history = controller->sessionLogToString();
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

    //start timing when pressing the on button... (can/will move to "new session" from menu later)
    //controller->startTimer();
    // start the timer for the battery consumption
    batteryInstance->startBatteryConsumption();
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

    qDebug() << "The date time is now: " <<ui->dateTimeEdit->dateTime();

    ui->dateTimeEdit->hide();
    ui->btn_setDate->hide();
}

void MainWindow::updateTreatmentTime(const QString& time) {
    ui->treamentTime->setText(time);
}


void MainWindow::updateBatteryLevel(int level) {
    ui->battery->setValue(level);
    //qDebug() << "Battery level updated to:" << level << "%";
}

void MainWindow::handleBatteryDepleted() {
    qDebug() << "Battery depleted. Application will now close.";
    QApplication::quit();
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
