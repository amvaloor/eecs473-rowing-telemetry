// rowing_espnow.cpp

#include "rowing_espnow.h"
#include <cstdint>
#include <cstring>
#include <esp_wifi.h>
namespace Rowing {
ESP_NOW *ESP_NOW::instance = nullptr;

void ESP_NOW::handleDataRecv(const uint8_t *mac, const uint8_t *incomingData,
                             int len) {
  if (len != sizeof(Packet))
    return;
  for (int i = 0; i < registered_followers; i++) {
    if (memcmp(followers[i].mac, mac, 6) == 0) {
      if (xSemaphoreTake(s, portMAX_DELAY) == pdTRUE) {
        memcpy(&followers[i].most_recent_packet, incomingData, sizeof(Packet));
        xSemaphoreGive(s);
      }
      break;
    }
  }
}

void ESP_NOW::handleDataSent(const uint8_t *mac, esp_now_send_status_t status) {
}

ESP_NOW::ESP_NOW() {
  followers = nullptr;
  s = nullptr;
}

ESP_NOW::~ESP_NOW() {
  stop();
  if (followers != nullptr) {
    delete[] followers;
    followers = nullptr;
  }
  if (s != nullptr) {
    vSemaphoreDelete(s);
    s = nullptr;
  }
}

bool ESP_NOW::begin(uint8_t channel, int follower_count) {
  instance = this;

  this->follower_count = follower_count;

  if (followers != nullptr) {
    delete[] followers;
  }

  if (follower_count > 0) {
    this->followers = new Node[follower_count];
  }

  esp_wifi_set_max_tx_power(78);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G |
                                         WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

  if (s == nullptr) {
    s = xSemaphoreCreateMutex();
  }

  if (esp_now_init() != ESP_OK) {
    return false;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecvWrapper));
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSentWrapper));

  return true;
}

void ESP_NOW::OnDataRecvWrapper(const uint8_t *mac, const uint8_t *incomingData,
                                int len) {
  if (instance != nullptr) {
    instance->handleDataRecv(mac, incomingData, len);
  }
}

void ESP_NOW::OnDataSentWrapper(const uint8_t *mac,
                                esp_now_send_status_t status) {
  if (instance != nullptr) {
    instance->handleDataSent(mac, status);
  }
}

bool ESP_NOW::stop() { return esp_now_deinit() == ESP_OK; }

bool ESP_NOW::get_packet(int node_id, Packet &p) {
  if (node_id < 0 || node_id >= registered_followers)
    return false;
  if (xSemaphoreTake(s, portMAX_DELAY) == pdTRUE) {
    memcpy(&p, &followers[node_id].most_recent_packet, sizeof(Packet));
    xSemaphoreGive(s);
  }
  return true;
}

bool ESP_NOW::get_all_packets(Packet *ps, uint32_t size) {
  if (ps == nullptr || size == 0)
    return false;
  uint32_t limit =
      (size < (uint32_t)registered_followers) ? size : registered_followers;
  if (xSemaphoreTake(s, portMAX_DELAY) == pdTRUE) {
    for (uint32_t i = 0; i < limit; i++) {
      memcpy(&ps[i], &followers[i].most_recent_packet, sizeof(Packet));
    }
    xSemaphoreGive(s);
  }
  return true;
}

bool ESP_NOW::send_packet(const Packet &p) {
  esp_err_t result = esp_now_send(target, (const uint8_t *)&p, sizeof(Packet));
  return result == ESP_OK;
}

bool ESP_NOW::register_target(const uint8_t *mac_addr) {
  memcpy(this->target, mac_addr, 6);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac_addr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return false;
  }
  return true;
}

bool ESP_NOW::register_follower(const uint8_t *mac_addr) {
  if (registered_followers >= follower_count)
    return false;
  memcpy(followers[registered_followers].mac, mac_addr, 6);
  memset(&followers[registered_followers].most_recent_packet, 0,
         sizeof(Packet));
  registered_followers++;
  return true;
}
} // namespace Rowing
