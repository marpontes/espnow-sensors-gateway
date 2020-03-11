ESP32 Sensors gateway
===

ESP32/ESPNow Network that contains:

* sender - Edge devices that send data ovre ESPNow to the receiver;
* receiver - gets data from sensors and forwards via I2C to the publisher;
* publisher - gets data asynchronously via I2C and publishes it to a MQTT topic;

