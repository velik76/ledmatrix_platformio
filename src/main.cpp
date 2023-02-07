#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED_GPIO 2
#define NETWORK_SSID "Velik1"       // Имя WiFi сети - пропишите здесь или задайте из программы на смартфоне
#define NETWORK_PASS "Odessa_0012_" // Пароль для подключения к WiFi сети - пропишите здесь или задайте из программы на смартфоне

char ssid[25] = ""; // SSID (имя) вашего роутера (конфигурируется подключением через точку доступа и сохранением в EEPROM)
char pass[17] = ""; // пароль роутера

void setup()
{
  pinMode(LED_GPIO, OUTPUT);
  Serial.begin(460800);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
}

void loop()
{
  digitalWrite(LED_GPIO, LOW);

  //  WiFi.disconnect(true);

  // WiFi.scanNetworks will return the number of networks found
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Trying to connect");
    WiFi.begin(NETWORK_SSID, NETWORK_PASS);
  }
  else
  {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  }
  Serial.print("Status: ");
  Serial.println(WiFi.status());

  digitalWrite(LED_GPIO, HIGH);

  // Wait a bit before scanning again
  delay(5000);

  // digitalWrite(LED_GPIO, HIGH);
  // Serial.println("L");

  // // wait for a second
  // delay(1000);
  // // turn the LED off by making the voltage LOW
  // digitalWrite(LED_GPIO, LOW);
  // // wait for a second
  // delay(1000);
}