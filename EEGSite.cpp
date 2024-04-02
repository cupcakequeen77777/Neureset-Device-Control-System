#include "EEGSite.h"

EEGSite::EEGSite(){
    EEGSite(1);
}

EEGSite::EEGSite(int i){
    id = i;
    isConnected = true;
}

void EEGSite::contactLost(){
    qDebug() << "contact lost on #" << id;
}
