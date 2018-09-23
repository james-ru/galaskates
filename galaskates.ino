#include <string>
#include <vector>
#include <sstream>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "WiFi.h"
#include "HTTPClient.h"
// Debug mode enables serial prints
bool debug = true;

// Lights
const int stickPin = 13;
const int indicatorPin1 = 12;
const int indicatorPin2 = 14;
const int amountOfLeds = 8;

// Animations (thanks to https://github.com/ndsh/neopixel-without-delay/)
int interval = 75;
long previousTransitionMillis = 0;
long previousChaseMillis = 0;
long previousInternalChaseMillis = 0;
int currentPixel = 0;
int previousMode = 0;
bool knockback = false;

// Wifi settings
const char* ssid = "HRVATSKA";
const char* password = "itstuesday";

// General settings
int red = 100, green = 100, blue = 100;
int intensity = 64;
int currentMode = 0;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(amountOfLeds, stickPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(indicatorPin1, OUTPUT);
  pinMode(indicatorPin2, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.begin(115200);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(indicatorPin1, !digitalRead(indicatorPin1));
    Serial.print('.');
    delay(100);
  }

  digitalWrite(indicatorPin1, LOW);
  digitalWrite(indicatorPin2, HIGH  );

  // Wifi information
  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  leds.begin();
  leds.setBrightness(intensity);
}

void loop() {
  HTTPClient http;

  http.begin("http://galapos.be:5000/r");
  int httpCode = http.GET();

  if (httpCode > 0) {
	// Arduino strings are an unoptimized mess
	std::vector<int> commands;
	String payload = http.getString();
	char *cstr_payload;

	strcpy(cstr_payload, payload.c_str());

	std::string cppstr_payload = std::string(cstr_payload);
	std::stringstream datastream(cppstr_payload);
	
	while (datastream.good()) {
		std::string substr;
		getline(datastream, substr, ',');
		commands.push_back(stoi(substr));
	}

    currentMode = commands[0];
	red = commands[1];
    green = commands[2];
    blue = commands[3];
  } else {
    Serial.println("error");
  }

  http.end();
  
  ledColor(false);

  delay(100);
}


// Lighting functions

// A solid color that doesn't change
void ledColor(bool spectrumShift) {
  for (int i = 0; i <= amountOfLeds; i++) {
    if (spectrumShift) {
      leds.setPixelColor(i, abs(red - i*8), abs(green - i*8), abs(blue - i*8));
    } else {
      leds.setPixelColor(i, red, green, blue);
    }
  }

  leds.show();
}

void customLedColor(int c) {
  for (int i = 0; i <= amountOfLeds; i++) {
    leds.setPixelColor(i, c);
  }

  leds.show();
}

// When a change occurs, slowly transition towards that color
void ledTransition() {
  leds.setPixelColor(currentPixel, red, green, blue);
  currentPixel++;

  if (currentPixel == amountOfLeds) {
    currentPixel = 0;
  }

  leds.show();
}

void ledChaseInternal() {
  // Make the background a solid color lighter than the primary one
  customLedColor(leds.Color(red / 10, green / 10, blue / 10));

  leds.setPixelColor(currentPixel, red, green, blue);
  leds.setPixelColor(amountOfLeds - 1 - currentPixel, red, green, blue);

  if (knockback) {
    currentPixel--;
  } else {
    currentPixel++;
  }
  
  if ((currentPixel == 3) || (currentPixel == 0)) {
    knockback = !knockback;
  }

  leds.show();  
}

// A constant chase from one side to another
void ledChase() {
  // Make the background a solid color lighter than the primary one
  customLedColor(leds.Color(red / 10, green / 10, blue / 10));
  leds.setPixelColor(currentPixel, red, green, blue);
  leds.setPixelColor(currentPixel-1, red, green, blue);
  leds.setPixelColor(currentPixel-2, red, green, blue);

  if (knockback) {
    currentPixel--;
  } else {
    currentPixel++;
  }

  if ((currentPixel == amountOfLeds + 1) || (currentPixel == 0)) {
    knockback = !knockback;
  }

  leds.show();
}
