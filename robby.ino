const int ECHO_FRONT = 2;
const int ECHO_LEFT = 3;
const int ECHO_RIGHT = 4;
const int TRIGGER_FRONT = 5;
const int TRIGGER_LEFT = 6;
const int TRIGGER_RIGHT = 12;
const int MOTOR_LEFT = 9;       // PMW pins. Die Richtung des Signals bestimmt das Tempo, in dem sich der Motor dreht. 
const int MOTOR_RIGHT = 10;

long duration_front = 0;
long duration_left = 0;
long duration_right = 0;

int values_front[5] = {0,0,0,0,0};
int values_left[5] = {0,0,0,0,0};
int values_right[5] = {0,0,0,0,0};

void setup() {
  Serial.begin(115200);
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
  pinMode(TRIGGER_RIGHT, OUTPUT);
  digitalWrite(TRIGGER_RIGHT, LOW);
  pinMode(MOTOR_LEFT, OUTPUT);
  digitalWrite(MOTOR_LEFT, LOW);
  pinMode(MOTOR_RIGHT, OUTPUT);
  digitalWrite(MOTOR_RIGHT, LOW);

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
  trigger_front();
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  Serial.print("F: ");
  Serial.println(duration_front);
}

void getDistance_left(){
  duration_left = 0;
  trigger_left();
  duration_left = pulseIn(ECHO_LEFT, HIGH);
  Serial.print("L: ");
  Serial.println(duration_left);
}

void getDistance_right(){
  duration_right = 0;
  trigger_right();
  duration_right = pulseIn(ECHO_RIGHT, HIGH);
  Serial.println(duration_right);
}

void quicksort(int array[], int left, int right) {
  int i = left;
  int j = right;
  int temp = 0;
  int pivot = array[(left+right)/2];

  while (i<=j) {
    while (array[i] < pivot) {
      i++;
    }
    while (array[j] > pivot){
      j--;
    }
    if (i<=j) {
      temp = array[i];
      array[i] = array[j];
      array[j] = temp;
      i++;
      j--;
    }
  }

  if (left < j) {
    quicksort(array, left, j);
  }
  if (i < right) {
    quicksort(array, i, right);
  }
}

void motor_left(){
  int pmw_left = 0;
  while ((duration_left < 51000) && (duration_left != 0)) {
    pmw_left = duration_left / 200;
    analogWrite(MOTOR_LEFT, pmw_left);
  }
}

void loop() {
Serial.println("Distanz Front:");
getDistance_front();
Serial.println("Distanz Links:");
getDistance_left();
Serial.println("Distanz Rechts:");
getDistance_right();
}
