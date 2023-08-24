#include "Arduino.h"
#include "main.h"

// Function prototypes
void flashLed();
InputCommand getInput();
void resetOutputs();

void setup() {
  Serial.begin(9600);

  // Steering wheel
  pinMode(BLACKPIN, INPUT_PULLUP);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(REDPIN, INPUT_PULLUP);
  pinMode(GREENPIN, OUTPUT);
  pinMode(YELLOWPIN, OUTPUT);
  pinMode(BROWNPIN, INPUT_PULLUP);

  // Output
  pinMode(VOL_DOWN_PIN, OUTPUT);
  pinMode(VOL_UP_PIN, OUTPUT);

  resetOutputs();

  flashLed();
}

#define CMD_DELAY 125

void loop() {
  resetOutputs();

  InputCommand command = getInput();
  switch(command) {
    case VolUp:
      Serial.println("Cmd volup");
      digitalWrite(VOL_UP_PIN, LOW);
      delay(CMD_DELAY);
      break;
    case VolDown:
      Serial.println("Cmd voldown");
      digitalWrite(VOL_DOWN_PIN, LOW);
      delay(CMD_DELAY);
      break;
  }
}

void resetOutputs() {
  digitalWrite(VOL_UP_PIN, HIGH);
  digitalWrite(VOL_DOWN_PIN, HIGH);
}

InputCommand getInput() {
  static unsigned char pin_cycle_current = 0;  // To keep track of which output pin is currently LOW
  static unsigned char pin_cycle_stored;       // To store the last known scrollwheel position
  static boolean first_run = true;             // After booting, there is no known last position for the scrollwheel
  // So on the first poll of the scrollwheel just store the current position and don't send a command
  unsigned char i;

  if (++pin_cycle_current > (OUT_PINS - 1)) pin_cycle_current = 0;     // Reset pin_cycle_current counter after last pin

  for (i = 0; i < OUT_PINS; i++) {                                     // Cycle through the output pins, setting one of them LOW and the rest HIGH
    if (i == pin_cycle_current)
      digitalWrite(out_pins[i], LOW);
    else
      digitalWrite(out_pins[i], HIGH);
  }

  if (!digitalRead(BROWNPIN)) {                                        // We're only interested if this pin is being pulled LOW
    if (pin_cycle_current != pin_cycle_stored) {                       // If the output that's currently LOW is different from the one that was LOW the last time
      //   we came through here, then the scrollwheel has changed position
      signed char scrollwheel_current = pin_cycle_current - pin_cycle_stored; // Result of this calculation can range from -2 to 2
      pin_cycle_stored = pin_cycle_current;                            // Store which output pin is currently LOW
      if (first_run) {                                                 // If this is the first run, don't send a command
        first_run = false;                                             //   (since there was no previously known scrollwheel position)
        return InputCommand::None;
      }
      if ((scrollwheel_current == 1) || (scrollwheel_current == -2)) { // If above calculation resulted in 1 or -2 the scrollwheel was rotated up (ccw)
        return InputCommand::FolderBack;
      }else {                                                          // If above calculation resulted in anything else the scrollwheel was rotated down (cw)
        return InputCommand::FolderForward;
      }
    }
  }

  if (!digitalRead(REDPIN)) {   // We're only interested if this pin is being pulled LOW
    switch(pin_cycle_current) {
      case 0:                     // RED (input) is LOW while GREEN (output) is LOW: bottom button pressed
        return InputCommand::Source;
      case 1:                     // RED (input) is LOW while BLUE (output) is LOW: volume + button pressed
        return InputCommand::VolUp;
      case 2:                     // RED (input) is LOW while YELLOW (output) is LOW: volume - button pressed
        return InputCommand::VolDown;
      default:
        return InputCommand::None;
    }
  }

  if (!digitalRead(BLACKPIN)) { // We're only interested if this pin is being pulled LOW
    switch(pin_cycle_current) {
      case 0:                     // BLACK (input) is LOW while GREEN (output) is LOW: top right button is pressed
        return InputCommand::TrackForward;
      case 1:                     // BLACK (input) is LOW while BLUE (output) is LOW: mute button is pressed
        return InputCommand::Mute;
      case 2:                     // BLACK (input) is LOW while YELLOW (output) is LOW: top left button is pressed
        return InputCommand::TrackBack;
      default:
        return InputCommand::None;
    }
  }

  return InputCommand::None;
}

void flashLed() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  for (unsigned char i = 0; i <= 7; i++) {
    delay(100);
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  }

  delay(100);
  digitalWrite(LEDPIN, LOW);
}
