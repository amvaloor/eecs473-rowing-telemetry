// rowing_imu.h
#pragma once

#include <Adafruit_BNO08x.h>

namespace Rowing {

/**
 * @brief Represents a unit quaternion for 3D orientation tracking
 */
struct Quaternion {
  float w{1.0f};
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};

  Quaternion() = default;
  Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
};

class IMU {
private:
  // Pin numbers
  int SDA;
  int SDC;

  // Accelerometer values (m/s^2)
  float accX{0.0f};
  float accY{0.0f};
  float accZ{0.0f};

  // Gyroscope values (deg/s)
  float gyroX{0.0f};
  float gyroY{0.0f};
  float gyroZ{0.0f};

  // Orientation data
  Quaternion currentQuat;
  Quaternion refQuatInv{1.0f, 0.0f, 0.0f, 0.0f}; // Inverse quaternion at zero position

  // Calculated relative oar angles (degrees)
  float sweep{0.0f};     // Horizontal stroke arc (Yaw)
  float elevation{0.0f}; // Vertical blade depth (Pitch)
  float feather{0.0f};   // Blade twist (Roll)

  // Sensor value struct
  sh2_SensorValue_t sensorValue;

  // Adafruit object
  Adafruit_BNO08x imu;

  // Internal helper to process relative quaternion into Euler angles
  void calculateOarAngles();

public:
  /**
   * @brief Creates an IMU object with specified I2C pins
   * @param SDA I2C data pin number
   * @param SDC I2C clock pin number
   */
  IMU(int SDA, int SDC);

  /**
   * @brief Creates an IMU object with default hardware I2C pins
   */
  IMU();

  /**
   * @brief Sets up the sensor and enables rotation vector reports
   * @return True if initialization succeeds
   */
  bool begin();

  /**
   * @brief Stops collecting data
   * @return True if success
   */
  bool stop();

  /**
   * @brief Reads all incoming sensor reports and updates member variables
   * @return True if new data was successfully processed
   */
  bool update();

  /**
   * @brief Captures current orientation as the reference (0,0,0) baseline angle
   */
  void tare();

  /**
   * @brief Gets horizontal oar stroke arc relative to tare position (Yaw)
   * @return Sweep angle in degrees
   */
  float get_sweep() const;

  /**
   * @brief Gets vertical oar depth relative to tare position (Pitch)
   * @return Elevation angle in degrees
   */
  float get_elevation() const;

  /**
   * @brief Gets oar blade twist relative to tare position (Roll)
   * @return Feather angle in degrees
   */
  float get_feather() const;

  /**
   * @brief Gets acceleration in X direction
   * @return Acceleration in m/s^2
   */
  float get_accX() const;

  /**
   * @brief Gets acceleration in Y direction
   * @return Acceleration in m/s^2
   */
  float get_accY() const;

  /**
   * @brief Gets acceleration in Z direction
   * @return Acceleration in m/s^2
   */
  float get_accZ() const;

  /**
   * @brief Gets angular velocity in Pitch axis
   * @return Angular velocity in deg/s
   */
  float get_gyroX() const;

  /**
   * @brief Gets angular velocity in Roll axis
   * @return Angular velocity in deg/s
   */
  float get_gyroY() const;

  /**
   * @brief Gets angular velocity in Yaw axis
   * @return Angular velocity in deg/s
   */
  float get_gyroZ() const;

  /**
   * @brief Gets raw absolute orientation quaternion
   * @return Quaternion struct
   */
  Quaternion get_quaternion() const;
};

} // namespace Rowing