#include <EnableInterrupt.h> // for the third pin (on arduino UNO only 2 pins are setup to handle interrupts)

const int output1Pin = 11;
const int output2Pin = 12;
volatile int output1State = LOW;
volatile int output2State = LOW;
unsigned long currentOutputTime;
unsigned long previousOutputTime = 0;
unsigned long outputTime = 300;

const int ledPin = 13;

const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 7;
volatile int button1State = 0;
volatile int button2State = 0;
volatile int button3State = 0;
int lastButton1State, lastButton2State, lastButton3State;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long ontime = 1; //default 1 minute
unsigned long offtime = 1; //default 1 minute
unsigned long onTime, offTime, onTimeMS, offTimeMS, delayTimeMS;
unsigned long timetostart, timetostartMS, currentMillis;
unsigned long previousMillis = 0; // last time LED was updated
unsigned long delayTime = 0;
int ledState = LOW;
int cycleMode = 0; // 0 = auto-cycling, 1 = manual (user-specified)

char menuInput;

String arduinoProgramVersion = "1.0";


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(output1Pin, OUTPUT);
  pinMode(output2Pin, OUTPUT);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  enableInterrupt(button1Pin, interruptChange1, CHANGE);
  enableInterrupt(button2Pin, interruptChange2, CHANGE);
  enableInterrupt(button3Pin, interruptChange3, CHANGE);

  Serial.begin(9600);
  Serial.println("UNO is ready!");
}

void loop() {
  if (Serial.available()) {
    menuOptions();
  }
  setLed(ontime, offtime, timetostart);
}


void interruptChange1() {
  unsigned long interruptTime = millis();

  if (interruptTime - lastDebounceTime > debounceDelay) {
    button1State = !button1State;
    Serial.print("Button 1 State: ");
    Serial.println(button1State);
  }
  lastDebounceTime = interruptTime;
}

void interruptChange2() {
  unsigned long interruptTime = millis();

  if (interruptTime - lastDebounceTime > debounceDelay) {
    button2State = !button2State;
    Serial.print("Button 2 State: ");
    Serial.println(button2State);
  }
  lastDebounceTime = interruptTime;
}

void interruptChange3() {
  unsigned long interruptTime = millis();

  if (interruptTime - lastDebounceTime > debounceDelay) {
    button3State = !button3State;
    Serial.print("Button 3 State: ");
    Serial.println(button3State);
  }
  lastDebounceTime = interruptTime;
}


void setLed(int onTime, int offTime, unsigned long delayTime) {
  if (cycleMode == 0) {
    currentMillis = millis();
    onTimeMS = onTime*60000;
    offTimeMS = offTime*60000;
    delayTimeMS = delayTime*60000;
  
    if (delayTime == 0) {
      if ((ledState == HIGH) && (currentMillis - previousMillis >= onTimeMS)) {
        previousMillis = currentMillis;
        switchCompressorOff();
      } else if ((ledState == LOW) && (currentMillis - previousMillis >= offTimeMS)) {
        previousMillis = currentMillis;
        switchCompressorOn();
      } else {
         // for testing:
         // Serial.print("Time remaining before switch: ");
         // Serial.print(onTimeMS - (currentMillis - previousMillis));
         // Serial.println(" milliseconds");
      }
    } else if (delayTime != 0) {
        if (ledState == HIGH) {
          if ((currentMillis - previousMillis >= delayTimeMS)) {
            Serial.println("On/off cycle starting now.");
            previousMillis = currentMillis;
            switchCompressorOff();
            delayTime = 0;
          } else {
            // for testing:
            // Serial.print("LED is currently on. Time remaining before turning off: ");
            // Serial.print(onTimeMS - (currentMillis - previousMillis));
            // Serial.println(" milliseconds");
          }
        } else { //ledState is LOW
          if ((currentMillis - previousMillis >= delayTimeMS)) {
            Serial.println("On/off cycle starting now.");
            previousMillis = currentMillis;
            switchCompressorOn();
            delayTime = 0;
          } else {
            // for testing:
            // Serial.print("LED is currently off. Time remaining before turning on: ");
            // Serial.print(offTimeMS - (currentMillis - previousMillis));
            // Serial.println(" milliseconds");
          }
      }
    }
  }
}

