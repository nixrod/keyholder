# Smart Keyholder
This makes your keyholder smart. Be notified, whenever keys are added or removed.

![keyholder](https://github.com/nixrod/keyholder/blob/master/keyholder.png)

## Technologies used
The project runs on a Nodemcu Esp8266 board. It integrates with [Blynk](http://www.blynk.cc/)

## Building the keyholder
I 3d printed the keyholder with the following stl file: https://www.thingiverse.com/thing:665048
Then I glued adhesive copper tape at the sides of the holder hooks
Afterwards I drilled little holes in it and fit wires from the back to the holders. Then I soldered the wires to the holder

## Connecting the cables
Find the Nodemcu Pinout [here](https://iotbytes.wordpress.com/nodemcu-pinout/)

Wire | GPIO Pin | Nodemcu Pin
--- | --- | ---
All black wires | *Ground* | *Ground*
Key 0 | 5 | d1
Key 1 | 4 | d2
Key 3 | 14 | d5
Key 4 | 12 | d6
Key 5 | 13 | d7

## Setup Blynk
![blynk](https://github.com/nixrod/keyholder/blob/master/blynk.png)

The blynk app uses the Table and the Notification widget.
After you created your app, add the API Key at Line 17 in your code.

## Setup Wifi for the first time
The ESP8266 uses the [wifimanager](https://github.com/tzapu/WiFiManager) library to manage your credentials.
When you first run the code, the ESP launches a server and provides a wifi network with the SSID *NodeMCU* to which you can connect.
There you can select your wifi network and enter the password. The password is then stored on the memory of the ESP and is used for all future connections with the network.

## Run it
Now add a key with a conducting keyring to your smart keyholder. You should get a notification telling you which key was added or removed. Additionally the table in your blynk app will be updated.
