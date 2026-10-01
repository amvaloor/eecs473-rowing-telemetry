// oar_unit.h
#pragma once
#include "espnow.h"
#include "imu.h"
#include "strain_gauge.h"
#include <cstdint>

class OarUnit {
private:
  ESP_NOW esp;
  IMU imu;
  StrainGauge sg1;
  StrainGauge sg2;

public:
  /**
   * @brief Create an OarUnit object
   *
   * @return An OarUnit object
   */
  OarUnit(/* imu pins */);

  /**
   * @brief Start hosting a wifi network and processing ESP data
   *
   * @param channel Wifi channel to use
   *
   * @return True if success
   */
  bool begin(uint8_t channel);

  /**
   * @brief Stop hosting network and processing ESP data
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Run background tasks and process data
   */
  void process();

  /**
   * @brief Send packet to central unit
   *
   * @return True if success
   */
  bool send_packet();

  /**
   * @brief Registers the central unit
   *
   * @param mac_addr The MAC address of the central unit in the form of an array
   *
   * @return True if success
   */
  bool register_central(const uint8_t *mac_addr);

  /**
   * @brief Gets the channel used to host the network
   *
   * @return Wifi channel
   */
  uint8_t get_chan() const;

  /**
   * @brief Checks if wifi network is currently active
   *
   * @return True if active
   */
  bool is_active() const;

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

  /**
   * @brief Gets the current force
   *
   * @return The current force in N
   */
  int get_force();
};
