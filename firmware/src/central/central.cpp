#include "central_unit.h"
#include <Arduino.h>

Rowing::CentralUnit cu("RowingTelemetry", "RowingPassword");

void setup() {
  // Initialization code
  cu.begin();
}

void loop() {
  // Main execution loop
  char buf[10];
  cu.rower_data_receive(buf, 10);
  cu.rower_data_send(10.0, 11.0);
}
