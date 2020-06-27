const int ledPin = 13;
const int buttonPin = 2;
const int button2Pin = 4;
const int button3Pin = 7;

int ontime = 5000;
int offtime = 5000;
unsigned long timetostart;
char menuInput;
int ledState = LOW;

int buttonState = 0;
int button2State = 0;
int button3State = 0;
int lastButtonState = 0;
int lastButton2State = 0;
int lastButton3State = 0;

unsigned long previousMillis = 0; // last time LED was updated
long onTime = 1000;  // default; will be changed through commands
long offTime = 1000; // default; will be changed through commands

String arduinoProgramVersion = "1.0";


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("UNO is ready!");
}


void loop() {
  if(Serial.available()) {
    menuOptions();
  }

  setLed(ontime, offtime);

  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if(buttonState == HIGH) {
      Serial.println("Button State: ON");
    } else {
      Serial.println("Button State: OFF");
    }
    delay(50);
  }
  lastButtonState = buttonState;

  button2State = digitalRead(button2Pin);
  if (button2State != lastButton2State) {
    if(button2State == HIGH) {
      Serial.println("Button 2 State: ON");
    } else {
      Serial.println("Button 2 State: OFF");
    }
    delay(50);
  }
  lastButton2State = button2State;

  button3State = digitalRead(button3Pin);
  if (button3State != lastButton3State) {
    if(button3State == HIGH) {
      Serial.println("Button 3 State: ON");
    } else {
      Serial.println("Button 3 State: OFF");
    }
    delay(50);
  }
  lastButton3State = button3State;
}


void setLed(int onTime, int offTime) {
  /* check to see if it's time to blink the LED: if the difference between
   *  the current time and the last time the LED blinked is bigger than the
   *  interval at which you want it to blink
   */
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
    case 'G':
      // switch compressor on NOW
      Serial.println("Switch compressor on NOW"); // cycle continues
      digitalWrite(ledPin, HIGH);
      break;
    case 'X':
      //switch compressor off NOW
      Serial.println("Switch compressor off NOW"); // cycle continues
      digitalWrite(ledPin, LOW);
      break;
    case 'Z':
      // start on/off cycle in xxx minutes
      timetostart = Serial.parseInt();
      Serial.print("UNO will start on/off cycle in: ");
      Serial.println(timetostart);
      delay(timetostart);   // can use delay since delay function does not disable interrupts!
      break;
    case 'S':
      // report status of all switches
      if (ledState == HIGH) {
        Serial.println("Status of pin: ON");
      } else if (ledState == LOW) {
        Serial.println("Status of pin: OFF");
      }
      break;
    case 'Q':
      // print name of version of arduino code
      Serial.println("Version of arduino code: " + arduinoProgramVersion);
      break;
    case '?':
      // print help screen
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
