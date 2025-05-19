#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
void setup() {
	Serial.begin(152000);
	pinMode(LED_BUILTIN, OUTPUT);
	// put your setup code here, to run once:
	//Wire.begin();
	delay(15000);
	Serial.println("Start: Hello World.");
}
bool led=0;
void loop() {
	led=!led;
	// put your main code here, to run repeatedly:
	Serial.println("Hello World.");
	Serial.println("ta");
	digitalWrite(LED_BUILTIN, led);
	delay(500);
}
