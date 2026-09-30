// network.h
#pragma once

#include "espnow.h"
#include "softap.h"
#include <cstdint>
class Network {
private:
  ESP_NOW esp;
  SoftAP ap;
  uint8_t channel;
  bool is_leader;

public:
  /**
   * @brief Create a unified network manager
   *
   * @param is_leader Whether this node is the leader or not
   * @param ap_ssid Name of wifi network to be hosted. Ignored if !is_leader
   * @param ap_pwd Password for wifi network to be hosted. Ignored if !is_leader
   *
   * @return A Network object
   */
  Network(bool is_leader, const char *ap_ssid, const char *ap_pwd);

  /**
   * @brief Begin hosting wifi network if needed and processing data
   *
   * @param channel Wifi channel to use
   *
   * @return True if success
   */
  bool begin();

  /**
   * @brief Stop wifi network and data processing
   *
   * @return True if success
   */
  bool stop();

  /**
   * @brief Run background tasks and process data
   */
  void process();

  /**
   * @brief Send packet to leader
   *
   * @return True if success
   */
  bool send_packet();

  /**
   * @brief Get most recent packet from a specified follower
   *
   * @param node_id The identification of the node from which to check the
   * packet
   * @param p The packet into which the collected data should be stored
   *
   * @return True if success
   */
  bool get_packet(int node_id, Packet &p);

  /**
   * @brief Get packets from all nodes, including this one
   *
   * @param ps The array into which the data should be copied
   * @param size Number of elements ps can hold
   *
   * Only returns more that one packet if this is the leader node
   *
   * @return True if success
   */
  bool get_all_packets(Packet *ps, uint32_t size);

  /**
   * @brief Registers the leader of this node
   *
   * @param mac_addr The MAC address of the leader in the form of an array
   *
   * @return True if success
   */
  bool register_leader(const uint8_t *mac_addr);

  /**
   * @brief Registers a follower of this node
   *
   * @param mac_addr The MAC address of the follower in the form of an array
   *
   * @return True if success
   */
  bool register_follower(const uint8_t *mac_addr);

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
