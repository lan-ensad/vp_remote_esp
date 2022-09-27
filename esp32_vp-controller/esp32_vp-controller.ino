#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

const char* ssid = "SSID_omod";
const char* password = "PASS_c'35t1EPa((§";
const char* mqttServer = "IP_MQTT_102";
const int mqttPort = 1883;

static const int servoPin = 3;

WiFiClient espClient;
PubSubClient client(espClient);
Servo servo;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  client.subscribe("vp");
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("vptrigg")) {
      Serial.println("connected");
      client.subscribe("vp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
    Serial.print(" VP Trigger ");
  }
  if (msg = "true") {
    trigger();
  }
  Serial.println();
  Serial.println("-----------------------");
}
void trigger() {
  servo.attach(servoPin);
  for (int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    servo.write(posDegrees);
    delay(2);
  }
  delay(50);
  for (int posDegrees = 180; posDegrees >= 0; posDegrees--) {
    servo.write(posDegrees);
    delay(2);
  }
  delay(50);
  servo.detach();
}
void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}