/* ------------------------------------------------- */

#include "ESPTelnet.h"          // https://github.com/LennartHennigs/ESPTelnet

/* ------------------------------------------------- */

#define SERIAL_SPEED  115200
#define WIFI_SSID     "YOUR SSID"
#define WIFI_PASSWORD "YOUR PASSWORD"

/* ------------------------------------------------- */

ESPTelnet telnet;
IPAddress ip;
uint16_t  port = 23;

/* ------------------------------------------------- */

void onTelnetConnect(String ip)
{
  telnet.println("Welcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
}

void setup()
{
  Serial.begin(SERIAL_SPEED);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.forceSleepWake();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  telnet.onConnect(onTelnetConnect);

  telnet.onInputReceived([](String str)
  {
    Serial.print(str);
  });

  telnet.begin(port);
}

void loop()
{
  telnet.loop();

  if(Serial.available())
  {
    telnet.write(Serial.read());
  }
}
