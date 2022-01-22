const int ECHO_FRONT = 2;
const int ECHO_LEFT = 3;
const int ECHO_RIGHT = 4;
const int TRIGGER_FRONT = 5;
const unsigned long SCAN_FREQ = 1UL * 1000UL;

unsigned long scan_timer_ = 0;

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

  // hier eine Art Kalibrierung bei bekannter Entfernung von 0 cm einführen
  //erst nach erfolgreicher Kalibrierung darf zusammen gearbeitet werden.
  // Kalibrierung nur mit Front, der Rest darf diesen Wert übernehmen.
  while (!pairing()) {
    delayMicroseconds(1);
  }
}

bool pairing() {
  int distance = 0;

  trigger_front();
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  distance = (duration_front * 0.03432) + 0.5;
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

void loop() {
  unsigned long act_time = millis();
  unsigned long time_span = act_time - scan_timer_;

  if (time_span > SCAN_FREQ) {
    Serial.println("Starte Messung");
    trigger_front();
    duration_front = pulseIn(ECHO_FRONT, HIGH); //returns micros
    Serial.println(duration_front);
    distance_front = (duration_front * 0.03432) + 0.5; // +0.5 zum Runden
    Serial.print("Front: ");
    Serial.print(distance_front);
    Serial.println(" cm");

    duration_left = pulseIn(ECHO_LEFT, HIGH);
    distance_left = (duration_left * 0.03432) + 0.5;
    Serial.print("Left: ");
    Serial.print(distance_left);
    Serial.println(" cm");

    duration_right = pulseIn(ECHO_RIGHT, HIGH);
    distance_right = (duration_right * 0.03432) + 0.5;
    Serial.print("Right: ");
    Serial.print(distance_right);
    Serial.println(" cm");
  }

}
