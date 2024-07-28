#include <Arduino.h>
#include "relay.h"
#include "pins.h"
#include "debug.h"

void setRelay(int pin) {
  digitalWrite(pin, HIGH);
}

void unsetRelay(int pin) {
  digitalWrite(pin, LOW);
}

void setAuxPower(bool on) {
  fSerialWrite("Switching aux power relay: %d\n", on);
  on ? setRelay(RE_AUX_POWER_PIN) : unsetRelay(RE_AUX_POWER_PIN);
}

void setAmpRemote(bool on) {
  fSerialWrite("Switching amp relay: %d\n", on);
  on ? setRelay(RE_AMP_REMOTE_PIN) : unsetRelay(RE_AMP_REMOTE_PIN);
}

bool isCarRemoteOn() {
  return digitalRead(RE_REMOTE_DETECT_PIN) == HIGH;
}