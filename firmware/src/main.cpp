#include "rowing_imu.h"
using namespace Rowing;

IMU imu;

void setup(){
  Serial.begin(115200);

  imu.begin();

}

void loop(){

  if (imu.update()) {
      Serial.print("Gyro (deg/s) | X: ");
      Serial.print(imu.get_gyroX(), 2);
      Serial.print("\t| Y: ");
      Serial.print(imu.get_gyroY(), 2);
      Serial.print("\t| Z: ");
      Serial.println(imu.get_gyroZ(), 2);
  }

}