#ifndef NEURESETCONTROLLER_H
#define NEURESETCONTROLLER_H

#include <QObject>
#include <EEGSite.h>
#include <defs.h>

class NeuresetController : public QObject {
    Q_OBJECT
public:
    static NeuresetController* getInstance();
    EEGSite* getEEGSite(int eegId);

    void disconnectSite(int eegId);

public slots:
    void contactLost();

signals:
    void lostContact();

protected:
    NeuresetController();
private:
    static NeuresetController* control;
    EEGSite* eegSites[NUM_EEGSITES];
};

#endif // NEURESETCONTROLLER_H
