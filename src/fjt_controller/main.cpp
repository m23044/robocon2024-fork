/*作成日: 2024/9月/11日
   作成者: D1 藤田
   2024ABチームコントローラ
   最終更新日:2024/9/15
*/

#include <Arduino.h>

#define LED A0

static uint8_t recvBuffer[50];
static uint8_t recvIndex;
static uint16_t wd;

void setup() {

  Serial.begin(19200);
  // mySerial.begin(19200);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  //  pinMode(A0, INPUT_PULLUP);
  //  pinMode(A1, INPUT_PULLUP);
  //  pinMode(A2, INPUT_PULLUP);
  //  pinMode(A3, INPUT_PULLUP);
  //  pinMode(A4, INPUT_PULLUP);
  //  pinMode(A5, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  recvIndex = 255;
  wd = 0;
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
  for (uint8_t i = 0; i < 60; i++) {
    while (Serial.available() > 0) {
      uint8_t data = Serial.read();
      if (data == ':') {
        recvIndex = 0;
      } else if (data == '\r') {
        if (recvIndex < 20) {
          digitalWrite(LED, HIGH);
          wd = 0;
        }
        recvIndex = 255;
      } else if (recvIndex < 20) {
        recvBuffer[recvIndex] = data;
      }
    }

    if (wd > 500) {
      digitalWrite(LED, LOW);
    } else {
      wd++;
    }

    delay(1);
  }
}
