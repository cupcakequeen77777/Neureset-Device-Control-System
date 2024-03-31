#ifndef EEGSITE_H
#define EEGSITE_H

#include <QObject>

class EEGSite : public QObject {
    Q_OBJECT
public:
    EEGSite();

private:
    int id;
    bool isConnected;
};

#endif // EEGSITE_H
