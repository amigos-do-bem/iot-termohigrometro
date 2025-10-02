#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include "global.h"
#include "TimeNtp.h"
#include "thingProperties.h"
#include "NoDelay.h"

//Display SH1106
U8G2_SH1106_128X64_NONAME_F_SW_I2C display(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);

//DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

//Timers com NoDelay
noDelay timerMedidas;
noDelay timerDisplay;
time_t tempoAtual;


void setup() {
  Serial.begin(9600);
  conectaWifi();

  timerMedidas.setdelay(5000);   // 5 segundos para medidas
  timerDisplay.setdelay(1000);   // 1 segundo para atualizar display

  preparaOled();
  imprimeOledCarregando();

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  dht.begin();
  preparaNtp();

  delay(2000);
}


void loop() {
  ArduinoCloud.update();  // Atualiza conexão com a nuvem

  if (timerMedidas.update()) {
    atualizaMedidas();
    exibeMedidasSerial();
  }

  if (timerDisplay.update()) {
    tempoAtual = now();  // Atualiza a variável global
    imprimeOledMedidas();
  }
}

//Conexão WiFi
void conectaWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Já está conectado ao WiFi.");
    return;
  }

  Serial.println("Conectando ao WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado.");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nErro ao conectar no WiFi.");
  }
}

//OLED Setup
void preparaOled() {
  display.begin();
  display.setFont(u8g2_font_7x14_tr); // Fonte sem serifa
  display.clearBuffer();
  display.sendBuffer();
}

//Tela de loading
void imprimeOledCarregando() {
  display.clearBuffer();
  display.drawStr(20, 30, "Carregando...");
  display.sendBuffer();
}

//Sensor leitura
void atualizaMedidas() {
  float t = dht.readTemperature();
  float u = dht.readHumidity();

  if (isnan(t) || isnan(u)) {
    Serial.println("Erro ao ler o sensor DHT!");
    return;
  }

  temperatura = t;
  umidade = u;
}

void exibeMedidasSerial() {
  Serial.print(umidade);
  Serial.print("%,  ");
  Serial.print(temperatura, 0);
  Serial.println("*C");
}

//Ícones personalizados
void desenhaGotinha(uint8_t x, uint8_t y) {
  display.drawTriangle(x + 4, y, x + 1, y + 6, x + 7, y + 6);
  display.drawEllipse(x + 4, y + 6, 3, 2, U8G2_DRAW_ALL);
}

void desenhaSol(uint8_t x, uint8_t y) {
  display.drawDisc(x + 4, y + 4, 3);
  display.drawLine(x + 4, y - 1, x + 4, y - 3);
  display.drawLine(x + 4, y + 9, x + 4, y + 11);
  display.drawLine(x - 1, y + 4, x - 3, y + 4);
  display.drawLine(x + 9, y + 4, x + 11, y + 4);
  display.drawLine(x + 1, y + 1, x, y);
  display.drawLine(x + 7, y + 1, x + 8, y);
  display.drawLine(x + 1, y + 7, x, y + 8);
  display.drawLine(x + 7, y + 7, x + 8, y + 8);
}

//Exibir dados no OLED
void imprimeOledMedidas() {
  char dataStr[20];
  char horaStr[10];
  sprintf(dataStr, "%02d/%02d/%04d", day(tempoAtual), month(tempoAtual), year(tempoAtual));
  sprintf(horaStr, "%02d:%02d:%02d", hour(tempoAtual), minute(tempoAtual), second(tempoAtual));

  display.clearBuffer();

  //Data e hora com fonte diferente
  display.setFont(u8g2_font_crox3hb_tr);
  display.drawStr(20, 15, dataStr);
  display.drawStr(30, 35, horaStr);

  //Alinhar icone aos dados
  int yLine = 60;

  //Umidade
  int xUmid = 0;
  display.setFont(u8g2_font_logisoso16_tf);
  display.setCursor(xUmid + 16, yLine);
  display.print((int)umidade);
  display.print("%");
  desenhaGotinha(xUmid, yLine - 14);

  //Temperatura
  int xTemp = 65;
  display.setCursor(xTemp + 16, yLine);
  display.print(temperatura, 1);
  display.print("C");
  desenhaSol(xTemp, yLine - 14);

  display.sendBuffer();
}
