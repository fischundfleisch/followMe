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
const long SIGNAL_LOSS = 65000;

long duration_front = 0;
long duration_left = 0;
long duration_right = 0;

long values_front[5] = {0, 0, 0, 0, 0};
long values_left[5] = {0, 0, 0, 0, 0};
long values_right[5] = {0, 0, 0, 0, 0};
int mem_size = 0;
int nr_of_elements = 0;

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

  while (!pairing()) {
    delay(2);
  }

  mem_size = sizeof(values_front);
  nr_of_elements = mem_size / sizeof(*values_front);
}

bool pairing() {
  int distance = 0;
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);
  duration_front = pulseIn(ECHO_FRONT, HIGH);
  distance = (duration_front * 0.03432);
  if ((distance <= 1) && (distance > 0)) {
    return true;
    Serial.println("Connected");
  }
  return false;
}

void getDistance() {
  duration_front = 0;
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);
  duration_front = pulseIn(ECHO_FRONT, HIGH);

  duration_left = 0;
  digitalWrite(TRIGGER_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_LEFT, LOW);
  duration_left = pulseIn(ECHO_LEFT, HIGH);

  duration_right = 0;
  digitalWrite(TRIGGER_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_RIGHT, LOW);
  duration_right = pulseIn(ECHO_RIGHT, HIGH);
}

void quicksort(long array[], int left, int right) {          // binary tree wird sortiert. beim Starten ist left 0 und right 0.
  int i = left;
  int j = right;
  long temp = 0;     // fürs umsortieren
  long pivot = array[(left + right) / 2];  //mittelpunkt vom array

  while (i <= j) {
    while (array[i] < pivot) {
      i++;
    }
    while (array[j] > pivot) {
      j--;
    }
    if (i <= j) {
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

long median(long values[], int nr_of_elements) {
  return values[(nr_of_elements - 1) / 2];
}

long moving_median(long value, long values[], const int mem_size) {    //mem_size = Größe des Arrays in bytes
  const int nr_of_elements = mem_size / sizeof(*values);              // Messwerte werden kopiert und sortiert
  long sorted_values[nr_of_elements];
  memcpy(sorted_values, values, mem_size);
  quicksort(sorted_values, 0, nr_of_elements - 1);

  for (int x = 0; x < nr_of_elements - 1; x++) {
    values[x] = values[x + 1];
  }

  values[nr_of_elements - 1] = value;                             // älterster Wert wird rausgeschmissen

  return  median(sorted_values, nr_of_elements);                // neuer Median wird übergeben
}

void motor_left() {
  int pwm_left = 0;
  long duration_all = (duration_left + duration_right + duration_front) / 3;     // mindestens einer muss ja ein Signal haben.
  if ((duration_all < SIGNAL_LOSS) && (duration_left != 0)) {
    pwm_left = (duration_left + duration_front) / 540;     // armseliger Versuch die Geschwindigkeit an die Entfernung anzupassen
    if (pwm_left > 255) {
      pwm_left = 255;
    }
    if ((pwm_left < 50) && (duration_left > 0)) {           // sonst läuft er nicht wirklich an, kann beim Prototypen wahrscheinlich ignoriert werden
      pwm_left = 50;
    }
    if (duration_left <= 0) {
      digitalWrite(MOTOR_LEFT, LOW);
    }
    analogWrite(MOTOR_LEFT, pwm_left);
    Serial.println(pwm_left);
  }
}

void motor_right() {
  int pwm_right = 0;
  long duration_all = (duration_left + duration_right + duration_front) / 3;
    if ((duration_all < SIGNAL_LOSS) && (duration_right != 0)) {
      pwm_right = (duration_front + duration_right) /   540;          // ACHTUNG: PWM hat kein Cap, es rollt over und fängt bei 0 wieder an
      if (pwm_right > 255) {
        pwm_right = 255;
      }
      if ((pwm_right < 50) && (duration_right > 0)) {
        pwm_right = 50;
      }
      if (duration_right <= 0) {
        digitalWrite(MOTOR_RIGHT, LOW);
      }
      analogWrite(MOTOR_RIGHT, pwm_right);
      Serial.println(pwm_right);
    }
}

void straight_ahead() {
  if ((duration_front < duration_left) && (duration_front < duration_right)) {      // Signal kommt definitiv von vorne, bitte einfach geradeaus fahren
    if (duration_front < 10000) {
      analogWrite(MOTOR_RIGHT, 50);
      analogWrite(MOTOR_LEFT, 50);
    }
    else if ((duration_front >= 10000) && (duration_front < 30000)) {
      analogWrite(MOTOR_RIGHT, 150);
      analogWrite(MOTOR_LEFT, 150);
    }
    else {
      analogWrite(MOTOR_RIGHT, 255);
      analogWrite(MOTOR_LEFT, 255);
    }
  }
}

bool signal_search() {
  if ((duration_front > duration_right) || (duration_front > duration_left)) {    // Signal kommt von der Seite oder von hinten
    digitalWrite(MOTOR_LEFT, LOW);
    digitalWrite(MOTOR_RIGHT, LOW);
    if (duration_right > duration_left) {         // das Signal ist links
      analogWrite(MOTOR_RIGHT, 50);               // dreh dich langsam nach links
    }
    if (duration_right < duration_left) {      // das Signal ist rechts
      analogWrite(MOTOR_LEFT, 50);                // dreh dich langsam nach rechts
    }
    return true;
  }
  return false;
}

void motor_stop() {
  long duration_all = (duration_left + duration_right + duration_front) / 3;
  if (duration_all > SIGNAL_LOSS) {
    digitalWrite(MOTOR_LEFT, LOW);
    digitalWrite(MOTOR_RIGHT, LOW);
    while (!pairing()) {
      delay(2);
    }
  }
}


void loop() {
  getDistance();
  duration_front = moving_median(duration_front, values_front, mem_size);
  duration_left = moving_median(duration_left, values_left, mem_size);
  duration_right = moving_median(duration_right, values_right, mem_size);

if (signal_search()==true){
  return;
}

  straight_ahead();       // braucht noch die Bedingung wann nur geradeaus gefahren werden soll
  motor_left();
  motor_right();
  motor_stop();
}
