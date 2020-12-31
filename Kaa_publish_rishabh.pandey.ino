#include <ESP8266WiFi.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "kaa.h"

const char* wifi = "DSX500_5G";
const char* password = "rishabh690893";
const char* mqtt_server = "cloud.kaaiot.com";

#define APPLICATION_VERSION = "bvn3v0tbhnjc0btlldlg-version78"  // Paste your application version
#define ENDPOINT_TOKEN = "token78"      // Paste your endpoint token
char *client_id = "00-14-22-01-23-45";

WiFiClient espClient;
PubSubClient client(espClient);

int led = 16;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);   //declare the baud rate
  delay(100);

  //start connecting the client (node MCU) to the WiFi and MQTT
  Serial.print("connecting to ");
  Serial.println(wifi);
  WiFi.begin(wifi, password);   //starts the procedure to connect client to the WiFi

  //till the wifi connects, print …….
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected to client");
  Serial.println("IP address is ");
  Serial.print(WiFi.localIP());   //IP address

  client.setServer(mqtt_server, 1883);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  publish("kp1/{APPLICATION_VERSION}/dcx/{ENDPOINT_TOKEN}/json/42");
  delay(2000);
}

void reconnect()
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(client_id)) {
      Serial.println("Connected to WiFi");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 3 seconds before retrying
      delay(3000);
    }
  }
}

void publish(String topic)
{
  Serial.println("Publishing on topic: " + topic);
  int str_len = topic.length() + 1;
  char char_array[str_len];
  topic.toCharArray(char_array, str_len);
  client.publish(char_array, "[{\"temperature\":23}]");
}
