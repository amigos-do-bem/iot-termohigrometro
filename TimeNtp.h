/*
 * TimeNTP_ESP8266WiFi.ino
 * Example showing time sync to NTP time source
 *
 * This sketch uses the ESP8266WiFi library
 *
 * Source:
 * https://github.com/PaulStoffregen/Time/blob/master/examples/TimeNTP_ESP8266WiFi/TimeNTP_ESP8266WiFi.ino
 */
 
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

time_t prevDisplay = 0; 

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = -3;    // Brazil
//const int timeZone = 1;   // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

const int maxTentativas = 5;
int tentativasAtual = 1;

WiFiUDP Udp;
unsigned int localPort = 8888;  
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[NTP_PACKET_SIZE];

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void preparaNtp()
{  
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

void printDigits(int digits)
{

  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


time_t getNtpTime()
{
  IPAddress ntpServerIP;

  while (Udp.parsePacket() > 0) ; 
  Serial.println("Transmit NTP Request");
  
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
if (tentativasAtual < maxTentativas ){
  tentativasAtual ++;
  Serial.println("Retentando NTP...");
  return getNtpTime();
}

  Serial.println("NTP Falhou ");
  return 0; 
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
