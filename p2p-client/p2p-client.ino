
#include <ESP8266WiFi.h>

const char *ssid = "iotmaker.vn";
const char *password = "@password";
IPAddress server_ip(192, 168, 4, 1);
#define PORT        23

WiFiClient client;

void setup() {
  uint8_t i = 0;
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }
}


unsigned long previousMillis = 0; 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    //connect if client was not connected
    while (!client.connected()) {
      if (!client.connect(server_ip, PORT)) {
        Serial.println("connection failed");
        delay(1000);
        return;
      }
    }

    //check if data available
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print("Client receive from Server:");
      Serial.print(line);
    }

    //send PING to server every 1000ms
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      client.write("PING\r");
    }
  }
}
