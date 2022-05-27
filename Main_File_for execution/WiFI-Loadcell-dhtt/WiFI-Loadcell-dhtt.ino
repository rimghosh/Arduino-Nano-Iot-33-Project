//This Code is created by Rimi Ghosh with the help of few internet references.
#include <ArduinoMqttClient.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <Wire.h>
#include <SimpleDHT.h>
#include "HX711.h"



StaticJsonDocument<256> doc;
static char potato[256];
//doc["sensor"] = "Nano 33 IOT";

float analog_pot=0;
#define DOUT 2
#define CLK 3
HX711 scale;
float calibration_factor =211000; 
float weight;


int cmdSerial = 0;
int pinDHT22 = 5;
int data01Serial = 0;
int data02Serial = 0;
SimpleDHT22 dht22(pinDHT22);
float temperature = 0;
float humidity = 0;
int errDHT22 = SimpleDHTErrSuccess;
int example=0;


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "wifi name";        // your network SSID
char pass[] = "password";    // your network password

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

const char broker[]      = "you cloud server";
int        port     = 8883;
const char topic[]  = "../dht22/temp";
const char topic2[]  = "../dht22/humid";
const char clientId[] = "client_1234_xxxx";
const char topic3[]  = "../hx711/weight";


const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)";

const char* certificate  = SECRET_CERTIFICATE;



void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);
  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // attempt to connect to Wifi network:
  Serial.print("Attempt on Messageing to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();
  mqttClient.setId(clientId);
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
    Serial.print("Attempting to connect to MQTT ");
    delay(5000);

  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic3);
  Serial.println();

  mqttClient.subscribe(topic3);


  Serial.print("Topic: ");
  Serial.println(topic3);

  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alive which avoids being disconnected by the broker
  errDHT22 = dht22.read2(&temperature, &humidity, NULL);
  if (errDHT22 != 0) {
    temperature = (-1 - 32)/1.8;
    humidity = -1;
    }
  temperature = 1.8*temperature + 32;
  Serial.println(String(" Temperature: ")+ temperature+ String("F"));

  Serial.println(String(" Humidity: ")+ humidity );



  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");

  Serial.println();
  weight = scale.get_units()*0.453592;
  Serial.println();


  


  
  mqttClient.beginMessage(topic3);
  doc["controller"] = "Nano 33 IOT";
  doc["time"] = WiFi.getTime();
  doc["UpTime"] = millis();
  doc["weight"] = weight;
  doc["Temperature"]= temperature;
  doc["Humidity"]= humidity;
  serializeJsonPretty(doc, mqttClient);
  mqttClient.endMessage();
  mqttClient.poll();
  delay (1000);
}


unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();
  Serial.println();
}