void switchCompressorOn() {
  // pull pin 11 high for 300ms and then off again
  previousOutputTime = millis();
  currentOutputTime = millis();
  ledState = HIGH;
  digitalWrite(ledPin, ledState);
  output1State = HIGH;
  digitalWrite(output1Pin, output1State);
  while (currentOutputTime - previousOutputTime < outputTime) {
    // Serial.println("in the while loop"); //for testing to see if/how long this runs
    currentOutputTime = millis();
  }
  output1State = LOW;
  digitalWrite(output1Pin, output1State);
}

void switchCompressorOff() {
 // pull pin 12 high for 300ms and then off again
  previousOutputTime = millis();
  currentOutputTime = millis();
  ledState = LOW;
  digitalWrite(ledPin, ledState);
  output2State = HIGH;
  digitalWrite(output2Pin, output2State);
  while (currentOutputTime - previousOutputTime < outputTime) {
    // Serial.println("in the while loop"); //for testing to see if/how long this loop runs
    currentOutputTime = millis();
  }
  output2State = LOW;
  digitalWrite(output2Pin, output2State);
}


void menuOptions() {
  menuInput = Serial.read();
  switch (menuInput) {
    case 'A':
      ontime = Serial.parseInt();
      Serial.print("UNO received ontime: ");
      Serial.print(ontime);
      Serial.println(" minutes");
      break;
    case 'B':
      offtime = Serial.parseInt();
      Serial.print("UNO received offtime: ");
      Serial.print(offtime);
      Serial.println(" minutes");
      break;
    case 'G':  // switch compressor on NOW and keep it on
      Serial.println("Switch compressor on NOW"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      switchCompressorOn();
      cycleMode = 1;
      break;
    case 'X':  //switch compressor off NOW and keep it off
      Serial.println("Switch compressor off NOW"); 
      digitalWrite(ledPin, LOW);
      ledState = LOW;
      switchCompressorOff();
      cycleMode = 1;
      break;
    case 'Z':  // start on/off cycle in xxx minutes
      timetostart = Serial.parseInt();
      Serial.print("UNO will start on/off cycle in: ");
      Serial.print(timetostart);
      Serial.println(" mins");
      cycleMode = 0;
      previousMillis = millis(); // cycle will restart (i.e. if it was on before, it would stay on for delayTime + onTime)
      // delete above line for cycle to continue from where it left off (i.e. if there was 15 seconds of ontime left, 
      // the led would stay on for (delayTime + 15 seconds remaining))
      delayTime = timetostart;
      break;
    case 'S':  // report status of all switches
      if (ledState == HIGH) {
        Serial.println("Status of LED: ON");
        Serial.print("Status of Button 1: ");
          if(button1State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
        Serial.print("Status of Button 2: ");
          if(button2State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
        Serial.print("Status of Button 3: ");
          if(button3State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
      } else if (ledState == LOW) {
        Serial.println("Status of LED: OFF");
        Serial.print("Status of Button 1: ");
          if(button1State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
        Serial.print("Status of Button 2: ");
          if(button2State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
        Serial.print("Status of Button 3: ");
          if(button3State == HIGH) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
      }
      break;
    case 'Q':  // print name of version of arduino code
      Serial.println("Arduino program version: " + arduinoProgramVersion);
      break;
    case '?':  // print help screen
      Serial.println("Help Screen: ");
      Serial.println("Axxx ... set on time to xxx minutes");
      Serial.println("Bxxx ... set off time to xxx minutes");
      Serial.println("G ... switch compressor on NOW");
      Serial.println("X ... switch compressor off NOW");
      Serial.println("Zxxx ... start on/off cycle in xxx minutes");
      Serial.println("S ... report status of all switches");
      Serial.println("Q ... print name of version of arduino code");
      Serial.println("? ... print help screen");
      break;
    case '\n':
      break;
    default:
      Serial.println("Invalid command. Enter ? for help screen");
  }
}
