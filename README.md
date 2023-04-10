# GPS-Tracker project. Devices side.

The device can transfer data to a Telegram Bot using MQTT.
The Telegram bot and the MQTT broker settings can be found in [the server side repo](https://github.com/Hvunt/PeepoTracker_Server).

In the configuration menu (menuconfig, "Device config" part) you can set your Wi-Fi and MQTT broker settings.

When the device will be connected to a MQTT broker it will set symbol "#" placed in circle. Otherwise it will "!" also encircled. When connection will be established, the device will subscribe to the device/{deviceID}/print topic. The device will print any string which will come. 
