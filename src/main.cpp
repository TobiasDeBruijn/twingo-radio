#include "Arduino.h"
#include "pins.h"
#include "steering_wheel.h"

#define CMD_DELAY 125

// Function prototypes
void flashLed();
void resetOutputs();
void handleInputCommand(InputCommand inputCommand);
void setupIoPins();
void unmute();

/// Volume 'pulses' since boot
int currentVolume = 0;
/// Whether we are currently muted
bool isMuted = false;

void setup() {
  Serial.begin(9600);
  setupIoPins();
  resetOutputs();
  flashLed();
}

void loop() {
  // Make sure the outputs are high, so that the commands dont
  resetOutputs();

  InputCommand command = getSteeringWheelInput();
  handleInputCommand(command);
}

void setupIoPins() {
  // Steering wheel
  pinMode(CAR_BLACKPIN, INPUT_PULLUP);
  pinMode(CAR_BLUEPIN, OUTPUT);
  pinMode(CAR_REDPIN, INPUT_PULLUP);
  pinMode(CAR_GREENPIN, OUTPUT);
  pinMode(CAR_YELLOWPIN, OUTPUT);
  pinMode(CAR_BROWNPIN, INPUT_PULLUP);

  // Output to the raspberry pi
  pinMode(VOL_DOWN_PIN, OUTPUT);
  pinMode(VOL_UP_PIN, OUTPUT);
  pinMode(MUTE_PIN, OUTPUT);
  pinMode(TRACK_NEXT_PIN, OUTPUT);
  pinMode(TRACK_PREV_PIN, OUTPUT);
}

void handleInputCommand(InputCommand inputCommand) {
  switch(inputCommand) {
    case VolUp: {
      Serial.println("Cmd volup");
      digitalWrite(VOL_UP_PIN, LOW);

      currentVolume++;
      // We unmute if the volume is changed
      unmute();

      delay(CMD_DELAY);
      break;
    }
    case VolDown: {
      Serial.println("Cmd voldown");
      digitalWrite(VOL_DOWN_PIN, LOW);

      // Decrement the current volume or set to zero.
      currentVolume = currentVolume == 0 ? currentVolume = 0 : currentVolume - 1;
      // We unmute if the volume is changed
      unmute();

      delay(CMD_DELAY);
      break;
    }
    case TrackForward: {
      Serial.println("Cmd Track Forward");

      digitalWrite(TRACK_NEXT_PIN, LOW);

      delay(CMD_DELAY);
      break;
    }
    case TrackBack: {
      Serial.println("Cmd Track Back");

      digitalWrite(TRACK_PREV_PIN, LOW);

      delay(CMD_DELAY);
      break;
    }
    case Mute: {
      Serial.println("Cmd mute");

      isMuted = true;
      digitalWrite(MUTE_PIN, LOW);

      delay(CMD_DELAY);
      break;
    }
  }
}

void unmute() {
  if(!isMuted) {
    return;
  }

  isMuted = false;

  // Set the volume back to what it was before muting
  for(int i = 0; i < currentVolume; i++) {
    digitalWrite(VOL_UP_PIN, LOW);
    delay(50);
    digitalWrite(VOL_UP_PIN, HIGH);
  }
}

/**
 * Reset the output pins going to the Raspberry Pi.
 */
void resetOutputs() {
  digitalWrite(VOL_UP_PIN, HIGH);
  digitalWrite(VOL_DOWN_PIN, HIGH);
  digitalWrite(MUTE_PIN, HIGH);
  digitalWrite(TRACK_NEXT_PIN, HIGH);
  digitalWrite(TRACK_PREV_PIN, HIGH);
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
