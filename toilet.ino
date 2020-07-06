#include <Servo.h>
#include "NewPing.h"
Servo servo;

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define SERVO_PIN 9
#define MAX_DISTANCE 400
#define QUICK_FLUSH_WINDOW_CM 10 // Holding hand close to the sensor flushes
#define OCCUPANCY_WINDOW_CM 40 // Tweak this based on your toilet's length. This is where your body is with respect to the distance sensor.
#define MINIMUM_SENSITIVITY 2 // motion sensor cannot detect less than 2 centimeters away.
#define ONE_SECOND_MS 1000
#define FIVE_SECONDS_MS 5000
#define THRESHOLD_SECONDS 3
#define TWO_SECONDS 2
#define FIVE_SECONDS 5
#define FLUSH_WAIT_TIME_SECONDS 10 
#define INACTIVE_THRESHOLD_SECONDS 5
#define TEN_MINUTES 600
#define RESET_POSITION 0
#define FLUSH_POSITION 90
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(0); 
  delay(1000);
}

// Add a quick flush mode where if you wave the hand real close for 2 seconds it flushes
bool isQuickFlush = false;

bool isUsingToilet() {
  float distance = 0;
  int activeSeconds = 0;
  int totalSeconds = 0;
  int quickFlushSeconds = 0;
  while(true) {
    distance = sonar.ping_cm();
    Serial.print("distance: ");
    Serial.println(distance);
    if (distance <= OCCUPANCY_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      activeSeconds = activeSeconds + 1;
    }
    
    if (distance <= QUICK_FLUSH_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      quickFlushSeconds = quickFlushSeconds + 1;
    }
    
    totalSeconds = totalSeconds + 1;
    delay(ONE_SECOND_MS);
    
    if (quickFlushSeconds >= TWO_SECONDS) {
      Serial.println("Is quick flush");
      isQuickFlush = true;
      break;
    }
    if (totalSeconds >= FIVE_SECONDS) {
      isQuickFlush = false;
      break;
    }
  }
  
  bool isInUse = activeSeconds >= THRESHOLD_SECONDS || isQuickFlush;
  if (isInUse) {
    Serial.print("Toilet is in use: ");
    Serial.println(isInUse); 
  }
  
  return isInUse;
}

bool shouldFlush() {
  Serial.print("Checking if we should flush");
  if (isQuickFlush) {
    Serial.print("Quick flush mode");
    isQuickFlush = false;
    return true;
  }
  float distance = 0;
  int activeSeconds = 0;
  int totalSeconds = 0;
  while(true) {
    distance = sonar.ping_cm();
    if (distance > OCCUPANCY_WINDOW_CM) {
      activeSeconds = activeSeconds + 1;
    } else if (distance <= OCCUPANCY_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      Serial.println("Resetting to 0");
      activeSeconds = 0;
    }
    
    totalSeconds = totalSeconds + 1;
    delay(ONE_SECOND_MS);
    Serial.print("No one using toilet for: ");
    Serial.println(activeSeconds);

    if (activeSeconds >= 10) {
      Serial.print("Five seconds of inactivity");
      break;
    }
    
    if (totalSeconds > TEN_MINUTES) {
      Serial.print("Ok buddy it's been ten minutes I'm flushing");
      break;
    }
  }
  
  Serial.print("Nobody in front of sensor for:");
  Serial.println(activeSeconds);
  return true;
}

void flush() {
  Serial.println("Flushing...");
  servo.write(FLUSH_POSITION);
  delay(FIVE_SECONDS_MS);
  servo.write(RESET_POSITION);
  Serial.println("Done flushing");
}

void loop() {
  if (isUsingToilet() && shouldFlush()) {
    flush();
  }
}
