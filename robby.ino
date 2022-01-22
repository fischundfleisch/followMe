const int ECHO_FRONT = 2;
const int ECHO_LEFT = 3;
const int ECHO_RIGHT = 4;
const unsigned long SCAN_FREQ = 1UL*1000UL;

unsigned long scan_timer_ =0;

void setup() {
Serial.begin(9600);
pinMode(ECHO_FRONT, INPUT);
  digitalWrite(ECHO_FRONT, LOW);
pinMode(ECHO_LEFT, INPUT);
  digitalWrite(ECHO_LEFT, LOW);
pinMode(ECHO_RIGHT, INPUT);
  digitalWrite(ECHO_RIGHT, LOW); 
}

void loop() {
  unsigned long act_time = millis();
  unsigned long time_span = act_time - scan_timer_;
  long duration_front = 0;
  long distance_front = 0;
  long duration_left = 0;
  long distance_left = 0;
  long duration_right = 0;
  long distance_right = 0;

  if (time_span > SCAN_FREQ) {
    duration_front = pulseIn(ECHO_FRONT, HIGH); //returns micros
    distance_front = (duration_front *0.03432)+0.5; // +0.5 zum Runden
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
