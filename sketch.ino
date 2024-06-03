#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 3
#define ECHO_PIN 2

// Define RGB LED pins
int redPin = 6;
int greenPin = 7;
int bluePin = 8;

// Define buzzer pin
int buzzerPin = 9;

// Speed calculation variables
unsigned long previousMillis = 0;
unsigned long interval = 100; 
float previousDistance = 0;
float speed = 0;

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize serial communication
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial1.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Calculate distance using the ultrasonic sensor
  float distance = getDistance();

  // Calculate speed
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    speed = abs(distance - previousDistance) / (interval / 1000.0);
    previousMillis = currentMillis;
    previousDistance = distance;

    // Display speed on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Speed: ");
    display.print(speed);
    display.println(" cm/s");
    display.display();

    // Indicate speed on RGB LED and sound buzzer if speeding
    if (speed < 100) {
      setColor(0, 255, 0); 
      noTone(buzzerPin); 
    } else if (speed < 500) {
      setColor(255, 255, 0);
      noTone(buzzerPin); 
    } else {
      setColor(255, 0, 0); 
      tone(buzzerPin, 1000); 
    }
  }
  
  delay(100);
}

float getDistance() {
  // Trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  float distance = (duration * 0.0343) / 2;

  return distance;
}

void setColor(int red, int green, int blue) {
  digitalWrite(redPin, red > 0 ? LOW : HIGH); 
  digitalWrite(greenPin, green > 0 ? LOW : HIGH); 
  digitalWrite(bluePin, blue > 0 ? LOW : HIGH)
}

