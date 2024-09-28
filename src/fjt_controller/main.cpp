/*作成日: 2024/9月/11日
   作成者: D1 藤田
   改変者: I3 藤本
   2024ABチームコントローラ
   最終更新日:2024/9/28
   注意：
   改変したプログラムは動作確認ができていません。（元のプログラムは動作確認済み）
*/

#include <Arduino.h>

#define LED A0

static bool colonReceved = false;
static uint16_t wd = 0;

void setup() {

  Serial.begin(19200);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {

  byte kL = ((digitalRead(3) == 0) ? 0x01 : 0x00) |
            ((digitalRead(4) == 0) ? 0x02 : 0x00) |
            ((digitalRead(5) == 0) ? 0x04 : 0x00) |
            ((digitalRead(6) == 0) ? 0x08 : 0x00);
  byte kH = ((digitalRead(7) == 0) ? 0x01 : 0x00) |
            ((digitalRead(8) == 0) ? 0x02 : 0x00) |
            ((digitalRead(9) == 0) ? 0x04 : 0x00) |
            ((digitalRead(10) == 0) ? 0x08 : 0x00);

  Serial.print("TXDA");
  Serial.print(" ");
  Serial.print(kH, HEX);
  Serial.println(kL, HEX);

  // wd
  while (Serial.available() > 0) {
    uint8_t data = Serial.read();
    if (data == ':') {
      colonReceved = true;
    } else if (data == '\r') {
      if (colonReceved) {
        digitalWrite(LED, HIGH);
        wd = 0;
      }
      colonReceved = false;
    }
  }

  if (wd > 500) {
    digitalWrite(LED, LOW);
  } else {
    wd++;
  }

  delay(1);
}
