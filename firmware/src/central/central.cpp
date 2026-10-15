#include "central_unit.h"
#include "rowing_softap.h"
#include <Arduino.h>

Rowing::SoftAP softap("RowingTelemetry", "RowingPassword");

void setup() {
  // Initialization code
  softap.begin(1);
}

void loop() {
  // Main execution loop
  softap.process();
  static unsigned long lastUpdate = 0;
  static unsigned long counter = 0;
  if (millis() - lastUpdate >= 50) {
    lastUpdate = millis();
    counter++;
    softap.broadcastCounter(counter);
  }
}
