#include <Mouse.h>
#include "Wire.h"
#include "Keyboard.h"

#define MESSAGE_BUFFER_SIZE 34

#define DIAL_1_PIN_1 4
#define DIAL_1_PIN_2 5
#define DIAL_1_BUTTON 6

#define DIAL_2_PIN_1 7
#define DIAL_2_PIN_2 8
#define DIAL_2_BUTTON 9

#define DIAL_3_PIN_1 10
#define DIAL_3_PIN_2 16
#define DIAL_3_BUTTON 14

#define DIAL_4_PIN_1 18
#define DIAL_4_PIN_2 19
#define DIAL_4_BUTTON 15

int d1Change = 0;
int d2Change = 0;
int d3Change = 0;
int d4Change = 0;

int d1Counter = 0;
int d2Counter = 0;
int d3Counter = 0;
int d4Counter = 0;

int d1LastValue = 1;
int d2LastValue = 1;
int d3LastValue = 1;
int d4LastValue = 1;

bool d1ButtonLastState = 0;
bool d2ButtonLastState = 0;
bool d3ButtonLastState = 0;
bool d4ButtonLastState = 0;

bool buttonPrevStates[] = {0, 0, 0, 0};
bool buttonStates[] = {0, 0, 0, 0};

unsigned long t = 0;

char buffer[MESSAGE_BUFFER_SIZE];

void readDials();
void dataRequested();

// escape, up, down, delete, shift, control, alt, os
byte modifiers[] = {B00000000, B00000010, B00000000, B00000000};

// Function to press profile modifier keys
// escape, up, down, delete, shift, control, alt, os pressModifiers
void pressModifiers(byte keyList) {
  if (bitRead(keyList, 7)) {Keyboard.write(KEY_ESC);}
  if (bitRead(keyList, 6)) {Keyboard.write(KEY_UP_ARROW);}
  if (bitRead(keyList, 5)) {Keyboard.write(KEY_DOWN_ARROW);}
  if (bitRead(keyList, 4)) {Keyboard.write(KEY_DELETE);}
  if (bitRead(keyList, 3)) {Keyboard.press(KEY_LEFT_SHIFT);}
  if (bitRead(keyList, 2)) {Keyboard.press(KEY_LEFT_CTRL);}
  if (bitRead(keyList, 1)) {Keyboard.press(KEY_LEFT_ALT);}
  if (bitRead(keyList, 0)) {Keyboard.press(KEY_LEFT_GUI);}
}

// Function to release profile modifier keys
// escape, up, down, delete, shift, control, alt, os
void releaseModifiers(byte keyList) {
  if (bitRead(keyList, 3)) {Keyboard.release(KEY_LEFT_SHIFT);}
  if (bitRead(keyList, 2)) {Keyboard.release(KEY_LEFT_CTRL);}
  if (bitRead(keyList, 1)) {Keyboard.release(KEY_LEFT_ALT);}
  if (bitRead(keyList, 0)) {Keyboard.release(KEY_LEFT_GUI);}
}

bool buttonClicked(uint8_t index) {
    return buttonStates[index] != buttonPrevStates[index] && buttonStates[index] == 0;
}

void setup () {
    Serial.begin(9600);
    //Wire.begin(5);
    //Wire.onRequest(dataRequested);

    pinMode(DIAL_1_PIN_1, INPUT_PULLUP);
    pinMode(DIAL_1_PIN_2, INPUT_PULLUP);
    pinMode(DIAL_1_BUTTON, INPUT_PULLUP);

    pinMode(DIAL_2_PIN_1, INPUT_PULLUP);
    pinMode(DIAL_2_PIN_2, INPUT_PULLUP);
    pinMode(DIAL_2_BUTTON, INPUT_PULLUP);

    pinMode(DIAL_3_PIN_1, INPUT_PULLUP);
    pinMode(DIAL_3_PIN_2, INPUT_PULLUP);
    pinMode(DIAL_3_BUTTON, INPUT_PULLUP);

    pinMode(DIAL_4_PIN_1, INPUT_PULLUP);
    pinMode(DIAL_4_PIN_2, INPUT_PULLUP);
    pinMode(DIAL_4_BUTTON, INPUT_PULLUP);
}

void loop () {
    bool buttonStatesTemp[] = {digitalRead(DIAL_1_BUTTON), digitalRead(DIAL_2_BUTTON), digitalRead(DIAL_3_BUTTON), digitalRead(DIAL_4_BUTTON)};
    memcpy(buttonStates, buttonStatesTemp, sizeof(buttonStatesTemp[0])*9);

    if (d1Counter != d1LastValue) {
        uint8_t d1Change = d1Counter < d1LastValue ? 0 : (d1Counter > d1LastValue ? 2 : 1);

        if (d1Change == 0) {
            Keyboard.write(KEY_LEFT_ARROW);
        } else if (d1Change == 2) {
            Keyboard.write(KEY_RIGHT_ARROW);
        }
    }

    if (d2Counter != d2LastValue) {
        uint8_t d2Change = d2Counter < d2LastValue ? 0 : (d2Counter > d2LastValue ? 2 : 1);

        if (d2Change == 0) {
            Keyboard.write(KEY_UP_ARROW);
        } else if (d2Change == 2) {
            Keyboard.write(KEY_DOWN_ARROW);
        }
    }

    if (d3Counter != d3LastValue) {
        uint8_t d3Change = d3Counter < d3LastValue ? 0 : (d3Counter > d3LastValue ? 2 : 1);

        if (d3Change == 0) {
            Mouse.move(0, 0, 1);
        } else if (d3Change == 2) {
            Mouse.move(0, 0, -1);
        }
    }

    if (buttonClicked(0)) {
        pressModifiers(modifiers[0]);

        Keyboard.write(KEY_HOME);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.write(KEY_END);
        Keyboard.release(KEY_LEFT_SHIFT);

        releaseModifiers(modifiers[0]);
    }

    if (buttonClicked(1)) {
        pressModifiers(modifiers[1]);

        Keyboard.write(KEY_LEFT_ARROW);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.write(KEY_RIGHT_ARROW);
        Keyboard.release(KEY_LEFT_SHIFT);

        releaseModifiers(modifiers[1]);
    }

    d1LastValue = d1Counter;
    d2LastValue = d2Counter;
    d3LastValue = d3Counter;
    d4LastValue = d4Counter;

    memcpy(buttonPrevStates, buttonStates, sizeof(buttonStates[0])*4);

    readDials();
    delay(1);
}

