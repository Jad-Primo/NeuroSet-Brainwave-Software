#include "neurofeedbacksystem.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "defs.h"
#include <chrono> // Used for gen random number seed

using namespace std;

neurofeedbacksystem::neurofeedbacksystem()
{
    battery = new Batteryclass();
    currentSession = new SessionData();
    delayTreatment = new QTimer();
    currentRound= 0;
    pausedTime = std::chrono::milliseconds(0);
    connect(delayTreatment, SIGNAL(timeout()), this, SLOT(handleTreatment()));
}

int neurofeedbacksystem::randomInRange(int low, int high) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(low, high);
    return uni(rng);
}

void neurofeedbacksystem::simulateEEGSite(int round, int site) {
    int generateBand = randomInRange(1, 4);

    int high = 0;
    int low = 0;

    switch (generateBand) {
        case 1:
            low = 1;
            high = 4;
            break;
        case 2:
            low = 4;
            high = 8;
            break;
        case 3:
            low = 8;
            high = 12;
            break;
        case 4:
            low = 12;
            high = 30;
            break;
    }

    int sumFreq = 0;
    int sumAmp = 0;

    float sumEq1 = 0;
    float sumEq2 = 0;

    for (int i = 0; i < 3; i++) {
        int frequency = randomInRange(low, high);
        sumFreq += frequency;
        int amplitude = randomInRange(0, 6);
        sumAmp += amplitude;

        sumEq1 += frequency * pow(amplitude, 2);
        sumEq2 += pow(amplitude, 2);
    }

    float waveFreq = sumFreq / 3.0;
    float waveAmp = sumAmp / 3.0;
    currentSession->addToWaveform(round,site,waveFreq,waveAmp);
    float dominantFreq = sumEq1 / sumEq2;
    currentSession->addToDominantList(round,site,dominantFreq);
}

void neurofeedbacksystem::simulateRound(int round) {

    for (int i = 0; i < NUM_SITES; i++) { // Simulates EEG sites
        simulateEEGSite(round, i);
    }
    int offset = 0;

    switch (round) { // Offset for each round
        case 0:
            offset = 5;
            break;
        case 1:
            offset = 10;
            break;
        case 2:
            offset = 15;
            break;
        case 3:
            offset = 20;
            break;
    }

    if(round == 0) { // Start BaseLine
        float sum = 0;
        for(int i = 0; i < NUM_SITES; i++) {
            sum += currentSession->dominantFrequencyList[round][i];
        }
        currentSession->addToBaseline(0,sum/NUM_SITES);

    }
    for (int i = 0; i < NUM_SITES; i++) { // Treatment with Offsets

        currentSession->dominantFrequencyList[round][i] += (0.0625) * (currentSession->dominantFrequencyList[round][i] + offset);
    }
    if(round == 3) { // End BaseLine
        float sum = 0;
        for(int i = 0; i < NUM_SITES; i++) {
            sum += currentSession->dominantFrequencyList[round][i];
        }
        currentSession->addToBaseline(1,sum/NUM_SITES);
    }
    cout << "Round #" << round+1 <<" applying treatment offset for dominant frequency "<<endl;
    cout << "Round #" << round+1 << " Analysis and treatment complete\n";
}
void neurofeedbacksystem::unpauseDevice(){
    cout<<"Device treatment unpaused"<<endl;
    delayTreatment->start(pausedTime); //basically resumes the timer from when we unpaused it
}

void neurofeedbacksystem::pauseSession(){
    cout<<"Device treatment paused"<<endl;
    pausedTime = delayTreatment->remainingTimeAsDuration();
    delayTreatment->stop();
}

void neurofeedbacksystem::endSession(){
    cout <<"Completed final analysis"<<endl;
    cout << "Therapy Session Ended"<<endl;
    currentRound = 0;
    delayTreatment->stop();
}

void neurofeedbacksystem::handleTreatment(){
    if(pausedTime!=std::chrono::milliseconds(0)){
        delayTreatment->start(SLEEP_TIME);
        pausedTime = std::chrono::milliseconds(0);
    }
    if(currentRound<4){
        simulateRound(currentRound);
        currentRound++;
        if(currentRound<4){
            cout << "Round #" << currentRound+1 <<" gathering EEG site information" << endl;
        }
        else{
            cout<<"Commencing final baseline analysis"<<endl;
        }
    }
    else{
        endSession();
    }
}


void neurofeedbacksystem::startSession() {
    currentSession = new SessionData();
    cout << "Therapy Session Started"<<endl;
    if(currentRound == 0){
      cout << "Round #" << currentRound+1 <<" gathering EEG site information" << endl;
    }
    delayTreatment->start(SLEEP_TIME+SECOND_INTERVAL);
}




