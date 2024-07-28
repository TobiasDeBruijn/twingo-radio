#include "debug.h"
#include "Arduino.h"

int fSerialWrite(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);

  char buf[64];
  vsprintf(buf, fmt, arg);

  va_end(arg);

  Serial.write(buf);
}