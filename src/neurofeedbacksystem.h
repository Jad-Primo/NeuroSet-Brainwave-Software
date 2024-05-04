#ifndef NEUROFEEDBACKSYSTEM_H
#define NEUROFEEDBACKSYSTEM_H

#include <QObject>
using namespace std;
#include <vector>
#include <random>
#include <cmath> // For sine wave calculations
#include "batteryclass.h"
#include "defs.h"
#include "sessiondata.h"

class neurofeedbacksystem:public QObject
{
    Q_OBJECT
public:
    neurofeedbacksystem();
    // System Battery
    Batteryclass* battery;
    SessionData* currentSession;

    bool sessionOn = false;
    bool isConnected = false; //boolean for whether the sites have been connected

    int randomInRange(int low, int high);
    void simulateEEGSite(int round, int site);
    void simulateRound(int round);
    void startSession();
    void pauseSession();
    void unpauseDevice();
    public slots:
        void handleTreatment();
        void endSession();
private:
    int currentRound;
    QTimer* delayTreatment;
    std::chrono::milliseconds pausedTime;
};

#endif // NEUROFEEDBACKSYSTEM_H
