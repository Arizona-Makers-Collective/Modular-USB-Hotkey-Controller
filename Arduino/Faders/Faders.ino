#include <Joystick.h>

#define FADER_1 A2
#define FADER_2 A1
#define FADER_3 A0

Joystick_ Joystick;

unsigned long t = 0;

void setup () {
    Serial.begin(9600);

    pinMode(FADER_1, INPUT);
    pinMode(FADER_2, INPUT);
    pinMode(FADER_3, INPUT);

    Joystick.begin();
}

void loop () {
    if (millis() - t > 50) {
        Serial.print("F1: ");
        Serial.print(analogRead(FADER_1));
        Serial.print(", F2: ");
        Serial.print(analogRead(FADER_2));
        Serial.print(", F3: ");
        Serial.println(analogRead(FADER_3));

        t = millis();
    }

    int16_t x = 1023 - analogRead(FADER_1);
    int16_t y = 1023 - analogRead(FADER_2);
    int16_t z = 1023 - analogRead(FADER_3);

    Joystick.setXAxis(x);
    Joystick.setYAxis(y);
    Joystick.setZAxis(z);


    delay(10);
}
