#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QVector>
#include "sessiondata.h"
#include <QTimer>
#include "neurofeedbacksystem.h"

class DataModel: public QObject
{
    Q_OBJECT
public:
    QVector<SessionData* > sessions;
    SessionData currentSession;
    neurofeedbacksystem* NeuroObject;

    DataModel();
    void addSession(SessionData* compeletedSession);
    void startNewSession();
    void pauseCurrentSession();
    void cancelCurrentSession();
    void endCurrentSession();
    void connectNodes();
    void disconnectNodes();

    public slots:
        void increaseProgress();
        void showTime();
        void sessionTimeout();
    signals:
        void colourblue();
         void lightblue();
         void darkred();
         void lightred();
         void lightgreen();
         void darkgreen();
        void updateProgressBar(int progress);
        void resumeSession(QString text);
        void addSessionToLog(SessionData* session);
        void updateTimer(int time);
private:
        QTimer *clockTimer;
        QTimer *progressBarTimer;
        QTimer *pauseTimer;
};

#endif // DATAMODEL_H
