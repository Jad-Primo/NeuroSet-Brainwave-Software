#ifndef SESSIONDATA_H
#define SESSIONDATA_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include "defs.h"

class SessionData: public QObject
{
    Q_OBJECT
public:
    SessionData();
    void startSession();
    void endSession();
    void pauseSession();
    int getProgress();
    QString getStartTime();
    void cancelSession();
    QString formattedDateDisplay;
    void incrementProgress();
    bool isPaused;
    bool isActive;
    int getTimeRemaining();
    void decrementTime();
    void addToBaseline(int order,float amount);
    void addToWaveform(int round,int site, float frequency,float amplitude);
    void addToDominantList(int round, int site, float dominantNum);
    void addToTimelist(QString time, int placement);
    float dominantFrequencyList[NUM_ROUNDS][NUM_SITES]; // change to 7 if needed
    float waveformsList[NUM_ROUNDS][NUM_SITES][2]; // change to 7 if needed
    float baseLineList[2]; // change to 7 if needed

private:
    int progress;
    int treatmentTime;
    int timeRemaining;
    QDateTime startDate;
    QDateTime endDate;
    QString startTime;
};

#endif // SESSIONDATA_H
