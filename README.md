#  IoT Termohigrômetro
Termohigrômetro desenvolvido em ESP8266 para medir a temperatura e umidade das farmácias do sertão 

![dispositivo](https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/636c2a2d-5a83-46d8-81e0-f07fa71c82a7)

![dashboard](https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/dcb96901-7c38-4b84-b378-38c6437a0a23)

## Objetivo
Os alunos de TI do Centro de Transformação da unidade Ceará criaram um dispositivo de IoT para medir a temperatura e umidade de ambientes (Termohigrômetro). 

A principal aplicação será nas quatro farmácias do sertão (Ceará, Alagoas, Pernambuco - Inajá, Pernambuco - Catimbau) e no depósito em São Paulo (onde armazena as doações de medicamentos). Pois é sabido que os medicamentos, em sua maioria, devem ser armazenados em até 25°C, sendo que acima disso, pode-se comprometer a sua eficácia ou validade, gerando descarte e prejuízo.

Com isso, as equipes de médicos e farmacêuticos dos Amigos do Bem, podem acompanhar essas mensurações em tempo real (por painel Web ou aplicativo de celular) sem a necessidade de visitarem pessoalmente esses locais.

A maioria dos termohigrômetro disponíveis no mercado não disponibilizam o armazenamento digital ou o acompanhamento remoto

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/e9efb266-0a4e-4b0c-a66e-e941a0ee7736" width="350">

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/f4dd2e3a-a8d9-4760-a6da-dd788f81ac86" width="262">

## Fotos
### Equipe
<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/2cc75e10-7915-462b-8b71-7c6778a0b54c" width="400">

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/5b93c580-0c38-46d9-a053-2950a9cca439" width="400">

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/eaf50e2f-80a9-4cf8-b330-bda182519df3" width="400">

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/ff4b81c1-7203-49b7-8ade-c767ba3d6d30" width="400">

### Os kits
<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/c1977f49-6ed4-40ee-9c0a-dbb27c4f579f" width="400">

<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/eb2660a1-54ee-42e6-96d2-5a28f3c5d36b" width="400">

## Equipe
* José Sérgio Pereira de Sá
* João Pedro Pequeno da Silva
* Janailson Firmino de Almeida
* Marcelo Jacó da Silva
* José Kenteni Alves Bendito
* José Armando Lustosa de Sousa
* Alysson Matheus Alencar:
* José Carlos Simião de Oliveira
* Marcos André de Andrade Amaro
* José Júnior Alexandre dos Santos


# Desenvolvimento
<img src="https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/c66e24d2-675f-43d1-a7f8-0123504da7cb" width="700">

Programação de um microcontrolador ESP8266 (compatível com Arduino) em linguagem C++ usando o Arduino IDE.

Obs: Este projeto precisa de internet para obter o horário atual (de um serviço de NTP) e para enviar as informações para a plataforma de IoT Arduino Cloud.


https://github.com/amigos-do-bem/iot-termohigrometro/assets/9891961/3865d0d2-aa39-4d6d-bef5-e9a55b23db73

(Demonstrando aumento de temperatura)

## Componentes
* 01 - Placa NodeMCU v2
* 01 - Sensor DHT-11/DHT-22
* 01 - Resistor 1K Ω Resistor (atuam como pull-up)
* 01 - Display LED modelo Adafruit 128x64 (SSD1306)

## Instruções
Abra o projeto no Arduino Ide. Altere o arquivo `global.h` com as configurações de acordo com as suas credencias de WiFi e da rede Arduino Cloud.

```c++
#define DHTPIN D7
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define WIFI_SSID "XXXXXXXXXXXXXXXXXXXXXXX"
#define WIFI_PWD "XXXXXXXXXXXXXXXXXXXXXXX"

#define DEVICE_LOGIN_NAME "XXXXXXXXXXXXXXXXXXXXXXX"
#define DEVICE_KEY "XXXXXXXXXXXXXXXXXXXXXXX"
```

## Bibliotecas utilizadas
* https://github.com/PaulStoffregen/Time/
* https://github.com/adafruit/DHT-sensor-library
* https://github.com/adafruit/Adafruit_SSD1306
* https://github.com/arduino-libraries/ArduinoIoTCloud

