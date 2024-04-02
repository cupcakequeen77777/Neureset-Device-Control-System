#include "EEGSite.h"

EEGSite::EEGSite(){
    EEGSite(1);
}

EEGSite::EEGSite(int i){
    id = i;
    isConnected = true;
    baselineFrequency = 10; //this is a default frequency, but the getBaseline() func will actually calculate the frequency of the site
}

/* An offset frequency of 5hz is added to the dominant frequency. In the
span of a single second, the offset frequency is added every 1/16 second, whereupon the
brainwave signal is measured again and the offset frequency is added to the new brainwave
signal, recalculating and repeating the process every 1/16 of a second for the duration of one
second. */
void EEGSite::deliverTreatment(){
    qDebug() << "Treating site #"<< id;
    qDebug() << "Initial baseline: " << baselineFrequency;
    for (int i=0; i<16; i++){
        qDebug() << "Adding 5hz to the dominant frequency";
        baselineFrequency += 5;
        qDebug() << "Frequency is now " << baselineFrequency;
    }
    qDebug() << "Site #"<< id << " has now been treated and has a new dominant frequency of "<<baselineFrequency;
}

// helper function for deliver treatment that recalculates the brainwave signal after each offset frequency of 5hz is added
int EEGSite::calcNewBaseline(int baselineFrequency){
    return baselineFrequency -1; // fixme: randomize this
}

void EEGSite::contactLost(){
    qDebug() << "contact lost on #" << id;
}
