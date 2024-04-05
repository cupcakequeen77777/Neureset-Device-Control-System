#ifndef EEGSITE_H
#define EEGSITE_H

#include <QObject>
#include <QDebug>

#define timeForAnalysis = 5000;
#define timeForFeedback = 1000;

class EEGSite : public QObject {
    Q_OBJECT
public:
    EEGSite();

    EEGSite(int id);

    bool getIsConnected();

    int getBaseline();

    void deliverTreatment(int);

    void disconnectSite();

    void reconnectSite();

signals:
    void contactLost(bool);

private:
    int id;
    bool isConnected;
    int baselineFrequency;
    int calcNewBaseline(int, int);
};

#endif // EEGSITE_H
