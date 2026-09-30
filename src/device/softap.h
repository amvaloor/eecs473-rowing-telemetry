// softap.h
#pragma once

#include <cstdint>
class SoftAP {
private:
  char ssid[32];
  char password[64];

public:
  /**
   * @brief Creates a SoftAP object
   *
   * @param ssid Name of the network to be created
   * @param password Password to be assigned to the network
   *
   * @return A SoftAP object
   */
  SoftAP(const char *ssid, const char *password = nullptr);

  /**
   * @brief Starts hosting the network
   *
   * @param channel Wifi channel to use. Must match the one used for ESP-NOW
   *
   * @return True if success
   */
  bool begin(uint8_t channel);

  /**
   * @brief Stops the network
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Process requests and perform background tasks
   */
  void process();

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
   * @brief Gets number of connected clients
   *
   * @return Number of connected clients
   */
  uint8_t get_client_count() const;
};
