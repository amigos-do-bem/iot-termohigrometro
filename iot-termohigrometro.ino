#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "global.h"
#include "TimeNtp.h"
#include "thingProperties.h"
#include "NoDelay.h"

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void preparaOled();
void atualizaMedidas();
void imprimeOledMedidas();
void exibeMedidasSerial();
void conectaWifi();
void imprimeOledCarregando();
noDelay timerMedidas;
noDelay timerOled;

void setup() {
  Serial.begin(9600);

  timerMedidas.setdelay(5000);
  timerOled.setdelay(1000);

  preparaOled();
  imprimeOledCarregando();
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  dht.begin();
  preparaNtp();
  delay(2000);
}

void loop() {
  // executa a cada 5 segundos
  if (timerMedidas.update()) {
    atualizaMedidas();
    ArduinoCloud.update();
    exibeMedidasSerial();
  }

  // executa a cada 1 segundo
  if (timerOled.update()) {
    imprimeOledMedidas();
  }
}

void preparaOled() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void atualizaMedidas() {
  float t = dht.readTemperature();
  float u = dht.readHumidity();

  if (isnan(t) || isnan(u)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  temperatura = t;
  umidade = u;
}

void exibeMedidasSerial() {
  Serial.print(umidade);
  Serial.print("%");
  Serial.print(",  ");
  Serial.print(temperatura, 0);
  Serial.println("*C");
}

void imprimeOledMedidas() {
  display.stopscroll();
  display.clearDisplay();
  char dataNtp[20];
  sprintf(dataNtp, "%02d/%02d/%02d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());

  display.setTextSize(2);
  display.setCursor(5, 0);
  display.print(dataNtp);
  display.print(" ");

  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Temperatura: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(temperatura, 1);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  display.setTextSize(1);
  display.setCursor(80, 35);
  display.print("Umidade: ");
  display.setTextSize(2);
  display.setCursor(80, 45);
  display.print(umidade, 0);
  display.print("%");

  display.display();
}

void conectaWifi() {
  Serial.begin(9600);
  delay(10);

  Serial.println("Conectando. ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
}

void imprimeOledCarregando() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("Carregando");
  display.display();

  display.startscrollleft(0x00, 0x07);
  delay(2000);
}
