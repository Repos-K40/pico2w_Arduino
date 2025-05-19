#include <Arduino.h>
/* GPIO設定 */
const int DATA_PIN = 18;
const int LATCH_PIN = 19;
const int CLOCK_PIN = 20;

const byte led_pattern[] = {
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
int pattern_num = 0;

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pattern_num = sizeof(led_pattern) / sizeof(led_pattern[0]);
}

void loop() {
  for (int i = 0; i < pattern_num; i++) {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, led_pattern[i]);
    digitalWrite(LATCH_PIN, HIGH);
    delay(1000);
  }
}