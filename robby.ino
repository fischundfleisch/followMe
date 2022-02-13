// Follow me robot
// Copyright Marlene and Alexander Kaser
// This little robot follows an ultrasound signal
// Copyright (C) 2022  by Marlene and Alexander Kaser

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation,  version 3 of the License. If you use, modify 
//    and/or redistribute it, you'll have to inform us at marlene.c.kaser@gmail.com

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


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
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  distance = (duration_front *0.03432);
  if ((distance <= 1) && (distance > 0)) {
    return true;
    Serial.println("Connected");
  }
  return false;
}

void getDistance_front(){
  duration_front=0;
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  Serial.print("F: ");
  Serial.println(duration_front);
}

void getDistance_left(){
  duration_left = 0;
  digitalWrite(TRIGGER_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_LEFT, LOW);
  duration_left = pulseIn(ECHO_LEFT, HIGH);
  Serial.print("L: ");
  Serial.println(duration_left);
}

void getDistance_right(){
  duration_right = 0;
  digitalWrite(TRIGGER_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_RIGHT, LOW);
  duration_right = pulseIn(ECHO_RIGHT, HIGH);
  Serial.println(duration_right);
}

void quicksort(int array[], int left, int right) {          // binary tree wird sortiert. beim Starten ist left 0 und right 0. 
  int i = left;
  int j = right;
  int temp = 0;     // fürs umsortieren
  int pivot = array[(left+right)/2];      //mittelpunkt vom array

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

  if (left < j) {                   // rekursiver aufruf mit dem kleineren Anteil des Arrays, mit neuer eingeschränkter oberen Grenze
    quicksort(array, left, j);
  }
  if (i < right) {                  // -=- mit eingeschränkter unterer Grenze
    quicksort(array, i, right);
  }
}

int median(int values[], int nr_of_elements){
return values[(nr_of_elements-1)/2];
}

int moving_median(int value, int values[],const int mem_size){      //mem_size = Größe des Arrays in bytes
const int nr_of_elements = mem_size / sizeof(*values);              // Messwerte werden kopiert und sortiert
int sorted_values[nr_of_elements];
memcpy(sorted_values, values, mem_size);
quick_sort(sorted_values, 0, nr_of_elements-1);

for(int x = 0; x < nr_of_elements-1; x++){                         
    values[x] = values[x+1];
}
  
values[nr_of_elements-1] = value;                               // älterster Wert wird rausgeschmissen

  return  median(sorted_values, nr_of_elements);                // neuer Median wird übergeben
}

void motor_left(){
  int pwm_left = 0;
  int duration_all = (duration_left + duration_right + duration_front) /3;      // mindestens einer muss ja ein Signal haben.
  if ((duration_all < 69900) && (duration_left != 0)) {
    pwm_left = (duration_left + duration_front)/ 400;       // ((duration front + duration left) / 2) /200
    analogWrite(MOTOR_LEFT, pwm_left);
  }
}

void motor_right() {
  int pwm_right = 0;
  int duration_all = (duration_left + duration_right + duration_front) / 3;
  if ((duration_all < 69900) && (duration_right != 0)) {
    pwm_right = (duration_right + duration_front) / 400;
    analogWrite(MOTOR_RIGHT, pwm_right);
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
