#ifndef GpioService_h
#define GpioService_h
#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


class GpioService
{
    private:
        int _photoresistorInput;
        int _photoresistorOutput;
        int _dht11Input;
        DHT _dht;

    public:
        GpioService(int photoresistorInput, int photoresistorOutput, int _dht11Input);
        int GetPhotoresistorValue();
        float GetTemperature();
        float GetHumidity();

};

#endif