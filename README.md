# Temperature-Humidity-ESP8266-AWS
![Untitled2](https://user-images.githubusercontent.com/100269450/182680637-3ee3af53-63e8-4430-bc93-e99a1e11f373.png)

In this tutorial we are going to see how to post data to the AWS, in this task we will interface DHT11 with ESP32 and post the Temperature and Humidity data to the AWS Using Arduino IDE over MQTT Protocol.

# Hardware :
- [ESP8266 ESP-01 WiFi Module](https://amzn.to/30fUWNS)
- [ESP8266 ESP-01 programmable USB](https://amzn.to/345egi6)
- [ESP8266 ESP-01 Breadboard Adapter](https://amzn.to/3kSFVcP)
- [DHT11 Temp & Humidity Sensor](https://amzn.to/2Qs9fcV)
- [Hardware / Storage Cabinet Drawer](https://amzn.to/36ehDpB)

# Software Used :
- Arduino IDE
- AWS

# Library Used :
- PubSubClient Library
- Wire.h
- AWS_IOT.h
- ArduinoJson.h
- DHT.h


# What is humidity? 
Humidity is the water vapour around you mixed with air. It is measured in %. So, if the humidity is 60%, then 60% of the air around you is water vapour. If it is 100%, then it means either the sensor is not correct, the sensor is broken/damaged, the Arduino crashed, the Arduino can't receive any signal, there's an error in the code or you're underwater(1). If it's 0%, it means all the reasons above except the last one, you're in space or you're in the middle of a desert(2).

(1) Correction: it means the air cannot hold any more water.
(2) The air in a desert does contain some water but it is a very little amount compared to a normal place. The Sahara Desert has a mean humidity of 25%.

<img align="right" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/dht11.jpeg?raw=true" style="max-width:100%;" height="250">

# How to measure Temperature & Humidity
To the right, is a [DHT11](https://amzn.to/2Qs9fcV) sensor, it is a temperature and humidity combined sensor with calibrated digital signal output. It's is a reliable sensor for determining temperature and humidity. There is a newer and more accurate module on the market called DHT22 but we will stick with the DHT11 for this demo<br><br>

# Steps involved in this tutorial :
1. Creating a Thing in the AWS, generating a certificate and attaching a policy to it.
2. Converting AWS credential(Certificate, Private Key, Root CA) from .pem to .der format
3. Installing ESP8266 sketch data upload tool in Arduino IDE
4. Arduino sketch and modifications according to the thing.
5. Uploading AWS certificates & code to the NodeMCU ESP8266
6. Testing/Subscription of thing on Amazon Web Services(AWS).
7. Results& Data Logging.

# Video Tutorial: Connecting NodeMCU with AWS IoT Core using Arduino IDE.
You can watch the following video to knew how you can do Temperature&Humidity Data record on AWS IoT Core with NodeMCU-ESP8266 using Arduino IDE and MQTT Protocol.
It will be a little bit difficult for me to explain everything here. I suggest you follow the video. I will provide the rest of the necessary information like source code and circuit diagram below the video.

[![ ALT TEXT](https://user-images.githubusercontent.com/100269450/182693420-650104fe-cbc8-4a99-bcf4-bc1c1f12d937.jpg)](https://www.youtube.com/watch?v=28FS2qix2u4&t=6s)

# OUTPUT
![alt tag](https://github.com/mjScientech/Monitoring-Temp-and-Humidity-using-AWS-ESP32/blob/master/AWS_Output.JPG)

