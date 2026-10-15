// rowing_softap.cpp
#include "rowing_softap.h"
#include <Wifi.h>
#include <cstdint>
#include <cstring>
namespace Rowing {
SoftAP::SoftAP(const char *ssid, const char *password) {
  strcpy(this->ssid, ssid);
  strcpy(this->password, password);
}

bool SoftAP::begin(uint8_t channel) {}

bool SoftAP::stop() {}

void SoftAP::process() {}

uint8_t SoftAP::get_chan() const {}

bool SoftAP::is_active() const {}

uint8_t SoftAP::get_client_count() const {}
} // namespace Rowing
