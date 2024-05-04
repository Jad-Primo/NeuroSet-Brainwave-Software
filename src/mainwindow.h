#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "batteryclass.h"
#include "datamodel.h"
#include "sessiondata.h"
#include <QListWidget>
#include <QDebug>
#include <QComboBox>
#include "defs.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QtCharts>
#include<QComboBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Batteryclass *battery;
    DataModel *dataModel;
    int randomRange(int low, int high);
    QLineSeries *series;
    QChart *chart;
    QChartView *chartView ;
    QVBoxLayout *layout ;
private:
    Ui::MainWindow *ui;


 public slots:
     void darkred();
     void lightred();
     void darkgreen();
     void lightgreen();
     void darkblue();
     void lightblue();
     void Waveclick();
     void almostclosing();
     void nopower(Batteryclass *battery);
     void updatebatterylabel(int health);
     void addSessionToLog(SessionData* session);
     void displayProgress(int num);
     void timeToScreen(int time);
     void menuPressed();
     void startSessionMenu();
     void sessionStart();
     void dateTimeMenu();
     void sessionHistoryMenu();
     void backToMainMenu();
     void stopCurrentSession();
     void powerOnDevice();
     void connectNodeHandler();
     void disconnectNodeHandler();
};
#endif // MAINWINDOW_H
