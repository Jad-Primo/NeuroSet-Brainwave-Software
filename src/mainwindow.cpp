#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "batteryclass.h"
#include <QDebug>
#include <QComboBox>
#include "defs.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include <cmath>
#include <QDateTime>
#include <QDir>
QT_CHARTS_USE_NAMESPACE
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Initialising UI elements
    ui->setupUi(this);
    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chartView = new QChartView(chart);
    layout = new QVBoxLayout(ui->waveWidget);
    layout->addWidget(chartView);
     QValueAxis *axisX = new QValueAxis;
     axisX->setRange(0, 100);
     axisX->setTitleText("Time");
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);
     QValueAxis *axisY = new QValueAxis;
      axisY->setRange(-6,6 );
       axisY->setTitleText("Amplitudee");
       chart->addAxis(axisY, Qt::AlignLeft);
      series->attachAxis(axisY);
  //up above made the chart

    QPixmap pixmap(":/pictures/battery"); //this is just a temp well make it better looking
    ui->Battery->setPixmap(pixmap);
    ui->Battery->setScaledContents(true);

    QPixmap Green(":/pictures/lightgreen");
    ui->Greenlight->setPixmap(Green);
    ui->Greenlight->setScaledContents(true);

    QPixmap Redd(":/pictures/lightred");
    ui->redlight->setPixmap(Redd);
    ui->redlight->setScaledContents(true);


    QPixmap blue(":/pictures/lightblue");
    ui->bluelight->setPixmap(blue);
    ui->bluelight->setScaledContents(true); //TThese  Connections AAre to chhangee the colours




    ui->progressBar->setOrientation(Qt::Horizontal);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    //Above just Makes the label a image of dataModel->NeuroObject->battery
    dataModel = new DataModel();

    ui->sessionList->hide();
    ui->sessionWidget->hide();
    ui->dateTextBox->hide();
    ui->menuList->hide();
    ui->waveWidget->hide();

    // Slot connections
    connect(dataModel,&DataModel::updateProgressBar,this,&MainWindow::displayProgress);
    connect(ui->startSession,&QPushButton::clicked, this, &MainWindow::sessionStart);
    connect(ui->pauseSession,&QPushButton::clicked, dataModel, &DataModel::pauseCurrentSession);
    connect(ui->connect,&QPushButton::clicked, this, &MainWindow::connectNodeHandler);
    connect(ui->disconnect,&QPushButton::clicked, this, &MainWindow::disconnectNodeHandler);
    connect(dataModel,&DataModel::addSessionToLog,this,&MainWindow::addSessionToLog);
    connect(dataModel,&DataModel::updateTimer,this,&MainWindow::timeToScreen);
    connect(ui->menuList,&QListWidget::itemClicked,this,&MainWindow::menuPressed);
    connect(ui->backButton,&QPushButton::clicked,this,&MainWindow::backToMainMenu);
    connect(ui->stopSession,&QPushButton::clicked,this,&MainWindow::stopCurrentSession);
    //ConnectPower BButton
    connect(ui->PowerButton,&QPushButton::clicked,this,&MainWindow::powerOnDevice);

    //Connect Signals of battery here
    connect(dataModel->NeuroObject->battery, &Batteryclass::LowBattery, this, &MainWindow::almostclosing);
    connect(dataModel->NeuroObject->battery, &Batteryclass::EmptyBattery, this, &MainWindow::nopower);
    connect(dataModel->NeuroObject->battery, &Batteryclass::updateLabel, this, &MainWindow::updatebatterylabel);
    connect(ui->RegenBattery,&QPushButton::clicked,dataModel->NeuroObject->battery, &Batteryclass::batteryHealthregen);
//Workinng belllow here  need to  make a new slot.


    ///mmake chart using Egg chart
    connect(ui->eegBox,QOverload<int>::of(&QComboBox::activated),this, &MainWindow::Waveclick);

    //Here is the coolours sswitch
    connect(dataModel,&DataModel::colourblue,this,&MainWindow::darkblue);
    connect(dataModel,&DataModel::lightblue,this,&MainWindow::lightblue);
    connect(dataModel,&DataModel::darkred,this,&MainWindow::darkred);
    connect(dataModel,&DataModel::lightred,this,&MainWindow::lightred);
    connect(dataModel,&DataModel::darkgreen,this,&MainWindow::darkgreen);
    connect(dataModel,&DataModel::lightgreen,this,&MainWindow::lightgreen);

    for(int i = 0 ; i < NUM_ROUNDS; i++){
        ui->roundBox->addItem(QString("Round #"+QString::number(i+1)));
    }
    for(int i = 0 ; i < NUM_SITES; i++){
        ui->eegBox->addItem(QString("Node #"+QString::number(i+1)));
    }
}

