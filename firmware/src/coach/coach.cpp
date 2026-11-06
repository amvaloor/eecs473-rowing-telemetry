#include <esp_now.h>
#include <WiFi.h>
#include "rowing_softap.h"

// Initialize the SoftAP with a network name and password for your tablet
Rowing::SoftAP coachAP("RowingCoach", "rowing123");

uint8_t coachAddress[] = {0x48, 0xAF, 0xF3, 0x69, 0x9D, 0x94};
uint8_t seat1Address[] = {0x48, 0xAF, 0xF3, 0x6A, 0x6E, 0x98};

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int seat_number;
  float gyroX;
  float gyroY;
  float gyroZ;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Flag to tell the main loop new data is ready for the tablet
volatile bool newData = false;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("seat number: ");
  Serial.println(myData.seat_number);
  Serial.print("Gyro (deg/s) | X: ");
  Serial.print(myData.gyroX);
  Serial.print("\t| Y: ");
  Serial.print(myData.gyroY);
  Serial.print("\t| Z: ");
  Serial.println(myData.gyroZ);

  // Set flag so the loop() knows to send it via WebSocket
  newData = true;
}
 
esp_now_peer_info_t peerInfo;
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device to act as BOTH a Wi-Fi Station and an Access Point
  WiFi.mode(WIFI_AP_STA);

  // Start the Access Point on Channel 1
  // (Ensure your sender ESP is also set to transmit on Channel 1!)
  if (!coachAP.begin(1)) {
    Serial.println("SoftAP failed to start.");
  } else {
    Serial.println("SoftAP started. Connect tablet to Wi-Fi: 'RowingCoach'");
  }

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Keep the WebServer and WebSocket server running
  coachAP.process();

  // If ESP-NOW received data, broadcast it to the tablet
  if (newData) {
    newData = false; // Reset the flag

    // Format the gyro data into a single string for the webpage
    String payload = "Seat: " + String(myData.seat_number) + 
                     " <br>X: " + String(myData.gyroX, 2) + 
                     " <br>Y: " + String(myData.gyroY, 2) + 
                     " <br>Z: " + String(myData.gyroZ, 2);

    coachAP.broadcastString(payload);
  }
}