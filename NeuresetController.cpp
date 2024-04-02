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

void NeuresetController::reconnectSites(){
    for (int i = 0; i< NUM_EEGSITES; i++) {
        if(!eegSites[i-1]->getIsConnected()){
            eegSites[i-1]->reconnectSite();
        }
    }

}


void NeuresetController::contactLost(bool x){
    if(x){
        qDebug() << "NeuresetController recives contactLost from EEG site";
    }else{
        qDebug() << "NeuresetController recives NOT contactLost from EEG site";
    }
    emit lostContact(x);
}
