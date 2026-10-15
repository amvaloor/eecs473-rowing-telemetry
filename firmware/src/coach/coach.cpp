#include "coach_unit.h"
#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>


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

}
 
 esp_now_peer_info_t peerInfo;
 


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

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

}