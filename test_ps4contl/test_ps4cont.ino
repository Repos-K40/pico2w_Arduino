#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "SerialBT.h"

int count = 0;

void setup() {
	Serial.begin(115200);
	SerialBT.begin();
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	if (Serial.available()){
		Serial.println("USB can send to PC.");
	}
	while (SerialBT.available()) {
	char c = SerialBT.read();
	SerialBT.println(c);
	}	

	count++;
	Serial.println(count);
	SerialBT.println(count);

	digitalWrite(LED_BUILTIN, count % 2);

	delay(1000);
}