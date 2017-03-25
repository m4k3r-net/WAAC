/*
 // © 2noodles llc
 // minh@2noodles.com
*/

#ifndef PWM4_h
#define PWM4_h

#define CHANNEL 4

#include "Arduino.h"
#include "Device.h" 

class PWM4 : public Device
{
  public:
    PWM4(char *in_name, int in_dependent_device_id);
    ~PWM4(); // destructor

    void loop(); // required
    void setSuspendTime(boolean in_suspend); //override
    
    void switchOn();
    void switchOff();
    void toggleState();
    
    void setEvent(char *in_string);
    void getEvent(char *string);
    
    int getDependentDevice();
    void setDependentDevice(int id);
    
    int dependentDeviceId;
    Device *dependentDeviceObject;
    
    void setPins(int red, int green, int blue, int white);
    void getPins(int *inArray);
    
    void setPWMs(int in_red, int in_green, int in_blue, int in_white);
    
  private:
    
    typedef struct {
        int pwm[4]; // - for each events, value of pwm
        int initColor;
        int currentColor;
        int pin;
        long colorStartTime;
    } colorAux;
    
    colorAux color[CHANNEL];
    

    boolean isDay;
    uint8_t timedIndexCounter;
    int hour[5];
    int minute[5];
    int second[5];
    int hourDuration[5];
    int minuteDuration[5];
    int secondDuration[5];
    
    long initMillis;
    char dow[8];
    bool oneTime;
    
};

#endif