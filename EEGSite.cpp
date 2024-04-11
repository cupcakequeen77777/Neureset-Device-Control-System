#include "EEGSite.h"

EEGSite::EEGSite(){
    EEGSite(1);
}

EEGSite::EEGSite(int i){
    id = i;
    isConnected = true;
    baselineFrequency = -1;
    generateWaveForm();
}

void EEGSite::generateWaveForm(){
    listenAlphaFrequencies(alpha);
    listenBetaFrequencies(beta);
    listenDeltaFrequencies(delta);
    listenThetaFrequencies(theta);
}

bool EEGSite::getIsConnected(){
    return isConnected;
}

int EEGSite::getBaselineFrequency(){
    return baselineFrequency;
}

void EEGSite::setBaseline(int baseline){
    baselineFrequency = baseline;
}

int EEGSite::calculateBaseline(int* data){
    int sum = 0;
    for (int i=0; i<60; ++i){
        sum += data[i];
    }
    baselineFrequency = sum / 60;

    return baselineFrequency;
}

void EEGSite::listenAlphaFrequencies(int *data){
    for (int i=0; i<60; i++){
        data[i] = rand() % 5 + 8;
    }
}

void EEGSite::listenBetaFrequencies(int *data){
    for (int i=0; i<60; i++){
        data[i] = rand() % 19 + 12;
    }
}

void EEGSite::listenDeltaFrequencies(int *data){
    for (int i=0; i<60; i++){
        data[i] = rand() % 4 + 1;
    }
}

void EEGSite::listenThetaFrequencies(int *data){
    for (int i=0; i<60; i++){
        data[i] = rand() % 4 + 4;
    }
}

//this function delivers treatment to the EEG site and then reanalysis/recalcuates the baseline frequency
void EEGSite::deliverTreatment(int offsetFrequency){
    int initialBaseline = baselineFrequency;
        for (int i=0; i<16; i++){
            //add offset frequency to the dominant frequency and recalculate the baselineFrequency
            baselineFrequency = calcNewBaseline(baselineFrequency, offsetFrequency);
        }
        qInfo() << "Site #"<< id << " has now been treated and has gone from " <<initialBaseline << "hz to "<<baselineFrequency << "hz";
}

// helper function for deliver treatment that recalculates the brainwave signal after each offset frequency is added
int EEGSite::calcNewBaseline(int baselineFrequency, int offsetFrequency){
    //generate a random number in the range of +/- the offsetFrequency for the given round
    int frequencyChange = (rand() % (1 + (offsetFrequency/5)*2)) - offsetFrequency/5;

    //edge cases to prevent frequency from going out of bounds
    if(baselineFrequency + frequencyChange < 0){
        return 0;
    }
    if(baselineFrequency + frequencyChange > 40){
        return 40;
    }
    return baselineFrequency + frequencyChange;
}


void EEGSite::disconnectSite(){
    qDebug() << "contact lost on #" << id;
    isConnected = false;
    emit contactLost(true);
}

void EEGSite::reconnectSite(){
    qDebug() << "contact reconnected on #" << id;
    isConnected = true;
    emit contactLost(false);
}

int* EEGSite::getWaveform(char type){
    if(type == 'a'){
        return alpha;
    }
    else if (type == 'b'){
        return beta;
    }
    else if(type == 'd'){
        return delta;
    }
    else{
        return theta;
    }
}
