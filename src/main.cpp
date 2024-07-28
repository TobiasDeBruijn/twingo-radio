#include "Arduino.h"
#include "pins.h"
#include "steering_wheel.h"
#include "android_auto.h"
#include "relay.h"
#include "pi.h"
#include "debug.h"

// Function prototypes
void flashLed();
void setupIoPins();
void unmuteSetVolume();

bool mainIsPiReady = false;

void setup() {
  // Serial debugging
  Serial.begin(9600);
  fSerialWrite("Hello World! Twingo Radio control unit.\n");

  // Configure IO
  setupIoPins();

  // Show a sign of life
  flashLed();

  // Enable aux power
  setAuxPower(true);
}

void loop() {
  // Handle incoming steering wheel commands
  SteeringWheelCommand command = getSteeringWheelInput();
  AACommand aaCommand;
  if(AAInputFromSteeringWheelCommand(command, &aaCommand)) {
    fSerialWrite("Received known command from steering wheel, sending to AA.\n");
    triggerAACommand(aaCommand);
  }

  // Check car remote
  if(!isCarRemoteOn()) {
    fSerialWrite("Car remote is no longer on, switching off.\n");

    // Disable speakers
    setAmpRemote(false);
    // Wait for the speakers to actually turn off
    delay(100);
    // Finally, disable AA and ourselves
    setAuxPower(false);

    // Execution stops here due to power cut
    return;
  }

  // Check if the Pi is ready for work
  if(isPiReady() && !mainIsPiReady) {
    fSerialWrite("Pi is ready. Enabling amplifier.\n");

    setAmpRemote(true);
    mainIsPiReady = true;
  }

  // Show a sign of life
  flashLed();
}

void setupIoPins() {
  fSerialWrite("Configuring IO pins.\n");
  // Firstly, set all IO pins to the correct mode

  // Arduino built-in
  pinMode(LEDPIN, OUTPUT);

  // Steering wheel
  pinMode(CAR_BLACKPIN, INPUT_PULLUP);
  pinMode(CAR_BLUEPIN, OUTPUT);
  pinMode(CAR_REDPIN, INPUT_PULLUP);
  pinMode(CAR_GREENPIN, OUTPUT);
  pinMode(CAR_YELLOWPIN, OUTPUT);
  pinMode(CAR_BROWNPIN, INPUT_PULLUP);

  // Output to the raspberry pi
  pinMode(AA_VOL_DOWN_PIN, OUTPUT);
  pinMode(AA_VOL_UP_PIN, OUTPUT);
  pinMode(AA_MUTE_PIN, OUTPUT);
  pinMode(AA_TRACK_NEXT_PIN, OUTPUT);
  pinMode(AA_TRACK_PREV_PIN, OUTPUT);

  // Input from the raspberry pi
  pinMode(PI_READY_PIN, INPUT_PULLDOWN);

  // Input from relays
  pinMode(RE_REMOTE_DETECT_PIN, INPUT_PULLDOWN);

  // Output to relays
  pinMode(RE_AMP_REMOTE_PIN, OUTPUT);
  pinMode(RE_AUX_POWER_PIN, OUTPUT);


  // Secondly, some pins must be high by default
  digitalWrite(AA_VOL_DOWN_PIN, HIGH);
  digitalWrite(AA_VOL_UP_PIN, HIGH);
  digitalWrite(AA_MUTE_PIN, HIGH);
  digitalWrite(AA_TRACK_NEXT_PIN, HIGH);
  digitalWrite(AA_TRACK_PREV_PIN, HIGH);
}

void flashLed() {
  digitalWrite(LEDPIN, LOW);

  for (unsigned char i = 0; i <= 7; i++) {
    delay(100);
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  }

  delay(100);
  digitalWrite(LEDPIN, LOW);
}
