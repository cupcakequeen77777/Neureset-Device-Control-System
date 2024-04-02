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
    qDebug() << "*****\nTreating site #"<< id;
    qDebug() << "Initial baseline: " << baselineFrequency;
    for (int i=0; i<16; i++){
        //add 5hz to the dominant frequency and recalculate the baselineFrequency
        baselineFrequency = calcNewBaseline(baselineFrequency+5);
    }
    qDebug() << "Site #"<< id << " has now been treated and has a new dominant frequency of "<<baselineFrequency;
}

// helper function for deliver treatment that recalculates the brainwave signal after each offset frequency of 5hz is added
// current logic is the new baseline is +/- 5 from the given frequency
int EEGSite::calcNewBaseline(int baselineFrequency){
    //generate a random number between -5 and +5
    int frequencyChange = rand() % 10 - 5;
    return baselineFrequency + frequencyChange;
}


void EEGSite::disconnectSite(int id){
    qDebug() << "contact lost on #" << id;
    emit contactLost();
}