void MainWindow::powerOnDevice(){
    if(dataModel->NeuroObject->battery->status){
        dataModel->cancelCurrentSession(); // cancel any active sessions
        dataModel->NeuroObject->battery->BatteryOn();
        ui->menuList->hide();
        ui->sessionList->hide();
        ui->sessionWidget->hide();
        ui->dateTextBox->hide();
        ui->waveWidget->hide();
        qDebug()<<"Device powered off";
    }
    else if(dataModel->NeuroObject->battery->getBatteryHealth()!=0){
        dataModel->NeuroObject->battery->BatteryOn();
        ui->menuList->show();
        ui->sessionList->hide();
        ui->sessionWidget->hide();
        ui->dateTextBox->hide();
        qDebug()<<"Device powered on";
    }
    else{
        qDebug()<<"Device has no energy left, please recharge before use";  //means we ran out of battery so we have to charge
    }
}


void MainWindow::Waveclick(){
    if(!dataModel->NeuroObject->battery->status){
        qDebug()<<"Device must be on with battery power to see wave outputs";
    }
    if(!dataModel->NeuroObject->isConnected){
        qDebug()<<"Must have electronodes connected to see wave outputs ";
    }
    else{
        if(dataModel->sessions.size()==0){
            QString generalDisplayText = "No session data to display at the moment (try selecting a node again once a session has been completed to see session info)";
            QString siteDisplayText="No site data to display at the moment (try selecting a node again once a session has been completed to see site info)";
            ui->sessionInfo->setText(generalDisplayText);
            ui->siteInfo->setText(siteDisplayText);
        }
        else{
            int sessionIndex = ui->sessionBox->currentIndex();
            int roundIndex = ui->roundBox->currentIndex();
            int eegIndex = ui->eegBox->currentIndex();

            QString generalDisplayText = "Session began on "+dataModel->sessions[sessionIndex]->formattedDateDisplay+
            " and had an overall pre-treatment baseline of "+QString::number(dataModel->sessions[sessionIndex]->baseLineList[0])+
            " and an post-treatment baseline of "+QString::number(dataModel->sessions[sessionIndex]->baseLineList[1]);

            QString siteDisplayText="The site #"+QString::number(eegIndex+1)+" had a waveform frequency of "
            +QString::number(dataModel->sessions[sessionIndex]->waveformsList[roundIndex][eegIndex][0])+
            " and amplitude of "+QString::number(dataModel->sessions[sessionIndex]->waveformsList[roundIndex][eegIndex][1]) +
            " with a dominating frequency of "+QString::number(dataModel->sessions[sessionIndex]->dominantFrequencyList[roundIndex][eegIndex]);
            ui->sessionInfo->setText(generalDisplayText);
            ui->siteInfo->setText(siteDisplayText);
        }

        int generateBand = randomRange(1,4);

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
         int frequency = randomRange(low, high);
         int amplitude = randomRange(1, 6);
          series->clear();
                 for (int x = 0; x <= 100; x++) {
                     double y = amplitude * (sin((1.0/frequency) *x));  //periood =  1 /frreq . Info on this function   a sin(B * x +C)+D  a is amp  c d not used    normaly b is   2p/u= period  .  u value is a number given  ex u =  2  period = pi  feq =  1/ppi  period is 1/ 1/1pi  =pi
                     series->append(x, y);
                 }
    }
}
int MainWindow::randomRange(int low, int high) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(low, high);
    return uni(rng);
}
void MainWindow::darkred() {
    QPixmap redd(":/pictures/brightred.png");
    ui->redlight->setPixmap(redd);
    ui->redlight->setScaledContents(true);
}
void MainWindow::darkblue() {
    QPixmap blue(":/pictures/darkblue.png");
    ui->bluelight->setPixmap(blue);
    ui->bluelight->setScaledContents(true);
}
void MainWindow::darkgreen() {
    QPixmap Green(":/pictures/darkgreen.png");
    ui->Greenlight->setPixmap(Green);
    ui->Greenlight->setScaledContents(true);
}

