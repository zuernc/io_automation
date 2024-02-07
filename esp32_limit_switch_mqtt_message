#include <ezButton.h> // https://esp32io.com/tutorials/esp32-limit-switch
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char* ssid = "SSID";
const char* password = "***";

// MQTT Broker
const char* mqtt_server = "192.168.1.1";
const int mqtt_port = 1883;
const char* mqtt_user = "DVES_USER";
const char* mqtt_password = "";
const char* mqtt_topic = "cmnd/tuya_socket_03/POWER1";

ezButton limitSwitch(17);  // create ezButton object that attach to ESP32 pin GPIO17

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
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
}

void callback(char* topic, byte* message, unsigned int length) {
  // Handle incoming messages from MQTT broker
}

void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Vault_Switch", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  limitSwitch.loop(); // MUST call the loop() function first

  if(limitSwitch.isPressed()) {
    Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
    client.publish(mqtt_topic, "on");
  }

  if(limitSwitch.isReleased()) {
    Serial.println("The limit switch: TOUCHED -> UNTOUCHED");
    client.publish(mqtt_topic, "off");
  }

  int state = limitSwitch.getState();
  if(state == HIGH)
    Serial.println("The limit switch: UNTOUCHED");
  else
    Serial.println("The limit switch: TOUCHED");
}
