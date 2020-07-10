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
#define RESET_WINDOW 5
#define THRESHOLD_SECONDS 5 // If 5 out of the last 8 seconds had object, consider toilet in use
#define TIMED_WINDOW 8
#define TWO_SECONDS 2
#define FIVE_SECONDS 5
#define FLUSH_WAIT_TIME_SECONDS 5 
#define INACTIVE_THRESHOLD_SECONDS 5
#define TEN_MINUTES 600
#define RESET_POSITION 0
#define FLUSH_POSITION 90

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 6, 7);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  servo.attach(SERVO_PIN);
  servo.write(0); 
  
  lcd.begin(16, 2); // Set row length and column height
  lcd.setCursor(0, 0);
  delay(1000);
}

// Add a quick flush mode where if you wave the hand real close for 2 seconds it flushes
bool isQuickFlush = false;

bool isUsingToilet() {
  float distance = 0;
  int activeSeconds = 0;
  int totalSeconds = 0;
  int quickFlushSeconds = 0;
  int threshholdSeconds = 0;
  while(true) {
    distance = sonar.ping_cm();

    if (distance <= OCCUPANCY_WINDOW_CM && distance > QUICK_FLUSH_WINDOW_CM) {
      activeSeconds = activeSeconds + 1;
      lcd.clear();
      threshholdSeconds = threshholdSeconds + 1;
      int possibleRemaining = THRESHOLD_SECONDS - threshholdSeconds;
      String detection = String("Flush mode in ") + possibleRemaining;
      lcd.print(detection);
      totalSeconds = totalSeconds - 1;
    } else if (distance <= QUICK_FLUSH_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      quickFlushSeconds = quickFlushSeconds + 1;
      int remaining = TWO_SECONDS - quickFlushSeconds;
      String quickPrint = String("Quick flush in ") + remaining;
      lcd.clear();
      lcd.print(quickPrint);
      threshholdSeconds = threshholdSeconds - 1; 
      totalSeconds = totalSeconds - 1;
    } else {
      threshholdSeconds - 1;
      totalSeconds = totalSeconds + 1;
    }
    
    if (threshholdSeconds < 0) {
      threshholdSeconds = 0;
    }
    
    if (totalSeconds < 0) {
      totalSeconds = 0;
    }
    
    delay(ONE_SECOND_MS);
    if (quickFlushSeconds >= TWO_SECONDS) {
      isQuickFlush = true;
      break;
    }
    if (threshholdSeconds >= THRESHOLD_SECONDS) {
      isQuickFlush = false;
      break;
    }
    
    if (totalSeconds >= RESET_WINDOW) {
      totalSeconds = 0;
      threshholdSeconds = 0;
      quickFlushSeconds = 0;
      lcd.clear();
    }
  }
  
  bool isInUse = activeSeconds >= THRESHOLD_SECONDS || isQuickFlush;
  if (isInUse) {
    lcd.clear();
    lcd.print("Toilet in use");
  } else {
    lcd.clear();
  }
  return isInUse;
}

bool shouldFlush() {
  if (isQuickFlush) {
    lcd.clear();
    lcd.print("Flushing...");
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
      int remaining = FLUSH_WAIT_TIME_SECONDS - activeSeconds;
      String flushText = String("Flushing in ") + remaining + "...";
      lcd.clear();
      lcd.print(flushText);
    } else if (distance <= OCCUPANCY_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      lcd.clear();
      lcd.print("Toilet in use");
      activeSeconds = 0;
    }
    
    totalSeconds = totalSeconds + 1;
    delay(ONE_SECOND_MS);

    if (activeSeconds >= FLUSH_WAIT_TIME_SECONDS) {
      lcd.clear();
      lcd.print("Flushing...");
      break;
    }
    

    if (totalSeconds > TEN_MINUTES) {
      lcd.clear();
      lcd.print("Ok Flushing...");
      break;
    }
  }
  
  return true;
}

void flush() {
  servo.write(FLUSH_POSITION);
  delay(FIVE_SECONDS_MS);
  servo.write(RESET_POSITION);
  lcd.clear();
  lcd.print("Done");
  // Don't want to start a new flush cycle yet.
  delay(FIVE_SECONDS_MS);
  lcd.clear();
}

void loop() {
  if (isUsingToilet() && shouldFlush()) {
    flush();
  }
}
