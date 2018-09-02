#include <Keyboard.h>

#define DIAL_1_PIN_1 18
#define DIAL_1_PIN_2 19
#define DIAL_1_BUTTON 20

#define RED_PIN 5
#define GREEN_PIN 9
#define BLUE_PIN 6

#define LED_1 4
#define LED_2 7
#define LED_3 8
#define LED_4 10
#define LED_5 14
#define LED_6 15

#define BUTTON 16

int d1Change = 0;
int d1Counter = 0;
int d1LastValue = 0;
bool d1ButtonLastState = 0;
bool buttonLastState = 0;

unsigned long t = 0;
unsigned long t2 = 0;

uint8_t color = 0;
uint8_t currentTool = 0;

void readDials();

byte modifiers[] = {B00000000, B00000011, B00000011, B00001010, B00000000, B00000000};

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

void turnOffAllLEDs() {
    digitalWrite(LED_1, 1);
    digitalWrite(LED_2, 1);
    digitalWrite(LED_3, 1);
    digitalWrite(LED_4, 1);
    digitalWrite(LED_5, 1);
    digitalWrite(LED_6, 1);
}

uint8_t getCurrentSector(uint8_t pos) {
    if (pos <= 3) {
        return 0;
    } else if (pos > 3 && pos <= 7) {
        return 1;
    } else if (pos > 7 && pos <= 10) {
        return 2;
    } else if (pos > 10 && pos <= 13) {
        return 3;
    } else if (pos > 13 && pos <= 17) {
        return 4;
    } else {
        return 5;
    }
}

void lightSingleLED(uint8_t tool) {
    turnOffAllLEDs();

    switch (tool) {
        case 0:
            digitalWrite(LED_5, 0);
            break;

        case 1:
            digitalWrite(LED_4, 0);
            break;

        case 2:
            digitalWrite(LED_3, 0);
            break;

        case 3:
            digitalWrite(LED_2, 0);
            break;

        case 4:
            digitalWrite(LED_1, 0);
            break;

        default:
            digitalWrite(LED_6, 0);
            break;
    }
}

void useTool(uint8_t tool) {
    pressModifiers(modifiers[tool]);

    switch (tool) {
        case 0:
            Keyboard.write(KEY_HOME);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.write(KEY_END);
            Keyboard.release(KEY_LEFT_SHIFT);
            break;

        case 1:
            Keyboard.print("u");
            break;

        case 2:
            Keyboard.print("r");
            break;

        case 3:
            Keyboard.print("a");
            break;

        case 4:
            break;

        default:
            break;
    }

    releaseModifiers(modifiers[tool]);
}

void setup () {
    Serial.begin(9600);

    pinMode(DIAL_1_PIN_1, INPUT_PULLUP);
    pinMode(DIAL_1_PIN_2, INPUT_PULLUP);
    pinMode(DIAL_1_BUTTON, INPUT_PULLUP);

    pinMode(BUTTON, INPUT_PULLUP);

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);
    pinMode(LED_5, OUTPUT);
    pinMode(LED_6, OUTPUT);

    digitalWrite(BLUE_PIN, 1);

    lightSingleLED(currentTool);
}

void loop () {
    bool dialButtonState = digitalRead(DIAL_1_BUTTON);
    bool buttonState = digitalRead(BUTTON);

    // if (d1Counter != d1LastValue) {
    //     currentTool = getCurrentSector(d1Counter);
    //     lightSingleLED(currentTool);
    // }

    if (d1Counter != d1LastValue) {
        uint8_t d1Change = d1Counter < d1LastValue ? 0 : (d1Counter > d1LastValue ? 2 : 1);

        if (d1Change == 0) {
            currentTool = currentTool == 0 ? 5 : currentTool-1;
        } else if (d1Change == 2) {
            currentTool = currentTool == 5 ? 0 : currentTool+1;
        }

        lightSingleLED(currentTool);
    }
    
    if (dialButtonState != d1ButtonLastState && dialButtonState == 0) {
        useTool(currentTool);
    }

    if (buttonState != buttonLastState && buttonState == 0) {
        d1Counter = 0;
        currentTool = getCurrentSector(d1Counter);
        lightSingleLED(currentTool);
    }

    d1LastValue = d1Counter;
    d1ButtonLastState = dialButtonState;
    buttonLastState = buttonState;

    if (millis() - t > 50) {
        Serial.print("D1: ");
        Serial.print(d1Counter);

        Serial.print(", Tool: ");
        Serial.print(currentTool);
        Serial.print(", Sector: ");
        Serial.println(getCurrentSector(d1Counter));

        t = millis();
    }

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
            //d1Counter = d1Counter == 19 ? 0 : d1Counter+1;
            d1Counter++;
        }
    } else if (d1Change == -1) {
        if (!d1x && !d1y) {
            d1Change = 0;
            //d1Counter = d1Counter == 0 ? 19 : d1Counter-1;
            d1Counter--;
        }
    } else {
        if (d1x) { d1Change = -1; }
        else if (d1y) { d1Change = 1; }
    }
}


/* if (millis() - t2 > 500) {
        color = color == 0 ? 1 : (color == 1 ? 2 : 0);

        if (color == 0) {
            digitalWrite(RED_PIN, 1);
            digitalWrite(GREEN_PIN, 0);
            digitalWrite(BLUE_PIN, 0);

            digitalWrite(LED_1, 0);
            digitalWrite(LED_2, 1);
            digitalWrite(LED_3, 1);
            digitalWrite(LED_4, 0);
            digitalWrite(LED_5, 1);
            digitalWrite(LED_6, 1);
        } else if (color == 1) {
            digitalWrite(RED_PIN, 0);
            digitalWrite(GREEN_PIN, 1);
            digitalWrite(BLUE_PIN, 0);

            digitalWrite(LED_1, 1);
            digitalWrite(LED_2, 0);
            digitalWrite(LED_3, 1);
            digitalWrite(LED_4, 1);
            digitalWrite(LED_5, 0);
            digitalWrite(LED_6, 1);
        } else if (color == 2) {
            digitalWrite(RED_PIN, 0);
            digitalWrite(GREEN_PIN, 0);
            digitalWrite(BLUE_PIN, 1);

            digitalWrite(LED_1, 1);
            digitalWrite(LED_2, 1);
            digitalWrite(LED_3, 0);
            digitalWrite(LED_4, 1);
            digitalWrite(LED_5, 1);
            digitalWrite(LED_6, 0);
        }

        t2 = millis();
    } */

    /* if (millis() - t > 50) {
        Serial.print("D1: ");
        Serial.print(d1Counter);

        bool d1Button = digitalRead(DIAL_1_BUTTON);
        bool d2Button = digitalRead(BUTTON);

        Serial.print(", B1: ");
        Serial.print(d1Button);
        Serial.print(", B2: ");
        Serial.println(d2Button);

        t = millis();
    } */