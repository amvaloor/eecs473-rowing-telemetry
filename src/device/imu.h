// imu.h
#pragma once

class IMU {
private:
  // TODO pin numbers
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;

public:
  /**
   * @brief Creates an IMU object
   *
   * @return An IMU object
   */
  IMU(/* TODO relevant pins go here*/);

  /**
   * @brief Sets up the object to start collecting data
   *
   * @return True if success
   */
  bool begin();

  /**
   * @brief Reads all data into member variables
   *
   * @return True if success
   */
  bool update();

  /**
   * @brief Gets the acceleration in the X direction
   *
   * @return The current acceleration in the X direction in m/s^2
   */
  float get_accX() const;

  /**
   * @brief Gets the acceleration in the Y direction
   *
   * @return The current acceleration in the Y direction in m/s^2
   */
  float get_accY() const;

  /**
   * @brief Gets the acceleration in the Z direction
   *
   * @return The current acceleration in the Z direction in m/s^2
   */
  float get_accZ() const;

  /**
   * @brief Gets the angular velocity in the pitch
   *
   * @return The angular velocity in the pitch in degrees/s
   */
  float get_gyroX() const;

  /**
   * @brief Gets the angular velocity in the roll
   *
   * @return The angular velocity in the roll in degrees/s
   */
  float get_gyroY() const;

  /**
   * @brief Gets the angular velocity in the yaw
   *
   * @return The angular velocity in the yaw in degrees/s
   */
  float get_gyroZ() const;
};
