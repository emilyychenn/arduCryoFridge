#include <EnableInterrupt.h> // for the third pin (on arduino UNO only 2 pins are setup to handle interrupts)

const int ledPin = 13;
const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 7;
volatile int button1State = 0;
volatile int button2State = 0;
volatile int button3State = 0;

int ontime = 5000;
int offtime = 5000;
unsigned long timetostart;
char menuInput;
int ledState = LOW;

unsigned long previousMillis = 0; // last time LED was updated
long onTime = 1000;  // default; will be changed through commands
long offTime = 1000; // default; will be changed through commands

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
  setLed(ontime, offtime);
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


void setLed(int onTime, int offTime) {
  unsigned long currentMillis = millis();

  // if the LED is off, turn it on, and vice-versa:
  if ((ledState == HIGH) && (currentMillis - previousMillis >= onTime)) {
    ledState = LOW;
    previousMillis = currentMillis;
    digitalWrite(ledPin, ledState);
  } else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime)) {
    ledState = HIGH;
    previousMillis = currentMillis;
    digitalWrite(ledPin, ledState);
  }
}


void menuOptions() {
  menuInput = Serial.read();
  switch (menuInput) {
    case 'A':
      ontime = Serial.parseInt();
      Serial.print("UNO received ontime: ");
      Serial.println(ontime);
      break;
    case 'B':
      offtime = Serial.parseInt();
      Serial.print("UNO received offtime: ");
      Serial.println(offtime);
      break;
    case 'G':  // switch compressor on NOW
      Serial.println("Switch compressor on NOW"); // cycle continues
      digitalWrite(ledPin, HIGH);
      break;
    case 'X':  //switch compressor off NOW
      Serial.println("Switch compressor off NOW"); // cycle continues
      digitalWrite(ledPin, LOW);
      break;
    case 'Z':  // start on/off cycle in xxx minutes
      timetostart = Serial.parseInt();
      Serial.print("UNO will start on/off cycle in: ");
      Serial.println(timetostart);
      delay(timetostart);   // can use delay since delay function does not disable interrupts!
      Serial.println("On/off cycle starting now.");
      break;
    case 'S':  // report status of all switches
      if (ledState == HIGH) {
        Serial.println("Status of pin: ON");
        // TODO PRINT STATUS OF BUTTONS
      } else if (ledState == LOW) {
        Serial.println("Status of pin: OFF");
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
