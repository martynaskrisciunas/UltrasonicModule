  
int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
float distance_cm,duration_us;
void setup() {
  
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  delay(200);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration_us = pulseIn(echoPin, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us; // speed of sound = 0.034 cm/µs 
  Serial.println(distance_cm);
}
