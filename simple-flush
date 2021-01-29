#include <Servo.h>
Servo servo;

// The output that sends a signal
const int trigPin = 2;

// The output that receives a signal
const int echoPin = 3;

// The output signal that controls the motor
const int servoPin = 9;

const int milliseconds = 1000;

const int flushWaitTimeSec = 5;
const int flushSeconds = milliseconds * flushWaitTimeSec;

long duration, dist;
long average[3]; 

void setup() {
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.write(0); // close cap on power on
  delay(1000);
  servo.detach();
}

void measure() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration/2) / 29.1; // obtain distance
}


void loop() {
  for (int i = 0; i <=2; i++) {
    measure();
    average[i] = dist;
    delay(50);
  }
  dist = (average[0] + average[1] + average[2]) /3;
    
  // Sensor has a minimum range of 2 centimetres and a maximum range of 40 cm
  
  
  if (dist < 60) { // centimetres
    servo.attach(servoPin);
    delay(1);
    servo.write(179);
    delay(flushSeconds); 
    servo.write(0);
    delay(1000);
    servo.detach();
  }
}


