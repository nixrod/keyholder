#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>

char auth[] = "<your_blynk_api_key>";

const int KEY_0 = 5; // node mcu d1
const int KEY_1 = 4; // node mcu d2
const int KEY_2 = 14; // node mcu d5
const int KEY_3 = 12; // node mcu d6
const int KEY_4 = 13; // node mcu d7

int status[5];

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

  WiFiManager wifiManager;
  //wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings on boot.
  wifiManager.autoConnect("NodeMCU");
  Serial.println("Wifi connected");
  Blynk.config(auth);

  updateTable(status);
  delay(3000);
}
 
void loop() {
  Blynk.run();
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
    String statusValue = updatedPin.value == 0 ? "added" : "removed";
    Serial.println(statusValue);
    Serial.println();
    //TODO: Update table and send push notification
    Blynk.notify("Activity on keychain. Key " + String(updatedPin.pin) + " " + statusValue);
    updateTable(status);
  }

  delay(1000);
}

void updateTable(int status[]) {
  Blynk.virtualWrite(V1, "clr");
  for (int i = 0; i <= 4; i++) {
    String statusValue = status[i] == 0 ? "available" : "missing";
    Blynk.virtualWrite(V1, "add", i, "Key " + String(i), statusValue);
  }
}

