#include "Arduino.h"
#include "pins.h"
#include "steering_wheel.h"
#include "android_auto.h"
#include "relay.h"
#include "pi.h"
#include "debug.h"

// Function prototypes
void setupIoPins();
void unmuteSetVolume();
void toggleLed();
void handeCarRemote();

/// Whether the Pi ready procedure has been executed
bool mainIsPiReady = false;
/// Whether the car's remote signal off procedure
/// has been executed
bool carRemoteOff = false;

void setup() {
  // Serial debugging
  Serial.begin(9600);
  fSerialWrite("Hello World! Twingo Radio control unit.\n");

  // Configure IO
  setupIoPins();

  // Enable aux power
  setAuxPower(true);
}

void loop() {
  // Handle incoming steering wheel commands
  SteeringWheelCommand command = getSteeringWheelInput();
  if(command != ST_None) {
    AACommand aaCommand;
    if(AAInputFromSteeringWheelCommand(command, &aaCommand)) {
      fSerialWrite("Received known command from steering wheel, sending to AA.\n");
      triggerAACommand(aaCommand);
    } else {
      fSerialWrite("Received unknown command from steering wheel\n");
    }
  }

  // Check car remote
  handeCarRemote();

  // Due to a flaky hardware connection I haven't bothered to fix,
  // and a broken pulldown resistor between PI_READY and GND,
  // the ready signal from the Pi usually doesn't reach us
  // bypass the pin check, and just go after 10s.

  // Check for 10sec since boot
  bool isPiReadyIsh = millis() / 1000 >= 10;

  // Incase the hardware connection is playing nice, and the pi is ready
  // before the timer has passed, we can proceed early.
  if((isPiReady() || isPiReadyIsh) && !mainIsPiReady) {
    fSerialWrite("Pi is ready. Enabling amplifier.\n");

    // Still want to give it some time
    delay(2000);

    setAmpRemote(true);
    mainIsPiReady = true;
  }

  toggleLed();
}

void handeCarRemote() {
  // Check whether the remote is not on anymore,
  // and check that this procedure hasn't already been executed
  if(!isCarRemoteOn() && !carRemoteOff) {
    // The car remote is off while starting the car, we need to bridge this
    delay(5000);
    if(isCarRemoteOn()) {
      // Car remote is on again.
      return;
    }

    fSerialWrite("Car remote is no longer on, switching off.\n");

    // Disable speakers
    setAmpRemote(false);

    // Wait for the speakers to actually turn off
    delay(2500);

    // Finally, disable AA and ourselves
    setAuxPower(false);

    // Execution stops here due to power cut, but in case it is not,
    // like when powering the arduino through a laptop for development,
    // this variable and the check above prevent a lot of spamming.
    carRemoteOff = true;
    return;
  }
}

void toggleLed() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
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
  pinMode(PI_READY_PIN, INPUT);

  // Input from relays
  pinMode(RE_REMOTE_DETECT_PIN, INPUT);

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