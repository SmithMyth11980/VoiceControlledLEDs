#include <avr/wdt.h>
#include <Wire.h>
#include "DFRobot_DF2301Q.h"
#include <FastLED.h>

// LED variables
#define LED_PIN     6                // The pin your data line is connected to
#define NUM_LEDS    300              // Your LED count
#define BRIGHTNESS  16               // Set brightness (0-255)
#define LED_TYPE    WS2812B          // Change this if you use APA102 or others
#define COLOR_ORDER GRB              // Common for WS2812B

CRGB leds[NUM_LEDS];
CRGB current_colour = CRGB::White;
uint8_t ghue = 0;

// Voice Recognition Module variables
DFRobot_DF2301Q_I2C asr;
const int robot_light_pin = 4;

// WARNING: The variables are not the voice commands, check the link for the proper voice commands https://wiki.dfrobot.com/sen0539-en/docs/21332
// Case variables
const int wake_robot = 1; // If no custom wake word has been trained, change to 2
const int on = 5;         // Trained custom word "On"
const int off = 6;        // Trained custom word "Off"
const int rainbow = 115;
const int red = 116;
const int orange = 117;
const int yellow = 118;
const int green = 119;
const int cyan = 120;
const int blue = 121;
const int purple = 122;
const int white = 123;

// Robot response light timer
unsigned long ledStartTime = 0;
const unsigned long ledDuration = 30000;
bool ledIsOn = false;

void turnLedOn() {
  digitalWrite(robot_light_pin, HIGH);
  ledStartTime = millis();
  ledIsOn = true;
}

void turnLedOff() {
  digitalWrite(robot_light_pin, LOW);
  ledIsOn = false;
}

//Voice Module and Arduino reset
unsigned long lastRestart = 0;
const unsigned long RESTART_INTERVAL = 3600000UL;
void resetArduino() {
  wdt_enable(WDTO_15MS);
  while (true){
    // Wait for 15ms to reboot
  }
}


void setup() {
  // Power up recovery delay
  delay(2000);
  pinMode(robot_light_pin, OUTPUT);

  // LED setup
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Voice Recognition Module setup
  Serial.begin(115200);
  Serial.println("System Starting...");
 
  while (!(asr.begin())) {
    Serial.println("Searching for ASR module... Check SDA/SCL wiring!");
    delay(2000);
  }

  Serial.println("ASR Module Connected Successfully!");
  asr.setVolume(0);
  asr.setMuteMode(1);
  asr.setWakeTime(30);

  Wire.begin();

}

void loop() {
  uint8_t cmd_id = asr.getCMDID();
  Serial.println(cmd_id);
  switch (cmd_id) {
    case wake_robot:
      if (!ledIsOn)
        turnLedOn();
      break;
    case on:
      fill_solid(leds, NUM_LEDS, current_colour);
      FastLED.show();
      break;
    case off:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      break;
    case red:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
      current_colour = CRGB::Red;
      break;
    case orange:
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
      FastLED.show();
      current_colour = CRGB::Orange;
      break;
    case yellow:
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      FastLED.show();
      current_colour = CRGB::Yellow;
      break;
    case green:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      FastLED.show();
      current_colour = CRGB::Green;
      break;
    case cyan:
      fill_solid(leds, NUM_LEDS, CRGB::Cyan);
      FastLED.show();
      current_colour = CRGB::Cyan;
      break;
   case blue:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      FastLED.show();
      current_colour = CRGB::Blue;
      break;
    case purple:
      fill_solid(leds, NUM_LEDS, CRGB::Purple);
      FastLED.show();
      current_colour = CRGB::Purple;
      break;
    case white:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      FastLED.show();
      current_colour = CRGB::White;
      break;
    case rainbow:
      uint8_t cmd_id_local = asr.getCMDID();
      Serial.print("Local cmd = ");
      Serial.println(cmd_id_local);
      while (cmd_id_local == 0) {
        Serial.print("Local cmd = ");
        Serial.println(cmd_id_local);
        fill_rainbow(leds, NUM_LEDS, ghue, 5);
        FastLED.show();
        ghue++;
        delay(10);
        if (ledIsOn && millis() - ledStartTime >= ledDuration)
          turnLedOff();
        cmd_id_local = asr.getCMDID();
      }
      break;
  }

  if (ledIsOn && millis() - ledStartTime >= ledDuration)
    turnLedOff();

  if (millis() - lastRestart >= RESTART_INTERVAL){
    Serial.println("Restarting the Arduino...");
    resetArduino();
  }
  delay(300);

}
