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
    void startNewSession();
    void setBaseline();
    void getAlphaFrequency();
    void getBetaFrequency();
    void getDeltaFrequency();
    void getThetaFrequency();
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


protected:
    NeuresetController();
private:
    static NeuresetController* control;
    EEGSite* eegSites[NUM_EEGSITES];

    //Frequency Data
    int waveformData[60];
    int alpha[60];
    int beta[60];
    int delta[60];
    int theta[60];


    QTimer* timer; //keep track treatment time
    QElapsedTimer elapsedTime;
    bool isPaused;
    qint64 pausedTime;
    qint64 pauseOffset;
    static constexpr qint64 treatmentDurationMs = 60000; // 1 minute (in ms)
    QTimer* timerForPausing; //track if pausing over 5mins

    int numberOfSessions = 0;
    QDateTime sessionLogDT[NUM_EEGSITES];
    int sessionLogA[NUM_EEGSITES][MAX_NUM_SESSIONS];
    int sessionLogB[NUM_EEGSITES][MAX_NUM_SESSIONS];
    QString history;

    //timer for treatment.
    QTimer* treatmentTimer; // Timer to manage treatment rounds
    int currentRound = 0; // Current treatment round
    static constexpr int totalRounds = 4; // Total number of rounds
    bool isResumed = false;

};

#endif // NEURESETCONTROLLER_H
