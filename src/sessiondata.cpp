#include "sessiondata.h"
#include "neurofeedbacksystem.h"
#include "defs.h"

SessionData::SessionData()  // This constructor is for new sessions that have not completed
{
    progress = 0;
    isActive = false;
    isPaused = false;
    timeRemaining = TREATMENT_TIME;
    startDate = QDateTime::currentDateTime();
    startTime = startDate.toString("hh:mm:ss");
    formattedDateDisplay = startDate.toString("dd.MM.yyyy hh:mm:ss");
}


void SessionData::startSession(){
    if(isPaused){
        isPaused = false;
    }
    else{
        progress = 0;
        isActive = true;
        timeRemaining=TREATMENT_TIME;
        startDate = QDateTime::currentDateTime();
        startTime = startDate.toString("hh:mm:ss");
        formattedDateDisplay = startDate.toString("dd.MM.yyyy hh:mm:ss");
    }
}

void SessionData::pauseSession(){
    isPaused = true;
    isActive = false;
}

void SessionData::cancelSession(){
    progress = 0;
    isActive = false;
    timeRemaining=TREATMENT_TIME;
}

void SessionData::endSession(){
    progress = 0;
    isActive = false;
}

QString SessionData::getStartTime(){return startTime;}

int SessionData::getTimeRemaining(){
    return timeRemaining;   // returns how much time is left is the current session
}

void SessionData::incrementProgress(){
    if(progress<100){
        progress++;
    }
}

void SessionData::decrementTime(){
    timeRemaining--;
}

void SessionData::addToBaseline(int order,float amount){baseLineList[order]=amount;}
void SessionData::addToWaveform(int round,int site, float frequency,float amplitude){
    waveformsList[round][site][0]=frequency;
    waveformsList[round][site][1]=amplitude;
}
void SessionData::addToDominantList(int round, int site, float dominantNum){dominantFrequencyList[round][site]=dominantNum;}
int SessionData::getProgress(){return progress;}
