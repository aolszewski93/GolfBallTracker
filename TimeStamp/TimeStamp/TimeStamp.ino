#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <ESPDateTime.h>
#include <DateTime.h>
// define timezone
#define TZ_America_New_York PSTR("EST5EDT,M3.2.0,M11.1.0")

// Insert your network credentials
#define WIFI_SSID "daddyshouse"
#define WIFI_PASSWORD "Babyyyyy"

unsigned long lastMs = 0;

unsigned long ms = millis();


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout

  // UTC+5 for EST
  DateTime.setTimeZone("UTC+5");
  //   DateTime.setServer("asia.pool.ntp.org");
  //   DateTime.begin(15 * 1000);
  DateTime.setServer("time.pool.aliyun.com");
  
  //DateTime.setTimeZone("CST-5");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  } else {
    Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp is %ld\n", DateTime.now());
  }
}

void showTime() {
  Serial.printf("TimeZone:      %s\n", DateTime.getTimeZone());
  Serial.printf("Up     Time:   %lu seconds\n", millis() / 1000);
  Serial.printf("Boot   Time:   %ld seconds\n", DateTime.getBootTime());
  Serial.printf("Cur    Time:   %ld seconds\n",
                DateTime.getBootTime() + millis() / 1000);
  Serial.printf("Now    Time:   %ld\n", DateTime.now());
  Serial.printf("OS     Time:   %ld\n", DateTime.osTime());
  Serial.printf("NTP    Time:   %ld\n", DateTime.ntpTime(2 * 1000L));
  // Serial.println();
  Serial.printf("Local  Time:   %s\n",
                DateTime.format(DateFormatter::SIMPLE).c_str());
  Serial.printf("ISO86  Time:   %s\n", DateTime.toISOString().c_str());
  Serial.printf("UTC    Time:   %s\n",
                DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
  Serial.printf("UTC86  Time:   %s\n",
                DateTime.formatUTC(DateFormatter::ISO8601).c_str());

  Serial.println("===========");
  time_t t = time(NULL);
  Serial.printf("OS local:     %s", asctime(localtime(&t)));
  Serial.printf("OS UTC:       %s", asctime(gmtime(&t)));
}

void setup() {
  Serial.begin(115200);

  //initialize wifi and sensors
  initWiFi();
  initDateTime();
  showTime();
}

void loop() {
  if (millis() - ms > 15 * 1000L) {
    ms = millis();
    Serial.println("--------------------");
    if (!DateTime.isTimeValid()) {
      Serial.println("Failed to get time from server, retry.");
      DateTime.begin();
    } else {
      showTime();
    }
  }
}
