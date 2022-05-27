# Arduino-Nano-Iot-33-Project
1)	Board needed: Arduino nano Iot 33
2)	Sensors needed: DHT22 (at pin 5), load cell with integrated Hx711 (at pin 2 and 3)
3)	Libraries you need: ArduinoMqttClient.h, ArduinoECCX08.h, ArduinoJson.h, ArduinoBearSSL.h, SPI.h ,WiFiNINA.h, Wire.h, SimpleDHT.h, HX711.h
4)	To be able to connect to internet you need to have access to wifi and you need to put the wifi name and password in WiFI-Loadcell-dhtt.ino
5)	Currently my AWS IOT(MQTT server) Can be accessed using x509 certificates. To access them you can install mqtt explorer software and connect to the iot endpoint over port 8883. Subscribe to “../hx711/weight” then add the certificates server, client  and client key

Finally connect to the end point 

6)	To verify that SNS notifications are being received to the gmail please log into the gmail you used in the SNS rule
7)	Tests that can be done to Verify that email notification will be received. 
{
  "controller": "Nano 33 IOT",
  "time": 1652912591,
  "UpTime": 2086798,
  "weight": 0.497491539,
  "Temperature": 72.31999969,
  "Humidity": 50
}
a.	Change weight in the above Json Snippet to be less than 0.200 and publish to “../hx711/weight”
b.	Change Humidity weight in the above Json Snippet to be greater than 90 and publish to “../hx711/weight”


