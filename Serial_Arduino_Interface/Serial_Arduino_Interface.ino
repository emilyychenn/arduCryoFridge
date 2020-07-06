#include <EnableInterrupt.h> // for the third pin (on arduino UNO only 2 pins are setup to handle interrupts)

const int ledPin = 13;
const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 7;
volatile int button1State = 0;
volatile int button2State = 0;
volatile int button3State = 0;

unsigned long ontime = 1; //default 1 minute
unsigned long offtime = 1; //default 1 minute
unsigned long onTime, offTime, onTimeMS, offTimeMS, delayTimeMS;
unsigned long timetostart, timetostartMS, currentMillis;
unsigned long previousMillis = 0; // last time LED was updated
unsigned long delayTime = 0;
int ledState = LOW;
int cycleMode = 0; // 0 = auto-cycling, 1 = manual

char menuInput;

String arduinoProgramVersion = "1.0";


void setup() {
  pinMode(ledPin, OUTPUT);

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
  if (button1State == HIGH) {
    button1State = LOW; // button state changed from high to low
    Serial.println("Button 1 State: OFF");
  } else if (button1State == LOW) {
    button1State = HIGH; // button state changed from low to high
    Serial.println("Button 1 State: ON");
  }
}

void interruptChange2() {
  if (button2State == HIGH) {
    button2State = LOW; // button state changed from high to low
    Serial.println("Button 2 State: OFF");
  } else if (button2State == LOW) {
    button2State = HIGH; // button state changed from low to high
    Serial.println("Button 2 State: ON");
  }
}

void interruptChange3() {
  if (button3State == HIGH) {
    button3State = LOW; // button state changed from high to low
    Serial.println("Button 3 State: OFF");
  } else if (button3State == LOW) {
    button3State = HIGH; // button state changed from low to high
    Serial.println("Button 3 State: ON");
  }
}


void setLed(int onTime, int offTime, unsigned long delayTime) {
  if (cycleMode == 0) {
    currentMillis = millis();
    onTimeMS = onTime*60000;
    offTimeMS = offTime*60000;
    delayTimeMS = delayTime*60000;
  
    if (delayTime == 0) {
      if ((ledState == HIGH) && (currentMillis - previousMillis >= onTimeMS)) {
        ledState = LOW;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
      } else if ((ledState == LOW) && (currentMillis - previousMillis >= offTimeMS)) {
        ledState = HIGH;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
      } else {
         // for testing:
          Serial.print("Time remaining before switch: ");
          Serial.print(onTimeMS - (currentMillis - previousMillis));
          Serial.println(" milliseconds");
      }
    } else if (delayTime != 0) {
        if (ledState == HIGH) {
          onTimeMS = onTimeMS + delayTimeMS;
          if ((currentMillis - previousMillis >= onTimeMS)) {
            Serial.println("On/off cycle starting now.");
            ledState = LOW;
            previousMillis = currentMillis;
            digitalWrite(ledPin, ledState);
            onTimeMS = onTimeMS - delayTimeMS; //returns it back to original ontime
            delayTime = 0;
          } else {
            // for testing:
            // Serial.print("LED is currently on. Time remaining before turning off: ");
            // Serial.print(onTimeMS - (currentMillis - previousMillis));
            // Serial.println(" milliseconds");
          }
        } else { //ledState is LOW
          offTimeMS = offTimeMS + delayTimeMS;
          if ((currentMillis - previousMillis >= offTimeMS)) {
            Serial.println("On/off cycle starting now.");
            ledState = HIGH;
            previousMillis = currentMillis;
            digitalWrite(ledPin, ledState);
            offTimeMS = offTimeMS - delayTimeMS; //returns it back to original offtime
            delayTime = 0;
          } else {
            // for testing:
             Serial.print("LED is currently off. Time remaining before turning on: ");
             Serial.print(offTimeMS - (currentMillis - previousMillis));
             Serial.println(" milliseconds");
          }
      }
    }
  }
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
      cycleMode = 1;
      break;
    case 'X':  //switch compressor off NOW and keep it off
      Serial.println("Switch compressor off NOW"); 
      digitalWrite(ledPin, LOW);
      ledState = LOW;
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
      Serial.println("Version of arduino code: " + arduinoProgramVersion);
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
