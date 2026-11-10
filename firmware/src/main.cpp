#include "rowing_imu.h"

using namespace Rowing;

IMU imu;

unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL_MS = 100; // Update display every 100ms (10 Hz)

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial connection (USB Native boards)

  Serial.println("\n==========================================");
  Serial.println("         Rowing IMU Test Sketch           ");
  Serial.println("==========================================");

  // Initialize sensor with error checking
  if (!imu.begin()) {
    Serial.println("CRITICAL ERROR: Failed to detect BNO08x!");
    Serial.println("Check SDA/SCL connections and I2C power.");
    while (1) { delay(10); } // Halt execution
  }

  Serial.println("BNO08x initialized successfully.");
  Serial.println("Instructions:");
  Serial.println("  1. Place oar in neutral position.");
  Serial.println("  2. Type 't' in the Serial Monitor and hit Enter to TARE (zero out).");
  Serial.println("==========================================\n");

  // Allow fusion engine 2 seconds to stabilize before auto-taring
  delay(2000);
  imu.update();
  imu.tare();
  Serial.println(">> Automatic Initial Tare Applied <<\n");
}

void loop() {
  // Keep internal IMU registers continuously updated
  imu.update();

  // Listen for 't' over Serial to manual tare on demand
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == 't' || ch == 'T') {
      imu.tare();
      Serial.println("\n*** OAR TARED (ZEROED) AT CURRENT POSITION ***\n");
    }
  }

  // Throttle output frequency to keep Serial Monitor readable
  if (millis() - lastPrintTime >= PRINT_INTERVAL_MS) {
    lastPrintTime = millis();

    // Print calculated relative oar angles
    Serial.print("SWEEP (Yaw): ");
    Serial.print(imu.get_sweep(), 1);
    Serial.print("°\t| ELEV (Pitch): ");
    Serial.print(imu.get_elevation(), 1);
    Serial.print("°\t| FEATHER (Roll): ");
    Serial.print(imu.get_feather(), 1);
    Serial.print("°\t|| Gyro Z: ");
    Serial.print(imu.get_gyroZ(), 2);
    Serial.println(" deg/s");
  }
}