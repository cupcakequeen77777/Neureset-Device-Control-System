#ifndef EEGSITE_H
#define EEGSITE_H

#include <QObject>
#include <QDebug>

class EEGSite : public QObject {
    Q_OBJECT
public:
    EEGSite();

    EEGSite(int id);
    void deliverTreatment();

signals:
    void contactLost();

 public slots:
    void disconnectSite(int id);


private:
    int id;
    bool isConnected;
    int baselineFrequency;
    int calcNewBaseline(int);

};

#endif // EEGSITE_H
