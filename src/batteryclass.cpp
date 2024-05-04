#include "batteryclass.h"
#include "defs.h"

Batteryclass::Batteryclass()
{BatteryHealth= 100;
status= false;
timer=nullptr;
}
void Batteryclass::BatteryOn(){
      if(status==true){
          status=false;
          BatteryOff();
      }
      else{
          timer = new QTimer(this);
           connect(timer, SIGNAL(timeout()), this, SLOT(DecreaseBattery()));
           timer->start(SECOND_INTERVAL);
           status = true;
           emit updateLabel(BatteryHealth);
      }

}

void Batteryclass::BatteryOff(){
       if (BatteryHealth <= 0 || !status) {
           timer->stop();
           status = false;
            emit updateLabel(200); //this will update label to ....% 200 is just a key tthat program knows its to write string %
       }
}

int Batteryclass::getBatteryHealth(){return BatteryHealth;}

void Batteryclass::DecreaseBattery(){
       if (BatteryHealth > 0)
           BatteryHealth--;
           emit updateLabel(BatteryHealth); // signal that runs function that shows the % in the ui  find out how to pass batteryhealth as parameters
       if (BatteryHealth == 20)
           emit LowBattery(); // make a message to user that ur low
       if(BatteryHealth<=0)
           emit EmptyBattery (this); //make a message to user that battery ran out and use function batteryoff() at the end

}

void Batteryclass::batteryHealthregen(){   
      if(status==true){
          BatteryHealth=100;
              emit updateLabel(100);
      }
      else{
          BatteryHealth=100;
         emit updateLabel(200);
      }

}
