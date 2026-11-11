#pragma once
#include "central_unit.h"
#include "IPAddress.h"
#include "rowing_softap.h"
#include <cstdio>

namespace Rowing {
CentralUnit::CentralUnit(const char *ap_ssid, const char *ap_pwd)
    : ap(ap_ssid, ap_pwd) {}

bool CentralUnit::begin() {
  ap.begin(1);
  return udp.begin(12345); // TODO
}

bool CentralUnit::stop() {}

void CentralUnit::process() {}

bool CentralUnit::send_packet() {}

bool CentralUnit::get_packet(int node_id, Packet &p) {}

bool CentralUnit::get_all_packets(Packet *ps, uint32_t size) {}

bool CentralUnit::register_coach(const uint8_t *mac_addr) {}

bool CentralUnit::register_oar(const uint8_t *mac_addr) {}

uint8_t CentralUnit::get_chan() const {}

bool CentralUnit::is_active() const {}

uint8_t CentralUnit::get_client_count() const {}

int CentralUnit::rower_data_receive(char *msg, int msg_len) {
  int udp_size = udp.parsePacket();
  if (udp_size) {
    return udp.read(msg, msg_len);
  }
  return 0;
}

bool CentralUnit::rower_data_send(float force, float angle, float dist, float speed) {
  udp.beginPacket(IPAddress(255, 255, 255, 255), 12345);

  char jsonbuf[128];
  snprintf(jsonbuf, 128, "{\"angle\":%.2f, \"force\":%.2f, \"distance\":%.2f, \"speed\":%.2f}", angle, force, dist, speed);

  udp.print(jsonbuf);
  udp.endPacket();
  return true;
}
} // namespace Rowing
