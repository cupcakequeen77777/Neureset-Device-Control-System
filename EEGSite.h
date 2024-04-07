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

    int getBaselineFrequency();

    int calculateBaseline(int* data);

    void deliverTreatment(int);

    void disconnectSite();

    void reconnectSite();
    int* getWaveform();

signals:
    void contactLost(bool);

private:
    int id;
    bool isConnected;
    int baselineFrequency;
    int waveForm[60];
    int calcNewBaseline(int, int);
    void generateWaveForm();
};

#endif // EEGSITE_H
