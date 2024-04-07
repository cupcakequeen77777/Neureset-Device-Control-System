#include "NeuresetController.h"

NeuresetController* NeuresetController::control = 0;

NeuresetController::NeuresetController(): isPaused(false), pausedTime(0), pauseOffset(0){
    for(int i = 0; i < NUM_EEGSITES; i++){
        eegSites[i] = new EEGSite(i+1);
        connect(eegSites[i], &EEGSite::contactLost, this, &NeuresetController::contactLost);
     }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NeuresetController::updateTimer);
    timerForPausing = new QTimer(this);
    timerForPausing->setSingleShot(true);
    connect(timerForPausing, &QTimer::timeout, this, &NeuresetController::handlePauseTimeout);
}


NeuresetController* NeuresetController::getInstance(){
    if (control == 0) {
    control = new NeuresetController();
    }
    return control;
}

void NeuresetController::startNewSession(){
    qDebug() << "Starting new session";

    //FIXME: treatment happens instantly, should take a minute...?
    for(int round=1; round <= 4; round++){
        qInfo() << "Beginning round #" << round << " with " << round*5 << "hz offset frequency";

        QDateTime currentDateTime = QDateTime::currentDateTime();
        sessionLogDT[numberOfSessions] = currentDateTime;

        qDebug() << "Current date and time:" << currentDateTime.toString("dd/MM/yy hh:mm:ss AP");

        for (int i=0; i< NUM_EEGSITES; ++i){
            sessionLogB[i][round-1] = eegSites[i]->getBaselineFrequency();
            eegSites[i]->deliverTreatment(round*5);
            sessionLogB[i][round-1] = eegSites[i]->getBaselineFrequency();
        }
        //FIXME: add delay for 15 seconds so the treatment actually takes a minute
        qInfo() << "Round #" << round << " completed\n*****";
    }

    numberOfSessions ++;
}

EEGSite* NeuresetController::getEEGSite(int eegId){
    return eegSites[eegId];
}

void NeuresetController::disconnectSite(int eegId){
    eegSites[eegId-1]->disconnectSite();
}


void NeuresetController::reconnectSites(){
    for (int i = 0; i< NUM_EEGSITES; i++) {
        if(!eegSites[i]->getIsConnected()){
            eegSites[i]->reconnectSite();
        }
    }

}

void NeuresetController::contactLost(bool x){
    if(x){
        qDebug() << "NeuresetController recives contactLost from EEG site";
    }else{
        qDebug() << "NeuresetController recives NOT contactLost from EEG site";
    }
    emit lostContact(x);
}

void NeuresetController::startTimer() {
    elapsedTime.start(); // Starts or restarts the elapsed timer
    pausedTime = 0;
    pauseOffset = 0;
    isPaused = false;
    timer->start(1000); // Start the timer with a 1-second interval
}


void NeuresetController::updateTimer() {
    qint64 currentElapsed = elapsedTime.elapsed() - pauseOffset;
    qint64 timeRemainingMs = treatmentDurationMs - currentElapsed;
    if (timeRemainingMs < 0) {
        timeRemainingMs = 0;
        stopTimer();
    }
    qint64 remainingSeconds = timeRemainingMs / 1000;
    QTime remainingTime = QTime(0, 0).addSecs(remainingSeconds);
    emit timeUpdated(remainingTime.toString("mm:ss"));

    int progress = 100 - static_cast<int>((timeRemainingMs * 100) / treatmentDurationMs);
    emit updatedProgressBar(progress); // Emit signal with the calculated progress
}



void NeuresetController::pauseTimer() {
    if (!isPaused) {
        timer->stop(); // Stop the QTimer
        pausedTime = elapsedTime.elapsed(); // record the elapsed time
        isPaused = true;

        timerForPausing->start(5000);
    }
}


void NeuresetController::resumeTimer() {
    if (isPaused) {
        qint64 currentPauseDuration = elapsedTime.elapsed() - pausedTime;
        pauseOffset += currentPauseDuration;
        timer->start(1000);
        isPaused = false;

        timerForPausing->stop();
    }
}

void NeuresetController::stopTimer() {
    timer->stop();
    elapsedTime.restart();
    isPaused = false;
    pausedTime = 0;
    pauseOffset = 0;
    emit timeUpdated("01:00"); // Reset the display to the initial time

    //reset the progress bar.
    emit updatedProgressBar(0);
}

void NeuresetController::handlePauseTimeout() {
    qDebug() << "Pause timeout reached. Ending session.";
    stopTimer();

    // emit signal to inform the rest of your application that the session has ended.
    emit sessionEnded(); // this does nothing for now...
}


void NeuresetController::setBaseline(){
    for(int i=0; i<NUM_EEGSITES; i++){
        EEGSite *sensor = getEEGSite(i);
        sensor->calculateBaseline(waveformData);
    }
}

QString NeuresetController::sessionLogToString(){
    QString log;
    for(int i = 0; i < numberOfSessions; i++){
        log.append(sessionLogDT[i].toString("dd/MM/yy hh:mm:ss AP"));
        qInfo() << sessionLogDT[i].toString("dd/MM/yy hh:mm:ss AP");
        log.append("\n");
    }

    return log;
}

QChart* NeuresetController::generateChart(int eegSite){
    QLineSeries *series = new QLineSeries();
    QChart *chart = new QChart();

    //special case, generating the average chart for all eeg sites
    if (eegSite == -1){
        for (int i=0; i<60; ++i){
            //random number between 1 and 30
            int randNum = rand() % 30;
            series->append(i, randNum);
            waveformData[i] = randNum;
        }
        chart->setTitle("Average EEG Waveform");
    }
    else{
        int *data = eegSites[eegSite-1]->getWaveform();
        for (int i=0; i<60; ++i){
            series->append(i, data[i]);
        }
        QString chartName = "EEG Waveform #";
        chart->setTitle(chartName.append(QString::number(eegSite)));
    }

    chart->legend()->hide();
    chart->addSeries(series);
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 60);
    axisX->setTickCount(4);
    axisX->setTitleText("time");
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 30);
    axisY->setTitleText("frequency");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    return chart;
}
