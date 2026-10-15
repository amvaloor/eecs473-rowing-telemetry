#include "central_unit.h"
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "rowing_imu.h"

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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 


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

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
   
  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // register first peer  
  memcpy(peerInfo.peer_addr, coachAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // register second peer  
  memcpy(peerInfo.peer_addr, seat1Address, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}

