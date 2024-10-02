

// // Sender 1




#include <ESP8266WiFi.h>
#include <espnow.h>

// Define a unique struct for Sender 1 (e.g., voltage sensor)
typedef struct struct_message_sender1 {
  char id[32];
  float voltage;
} struct_message_sender1;

uint8_t broadcastAddress[] = {0xE8, 0x9F, 0x6D, 0x94, 0x09, 0x29};
struct_message_sender1 myData1;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (sendStatus == 0) {
    Serial.println("Delivery success");
    digitalWrite(2, LOW);
  } else {
    Serial.println("Delivery fail");
    digitalWrite(2, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    delay(60);
    digitalWrite(2, LOW); 
    delay(60);
    digitalWrite(2, HIGH);
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // Simulate voltage sensor reading
  myData1.voltage = analogRead(A0) * (3.3 *5.0 / 1023.0);  // Example conversion
  strcpy(myData1.id, "Node 1");

  Serial.print("Sending Voltage: ");
  Serial.println(myData1.voltage);

  esp_now_send(broadcastAddress, (uint8_t *) &myData1, sizeof(myData1));
  delay(300);
}

