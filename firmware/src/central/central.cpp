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
  static int force = 0;
  static int angle = 0;
  static int dist = 0;
  static int speed = 0;

  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 50) {
    lastSend = millis();
    cu.rower_data_send(force, angle, dist, speed);
    force = (force + 20) % 900;
    angle = (angle + 20) % 800;
    dist += speed;
    speed = random(10);
  }
  delay(2);
}
