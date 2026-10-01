// coach_unit.h
#pragma once

#include "espnow.h"
#include "usb.h"
class CoachUnit {
private:
  ESP_NOW esp;
  USB usb;

public:
  /**
   * @brief Create a new CoachUnit
   *
   * @return A new CoachUnit
   */
  CoachUnit();

  /**
   * @brief Start up relevant devices
   *
   * @return True if success
   */
  bool begin();

  /**
   * @brief Stop relevant devices
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Get the most recently received set of data
   *
   * @param ps The array into which the data should be copied
   * @param size Number of elements ps can hold
   *
   * @return True if success
   */
  bool get_packets(Packet *ps, uint32_t size);

  /**
   * @brief Send data to laptop via USB
   *
   * @return True if success
   */
  bool send_data();
};
