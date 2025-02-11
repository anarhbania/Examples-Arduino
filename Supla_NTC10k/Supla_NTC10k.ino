#include <SuplaDevice.h>
#include <supla/control/button.h>
#include <supla/device/supla_ca_cert.h>
#include <supla/network/esp_web_server.h>
#include <supla/network/esp_wifi.h>
#include <supla/network/html/device_info.h>
#include <supla/network/html/protocol_parameters.h>
#include <supla/network/html/wifi_parameters.h>
#include <supla/storage/eeprom.h>
#include <supla/storage/littlefs_config.h>

#include <supla/sensor/virtual_thermometer.h>

#define PINOUT_BUTTON 0
#define PINOUT_NTC   A0

Supla::Eeprom eeprom;
Supla::ESPWifi wifi;
Supla::EspWebServer suplaServer;
Supla::LittleFsConfig configSupla;

auto suplaButtonCfg = new Supla::Control::Button(PINOUT_BUTTON, true, true);

auto suplaTemperatura = new Supla::Sensor::VirtualThermometer;

void setup() 
{
  suplaButtonCfg->configureAsConfigButton(&SuplaDevice);
	
  new Supla::Html::DeviceInfo(&SuplaDevice);
  new Supla::Html::WifiParameters;
  new Supla::Html::ProtocolParameters;

  SuplaDevice.setSuplaCACert(suplaCACert);
  SuplaDevice.setSupla3rdPartyCACert(supla3rdCACert);

  SuplaDevice.begin();
}

void loop()
{
  SuplaDevice.iterate();

  static uint64_t lastTime = 0;
  if(millis() - lastTime > 2000)
  {
    lastTime = millis();
    
    suplaTemperatura->setValue(NTC_Convert(analogRead(PINOUT_NTC)));
  }
}

float NTC_Convert(uint16_t value)
{
	const float resistor = 10000;
	const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

	float resistanceLog, resistance, temperature;

	resistance = resistor * (1023.0 / (float)value - 1.0);
	resistanceLog = log(resistance);
	temperature = (1.0 / (c1 + c2 * resistanceLog + c3 * resistanceLog * resistanceLog * resistanceLog));
	temperature -= 273.15;

	return temperature;
}
