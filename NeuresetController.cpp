#include "NeuresetController.h"

NeuresetController::NeuresetController(){
    for(int i = 0; i < NUM_EEGSITES; i++){
        eegSites[i] = new EEGSite(i+1);
     }
}


NeuresetController* NeuresetController::getInstance(){
    if (control == 0) {
    control = new NeuresetController();
    }
    return control;
}
