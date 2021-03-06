/*
  Relay
*/
// © 2noodles llc
// minh@2noodles.com

#include "Arduino.h"
#include "Relay.h"
#include "Device.h"
#include "DeviceDelegate.h"
#include <Time.h>

Relay::Relay(char *in_name, int in_pin, int in_dependent_device_id) : Device()
{
    // Device() call super to init vars
    dependentDeviceId = in_dependent_device_id;
    //find dependent device once - not all the time
    setDependentDevice(dependentDeviceId);
    
    
    //deviceID is automatically set my deviceDeleGate

    strcpy(deviceName,in_name);
    //classType inherit from base
    strcpy(classType,"Relay");

    pin = in_pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW); // weird where pin gets high on startup
                           // devices need pullup or pulldown for desired output
    
    isDay = false; // isDay is the day an event is to occur
    timedIndexCounter = 0;
    
}


void Relay::loop()
{
    if(suspendTime == false) {
        // when no time events
        if(timedIndexCounter == 0) {
            if(dependentDeviceObject) { //if there's a dependent device
                //find dependent device - call DeviceDelegate
                boolean temp = dependentDeviceObject->getDeviceState();
                
                if(temp) {
                    switchOn();
                } else {
                    switchOff();
                }
            }
            
        }
        
        //loop through events & check for time
        for (uint8_t i=0; i < timedIndexCounter; i++) {
            //dow comming in '1111110' 7 places 0 - sunday
            // isDay is the day an event is to occur
            //Time.h sunday is 1
            if(dow[i][::weekday()-1] == '1') {
                isDay = true;
                //Serial.println("isDay of event");
            } else {
                isDay = false;
                //Serial.println("notDay!");
            }
            
            
            
            if(isDay) {
                //::hour() get time from Time.h
                long currentTime = convertToSeconds(::hour(),::minute(),::second());
                //get start time
                long eventTime = convertToSeconds(hour[i],minute[i],second[i]);
                long durationTime = convertToSeconds(hourDuration[i],minuteDuration[i],secondDuration[i]) + convertToSeconds(hour[i],minute[i],second[i]);
                //rollover midnight
                if (durationTime > 86400L) {
                    durationTime = durationTime - 86400L;
                }
                
                if(dependentDeviceId > 0) { //if there's a dependent device
                    if(currentTime >= eventTime && currentTime <= durationTime)
                    {
                        boolean temp = dependentDeviceObject->getDeviceState();
                        
                        if(temp) {
                            switchOn();
                        } else {
                            switchOff();
                        }
                    } else {
                        switchOff();
                        
                    }
                    
                } else {
                    // if no dependent
                    if(currentTime == eventTime) {
                        switchOn();
                    } else if(currentTime == durationTime) {
                        switchOff();
                    }
                }
                
                
                
            }//end isDay
        }// end for loop
    }// end suspend time
}



void Relay::setEvent(char *in_string)
{
    if(in_string[0] == '\0') return;
    // start time, duration, dow
    // "8:00:00,1:00:00,1111111,9:30:00,1:30:01,1101111,15:34:02,1:05:02,1010101"
    //you can only add up to 4 events- each event is -on and duration -off pairs

    char events[12][67];
    
    //parse incoming string *** MAKE ROOM FOR THE NUL TERMINATOR in the string!
    char *tok1;
    int i = 0;
    tok1 = strtok(in_string, ",");
    while (tok1 != NULL) {
        strcpy(events[i],tok1);
        tok1 = strtok(NULL, ",");
        i++;
    }
    
    int j = 0;
    //char *token_s;
    //int temp[3];
    
    timedIndexCounter = i/3;
    //uint8_t m = 0;
    
    
    for (int l=0; l < timedIndexCounter; l++) {
        sscanf(events[j], "%d:%d:%d", &hour[l],&minute[l],&second[l]);
        j++;
        sscanf(events[j], "%d:%d:%d", &hourDuration[l],&minuteDuration[l],&secondDuration[l]);
        j++;
        strcpy(dow[l],events[j]);
        j++;
        
    }

    
    /*
    for (uint8_t l=0; l < timedIndexCounter; l++) {
        
        m = 0;
        //start
        token_s = strtok(events[j], ":");
        while (token_s != NULL) {
            temp[m] = atoi(token_s);
            token_s = strtok(NULL, ":");
            m++;
        }
        hour[l] = temp[0];
        minute[l] = temp[1];
        second[l] = temp[2];
        j++;
        
        
        //duration
        m = 0;
        token_s = strtok(events[j], ":");
        while (token_s != NULL) {
            temp[m] = atoi(token_s);
            token_s = strtok(NULL, ":");
            m++;
        }
        hourDuration[l] = temp[0];
        minuteDuration[l] = temp[1];
        secondDuration[l] = temp[2];
        j++;
        
        
        //dow
        strcpy(dow[l],events[j]);
        j++;
        
        
    }
     */
}


void Relay::getEvent(char *string) {
    //manipulat incoming string rather than returning. Saves memory?
    if(timedIndexCounter > 0) {
   
        char buf[104];
        sprintf(buf, "%02d:%02d:%02d,%02d:%02d:%02d,%s", hour[0],minute[0],second[0],hourDuration[0],minuteDuration[0],secondDuration[0],dow[0]);
        strcpy(string, buf);
        
        for (uint8_t i=1; i < timedIndexCounter; i++) {
            sprintf(buf, ",%02d:%02d:%02d,%02d:%02d:%02d,%s", hour[i],minute[i],second[i],hourDuration[i],minuteDuration[i],secondDuration[i],dow[i]);
            strcat(string,buf);
        }
    } else {
        string[0] = '\0';
    }
}

int Relay::getDependentDevice() {
    return dependentDeviceId;
    
}

void Relay::setDependentDevice(int id) {
    dependentDeviceId = id;
    if(dependentDeviceId > 0) {
        dependentDeviceObject = deviceDelegate.findDevice(dependentDeviceId);
    } else {
        dependentDeviceObject = NULL;
    }

    
}



void Relay::switchOn()
{
    // address is defined in the device cpp file
    //Serial.println("switching on");
   
    digitalWrite(pin,HIGH);
    deviceState = true;
}

void Relay::switchOff()
{
    // address is defined in the device cpp file
    //Serial.println("switching off");
    
    digitalWrite(pin,LOW);
    deviceState = false;
}

void Relay::toggleState()
{
   //Serial.println("toggle called");
    if (deviceState) {
        //digitalWrite(pin,LOW);
        //deviceState = false;
        
        switchOff();
        //Serial.println("toggle off");
    } else {
        switchOn();
        //digitalWrite(pin,HIGH);
        //deviceState = true;
         //Serial.println("toggle on");
    }
}


