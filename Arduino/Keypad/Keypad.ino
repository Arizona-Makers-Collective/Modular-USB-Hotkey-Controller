#include <Keyboard.h>

#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6
#define BUTTON4 7
#define BUTTON5 8
#define BUTTON6 9
#define BUTTON7 10
#define BUTTON8 14
#define BUTTON9 16

unsigned long t = 0;

bool buttonPrevStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool buttonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// escape, up, down, delete, shift, control, alt, os
byte modifiers[] = {B00000000, B00000000, B00000000, B00000011, B00000000, B00001010, B00000011, B00000011, B00000011};

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

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON6, INPUT_PULLUP);
    pinMode(BUTTON7, INPUT_PULLUP);
    pinMode(BUTTON8, INPUT_PULLUP);
    pinMode(BUTTON9, INPUT_PULLUP);

    Keyboard.begin();
}

void loop () {
    bool buttonStatesTemp[] = {digitalRead(BUTTON1), digitalRead(BUTTON2), digitalRead(BUTTON3), digitalRead(BUTTON4), digitalRead(BUTTON5), digitalRead(BUTTON6), digitalRead(BUTTON7), digitalRead(BUTTON8), digitalRead(BUTTON9)};
    memcpy(buttonStates, buttonStatesTemp, sizeof(buttonStatesTemp[0])*9);

    if (buttonClicked(0)) {
        pressModifiers(modifiers[0]);

        Keyboard.write(KEY_HOME);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.write(KEY_END);
        Keyboard.release(KEY_LEFT_SHIFT);

        releaseModifiers(modifiers[0]);
    }

    if (buttonClicked(3)) {
        pressModifiers(modifiers[3]);
        Keyboard.write(KEY_UP_ARROW);
        releaseModifiers(modifiers[3]);
    }

    if (buttonClicked(6)) {
        pressModifiers(modifiers[6]);
        Keyboard.write(KEY_DOWN_ARROW);
        releaseModifiers(modifiers[6]);
    }

    if (buttonClicked(4)) {
        pressModifiers(modifiers[4]);
        Keyboard.write(KEY_F1);
        Keyboard.print("serial");
        Keyboard.write(KEY_RETURN);
        releaseModifiers(modifiers[4]);
    }

    if (buttonClicked(5)) {
        pressModifiers(modifiers[5]);
        Keyboard.print("a");
        releaseModifiers(modifiers[5]);
    }

    if (buttonClicked(7)) {
        pressModifiers(modifiers[7]);
        Keyboard.print("r");
        releaseModifiers(modifiers[7]);
    }

    if (buttonClicked(8)) {
        pressModifiers(modifiers[8]);
        Keyboard.print("u");
        releaseModifiers(modifiers[8]);
    }

    memcpy(buttonPrevStates, buttonStates, sizeof(buttonStates[0])*9);
    
    /* if (millis() - t > 100) {
        Serial.print("B1: ");
        Serial.print(button1State);
        Serial.print(", B2: ");
        Serial.print(button2State);
        Serial.print(", B3: ");
        Serial.print(button3State);
        Serial.print(", B4: ");
        Serial.print(button4State);
        Serial.print(", B5: ");
        Serial.print(button5State);
        Serial.print(", B6: ");
        Serial.print(button6State);
        Serial.print(", B7: ");
        Serial.print(button7State);
        Serial.print(", B8: ");
        Serial.print(button8State);
        Serial.print(", B9: ");
        Serial.println(button9State);

        t = millis();
    } */

    delay(5);
}
