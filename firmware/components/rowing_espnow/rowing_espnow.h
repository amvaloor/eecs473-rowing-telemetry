// rowing_espnow.h
#pragma once

#include <cstdint>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
namespace Rowing {
struct Packet {
  // TODO
};

struct Node {
  uint8_t mac[6];
  Packet most_recent_packet;
};

class ESP_NOW {
private:
  uint8_t target[6]; // MAC address of target
  Node *followers;
  int follower_count = 0;
  int registered_followers = 0;

  SemaphoreHandle_t s;

  static ESP_NOW *instance;

  void handleDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

  void handleDataSent(const uint8_t *mac, esp_now_send_status_t status);

public:
  /**
   * @brief Create a new ESP_NOW object
   *
   * @return A new ESP_NOW object
   */
  ESP_NOW();

  /**
   * @brief Destroy the ESP_NOW object
   */
  ~ESP_NOW();

  /**
   * @brief Start ESP-NOW
   *
   * @param channel Wifi channel to use. Must match the one used for SoftAP
   * @param follower_count The number of followers this node can have. Only
   * important if this is the central unit
   *
   * @return True if success
   */
  bool begin(uint8_t channel, int follower_count);

  static void OnDataRecvWrapper(const uint8_t *mac, const uint8_t *incomingData,
                                int len);

  static void OnDataSentWrapper(const uint8_t *mac,
                                esp_now_send_status_t status);
  /**
   * @brief Stop ESP-NOW
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Get the most recent packet sent by a given node
   *
   * @param node_id The identification of the node from which to check the
   * packet
   * @param p The packet into which the collected data should be stored
   *
   * @return True if success
   */
  bool get_packet(int node_id, Packet &p);

  /**
   * @brief Get the most recently received packet from each node, including
   * itself
   *
   * @param ps The array into which the data should be copied
   * @param size Number of elements ps can hold
   *
   * @return True if success
   */
  bool get_all_packets(Packet *ps, uint32_t size);

  /**
   * @brief Send packet to the destination
   *
   * @param p The packet to send
   *
   * @return True if success
   */
  bool send_packet(const Packet &p);

  /**
   * @brief Registers the target of this node
   *
   * @param mac_addr The MAC address of the target in the form of an array
   *
   * @return True if success
   */
  bool register_target(const uint8_t *mac_addr);

  /**
   * @brief Registers a follower of this node
   *
   * @param mac_addr The MAC address of the follower in the form of an array
   *
   * @return True if success
   */
  bool register_follower(const uint8_t *mac_addr);
};
} // namespace Rowing
