#include <Keyboard.h>
#include <Mouse.h>

#define DIAL_1_PIN_1 18
#define DIAL_1_PIN_2 19
#define DIAL_1_BUTTON 20

#define RED_PIN 5
#define GREEN_PIN 9
#define BLUE_PIN 6

#define LED_1 14
#define LED_2 10
#define LED_3 8
#define LED_4 7
#define LED_5 4
#define LED_6 15

#define BUTTON 16

int d1Change = 0;
int d1Counter = 0;
int d1LastValue = 0;
bool d1ButtonLastState = 0;
bool buttonLastState = 0;
uint8_t lightDuration = 1;

unsigned long t = 0;
unsigned long t2 = 0;

uint8_t color = 0;
uint8_t currentTool = 0;

void readDials();
void turnOffAllLEDs();
void updateLEDs(uint8_t tool);

uint8_t tool1Lights[] = {0, 0, 0};
uint8_t tool2Lights[] = {0, 0, 255};
uint8_t tool3Lights[] = {0, 255, 0};
uint8_t tool4Lights[] = {255, 0, 0};
uint8_t tool5Lights[] = {255, 255, 200};
uint8_t tool6Lights[] = {255, 200, 0};

void useTool(uint8_t tool) {
    for (int i=0; i<20; i++) {
      Mouse.move(0, -100, 0);
    }

    for (int i=0; i<30; i++) {
      Mouse.move(-100, 0, 0);
    }

    switch (tool) {
        case 0:
            Mouse.move(49, 0, 0);
            Mouse.move(0, 35, 0);
            break;

        case 1:
            Mouse.move(57, 0, 0);
            Mouse.move(0, 35, 0);
            break;

        case 2:
            Mouse.move(65, 0, 0);
            Mouse.move(0, 35, 0);
            break;

        case 3:
            Mouse.move(73, 0, 0);
            Mouse.move(0, 35, 0);
            break;

        case 4:
            Mouse.move(81, 0, 0);
            Mouse.move(0, 35, 0);
            break;

        case 5:
            Mouse.move(89, 0, 0);
            Mouse.move(0, 35, 0);
            break;
    }

    Mouse.click();
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

    t = millis();
    t2 = millis();

    Mouse.begin();
}

void loop () {
    bool dialButtonState = digitalRead(DIAL_1_BUTTON);
    bool buttonState = digitalRead(BUTTON);

    if (d1Counter != d1LastValue) {
        uint8_t d1Change = d1Counter < d1LastValue ? 0 : (d1Counter > d1LastValue ? 2 : 1);

        if (d1Change == 0) {
            currentTool = currentTool == 0 ? 5 : currentTool-1;
        } else if (d1Change == 2) {
            currentTool = currentTool == 5 ? 0 : currentTool+1;
        }
        
        useTool(currentTool);

        t2 = millis();
    }
    
    if (dialButtonState != d1ButtonLastState && dialButtonState == 0) {
        t2 = millis();
    }

    if (buttonState != buttonLastState && buttonState == 0) {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.print("z");
        Keyboard.release(KEY_LEFT_CTRL);
        t2 = millis();
    }

    d1LastValue = d1Counter;
    d1ButtonLastState = dialButtonState;
    buttonLastState = buttonState;

    if (millis() - t > 500) {
        Serial.print("D1: ");
        Serial.print(d1Counter);

        Serial.print(", Tool: ");
        Serial.println(currentTool);

        t = millis();
    }

    if (millis() - t2 > 5000) {
      turnOffAllLEDs();
    } else {
      updateLEDs(currentTool);
    }

    readDials();
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
}

void turnOffAllLEDs() {
    digitalWrite(LED_1, 1);
    digitalWrite(LED_2, 1);
    digitalWrite(LED_3, 1);
    digitalWrite(LED_4, 1);
    digitalWrite(LED_5, 1);
    digitalWrite(LED_6, 1);
}

void updateLEDs(uint8_t tool) {
    turnOffAllLEDs();    
    analogWrite(RED_PIN, tool1Lights[0]);
    analogWrite(GREEN_PIN, tool1Lights[1]);
    analogWrite(BLUE_PIN, tool1Lights[2]);
    digitalWrite(LED_1, 0);

    if (tool == 0) {delay(lightDuration*5);}
    delay(lightDuration);

    turnOffAllLEDs();    
    analogWrite(RED_PIN, tool2Lights[0]);
    analogWrite(GREEN_PIN, tool2Lights[1]);
    analogWrite(BLUE_PIN, tool2Lights[2]);
    digitalWrite(LED_2, 0);

    if (tool == 1) {delay(lightDuration*5);}
    delay(lightDuration);

    turnOffAllLEDs();    
    analogWrite(RED_PIN, tool3Lights[0]);
    analogWrite(GREEN_PIN, tool3Lights[1]);
    analogWrite(BLUE_PIN, tool3Lights[2]);
    digitalWrite(LED_3, 0);

    if (tool == 2) {delay(lightDuration*5);}
    delay(lightDuration);

    turnOffAllLEDs();
    analogWrite(RED_PIN, tool4Lights[0]);
    analogWrite(GREEN_PIN, tool4Lights[1]);
    analogWrite(BLUE_PIN, tool4Lights[2]);
    digitalWrite(LED_4, 0);

    if (tool == 3) {delay(lightDuration*5);}
    delay(lightDuration);

    turnOffAllLEDs();
    analogWrite(RED_PIN, tool5Lights[0]);
    analogWrite(GREEN_PIN, tool5Lights[1]);
    analogWrite(BLUE_PIN, tool5Lights[2]);
    digitalWrite(LED_5, 0);

    if (tool == 4) {delay(lightDuration*5);}
    delay(lightDuration);

    turnOffAllLEDs();
    analogWrite(RED_PIN, tool6Lights[0]);
    analogWrite(GREEN_PIN, tool6Lights[1]);
    analogWrite(BLUE_PIN, tool6Lights[2]);
    digitalWrite(LED_6, 0);

    if (tool == 5) {delay(lightDuration*5);}
    delay(lightDuration);
}
