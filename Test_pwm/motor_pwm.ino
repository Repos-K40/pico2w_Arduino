#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup(){
    //Wire.begin();
    Wire.setSCL(17);
    Wire.setSDA(16);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    pwm.setPWM(0,0,0);
    delay(100);
 }
void loop(){
    digitalWrite(14, HIGH);
    digitalWrite(15, LOW);
    pwm.setPWM(0,0,4095);
    delay(5000);
    
    digitalWrite(14, LOW);
    digitalWrite(15, HIGH);
    pwm.setPWM(0,0,1000);
    delay(5000);

    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    pwm.setPWM(0,0,0);
    delay(5000);

    
    digitalWrite(14, LOW);
    digitalWrite(15, HIGH);
    pwm.setPWM(0,0,4095);
    delay(5000);

    digitalWrite(14, HIGH);
    digitalWrite(15, LOW);
    pwm.setPWM(0,0,1000);
    delay(5000);
    
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    pwm.setPWM(0,0,0);
    delay(100);

    exit(0);
}