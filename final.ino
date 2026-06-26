#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// --- Configuration ---
const String EMERGENCY_PHONE = "+918122605958"; 
#define IMPACT_PIN 2
#define LED_PIN 6     
#define BUZZER_PIN 7  
#define I2C_ADDRESS 0x3C

// ADXL335 Tuned Thresholds (Based on your flat surface data)
#define TILT_LOW 290 // Triggers if tilted severely left/back
#define TILT_HIGH 390 // Triggers if tilted severely right/forward
#define Z_FLIP 320 // Triggers if the car goes upside-down

// --- Objects ---
SSD1306AsciiWire oled;
TinyGPSPlus gps;

// GPS on Pins 9/8 | GSM on Pins 10/11
SoftwareSerial gpsSerial(9, 8);   
SoftwareSerial gsmSerial(10, 11); 

void handleEmergency(String cause);

void setup() {
  Serial.begin(9600);
  
  pinMode(IMPACT_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(BUZZER_PIN, LOW); 
  digitalWrite(LED_PIN, LOW);

  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();
  oled.println("BOOTING....");
  delay(1000);
  
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);
  
  oled.clear();
  oled.println("COLLISION DETECTION");
}

void loop() {
  // 1. Listen to GPS
  gpsSerial.listen();
  unsigned long start = millis();
  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
  }

  // 2. Update OLED with Coordinates on Separate Lines
  oled.setCursor(0, 3);
  oled.print("GPS COORDINATES: ");
  oled.print(gps.satellites.value());
  oled.println("  "); // Clear trailing space

  if (gps.location.isValid()) {
    oled.setCursor(0, 5);
    oled.print("LAT: "); oled.println(gps.location.lat(), 6);
    oled.print("LON: "); oled.println(gps.location.lng(), 6);
  } else {
    oled.setCursor(0, 5);
    oled.println("SEARCHING SKY...     ");
    oled.print("CHARS: "); oled.println(gps.charsProcessed());
  }

  // 3. Sensor Checks (Now includes Z-Axis for flips)
  int xVal = analogRead(A0);
  int yVal = analogRead(A1);
  int zVal = analogRead(A2); 
  
  bool isFlipped = (xVal < TILT_LOW || xVal > TILT_HIGH || 
                    yVal < TILT_LOW || yVal > TILT_HIGH || 
                    zVal < Z_FLIP);
                    
  bool isImpact = (digitalRead(IMPACT_PIN) == LOW);

  if (isImpact || isFlipped) {
    String cause = isImpact ? "IMPACT" : "ROLLOVER";
    handleEmergency(cause);
  }
}

void handleEmergency(String cause) {
  digitalWrite(BUZZER_PIN, HIGH); 
  digitalWrite(LED_PIN, HIGH); 

  oled.clear();
  oled.set2X(); 
  oled.println("!! CRASH !!");
  oled.set1X();
  oled.println();
  oled.println("CAUSE: " + cause);
  oled.println();
  oled.println("Sending SMS...");

  gsmSerial.listen();
  delay(1000);

  String lat = gps.location.isValid() ? String(gps.location.lat(), 6) : "Unknown";
  String lng = gps.location.isValid() ? String(gps.location.lng(), 6) : "Unknown";
  String mapLink = "http://maps.google.com/?q=" + lat + "," + lng;
  
  gsmSerial.println("AT+CMGF=1"); 
  delay(500);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(EMERGENCY_PHONE);
  gsmSerial.println("\"");
  delay(500);
  gsmSerial.print("EMERGENCY! " + cause + " detected. Location: " + mapLink);
  delay(500);
  gsmSerial.write(26); 
  
  oled.println();
  oled.println("SMS SENT!");
  
  while(1) { // Siren and Strobe
    digitalWrite(BUZZER_PIN, HIGH); digitalWrite(LED_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW); digitalWrite(LED_PIN, LOW);
    delay(150);
  }
}
