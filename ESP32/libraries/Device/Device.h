// © 2noodles llc
// minh@2noodles.com


#ifndef Device_h
#define Device_h

#include "Arduino.h"

class Device
{
  public:
    Device();
    virtual ~Device() {}; //destructor
    virtual void loop() = 0; //child must define - will be called from delegate loop()
    
    //can override
    virtual void setSuspendTime(bool in_suspend);
    virtual const char * read(){ return "0.0"; };
    virtual void setEvent(char *in_string){};
    virtual void getEvent(char *string){};
    virtual int getDependentDevice(){};
    virtual void setDependentDevice(int id){};
    virtual void toggleState(){};
    //
    virtual bool isI2C() { return false; };
    virtual bool isVideo() { return false; };
    virtual bool isAlert() { return false; };
    virtual bool isSensor() { return false; };

    //child will inherit
    //get from Sensor to expose to Device
    void setMin(float in_min);
    float getMin();
    
    void setMax(float in_max);
    float getMax();

    void setUnit(char *in_unit);
    char * getUnit();

    float min;
    float max;
    char unit[35];

    //

    void stripTime(char *inString, int *inArray);
    
    char * getClassName();
    
    char * getDeviceName();
    void setDeviceName(char *name);
    
    int getPin();
    void setPin(int in_pin);
    
    int getDeviceId();
    void setDeviceId(int id);

    void setStyle(char *image, char *color);
    void setImageName(char *image);
    void setDeviceColor(char *color);
    char * getImageName();
    char * getDeviceColor();
    
    bool getDeviceState();
    void setDeviceState(bool state);
    
    bool getSuspendTime();
    
    int pin;
    char classType[20];
    char deviceName[35];
    char imageName[20];
    char deviceColor[7];
    //boolean crashes!!!!!!!!!
    //boolean isI2C;
    //boolean isVideo;
    //boolean isAlert;
    bool deviceState;
    bool suspendTime;
    
    int deviceId;
    long convertToSeconds(int in_hour, int in_minute, int in_second);

  private:
    
    
    
};

#endif

