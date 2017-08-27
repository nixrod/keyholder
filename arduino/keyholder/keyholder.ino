#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#define NTP_OFFSET   2 * 60 * 60  // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

char auth[] = "<your_blynk_api_key>";

const int KEY_0 = 5; // node mcu d1
const int KEY_1 = 4; // node mcu d2
const int KEY_2 = 14; // node mcu d5
const int KEY_3 = 12; // node mcu d6
const int KEY_4 = 13; // node mcu d7

int status[5];
boolean tableInitialized = false;

void setup () {
  pinMode(KEY_0, INPUT_PULLUP);
  pinMode(KEY_1, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);

  int status[] = {
    digitalRead(KEY_0),
    digitalRead(KEY_1),
    digitalRead(KEY_2),
    digitalRead(KEY_3),
    digitalRead(KEY_4)
  };

  Serial.begin(115200);
  timeClient.begin();

  WiFiManager wifiManager;
  //wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings on boot.
  wifiManager.autoConnect("NodeMCU");
  Serial.println("Wifi connected");
  Blynk.config(auth);
}
 
void loop() {
  Blynk.run();
  if (!tableInitialized) {
    initTable(status);
    tableInitialized = true;
  }
  
  timeClient.update();
  checkForUpdate();

  delay(1000);
}

void checkForUpdate() {
  /* 
   * A maximum of one pin is updated each loop. This is because we are limited in the 
   * amount of push notifications we can send. Also the use case does not require too
   * frequent updates.
   */
   struct Pin {
    int pin;
    int value;
   };

   Pin updatedPin = {-1, -1};
  
  int new_status[] = {
    digitalRead(KEY_0),
    digitalRead(KEY_1),
    digitalRead(KEY_2),
    digitalRead(KEY_3),
    digitalRead(KEY_4)
  };

  for (int i = 0; i <= 4; i++) {
    if (new_status[i] != status[i]) {
      updatedPin.pin = i;
      updatedPin.value = new_status[i];
    }
  }

  if (updatedPin.pin != -1) {
    Serial.println("A pin changed");
    Serial.println(updatedPin.pin);
    Serial.println(updatedPin.value);
    
    status[updatedPin.pin] = updatedPin.value;
    // value of 0 is connected, value of 1 is disconnected
    String statusValue = updatedPin.value == 0 ? "available" : "missing";
    Serial.println(statusValue);
    Serial.println();

    Blynk.notify("Activity on keychain. Key " + String(updatedPin.pin) + " " + statusValue);
    Blynk.virtualWrite(V0, "update", updatedPin.pin, "Key " + String(updatedPin.pin), statusValue);
    Blynk.virtualWrite(V0, "update", 5, "Last Change (Key " + String(updatedPin.pin) + ")", getDate());
  }
}

void initTable(int status[]) {
  Blynk.virtualWrite(V0, "clr");
  for (int i = 0; i <= 4; i++) {
    String statusValue = status[i] == 0 ? "available" : "missing";
    Blynk.virtualWrite(V0, "add", i, "Key " + String(i), statusValue);
  }
  Blynk.virtualWrite(V0, "add", 5, "Last Change", getDate());
  Blynk.virtualWrite(V0, "add", 6, "Last Init", getDate());
}

String getDate() {
  timeClient.update();
  time_t t = timeClient.getEpochTime();
  return String(year(t)) + "-" + String(month(t)) + "-" + String(day(t));
}

