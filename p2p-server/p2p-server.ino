
#include <ESP8266WiFi.h>

#define PORT        23
#define MAX_CLIENTS 1

const char *ssid = "iotmaker.vn";
const char *password = "@password";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);


WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Telnet server started");

}

void loop() {
  uint8_t i;

  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_CLIENTS; i++) {
      //find free/disconnected spot
      if (!clients[i] || !clients[i].connected()) {
        if (clients[i]) clients[i].stop();
        clients[i] = server.available();
        Serial.print("New client: "); Serial1.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  //check clients for data
  for (i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected()) {
      if (clients[i].available()) {
        //get data from the telnet client and push it to the UART
        String line = clients[i].readStringUntil('\r');
        Serial.print("Server receive from Client:");
        Serial.print(line);
        //response to client
        String resp = String(line.reserve(line.length()-1));
        Serial.print("Then, response back to client:");
        Serial.print(resp);
        clients[i].write(resp.c_str());
      }
    }
  }
}
