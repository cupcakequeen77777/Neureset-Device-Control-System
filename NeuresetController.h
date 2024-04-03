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
    void startTimer();
    void pauseTimer();
    void resumeTimer();
    void stopTimer();

public slots:
    void updateTimer();

signals:
    void timeUpdated(const QString& timeString);

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


};

#endif // NEURESETCONTROLLER_H
