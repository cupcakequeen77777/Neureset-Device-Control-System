#include "Battery.h"
#include <QDebug>

Battery::Battery(QObject *parent) : QObject(parent), batteryLevel(100) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Battery::consumePower);
}

Battery::~Battery() {
    if (timer) timer->stop();
}

void Battery::startBatteryConsumption() {
    timer->start(15000); // 60000 milliseconds = 60 seconds
                         // we can adjust this later
    qDebug ("Battery consumption started.");
}


void Battery::stopBatteryConsumption() {
    if (timer->isActive()) {
        timer->stop();
    }
}

void Battery::consumePower() {
    batteryLevel -= 5; // Decrease battery by 5 percent
    if(batteryLevel <= 0) {
        batteryLevel = 0;
        emit batteryDepleted(); // Emit the batteryDepleted signal
        timer->stop(); // Stop the timer as the battery is dead
    }
    else if (batteryLevel <= 20) {
       emit lowBatteryWarning();
    }
    emit batteryLevelChanged(batteryLevel);
    qDebug() << "Battery level is now at" << batteryLevel << "%";
}


int Battery::getBatteryLevel() const {
    return batteryLevel;
}

