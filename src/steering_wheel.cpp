#include "steering_wheel.h"
#include "pins.h"
#include <Arduino.h>

/// Output pins going to the steering wheel
unsigned char out_pins[NUM_CAR_OUTPUT_PINS] = {CAR_GREENPIN, CAR_BLUEPIN, CAR_YELLOWPIN};

// After booting, there is no known last position for the scrollwheel
static boolean first_run = true;

// To store the last known scrollwheel position
static unsigned char pin_cycle_stored;

// To keep track of which output pin is currently LOW
static unsigned char pin_cycle_current = 0;

/**
 * Parse button presses from the steering wheel buttons
 * @return The button pressed
 */
SteeringWheelCommand getSteeringWheelInput() {
  // So on the first poll of the scrollwheel just store the current position and don't send a command
  unsigned char i;

  if (++pin_cycle_current > (NUM_CAR_OUTPUT_PINS - 1)) {
    // Reset pin_cycle_current counter after last pin
    pin_cycle_current = 0;
  }

  // Cycle through the output pins, setting one of them LOW and the rest HIGH
  for (i = 0; i < NUM_CAR_OUTPUT_PINS; i++) {
    if (i == pin_cycle_current)
      digitalWrite(out_pins[i], LOW);
    else
      digitalWrite(out_pins[i], HIGH);
  }

  // We're only interested if this pin is being pulled LOW
  // and the output that's currently LOW is different from the one that was LOW the last time
  if (!digitalRead(CAR_BROWNPIN) && pin_cycle_current != pin_cycle_stored) {
    // We came through here, then the scrollwheel has changed position
    // Result of this calculation can range from -2 to 2
    signed char scrollwheel_current = pin_cycle_current - pin_cycle_stored;

    // Store which output pin is currently LOW
    pin_cycle_stored = pin_cycle_current;

    // If this is the first run, don't send a command
    // (since there was no previously known scrollwheel position)
    if (first_run) {
      first_run = false;
      return ST_None;
    }

    // If above calculation resulted in 1 or -2 the scrollwheel was rotated up (ccw)
    // If above calculation resulted in anything else the scrollwheel was rotated down (cw)
    if ((scrollwheel_current == 1) || (scrollwheel_current == -2)) {
      return ST_FolderBack;
    } else {
      return ST_FolderForward;
    }
  }

  // We're only interested if this pin is being pulled LOW
  if (!digitalRead(CAR_REDPIN)) {
    switch(pin_cycle_current) {
      case 0:
        // RED (input) is LOW while GREEN (output) is LOW: bottom button pressed
        return ST_Source;
      case 1:
        // RED (input) is LOW while BLUE (output) is LOW: volume + button pressed
        return ST_VolUp;
      case 2:
        // RED (input) is LOW while YELLOW (output) is LOW: volume - button pressed
        return ST_VolDown;
      default:
        return ST_None;
    }
  }

  // We're only interested if this pin is being pulled LOW
  if (!digitalRead(CAR_BLACKPIN)) {
    switch(pin_cycle_current) {
      case 0:
        // BLACK (input) is LOW while GREEN (output) is LOW: top right button is pressed
        return ST_TrackForward;
      case 1:
        // BLACK (input) is LOW while BLUE (output) is LOW: mute button is pressed
        return ST_Mute;
      case 2:
        // BLACK (input) is LOW while YELLOW (output) is LOW: top left button is pressed
        return ST_TrackBack;
      default:
        return ST_None;
    }
  }

  return ST_None;
}