#ifndef BATTERYCLASS_H
#define BATTERYCLASS_H
//perfect, now make button Start PCc that Will Start and will call StartTimer That  makes timer go off
#include <QObject>
#include <QTimer>
class Batteryclass: public QObject
{
    Q_OBJECT;
public:
    Batteryclass();
     void BatteryOff(); // when Battery empty or status is false  This function will  make qtimer stop going if battery is empty or off and it This gets called by decreasebattery or batteryon
     bool status; //says if battery is on or off
     int getBatteryHealth();
signals:    //Qtimer will always call decreasebattery
    void LowBattery(); // connect to a function in main
    void EmptyBattery(Batteryclass *battery);  // This signal is made when batteryhealth is 0 and pput sstatus false  and when it is call  batteryoff in main
    void updateLabel(int BatteryHealth); //updates % label
public slots:
    void DecreaseBattery(); //decreases battery timer calls this function here its normal - and then if health <=20 emit Lowbattery if health==0 emit emptybattery
    void batteryHealthregen(); // If user in main presses Button Recharge It calls this function. no need to stop the timer put health =100
    void BatteryOn();// This starts the pc allso when you start the timer change the percentage to 100% instead of ....%   also make status true is on
private:
    int BatteryHealth; //Tracks bat
    QTimer *timer;
};

#endif // BATTERYCLASS_H
