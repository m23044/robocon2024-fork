/*作成日: 2024/9月/11日
   作成者: D1 藤田
   2024ABメイン
   最終更新日:2024/9/15
*/
#include <MsTimer2.h>
#include <Arduino.h>

// 変数定義
// 入力
#define RF_IN (recvData[0] & 0x08)
#define RR_IN (recvData[0] & 0x04)
#define LF_IN (recvData[0] & 0x02)
#define LR_IN (recvData[0] & 0x01)
#define SF_IN (recvData[0] & 0x80)
#define SR_IN (recvData[0] & 0x40)
#define CF_IN (recvData[0] & 0x20)
#define CR_IN (recvData[0] & 0x10)
// 出力
#define RF_OUT 2
#define RR_OUT 4
#define LF_OUT 7
#define LR_OUT 8
#define SF_OUT 17
#define SR_OUT 16
#define CF_OUT 15
#define CR_OUT 14

#define RF_OUT_D2 (_BV(PORTD2)) //_BVはビットセット
#define RR_OUT_D4 (_BV(PORTD4))
#define LF_OUT_D7 (_BV(PORTD7))
#define LR_OUT_B0 (_BV(PORTB0))
#define SF_OUT_C3 (_BV(PORTC3))
#define SR_OUT_C2 (_BV(PORTC2))
#define CF_OUT_C1 (_BV(PORTC1))
#define CR_OUT_C0 (_BV(PORTC0))

// wd
#define WD 50
uint8_t wd = WD;

#define brt 19200

uint8_t recvIndex = 255;
uint8_t recvBuffer[50];
uint8_t recvData[2];

// 関数のプロトタイプ宣言
void Timer2_OVF_vect(); // タイマ割込み関数
void serialEvent();
void IM920Write(const uint8_t *pdata, uint8_t n);

void setup() {

  Serial.begin(brt);

  pinMode(RF_OUT, OUTPUT);
  pinMode(RR_OUT, OUTPUT);
  pinMode(LF_OUT, OUTPUT);
  pinMode(LR_OUT, OUTPUT);
  pinMode(SF_OUT, OUTPUT);
  pinMode(SR_OUT, OUTPUT);
  pinMode(CR_OUT, OUTPUT);
  pinMode(CF_OUT, OUTPUT);

  MsTimer2::set(8, Timer2_OVF_vect); // 時間(ms)と関数の設定
  MsTimer2::start();                 // タイマスタート
}

void loop() {}

void serialEvent() {
  while (Serial.available()) {
    uint8_t data = Serial.read();
    uint8_t i;
    if (recvIndex == 255 && data == ':') { //':'を受信すると
      recvIndex = 0;                       // そこのデータから詰める
    } else if (recvIndex != 255 && data == 0x0d) { // 改行を受信すると
      // IM920Write();
      wd = 0; // ウォッチドッグをクリア
      // 16進数データを2進数に変換
      for (i = 0; i * 3 < recvIndex; i++) {
        recvData[i] =
            ((recvBuffer[i * 3] <= '9' ? recvBuffer[i * 3] - '0'
                                       : recvBuffer[i * 3] - 'A' + 10)
             << 4) +
            (recvBuffer[i * 3 + 1] <= '9' ? recvBuffer[i * 3 + 1] - '0'
                                          : recvBuffer[i * 3 + 1] - 'A' + 10);
      }
      recvIndex = 255;
      IM920Write(recvData, 2);

    } else if (recvIndex >= 50) {
      // データ数が1より小さく50より大きい時は無視
    } else {
      recvBuffer[recvIndex] = data; // 受信データを代入
      recvIndex++;
    }
  }
}

void Timer2_OVF_vect() { // timer2がオーバーフローしたとき割り込み

  if (wd >= WD) { // 非常用
    digitalWrite(RF_OUT, LOW);
    digitalWrite(RR_OUT, LOW);
    digitalWrite(LF_OUT, LOW);
    digitalWrite(LR_OUT, LOW);
    digitalWrite(SF_OUT, LOW);
    digitalWrite(SR_OUT, LOW);
    digitalWrite(CF_OUT, LOW);
    digitalWrite(CR_OUT, LOW);
  } else {
    wd += 1;
    digitalWrite(RF_OUT, RF_IN ? HIGH : LOW);
    digitalWrite(RR_OUT, RR_IN ? HIGH : LOW);
    digitalWrite(LF_OUT, LF_IN ? HIGH : LOW);
    digitalWrite(LR_OUT, LR_IN ? HIGH : LOW);
    digitalWrite(SF_OUT, SF_IN ? HIGH : LOW);
    digitalWrite(SR_OUT, SR_IN ? HIGH : LOW);
    digitalWrite(CF_OUT, CF_IN ? HIGH : LOW);
    digitalWrite(CR_OUT, CR_IN ? HIGH : LOW);
  }
}

void IM920Write(const uint8_t *pdata, uint8_t n) {
  Serial.print("TXDA");
  for (uint8_t i = 0; i < n; i++) {
    Serial.print(pdata[i], HEX);
  }
  Serial.println();
}
