#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "global.h"

float temperatura = 0;
float umidade = 0;

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(temperatura, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(umidade, READ, ON_CHANGE, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PWD);
