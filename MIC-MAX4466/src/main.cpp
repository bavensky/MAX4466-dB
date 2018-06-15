#include <Arduino.h>

#define MicPin A0

int avgRate = 5;
unsigned long sumSound = 0;

int multiMap(int val, int *_in, int *_out, uint8_t size)
{
    if (val <= _in[0])
        return _out[0];
    if (val >= _in[size - 1])
        return _out[size - 1];

    uint8_t pos = 1; 
    while (val > _in[pos])
        pos++;

    if (val == _in[pos])
        return _out[pos];

    return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    for (int x = 0; x < avgRate; x++)
    {
        int arraysum[2000] = {0};
        int max = 0;
        int min = 1023;

        for (int i = 0; i < 2000; i++)
        {
            arraysum[i] = analogRead(MicPin);
            delayMicroseconds(25);

            if (arraysum[i] > max)
            {
                max = arraysum[i];
            }
            if (arraysum[i] < min)
            {
                min = arraysum[i];
            }
        }
        int peakToPeak = max - min;
        sumSound += peakToPeak;
    }
    sumSound = sumSound / avgRate;

    int out[] = {0, 60,  70,  80,  90, 100};
    int in[] =  {0, 530, 570, 680, 1200, 1278};
    int cm = multiMap(sumSound, in, out, 6);

    // Serial.print(analogRead(MicPin));
    // Serial.print("\t");
    Serial.print(sumSound);
    Serial.print("\t");
    Serial.print(cm);
    Serial.print(" dB\n");
}
