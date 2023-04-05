# _Tracker sample project_

Transfering data between Telegram Bot and ESP32 device using MQTT.
The Telegram bot and the MQTT broker settings can be found in ___.

In the configuration menu (menuconfig) you can set your Wi-Fi and MQTT broker settings.

When the device will be connected to a MQTT broker it will set symbol "#" placed in circle. Otherwise it will "!" also encircled. When connection will be established, the device will subscribe to the device/{deviceID}/print topic. The device will print any string which will come. 

