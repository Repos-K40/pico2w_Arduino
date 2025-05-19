#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define MOTOR_NUM 4
#define MOTOR_MAX 4095
#define MOTOR_MIN 0

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int Pin_Serial = 18;
const int Pin_Latch = 19;
const int Pin_CLK = 20;

//0b(左後)(左前)(右後)(右前)
const byte motor_patterns[]={ 
    0b00000000, //停止
    0b01010101, //前進
    0b10101010, //後退
    0b10100101, //左回
    0b01011010, //右回
    0b01111101, //左前斜め
    0b11101011, //右後斜め
    0b11010111, //右前斜め
    0b10111110, //左後斜め
    0b11111111  //ブレーキ
};


// pattern, verocity, sec;
void pwm_Motor(const byte Move_Pattern ,unsigned int pwm_pulse, unsigned int sec){
    //MOTOR_NUM分のPWMを0.
    int num=0;
    Serial.println("Move shift.");
    pwm.setPWM(5, 0, MOTOR_MIN);
    pwm.setPWM(6, 0, MOTOR_MIN);
    pwm.setPWM(8, 0, MOTOR_MIN);
    pwm.setPWM(9, 0, MOTOR_MIN);
    //for(num=0; num<MOTOR_NUM; num++) pwm.setPWM(num, 0, 0);
    delay(900);
    //モータドライバの動作パターンを変更(8bitシフトレジスタを使用)
    digitalWrite(Pin_Latch, LOW);   //書き込み開始
    shiftOut(Pin_Serial, Pin_CLK, LSBFIRST, Move_Pattern);  //パターン書き込み
    digitalWrite(Pin_Latch, HIGH);  //書き込み修了
    delay(100);
    //MOTOR_NUM分のPWMを再設定.
    //for(num=3; num<MOTOR_NUM+4; num++) pwm.setPWM(num, 0, pwm_pulse);
    pwm.setPWM(5, 0, pwm_pulse);
    pwm.setPWM(6, 0, pwm_pulse);
    pwm.setPWM(8, 0, pwm_pulse);
    pwm.setPWM(9, 0, pwm_pulse);
    delay(sec);
}

void setup(){
    delay(1000);
    Serial.begin(9600);
    Serial.println("Start.");
    Wire.setSCL(17);
    Wire.setSDA(16);
    pwm.begin();
    //pwm.setOscillatorFrequency(27000000); //using servo motor(50 or 60 Hz)
    pwm.setPWMFreq(1600);
    delay(10);
    pinMode(Pin_Serial, OUTPUT);
    pinMode(Pin_Latch, OUTPUT);
    pinMode(Pin_CLK, OUTPUT);
    digitalWrite(Pin_Latch, LOW);
    pwm_Motor(motor_patterns[0], MOTOR_MIN, 1000);
 }
void loop(){
     Serial.println("one.");
    pwm_Motor(motor_patterns[1], 4095*2/4, 5000);
    Serial.println("two.");
    pwm_Motor(motor_patterns[2], 4000, 5000);
    Serial.println("three.");
    pwm_Motor(motor_patterns[3], 4095/2, 5000);
    Serial.println("four.");
    pwm_Motor(motor_patterns[4], 4095*3/4, 5000);
    Serial.println("zero.");
    pwm_Motor(motor_patterns[0], 0, 1000);

    Serial.println("Done.");
    exit(0);    //loop修了
}