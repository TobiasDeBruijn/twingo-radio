#include <Arduino.h>
#include "android_auto.h"
#include "pins.h"

const int TRIGGER_DELAY = 125;

int aaCurrentVolume = 0;
bool aaIsMuted = false;

void triggerPin(int pin) {
  digitalWrite(pin, LOW);
  delay(TRIGGER_DELAY);
  digitalWrite(pin, HIGH);
}

void unmuteSetVolume() {
  aaIsMuted = false;

  // Set the volume back to what it was before muting
  for(int i = 0; i < aaCurrentVolume; i++) {
    triggerPin(AA_VOL_UP_PIN);
  }
}

void triggerAACommand(AACommand inputType) {
    switch(inputType) {
      case AA_VOL_UP:
        if(aaIsMuted) {
          unmuteSetVolume();
        }

        aaCurrentVolume++;
        triggerPin(AA_VOL_UP_PIN);
        break;
      case AA_VOL_DOWN:
        if(aaIsMuted) {
          unmuteSetVolume();
        }

        aaCurrentVolume = min(aaCurrentVolume - 1, 0);
        triggerPin(AA_VOL_DOWN_PIN);
        break;
      case AA_TRACK_NEXT:
        triggerPin(AA_TRACK_NEXT_PIN);
        break;
      case AA_TRACK_PREV:
        triggerPin(AA_TRACK_PREV_PIN);
        break;
      case AA_MUTE:
        triggerPin(AA_TRACK_PREV_PIN);
        if(aaIsMuted) {
          unmuteSetVolume();
        } else {
          aaIsMuted = true;
        }
    }
}

bool AAInputFromSteeringWheelCommand(SteeringWheelCommand cmd, AACommand *out) {
  switch(cmd) {
    case ST_VolUp:
      *out = AA_VOL_UP;
      return true;
    case ST_VolDown:
      *out = AA_VOL_DOWN;
      return true;
    case ST_Mute:
      *out = AA_MUTE;
      return true;
    case ST_FolderForward:
      *out = AA_TRACK_NEXT;
      return true;
    case ST_FolderBack:
      *out = AA_TRACK_PREV;
      return true;
    default:
      return false;
  }
}