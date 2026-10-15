// rowing_softap.cpp
#include "rowing_softap.h"
#include <WiFi.h>
#include <cstdint>
#include <cstring>
namespace Rowing {
SoftAP::SoftAP(const char *ssid, const char *password) {
  strcpy(this->ssid, ssid);
  strcpy(this->password, password);
}

bool SoftAP::begin(uint8_t channel) {
  return WiFi.softAP(ssid, password, channel);
}

bool SoftAP::stop() { return WiFi.softAPdisconnect(true); }

void SoftAP::process() {}

uint8_t SoftAP::get_chan() const { return WiFi.channel(); }

bool SoftAP::is_active() const {
  return WiFi.softAPIP() != IPAddress(0, 0, 0, 0);
}

uint8_t SoftAP::get_client_count() const {}
} // namespace Rowing
