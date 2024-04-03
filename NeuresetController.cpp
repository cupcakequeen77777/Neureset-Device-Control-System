#include "NeuresetController.h"

NeuresetController* NeuresetController::control = 0;

NeuresetController::NeuresetController(): isPaused(false), pausedTime(0), pauseOffset(0){
    for(int i = 0; i < NUM_EEGSITES; i++){
        eegSites[i] = new EEGSite(i+1);
     }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NeuresetController::updateTimer);
}


NeuresetController* NeuresetController::getInstance(){
    if (control == 0) {
    control = new NeuresetController();
    }
    return control;
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
}

void NeuresetController::pauseTimer() {
    if (!isPaused) {
        timer->stop(); // Stop the QTimer
        pausedTime = elapsedTime.elapsed(); // record the elapsed time
        isPaused = true;
    }
}


void NeuresetController::resumeTimer() {
    if (isPaused) {
        qint64 currentPauseDuration = elapsedTime.elapsed() - pausedTime;
        pauseOffset += currentPauseDuration;
        timer->start(1000);
        isPaused = false;
    }
}

void NeuresetController::stopTimer() {
    timer->stop();
    elapsedTime.restart();
    isPaused = false;
    pausedTime = 0;
    pauseOffset = 0;
    emit timeUpdated("01:00"); // Reset the display to the initial time
}

