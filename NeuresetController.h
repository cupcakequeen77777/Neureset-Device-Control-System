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
#include <QtCharts>
#include <QLineSeries>

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
    void startNewSession(char);
    QString sessionLogToString(int);
    QString sessionLog();
    QChart* generateChart(int, char);

public slots:
    void contactLost(bool);
    void updateTimer();
    void handlePauseTimeout();
    void handleTreatmentRound();

signals:
    void lostContact(bool);
    void treatmentDelivered(bool);
    void timeUpdated(const QString& timeString);
    void updatedProgressBar(int progress); // signal for the progress bar
    void reset();

protected:
    NeuresetController();
private:
    static NeuresetController* control;

    EEGSite* eegSites[NUM_EEGSITES];

    bool isPaused;
    bool isResumed = false;
    bool isStarted; //to control the timer behaviour

    int numberOfSessions = 0;
    int currentRound = 0; // Current treatment round
    static constexpr int totalRounds = 4; // Total number of rounds

    QString history;

    QDateTime sessionLogDT[NUM_EEGSITES];
    int sessionLogA[NUM_EEGSITES][MAX_NUM_SESSIONS];
    int sessionLogB[NUM_EEGSITES][MAX_NUM_SESSIONS];

    QTimer* timerForPausing; //track if pausing over 5mins
    //timers for treatment.
    QTimer* treatmentTimer; //keep track treatment time
    QTimer* treatmentRoundTimer; // Timer to manage treatment rounds
    QElapsedTimer elapsedTime;
    
    qint64 pausedTime;
    qint64 pauseOffset;
    static constexpr qint64 treatmentDurationMs = 60000; // 1 minute (in ms)
    
};

#endif // NEURESETCONTROLLER_H
