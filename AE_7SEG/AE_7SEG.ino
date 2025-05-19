//ARDUINO UNO
//AE-7SEG-BOARD*2
//SCK D13
//LATCH D10
//SDI D11
#include<SPI.h>
int latch = 9;
int sdi = 10;
int sck = 11;
int scroll_speed = 1000;
const byte digits[] = {
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110, // 9
};
//SPIの設定
SPISettings mySPISettings = SPISettings(1000000, LSBFIRST, SPI_MODE0);

void setup() {
  //SPI1.setRX(12);
  SPI1.setTX(11);
  SPI1.setSCK(10);
  pinMode(latch, OUTPUT);
  //pinMode(sck, OUTPUT);
  //pinMode(sdi, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(152000);
  SPI1.begin();
  SPI1.beginTransaction(mySPISettings);
  delay(10000);
  //SPI1.setBitOrder(LSBFIRST);
  //SPI1.setDataMode(0);
}

bool led=0;
void loop() {
  Serial.println("Init");
  for (int j = 0; j < 10; j ++) {//10の桁
    for (int i = 0; i < 10; i ++) {//1の桁
      //Digital Out
      led=!led;
      digitalWrite(LED_BUILTIN, led);

      //Serial
      Serial.print(j);
      Serial.println(i);
      //SPI
      digitalWrite(latch, 0);
      SPI1.transfer (digits[j]);//10の桁 +1でドット表示
      SPI1.transfer (digits[i]);//1の桁
      digitalWrite(latch, 1);
      //
      delay(scroll_speed);
    }
  }
}

