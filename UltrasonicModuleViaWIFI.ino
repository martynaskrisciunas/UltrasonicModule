#include "WiFiS3.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "" // Your SSID
#define WLAN_PASS       "" // SSID password

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883           // use 8883 for SSL
#define AIO_USERNAME    "" // your account username
#define AIO_KEY         "" // your account key

WiFiClient wifiClient;
Adafruit_MQTT_Client mqtt(&wifiClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish distance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/distance");

int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
float distance_cm,duration_us;
void setup() {
  
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to WiFi
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(WLAN_SSID);
  while (WiFi.begin(WLAN_SSID, WLAN_PASS) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);
  MQTT_connect();

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration_us = pulseIn(echoPin, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us; // speed of sound = 0.034 cm/µs 
  Serial.print(F("\nSending distance val "));
  distance.publish(distance_cm);
}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}