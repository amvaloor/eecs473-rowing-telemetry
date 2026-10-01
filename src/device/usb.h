// usb.h
#pragma once

#include <cstdint>
class USB {
private:
  uint32_t baud_rate;

public:
  /**
   * @brief Create a USB object
   *
   * @return A new USB object
   */
  USB(uint32_t baud_rate);

  /**
   * @brief Starts using the port
   *
   * @return True if success
   */
  bool begin();

  /**
   * @brief Stops using the port
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Write to USB
   *
   * @param data Data to write
   * @param data_size Length of data
   *
   * @return True if success
   */
  bool write(char *data, uint32_t data_size);
};
