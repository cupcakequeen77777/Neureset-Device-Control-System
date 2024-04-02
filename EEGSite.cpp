#include "EEGSite.h"

EEGSite::EEGSite(){
    EEGSite(1);
}

EEGSite::EEGSite(int i){
    id = i;
    isConnected = true;
}


void EEGSite::disconnectSite(int id){
    qDebug() << "contact lost on #" << id;
    emit contactLost();
}
