const int ECHO_FRONT = 2;
const int ECHO_LEFT = 3;
const int ECHO_RIGHT = 4;
const int TRIGGER_FRONT = 5;
const int TRIGGER_LEFT = 6;
const int TRIGGER_RIGHT = 7;

long duration_front = 0;
long distance_front = 0;
long duration_left = 0;
long distance_left = 0;
long duration_right = 0;
long distance_right = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starte...");
  pinMode(ECHO_FRONT, INPUT);
  digitalWrite(ECHO_FRONT, LOW);
  pinMode(ECHO_LEFT, INPUT);
  digitalWrite(ECHO_LEFT, LOW);
  pinMode(ECHO_RIGHT, INPUT);
  digitalWrite(ECHO_RIGHT, LOW);
  pinMode(TRIGGER_FRONT, OUTPUT);
  digitalWrite(TRIGGER_FRONT, LOW);       //kleiner Scheißer.
  pinMode(TRIGGER_LEFT, OUTPUT);
  digitalWrite(TRIGGER_LEFT, LOW);
  pinMode(TRIGGER_RIGHT, LOW);
  digitalWrite(TRIGGER_RIGHT, LOW);

  // hier eine Art Kalibrierung bei bekannter Entfernung von 0 cm einführen
  //erst nach erfolgreicher Kalibrierung darf zusammen gearbeitet werden.
  // Kalibrierung nur mit Front, der Rest darf diesen Wert übernehmen.
  while (!pairing()) {
    delay(2);
  }
}

bool pairing() {
  int distance = 0;

  trigger_front();
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  distance = (duration_front *0.03432);
  if ((distance <= 1) && (distance > 0)) {
    return true;
    Serial.println("Connected");
  }
  return false;
}

void trigger_front() {
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);
}

void trigger_left(){
  digitalWrite(TRIGGER_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_LEFT, LOW);
  }
  

void trigger_right(){
  digitalWrite(TRIGGER_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_RIGHT, LOW);
  }

void getDistance_front(){
  duration_front=0;
//    trigger_front();
//    while (digitalRead(ECHO_FRONT)==HIGH);
//    delay(2);
    trigger_front();
    duration_front = pulseIn(ECHO_FRONT, HIGH);
    distance_front = (duration_front * 3432);    // 0,03432 = cm
//    Serial.println(distance_front);
}

void getDistance_left(){
  duration_left = 0;
  trigger_left();
  duration_left = pulseIn(ECHO_LEFT, HIGH);
  distance_left = (duration_left*3432);
  Serial.println(distance_left);
}

void getDistance_right(){
  duration_right = 0;
  trigger_right();
  duration_right = pulseIn(ECHO_RIGHT, HIGH);
  distance_right = (duration_right *3432);
//  Serial.println(distance_right);
}

void loop() {
//Serial.println("Distanz Front:");
getDistance_front();
//Serial.println("Distanz Links:");
getDistance_left();
//Serial.println("Distanz Rechts:");
getDistance_right();
}
