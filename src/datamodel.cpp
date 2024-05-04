#include "datamodel.h"
#include "defs.h"
#include <iostream>

DataModel::DataModel()
{
    progressBarTimer = new QTimer(this);
    clockTimer = new QTimer(this);
    pauseTimer = new QTimer(this);
    connect(progressBarTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(pauseTimer, SIGNAL(timeout()), this, SLOT(sessionTimeout()));
    NeuroObject = new neurofeedbacksystem();
}

void DataModel::addSession(SessionData* compeletedSession){
   sessions.append(compeletedSession);
   emit addSessionToLog(compeletedSession);
}

void DataModel::startNewSession(){ //Ok so herre  just have the Dark Greenn coloour  in the else sstatments

    if(currentSession.isPaused){    // if we've paused the session we don't want to start from scratch
        if(!NeuroObject->isConnected){
            std::cout<<"Please reconnect the electrodes before resuming a session"<<std::endl;
        }
        else{
            progressBarTimer->start(TIME_INTERVAL);
            clockTimer->start(SECOND_INTERVAL);
            currentSession.isPaused = false;
            currentSession.isActive = true;
            NeuroObject->unpauseDevice();
            emit darkgreen();
        }
    }
    else{   // if its not paused we can restart/start a new session
        if(!NeuroObject->isConnected){
            std::cout<<"Please connect the electrodes before beginning a session"<<std::endl;
        }
        else{
            currentSession.startSession();
            NeuroObject->startSession();
            progressBarTimer->start(TIME_INTERVAL);
            clockTimer->start(SECOND_INTERVAL);
            emit darkgreen();
        }
    }
}

void DataModel::pauseCurrentSession(){  //Make treatment signal light green
    if(!NeuroObject->isConnected){
        std::cout<<"Electrodes disconnected during session"<<std::endl;
    }
    std::cout<<"Session paused, you have 5 minutes to resume session before the session gets wiped"<<std::endl;
    currentSession.pauseSession();
    NeuroObject->pauseSession();
    pauseTimer->start(SECOND_INTERVAL*60*5);
    progressBarTimer->stop();
    clockTimer->stop();
    emit updateProgressBar(currentSession.getProgress());
    emit updateTimer(currentSession.getTimeRemaining());
    emit lightgreen();

}

void DataModel::cancelCurrentSession(){ //Hhave  this emit for  light green
    currentSession.cancelSession();
    emit updateProgressBar(currentSession.getProgress());
    emit updateTimer(currentSession.getTimeRemaining());
    progressBarTimer->stop();
    clockTimer->stop();
    NeuroObject->endSession();
    emit lightblue();
    emit lightred();
    emit  lightgreen();
}

void DataModel::endCurrentSession(){ //have this emit for light green
    currentSession.endSession();
    emit updateProgressBar(currentSession.getProgress());
    emit updateTimer(currentSession.getTimeRemaining());
    addSession(NeuroObject->currentSession);
    emit lightblue();
    emit lightred();
    emit lightgreen();
}

void DataModel::increaseProgress(){
    if(currentSession.getProgress()<100){
        currentSession.incrementProgress();
        emit updateProgressBar(currentSession.getProgress());
    }
    if(currentSession.getProgress()>=100){
        progressBarTimer->stop();
        endCurrentSession();
    }
}

void DataModel::showTime(){
    std::cout<<currentSession.getTimeRemaining()<<" seconds left in this session"<<std::endl;
    if(currentSession.getTimeRemaining()>0){
        currentSession.decrementTime();
        emit updateTimer(currentSession.getTimeRemaining());
    }
    else{
        clockTimer->stop();
    }
}

void DataModel::connectNodes(){ // Make This dark blue when Yu coonnect nnodes
    if(!NeuroObject->battery->status){
        std::cout<<"Device is not powered on"<<std::endl;
    }
    else if(currentSession.isActive){
        std::cout<<"Session is already on going"<<std::endl;
    }
    else if(NeuroObject->isConnected){
        std::cout<<"Electrodes already connected"<<std::endl;
    }
    else{
        NeuroObject->isConnected=true;
        emit colourblue();
        emit lightred();
    }
}

void DataModel::sessionTimeout(){
    std::cout<<"Session was not resumed in 5 minutes, session terminating"<<std::endl;
    endCurrentSession();
}

void DataModel::disconnectNodes(){
    if(!NeuroObject->isConnected){
        std::cout<<"Electrodes aren't current in use"<<std::endl;
    }
    else if(currentSession.isActive){
        std::cout<<"Electrodes disconnected mid-session, pausing session"<<std::endl;
        pauseCurrentSession();
        NeuroObject->isConnected=false;
        emit lightblue();
        emit darkred();
    }
    else{
        NeuroObject->isConnected=false;
        std::cout<<"Electrodes disconnected"<<std::endl;
        emit lightblue();
        emit darkred();
    }
}
