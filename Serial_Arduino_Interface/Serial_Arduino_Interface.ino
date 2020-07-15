#include <EnableInterrupt.h> // for the third pin (on arduino UNO only 2 pins are setup to handle interrupts)

const int output1Pin = 11;
const int output2Pin = 12;
volatile int output1State = LOW;
volatile int output2State = LOW;
unsigned long currentOutputTime;
unsigned long previousOutputTime = 0;
unsigned long outputTime = 300; //pulls output1Pin high for 300ms and then off again

const int ledPin = 13;

const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 7;
volatile int button1State, button2State, button3State;
int previousButton1State = 0;
int previousButton2State = 0;
int previousButton3State = 0;

unsigned long ontime = 1; //default 1 minute
unsigned long offtime = 1; //default 1 minute
unsigned long onTime, offTime, onTimeMS, offTimeMS, delayTimeMS;
unsigned long timetostart, timetostartMS, currentMillis;
unsigned long previousMillis = 0; // last time LED was updated
unsigned long delayTime = 0;
int ledState = LOW;
int cycleMode = 0; // 0 = auto-cycling, 1 = manual (user-specified)

int button1presses = 0;
int press1timed = 0;
int button2presses = 0;
int button3presses = 0;
unsigned long timeToggled = 0; // the last time the output pin was toggled
unsigned long timeToggled2 = 0;
unsigned long timeToggled3 = 0;
unsigned long debounce = 100; // the debounce time, increase if the output flickers

char menuInput;

String arduinoProgramVersion = "1.0";


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(output1Pin, OUTPUT);
  pinMode(output2Pin, OUTPUT);
  
  pinMode(button1Pin, INPUT_PULLUP);
  previousButton1State = digitalRead(button1Pin);
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

  // deal with a flag from interrupt handler but wait for button state to settle
  if (button1presses) {
    delay(debounce);
    // deal with the change of state if we're still in the new changed state
    button1State = digitalRead(button1Pin);
    if (previousButton1State != button1State) {
      previousButton1State = button1State;
      Serial.print("new button1State: ");
      Serial.println(button1State);
    }
    press1timed = 0;
    button1presses = 0;
  }

  
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time

  if (button2presses) {
    if ((previousButton2State != button2State) && (millis() - timeToggled2 > debounce)) {
      if (button2State == HIGH) {
        Serial.println("button2State: ON");
        previousButton2State = HIGH;
        timeToggled2 = millis();  
      } else if (button2State == LOW) {
        Serial.println("button2State: OFF");
        previousButton2State = LOW;
        timeToggled2 = millis(); 
      }
    }
    //digitalWrite(button2Pin, button2State);
    button2presses = 0;
  }

  if (button3presses) {
    if ((previousButton3State != button3State) && (millis() - timeToggled3 > debounce)) {
      if (button3State == HIGH) {
        Serial.println("button3State: ON");
        previousButton3State = HIGH;
        timeToggled3 = millis();  
      } else if (button3State == LOW) {
        Serial.println("button3State: OFF");
        previousButton3State = LOW;
        timeToggled3 = millis(); 
      }
    }
    //digitalWrite(button3Pin, button3State);
    button3presses = 0;
  }
}


void interruptChange1() {
  button1State = digitalRead(button1Pin);
  button1presses++;
}

void interruptChange2() {
  button2State = digitalRead(button2Pin);
  button2presses++;
}

void interruptChange3() {
  button3State = digitalRead(button3Pin);
  button3presses++;
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
    // TODO: report time remaining until switch
      if (ledState == HIGH) {
        Serial.println("Status of LED: ON");
        Serial.print("Status of Button 1: ");
        Serial.println(button1State);
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
        Serial.println(button1State);
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
