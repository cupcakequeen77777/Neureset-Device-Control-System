#include "NeuresetController.h"

NeuresetController* NeuresetController::control = 0;

NeuresetController::NeuresetController(): isPaused(false), pausedTime(0), pauseOffset(0){
    for(int i = 0; i < NUM_EEGSITES; i++){
        eegSites[i] = new EEGSite(i+1);
        connect(eegSites[i], &EEGSite::contactLost, this, &NeuresetController::contactLost);
        for (int j = 0; j < MAX_NUM_SESSIONS; j++){
            sessionLogA[i][j] = 0;
            sessionLogB[i][j] = 0;
        }

     }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NeuresetController::updateTimer);
    timerForPausing = new QTimer(this);
    timerForPausing->setSingleShot(true);
    connect(timerForPausing, &QTimer::timeout, this, &NeuresetController::handlePauseTimeout);

    //timer for treatment
    treatmentTimer = new QTimer(this);
    treatmentTimer->setInterval(ROUND_TIME); // Set interval to 15 seconds for each round
    treatmentTimer->setSingleShot(false); // Make sure the timer repeats
    connect(treatmentTimer, &QTimer::timeout, this, &NeuresetController::handleTreatmentRound);
}


NeuresetController* NeuresetController::getInstance(){
    if (control == 0) {
    control = new NeuresetController();
    }
    return control;
}

//please confirms if the timer logic makes sense
void NeuresetController::startNewSession(){
    qDebug() << "Starting new session";
    currentRound = 1; // Reset current round
    for (int i=0; i< NUM_EEGSITES; ++i){
        sessionLogB[i][numberOfSessions] = eegSites[i]->getBaselineFrequency();
    }
    qDebug() << "Flash on"; // FIXME: remove once there is a delay so the lights flash
    emit treatmentDelivered(false);
    treatmentTimer->start(); // Start the treatment rounds
}

void NeuresetController::handleTreatmentRound() {
    qInfo() << "Beginning round #" << currentRound << " with " << currentRound * 5 << "Hz offset frequency";



    // Log the date and time at the start of the first round or if you need it logged at every round?
    if (currentRound == 1) {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        sessionLogDT[numberOfSessions] = currentDateTime;
    }

    // Treatment logic for each EEG site
    for (int i=0; i< NUM_EEGSITES; ++i){
        sessionLogB[i][currentRound-1] = eegSites[i]->getBaselineFrequency();

        eegSites[i]->deliverTreatment(currentRound*5);
        sessionLogA[i][currentRound-1] = eegSites[i]->getBaselineFrequency();
    }

    qInfo() << "Round #" << currentRound << " completed\n*****";

    if (isResumed && treatmentTimer->interval() != ROUND_TIME) {
        treatmentTimer->start(ROUND_TIME); // Reset to normal interval for next rounds
        isResumed = false; // Reset the flag after handling the resumed round

    }

    if (currentRound == totalRounds) {
        treatmentTimer->stop(); // Stop the timer if all rounds are completed
        qDebug() << "Treatment session completed";

        // Increment the session count now that treatment is complete.
        for (int i=0; i< NUM_EEGSITES; ++i){
            sessionLogA[i][numberOfSessions] = eegSites[i]->getBaselineFrequency();
        }
        numberOfSessions++;

        emit treatmentDelivered(true);
        qDebug() << "Flash off"; // FIXME: remove once there is a delay so the lights flash

        return;
    }

    currentRound++; // Prepare for the next round
}


//old version, please confirms if the timer logic makes sense
//void NeuresetController::startNewSession(){
//    qDebug() << "Starting new session";

//    //FIXME: treatment happens instantly, should take a minute...?
//    for(int round=1; round <= 4; round++){
//        qInfo() << "Beginning round #" << round << " with " << round*5 << "hz offset frequency";

//        QDateTime currentDateTime = QDateTime::currentDateTime();
//        sessionLogDT[numberOfSessions] = currentDateTime;

//        qDebug() << "Current date and time:" << currentDateTime.toString("dd/MM/yy hh:mm:ss AP");

//        for (int i=0; i< NUM_EEGSITES; ++i){
//            sessionLogB[i][round-1] = eegSites[i]->getBaselineFrequency();
//            eegSites[i]->deliverTreatment(round*5);
//            sessionLogB[i][round-1] = eegSites[i]->getBaselineFrequency();
//        }
//        //FIXME: add delay for 15 seconds so the treatment actually takes a minute
//        qInfo() << "Round #" << round << " completed\n*****";
//    }


//    numberOfSessions ++;
//}

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
        treatmentTimer->stop(); //stop the treatment timer.
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

        // Calculate how much time had elapsed in the current round at pause
        qint64 timeInCurrentRound = (elapsedTime.elapsed() - pauseOffset) % ROUND_TIME; // Modulo gives time in the current round
        qint64 remainingTimeForRound = ROUND_TIME - timeInCurrentRound; // Time left to finish the round

        isResumed = true;
        // Restart the treatment timer for the remaining time of the current round
        treatmentTimer->start(remainingTimeForRound);
    }
}


void NeuresetController::stopTimer() {
    timer->stop();
    elapsedTime.restart();
    isPaused = false;
    pausedTime = 0;
    pauseOffset = 0;
    emit timeUpdated("01:00"); // Reset the display to the initial time

    treatmentTimer->stop();
    currentRound = 0;

    //reset the progress bar.
    emit updatedProgressBar(0);
}

void NeuresetController::handlePauseTimeout() {
    qDebug() << "Pause timeout reached. Ending session.";
    stopTimer();
}


void NeuresetController::setBaseline(){
    for(int i=0; i<NUM_EEGSITES; i++){
        EEGSite *sensor = getEEGSite(i);
        sensor->calculateBaseline(waveformData);
    }
}

QString NeuresetController::sessionLogToString(int session){
    QString log;

    for(int i = 0; i < NUM_EEGSITES; i++){
        QString baseline;
        qInfo() << "EEG site #" << i+1 << ":" << sessionLogB[i][session] << "hz ->" << sessionLogA[i][session] << "hz";
        log.append("EEG site #");
        log.append(QString::number(i+1));
        log.append(": ");
        log.append(QString::number(sessionLogB[i][session]));
        log.append("hz -> ");
        log.append(QString::number(sessionLogA[i][session]));
        log.append("hz\n");
    }

    return log;
}

QString NeuresetController::history(){
    QString history = "";

    if(numberOfSessions==0){
        return history;
    }


    history.append(sessionLogDT[numberOfSessions-1].toString("dd/MM/yy hh:mm:ss AP\n"));
    qInfo() << sessionLogDT[numberOfSessions-1].toString("dd/MM/yy hh:mm:ss AP");

    for(int i = 0; i < numberOfSessions; i++){
        history.append(sessionLogToString(i));
    }

    return history;
}

QChart* NeuresetController::generateChart(int eegSite, char type){
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
        int *data = eegSites[eegSite-1]->getWaveform(type);

        for (int i=0; i<60; ++i){
            series->append(i, data[i]);
        }
        QString chartName = "EEG Waveform #";
        chart->setTitle(chartName.append(QString::number(eegSite)));
    }

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).back()->setRange(0, 60);
    chart->axes(Qt::Vertical).back()->setRange(0, 30);
    chart->axes(Qt::Horizontal).back()->setTitleText("time");
    chart->axes(Qt::Vertical).back()->setTitleText("frequency");
    return chart;
}
