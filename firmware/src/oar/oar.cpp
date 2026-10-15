#include "oar_unit.h"
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "rowing_imu.h"
uint8_t centralAddress[] = {0x48, 0xAF, 0xF3, 0x6A, 0x72, 0xF4};
using namespace Rowing;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int seat_number;
  float gyroX;
  float gyroY;
  float gyroZ;
} struct_message;

IMU imu;
// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  if (!imu.begin()) {
    Serial.println("Error: IMU failed to initialize! Check wiring or address (0x4A / 0x4B).");
  } else {
    Serial.println("IMU successfully initialized.");
  }  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, centralAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  imu.update();
  myData.seat_number = 8;
  myData.gyroX = imu.get_gyroX();
  myData.gyroY = imu.get_gyroY();
  myData.gyroZ = imu.get_gyroZ();

  Serial.printf("Gyro (deg/s) | X: %.2f | Y: %.2f | Z: %.2f\n", 
                myData.gyroX, myData.gyroY, myData.gyroZ);
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(centralAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}