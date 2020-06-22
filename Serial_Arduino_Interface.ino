int ontime = 1000;
int offtime = 1000;
int timetostart;
char menuInput;
const int ledPin = 12;
int ledState = LOW;

//store last time LED was updated
unsigned long previousMillis = 0;
long onTime = 1000;  // default; will be changed through commands
long offTime = 1000; // default; will be changed through commands


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("UNO is ready!");
}


void loop() {
  if(Serial.available()) {
    menuOptions();
  }

  setLed(ontime, offtime);
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
      delay(timetostart);
      break;
    case 'S':
      // report status of all switches
      Serial.println("Report status of all switches");
      if (ledState == HIGH) {
        Serial.println("Status of pin: ON");
      } else if (ledState == LOW) {
        Serial.println("Status of pin: OFF");
      }
      break;
    case 'Q':
      // print name of version of arduino code
      Serial.println("Version of arduino code: ___");
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
