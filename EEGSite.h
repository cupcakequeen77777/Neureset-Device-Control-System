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
    void setBaseline(int);
    int calculateBaseline(int* data);
    void listenAlphaFrequencies(int * data);
    void listenBetaFrequencies(int * data);
    void listenDeltaFrequencies(int * data);
    void listenThetaFrequencies(int * data);
    void deliverTreatment(int);
    void disconnectSite();
    void reconnectSite();
    int* getWaveform(char);

signals:
    void contactLost(bool);

private:
    int id;
    bool isConnected;
    int baselineFrequency;
    int alpha[60];
    int beta[60];
    int theta[60];
    int delta[60];
    int calcNewBaseline(int, int);
    void generateWaveForm();
};

#endif // EEGSITE_H
