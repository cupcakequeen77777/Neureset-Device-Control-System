#include "NeuresetController.h"

NeuresetController* NeuresetController::control = 0;

NeuresetController::NeuresetController(){
    for(int i = 0; i < NUM_EEGSITES; i++){
        eegSites[i] = new EEGSite(i+1);
        connect(eegSites[i], &EEGSite::contactLost, this, &NeuresetController::contactLost);
     }

}


NeuresetController* NeuresetController::getInstance(){
    if (control == 0) {
    control = new NeuresetController();
    }
    return control;
}


EEGSite* NeuresetController::getEEGSite(int eegId){
    return eegSites[eegId];
}

void NeuresetController::disconnectSite(int eegId){
    eegSites[eegId-1]->disconnectSite();
}


void NeuresetController::contactLost(){
    qDebug() << "NeuresetController recives contactLost from EEG site";
    emit lostContact();
}
