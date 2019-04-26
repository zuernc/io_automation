//ESP32
//Deep Sleep (http://educ8s.tv/esp32-deep-sleep-tutorial)
//Moisture Sensor
//Sonar Sensor

#include <WiFi.h>
#include <PubSubClient.h>
#define echoPin 2 // Echo Pin
#define trigPin 5 // Trigger Pin
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 1800      /* Time ESP32 will go to sleep (in seconds) */

// 3800 leads to instant restart ... maximum sleep time?

RTC_DATA_ATTR int bootCount = 0;

const char* ssid = "*********";
const char* password =  "*********";
const char* mqttServer = "192.168.1.1";
const int mqttPort = 1883;
const char* mqttUser = "*********";
const char* mqttPassword = "*********";

char msg[50];
char dst[50];
int value = 0;

int trigger = 5;
int echo = 2;
long duration, cm;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

++bootCount;
  Serial.println("Boot number: " + String(bootCount));

// uncomment if+else to send only one sensor reading depending on odd/even bootCount
//  if (bootCount % 2) {
      int sensorValue = (3800 - analogRead(A0)) / 18;
      sensorValue;
      snprintf (msg, sizeof(msg), "%ld", sensorValue);
      client.publish("esp/earth/", msg);
      Serial.print("Soil Moisuture: ");
      Serial.print(msg);
      Serial.println(" %");
 // }
 // else {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
   
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      pinMode(echoPin, INPUT);
      duration = pulseIn(echoPin, HIGH);
      int cm = 30 - duration * 0.01715; // (duration / 58.2)
      cm;
      snprintf (dst, sizeof(dst), "%ld", cm);
      client.publish("esp/water/", dst);
      Serial.print("Cistern Level: ");
      Serial.print(cm);
      Serial.println(" cm");
      delay(1000);
//  }

  //delay(1000); 

  Serial.println("ESP32 goes to sleep for " + String(TIME_TO_SLEEP) +
  " Seconds");

  /*
  API docs: http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}
  void loop(){
  //This is not going to be called
}
