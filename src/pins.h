#include "Arduino.h"

#ifndef TWINGO_RADIO_PINS_H
#define TWINGO_RADIO_PINS_H

// Input pins from car steering wheel
#define CAR_BLACKPIN 3
#define CAR_BLUEPIN 5
#define CAR_REDPIN 4
#define CAR_GREENPIN 6
#define CAR_YELLOWPIN 7
#define CAR_BROWNPIN 2

#define NUM_CAR_OUTPUT_PINS 3

// Raspberry pi outputs
#define AA_VOL_DOWN_PIN 14
#define AA_VOL_UP_PIN 10
#define AA_MUTE_PIN 9
#define AA_TRACK_NEXT_PIN 15
#define AA_TRACK_PREV_PIN 16

// Raspberry pi inputs
#define PI_READY_PIN 8

// Relay outputs
#define RE_AMP_REMOTE_PIN 20
#define RE_AUX_POWER_PIN 17

// Relay inputs
#define RE_REMOTE_DETECT_PIN 21

// Arduino pins
#define LEDPIN 13


#endif //TWINGO_RADIO_PINS_H
