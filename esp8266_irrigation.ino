#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <PubSubClient.h>
#define echoPin D7 // Echo Pin
#define trigPin D6 // Trigger Pin

const char* ssid = "***";
const char* password =  "***";
const char* mqttServer = "192.168.1.1";
const int mqttPort = 1883;
const char* mqttUser = "***";
const char* mqttPassword = "***";

long lastMsg1 = 0;
long lastMsg2 = 0;
long lastReconnectAttempt = 0;
char msg[50];
char dst[50];
int value = 0;

int trigger = 0;
int echo = 2;
long duration, distance;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient espClient;
PubSubClient client(espClient);

boolean reconnect() {
  if (client.connect("arduinoClient")) {
    // Once connected, publish an announcement...
    client.publish("outTopic","hello world");
    // ... and resubscribe
    client.subscribe("inTopic");
  }
  return client.connected();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  //Serial.println();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }
}

void loop() {

if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {

  client.loop(); {

    int sensorValue = (740-analogRead(A0))/3;
    long now = millis();
    
    if (now - lastMsg1 >  300000) {
      lastMsg1 = now;
      sensorValue;
      snprintf (msg, 75, "%ld", sensorValue);
      client.publish("esp/erde/wemos/", msg);
      Serial.print("Erdfeuchtigkeit: ");
      Serial.print(msg);
      Serial.println(" %");
      }
      
     //if (now - lastMsg2 > 10000) {
      //lastMsg2 = now;
      //digitalWrite(trigPin, LOW);
      //delayMicroseconds(2);
      //digitalWrite(trigPin, HIGH);
      //delayMicroseconds(10);
      //digitalWrite(trigPin, LOW);
      //duration = pulseIn(echoPin, HIGH);
      //Calculate the distance (in cm) based on the speed of sound.
      //distance = 23-(duration/58.2); 
      //snprintf (dst, 75, "%ld", distance);
      //client.publish("esp/wasser/wemos/", dst);
      //Serial.print("Wassertank: ");
      //Serial.print(dst);
      //Serial.println (" cm");
      //}
    }
  }
}