void MainWindow::lightred() {
    QPixmap redd(":/pictures/lightred.png");
    ui->redlight->setPixmap(redd);
    ui->redlight->setScaledContents(true);
}
void MainWindow::lightblue() {
    QPixmap blue(":/pictures/lightblue.png");
    ui->bluelight->setPixmap(blue);
    ui->bluelight->setScaledContents(true);
}
void  MainWindow::lightgreen() {
    QPixmap Green(":/pictures/lightgreen.png");
    ui->Greenlight->setPixmap(Green);
    ui->Greenlight->setScaledContents(true);
}


void MainWindow::displayProgress(int num){
    if (num>=100){
    qDebug() <<"session is complete :"<<num;
    }
    ui->progressBar->setValue(num);
}

void MainWindow::addSessionToLog(SessionData* session){
    ui->sessionBox->addItem("Session started at "+session->getStartTime());
    ui->sessionList->addItem(QString("Session #"+QString::number(dataModel->sessions.size())+" that started on "+session->formattedDateDisplay));
}

void MainWindow::timeToScreen(int time){
    ui->timeDisplay->display(time);
}

void MainWindow::connectNodeHandler(){
   if(dataModel->NeuroObject->battery->status){
        ui->waveWidget->show();
   }
   dataModel->connectNodes();
}

void MainWindow::disconnectNodeHandler(){
    dataModel->disconnectNodes();
    if(!dataModel->NeuroObject->isConnected){
        ui->waveWidget->hide();
    }
}

  //Make a function that for lowdataModel->NeuroObject->battery signal  that will Show user  a message to Be careful Less than 20%
void MainWindow::almostclosing(){
    qDebug() << "Battery is below 20% watch out ";
}
   //Make a  function for empty dataModel->NeuroObject->battery signal That calls dataModel->NeuroObject->batteryoff function  and says Power Off To user SScreen
void MainWindow::nopower(Batteryclass *battery){
     qDebug() << "Battery ran out ";
     if(dataModel->currentSession.isActive){
        dataModel->cancelCurrentSession();
     }
     battery->BatteryOff();
     backToMainMenu();
     ui->menuList->hide();
     ui->waveWidget->hide();
}

void MainWindow::updatebatterylabel(int health){
    if(health== 200){
        ui->Percentlabel->setText("......%");
    }

    else{
        QString message = QString::number(health);
        ui->Percentlabel->setText(message+" %");
    }

}

void MainWindow::menuPressed(){
    if(ui->menuList->currentIndex().row()==0){
        ui->menuList->hide();
        startSessionMenu();
    }
    else if(ui->menuList->currentIndex().row()==1){
        ui->menuList->hide();
        sessionHistoryMenu();
    }
    else{
        ui->menuList->hide();
        dateTimeMenu();
    }
}

void MainWindow::startSessionMenu(){
    ui->sessionWidget->show();
}

void MainWindow::sessionHistoryMenu(){
    ui->sessionList->show();
}

void MainWindow::dateTimeMenu(){
    ui->dateTextBox->show();
    QDateTime date = QDateTime::currentDateTime();
    QString textDisplay = "Date is: "+date.toString("dd.MM.yyyy hh:mm:ss");
    ui->dateTextBox->setText(textDisplay);
}

void MainWindow::sessionStart(){
    if(dataModel->currentSession.isActive){
        qDebug() <<"Session already active";
    }
    else{
        if(ui->sessionWidget->isVisible()){
            dataModel->startNewSession();
        }
        else{
            qDebug() <<"Cannot start session from this screen, go to start session screen to start a session";
        }
    }
}

void MainWindow::backToMainMenu(){
    if(ui->menuList->isVisible()){
        qDebug() <<" Cannot go back as we are on the main menu";
    }
    if(ui->sessionList->isVisible()){
        ui->sessionList->hide();
        ui->menuList->show();
    }
    if(ui->dateTextBox->isVisible()){
        ui->dateTextBox->hide();
        ui->menuList->show();
    }
    if(ui->sessionWidget->isVisible()){
        if(dataModel->currentSession.isActive){
            qDebug() <<"Cannot back out as session is active";
        }
        else{
            ui->sessionWidget->hide();
            ui->menuList->show();
        }
    }
}

void MainWindow::stopCurrentSession(){
    if(ui->sessionWidget->isVisible()){
        dataModel->cancelCurrentSession();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

