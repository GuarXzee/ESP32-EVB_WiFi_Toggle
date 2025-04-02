#include "SPI.h"
#include <WiFi.h>

const char* ssid = "ToggleAP";
const char* password = "12345678";

#define RELAY1 32
#define RELAY2 33

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  delay(1000);
  Serial.begin(115200);
  Serial.print("Demo started");
}

void loop(void)
{
  bool answer = Get_yes_no();
  if (answer == true)
  {
    Serial.println("True");
    digitalWrite(RELAY2, LOW);
    delay(1000);
    digitalWrite(RELAY1, HIGH);
  }
  else
  {
    Serial.println("False");
    digitalWrite(RELAY1, LOW);
    delay(1000);
    digitalWrite(RELAY2, HIGH);
  }
}
