#ifndef NEURESETCONTROLLER_H
#define NEURESETCONTROLLER_H

#include <QObject>
#include <EEGSite.h>
#include <defs.h>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QString>
#include <QtDebug>

class NeuresetController : public QObject {
    Q_OBJECT
public:
    static NeuresetController* getInstance();
    EEGSite* getEEGSite(int eegId);
    void disconnectSite(int eegId);
    void reconnectSites();
    void startTimer();
    void pauseTimer();
    void resumeTimer();
    void stopTimer();
    void startNewSession();

public slots:
    void contactLost(bool);
    void updateTimer();
    void handlePauseTimeout();

signals:
    void lostContact(bool);
    void timeUpdated(const QString& timeString);
    void sessionEnded(); //placeholder for session end signal.

protected:
    NeuresetController();
private:
    static NeuresetController* control;
    EEGSite* eegSites[NUM_EEGSITES];
    QTimer* timer; //keep track treatment time
    QElapsedTimer elapsedTime;
    bool isPaused;
    qint64 pausedTime;
    qint64 pauseOffset;
    static constexpr qint64 treatmentDurationMs = 60000; // 1 minute (in ms)
    QTimer* timerForPausing; //track if pasuing over 5mins

    int numberOfSessions = 0;
    QDateTime* sessionLog[];

};

#endif // NEURESETCONTROLLER_H
