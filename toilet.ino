#include <Servo.h>
#include "NewPing.h"
Servo servo;

#define LED_OUTPUT 13
#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define SERVO_PIN 9
#define MAX_DISTANCE 400
#define QUICK_FLUSH_WINDOW_START_CM 20 // Holding hand close to the sensor flushes
#define OCCUPANCY_WINDOW_CM 80 // Tweak this based on your toilet's length. This is where your body is with respect to the distance sensor.
#define MINIMUM_SENSITIVITY 2 // motion sensor cannot detect less than 2 centimeters away.
#define ONE_SECOND_MS 1000
#define FIVE_SECONDS_MS 5000
#define SEVEN_SECONDS_MS 7000
#define RESET_WINDOW 5
#define FLUSH_THRESHOLD_SECONDS 5
#define TIMED_WINDOW 8
#define TWO_SECONDS 2
#define FIVE_SECONDS 5
#define FLUSH_WAIT_TIME_SECONDS 5
#define INACTIVE_THRESHOLD_SECONDS 5
#define TEN_MINUTES 600
#define RESET_POSITION 0
#define FLUSH_POSITION 100
long FASTEST_BLINK = 100;
long FAST_BLINK = 200;
long SLOW_BLINK = 500;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setIndicatorLED(int state) {
  digitalWrite(LED_OUTPUT, state);
}


void setup() {
  servo.attach(SERVO_PIN);
  servo.write(0);
  pinMode(LED_OUTPUT, OUTPUT);
  setIndicatorLED(HIGH);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Setup complete");
}

long blinkInterval = FAST_BLINK;
bool shouldBlink = false;

void setBlinkSpeed(long blinkSpeed) {
  shouldBlink = true;
  blinkInterval = blinkSpeed;
}



void blinkAndDelay(long blinkSpeed, long numMilliseconds) {
  unsigned long previousMillis = millis();
  unsigned long startMillis = millis();
  int ledState = HIGH;
  while (true) {
    unsigned long currentMillis = millis();
    long elapsedMillis = currentMillis - previousMillis;
    if (elapsedMillis >= blinkSpeed) {
      previousMillis = currentMillis;

      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      setIndicatorLED(ledState);
    }
    if (currentMillis - startMillis >= numMilliseconds) {
      break;
    }
  }
}

void blinkAndDelay(long numMilliseconds) {
  blinkAndDelay(blinkInterval, numMilliseconds);
}

bool isToiletSeatOccupied() {
  setIndicatorLED(HIGH);

  float distance = 0;
  int totalSeconds = 0;
  int threshholdSeconds = 0;

  while (true) {
    distance = sonar.ping_cm();
    if (distance <= OCCUPANCY_WINDOW_CM && distance > QUICK_FLUSH_WINDOW_START_CM) {
      threshholdSeconds = threshholdSeconds + 1;
      totalSeconds = totalSeconds - 1;
      setBlinkSpeed(FASTEST_BLINK);
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

    if (threshholdSeconds > 0) {
      Serial.println(threshholdSeconds);
      blinkAndDelay(ONE_SECOND_MS);
    } else {
      delay(ONE_SECOND_MS);
      setIndicatorLED(HIGH);
    }


    if (totalSeconds >= RESET_WINDOW) {
      totalSeconds = 0;
      threshholdSeconds = 0;
    }
    
    if (threshholdSeconds > FLUSH_THRESHOLD_SECONDS) {
      Serial.println("Breaking out of loop");

      break;
    }
    
  }

  bool isInUse = threshholdSeconds >= FLUSH_THRESHOLD_SECONDS;

  if (!isInUse) {
    setIndicatorLED(HIGHT);
  }
  
  return isInUse;
}

bool didPersonLeaveSeat() {
  Serial.println("Waiting to flush");
  setIndicatorLED(HIGH);

  float distance = 0;
  int activeSeconds = 0;
  int totalSeconds = 0;
  while (true) {
    distance = sonar.ping_cm();
    if (distance <= OCCUPANCY_WINDOW_CM && distance > MINIMUM_SENSITIVITY) {
      activeSeconds = 0;
    } else {
      activeSeconds = activeSeconds + 1;
      int remaining = FLUSH_WAIT_TIME_SECONDS - activeSeconds;
      String flushText = String("Flushing in ") + remaining + "...";
    }

    totalSeconds = totalSeconds + 1;

    if (activeSeconds > 0) {
      Serial.println(activeSeconds);
      blinkAndDelay(ONE_SECOND_MS);
    } else {
      setIndicatorLED(HIGH);
      delay(ONE_SECOND_MS);
    }

    if (activeSeconds >= FLUSH_WAIT_TIME_SECONDS) {
      break;
    }

    if (totalSeconds > TEN_MINUTES) {
      break;
    }
  }

  return true;
}

void flush() {
  Serial.println("Flushing. Reset position");
  setIndicatorLED(LOW);
  servo.write(RESET_POSITION);
  delay(ONE_SECOND_MS);
  setIndicatorLED(HIGH);
  Serial.println("Turning servo arm");
  servo.write(FLUSH_POSITION);
  delay(SEVEN_SECONDS_MS);
  setIndicatorLED(LOW);
  servo.write(RESET_POSITION);
  delay(FIVE_SECONDS_MS);
}

void loop() {
  if (isToiletSeatOccupied() && didPersonLeaveSeat()) {
    flush();
  }
}