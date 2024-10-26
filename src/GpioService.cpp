#include "GpioService.h"

GpioService::GpioService(int photoresistorInput, int photoresistorOutput, int dht11Input) : 
    _photoresistorInput(photoresistorInput),
    _photoresistorOutput(photoresistorOutput),
    _dht11Input(dht11Input),
    _dht(dht11Input, DHT11)
{
    pinMode(_photoresistorInput, INPUT);
    pinMode(_photoresistorOutput, OUTPUT);
    pinMode(_dht11Input, INPUT);
    _dht.begin(); 
}

int GpioService::GetPhotoresistorValue()
{
    digitalWrite(_photoresistorOutput, HIGH);

    int totalValue;
    int count = 10;

    for(int i = 0; i < count; i++)
    {
        totalValue += analogRead(_photoresistorInput);
    }

    digitalWrite(_photoresistorOutput, LOW);

    int averageValue = totalValue / count;

    return averageValue;
}

float GpioService::GetTemperature()
{
    return _dht.readTemperature();
}

float GpioService::GetHumidity()
{
    return _dht.readHumidity();
}