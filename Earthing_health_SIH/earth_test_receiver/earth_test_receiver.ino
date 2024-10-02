

//Receiver



#include <ESP8266WiFi.h>
#include <espnow.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Structs for receiving data from different nodes
typedef struct struct_message_sender1 {
  char id[32];
  float voltage;
} struct_message_sender1;

typedef struct struct_message_sender2 {
  char id[32];
  float voltage;
} struct_message_sender2;

struct_message_sender1 dataSender1;
struct_message_sender2 dataSender2;

unsigned long lastNode1 = 0;
unsigned long lastNode2 = 0;
const unsigned long timeout = 1500;


// Callback function executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  // Temporary buffer for ID
  char incomingID[32];
  memcpy(&incomingID, incomingData, sizeof(incomingID));

  // Check the ID to decide which struct to use
  if (strcmp(incomingID, "Node 1") == 0) {
    memcpy(&dataSender1, incomingData, sizeof(dataSender1));
    lastNode1 = millis();
    digitalWrite(2, LOW);
    Serial.print("Earth Voltage from Node 1: ");
    Serial.print(dataSender1.voltage);
    Serial.println(" V");
    int Data1 = dataSender1.voltage;

    if(Data1 < 0.10){
      Serial.print("Earth voltage from Node 1: ");
    Serial.println("NO EARTHING...");
      //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Node 1: ");
    lcd.print("NO EARTH");
    }
    else if(Data1 >= 0.10 && Data1 <= 3.30){
     Serial.print("Earth voltage from Node 1: ");
    Serial.println("GOOD EARTHING...");
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Node 1: ");
    lcd.print("GOOD    ");
    }
    else{
    Serial.print("Earth voltage from Node 1: ");
    Serial.println("FAULT IN DEVICES...");
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Node 1: ");
    lcd.print("FAULT   ");
    }
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Node 1: ");
    // lcd.print(dataSender1.voltage, 2);
    // lcd.print(" V");

  }
  else if (strcmp(incomingID, "Node 2") == 0) {
    memcpy(&dataSender2, incomingData, sizeof(dataSender2));
    lastNode2 = millis();
    digitalWrite(2, LOW);
    Serial.print("Earth voltage from Node 2: ");
    Serial.print(dataSender2.voltage);
    Serial.println(" V ");
    int Data2 = dataSender2.voltage;

    if(Data2 < 0.10){
      
      Serial.print("Earth voltage from Node 2: ");
      Serial.println("NO EARTHING...");
    lcd.setCursor(0, 1);
    lcd.print("Node 2: ");
    lcd.print("NO EARTH");
    }
    else if(Data2 >= 0.10 && Data2 <= 3.30){
    Serial.print("Earth voltage from Node 2: ");
      Serial.println("GOOD EARTHING...");
    lcd.setCursor(0, 1);
    lcd.print("Node 2: ");
    lcd.print("GOOD    ");
    }
    else{
    Serial.print("Earth voltage from Node 2: ");
    Serial.println("FAULT IN DEVICES...");
    lcd.setCursor(0, 1);
    lcd.print("Node 2: ");
    lcd.print("FAULT   ");
    }

    // lcd.setCursor(0, 1);
    // lcd.print("Node 2: ");
    // lcd.print(dataSender2.voltage, 2);
    // lcd.print(" V ");
    lcd.setCursor(0, 2);
    lcd.print(" ");
  }
  else{
    delay(300);
  }
  
}

void setup() {
  // Initialize Serial Monitor and LCD
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0);
  lcd.cursor(); 
  lcd.print(" CONNECTING ");
  lcd.setCursor(0, 1);
  lcd.print(" TO SENDER... ");
  delay(900);

  WiFi.mode(WIFI_STA);
  Serial.println();
   Serial.print("Board MAC Address:  ");
  //WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    Serial.println("Error initializing ESP-NOW");
     lcd.clear(); // Clear the LCD screen
    digitalWrite(2, HIGH); 
    lcd.setCursor(0, 0);
    lcd.print(" CONNECTION");
    lcd.setCursor(0, 1);
    lcd.print("  FAILED ");
    delay(60);
    digitalWrite(2, LOW); 
    delay(60);
    digitalWrite(2, HIGH); 
    delay(900);
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // ESP-NOW will automatically call the callback when data is received
  unsigned long currentTime = millis();
  if (currentTime - lastNode1 > timeout) {
    
    digitalWrite(2, HIGH);
    Serial.println("Earth Voltage from Node 1: OFFLINE ");
    

    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Node 1: ");
    
    lcd.print("OFFLINE ");

  }
   if (currentTime - lastNode2 > timeout) {
    
    digitalWrite(2, HIGH);
    Serial.print("Earth voltage from Node 2: ");
    
    Serial.println(" OFFLINE ");

    lcd.setCursor(0, 1);
    lcd.print("Node 2: ");
    
    lcd.print("OFFLINE ");
    
  }
  delay(500);

}

