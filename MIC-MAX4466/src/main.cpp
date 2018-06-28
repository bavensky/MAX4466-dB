#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.clearDisplay();
    delay(2000);
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

    int out[] = {0, 60, 70, 80, 90, 100};
    int in[] = {0, 530, 570, 680, 1200, 1278};
    int cm = multiMap(sumSound, in, out, 6);

    display.setCursor(0, 8);
    display.setTextSize(3);
    display.print(" ");
    display.print(cm);
    display.print(" dB");
    display.print("\n");
    display.display();
    delay(1000);
    display.clearDisplay();

    Serial.print(sumSound);
    Serial.print("\t");
    Serial.print(cm);
    Serial.print(" dB\n");
}
