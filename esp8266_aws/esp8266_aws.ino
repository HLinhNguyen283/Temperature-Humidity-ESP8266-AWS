#include "FS.h"
#include "DHT.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>;
#include <PubSubClient.h>;
#include <NTPClient.h>;
#include <WiFiUdp.h>;

#define DHTPIN 4                      // Digital pin GPIO4 (D2) connected to the DHT sensor.
#define DHTTYPE DHT11                 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiClientSecure espClient;

float h ;
float t;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 2000;

// Update these with values suitable for your network.
const char* ssid = "xxxxxxxxxxx";         // your wifi ssid
const char* password = "xxxxxxxxxxxxxx";  //your wifi password
const char* AWS_endpoint = "xxxxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com"; //MQTT broker ip.

//---------------------------------------------------- Read Message from AWS -----------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  for (int i = 0; i < (length); i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


//set MQTT port number to 8883 as per //standard
PubSubClient client(AWS_endpoint, 8883, callback, espClient);

//------------------------------------------------------- connecting to a WiFi network --------------------------------------------------
void setup_wifi() {
  delay(10);
  espClient.setBufferSizes(512, 512);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESPthing")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      espClient.getLastSSLError(buf, 256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//----------------------------------------------------- Send Message to AWS------------------------------------------------------------
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish("outTopic", jsonBuffer);
}

//**********************************************SETUP**********************************************************
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // initialize digital pin LED_BUILTIN as an output.
  dht.begin();


  setup_wifi();
  delay(1000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  }
  else
    Serial.println("Success to open cert file");
  delay(1000);
  if (espClient.loadCertificate(cert))
    Serial.println("cert loaded");
  else
    Serial.println("cert not loaded");
  // Load private key file
  File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  }
  else
    Serial.println("Success to open private cert file");
  delay(1000);
  if (espClient.loadPrivateKey(private_key))
    Serial.println("private key loaded");
  else
    Serial.println("private key not loaded");
  // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  }
  else
    Serial.println("Success to open ca");
  delay(1000);
  if (espClient.loadCACert(ca))
    Serial.println("ca loaded");
  else
    Serial.println("ca failed");
  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
}

//***********************************************LOOP***********************************************************
void loop() 
{
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  delay(1000);

  if (!client.connected())
  {
    reconnect();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 1000)             //Update every single.
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}
