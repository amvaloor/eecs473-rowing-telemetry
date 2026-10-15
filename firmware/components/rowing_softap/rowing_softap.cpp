// rowing_softap.cpp
#include "rowing_softap.h"
#include "stdlib_noniso.h"
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <cstdint>
#include <cstring>
#include <string>
static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<body style="font-size: 5em; font-family: sans-serif; text-align: center;">
  Counter: <span id="c">0</span>
  <script>
    new WebSocket('ws://' + location.hostname + ':81/').onmessage = e => c.innerText = e.data;
  </script>
</body>
)rawliteral";
namespace Rowing {
SoftAP::SoftAP(const char *ssid, const char *password) {
  strcpy(this->ssid, ssid);
  strcpy(this->password, password);
  w = new WebServer(80);
  ws = new WebSocketsServer(81);
}

SoftAP::~SoftAP() {
  delete w;
  delete ws;
}

bool SoftAP::begin(uint8_t channel) {
  if (!WiFi.softAP(ssid, password, channel)) {
    return false;
  }
  w->on("/", [this]() { w->send(200, "text/html", INDEX_HTML); });
  w->begin();

  ws->begin();

  return true;
}

bool SoftAP::stop() { return WiFi.softAPdisconnect(true); }

void SoftAP::process() {
  if (w)
    w->handleClient();
  if (ws)
    ws->loop();
}

uint8_t SoftAP::get_chan() const { return WiFi.channel(); }

bool SoftAP::is_active() const {
  return WiFi.softAPIP() != IPAddress(0, 0, 0, 0);
}

uint8_t SoftAP::get_client_count() const {}
void SoftAP::broadcastCounter(int counter) {
  if (ws) {
    ws->broadcastTXT(String(counter).c_str());
  }
}
} // namespace Rowing
