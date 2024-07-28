#include <Arduino.h>
#include "pi.h"
#include "pins.h"

bool isPiReady() {
  return digitalRead(PI_READY_PIN) == HIGH;
}