void readDials () {
    bool d1x = !digitalRead(DIAL_1_PIN_1);
    bool d1y = !digitalRead(DIAL_1_PIN_2);

    // Track rotary position
    if (d1Change == 1) {
        if (!d1x && !d1y) {
            d1Change = 0;
            d1Counter++;
        }
    } else if (d1Change == -1) {
        if (!d1x && !d1y) {
            d1Change = 0;
            d1Counter--;
        }
    } else {
        if (d1x) { d1Change = -1; }
        else if (d1y) { d1Change = 1; }
    }

    bool d2x = !digitalRead(DIAL_2_PIN_1);
    bool d2y = !digitalRead(DIAL_2_PIN_2);

    // Track rotary position
    if (d2Change == 1) {
        if (!d2x && !d2y) {
            d2Change = 0;
            d2Counter++;
        }
    } else if (d2Change == -1) {
        if (!d2x && !d2y) {
            d2Change = 0;
            d2Counter--;
        }
    } else {
        if (d2x) { d2Change = -1; }
        else if (d2y) { d2Change = 1; }
    }

    bool d3x = !digitalRead(DIAL_3_PIN_1);
    bool d3y = !digitalRead(DIAL_3_PIN_2);

    // Track rotary position
    if (d3Change == 1) {
        if (!d3x && !d3y) {
            d3Change = 0;
            d3Counter++;
        }
    } else if (d3Change == -1) {
        if (!d3x && !d3y) {
            d3Change = 0;
            d3Counter--;
        }
    } else {
        if (d3x) { d3Change = -1; }
        else if (d3y) { d3Change = 1; }
    }

    bool d4x = !digitalRead(DIAL_4_PIN_1);
    bool d4y = !digitalRead(DIAL_4_PIN_2);

    // Track rotary position
    if (d4Change == 1) {
        if (!d4x && !d4y) {
            d4Change = 0;
            d4Counter++;
        }
    } else if (d4Change == -1) {
        if (!d4x && !d4y) {
            d4Change = 0;
            d4Counter--;
        }
    } else {
        if (d4x) { d4Change = -1; }
        else if (d4y) { d4Change = 1; }
    }
}

/* if (millis() - t > 100) {
        Serial.print("D1: ");
        Serial.print(d1Counter);
        Serial.print(", D2: ");
        Serial.print(d2Counter);
        Serial.print(", D3: ");
        Serial.print(d3Counter);
        Serial.print(", D4: ");
        Serial.println(d4Counter);

        bool d1Button = digitalRead(DIAL_1_BUTTON);
        bool d2Button = digitalRead(DIAL_2_BUTTON);
        bool d3Button = digitalRead(DIAL_3_BUTTON);
        bool d4Button = digitalRead(DIAL_4_BUTTON);

        Serial.print("B1: ");
        Serial.print(d1Button);
        Serial.print(", B2: ");
        Serial.print(d2Button);
        Serial.print(", B3: ");
        Serial.print(d3Button);
        Serial.print(", B4: ");
        Serial.println(d4Button);

        t = millis();
    } */

    /* void dataRequested () {
    bool d1Button = digitalRead(DIAL_1_BUTTON);
    bool d2Button = digitalRead(DIAL_2_BUTTON);
    bool d3Button = digitalRead(DIAL_3_BUTTON);
    bool d4Button = digitalRead(DIAL_4_BUTTON);

    uint8_t d1Change = d1Counter < d1LastValue ? 0 : (d1Counter > d1LastValue ? 2 : 1);
    uint8_t d2Change = d2Counter < d2LastValue ? 0 : (d2Counter > d2LastValue ? 2 : 1);
    uint8_t d3Change = d3Counter < d3LastValue ? 0 : (d3Counter > d3LastValue ? 2 : 1);
    uint8_t d4Change = d4Counter < d4LastValue ? 0 : (d4Counter > d4LastValue ? 2 : 1);

    snprintf(buffer, MESSAGE_BUFFER_SIZE, "D1:%d:2:%d:3:%d:4:%d:B1:%d:2:%d:3:%d:4:%d:", d1Change, d2Change, d3Change, d4Change, d1Button, d2Button, d3Button, d4Button);

    d1LastValue = d1Counter;
    d2LastValue = d2Counter;
    d3LastValue = d3Counter;
    d4LastValue = d4Counter;

    Wire.write(buffer);
} */
