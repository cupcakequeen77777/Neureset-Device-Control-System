#include "EEGSite.h"

EEGSite::EEGSite(){
    EEGSite(1);
}

EEGSite::EEGSite(int i){
    id = i;
    isConnected = true;
    baselineFrequency = 10; //this is a default frequency, but the generateBaseline() func will actually calculate the frequency of the site
    generateWaveForm();
}

void EEGSite::generateWaveForm(){
    for(int i=0; i<60; ++i){
        waveForm[i] = id+i;
    }
}

bool EEGSite::getIsConnected(){
    return isConnected;
}

int EEGSite::getBaselineFrequency(){
    return baselineFrequency;
}

int EEGSite::calculateBaseline(int* data){
    int sum = 0;
    for (int i=0; i<60; ++i){
        sum += data[i];
    }
    baselineFrequency = sum / 60;

    return baselineFrequency;
}

//this function delivers treatment to the EEG site in four rounds. Each round increase then offset frequency by 5 and then add that
//offset frequency 16 times over the span of a second to the baseline frequency and reanalysis/recalcuates the baseline frequency
void EEGSite::deliverTreatment(int offsetFrequency){
    int initialBaseline = baselineFrequency;
        for (int i=0; i<16; i++){
            //add offset frequency to the dominant frequency and recalculate the baselineFrequency
            baselineFrequency = calcNewBaseline(baselineFrequency, offsetFrequency);
        }
        qInfo() << "Site #"<< id << " has now been treated and has gone from " <<initialBaseline << "hz to "<<baselineFrequency << "hz";
}

// helper function for deliver treatment that recalculates the brainwave signal after each offset frequency is added
// current logic is the new baseline is the old baseline +/- some value in the range of the offset frequency divided by 5.
// For example if the offset frequency was 15hz the baseline would be modified by some value in the range (-3, 3)
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

int* EEGSite::getWaveform(){
    return waveForm;
}
