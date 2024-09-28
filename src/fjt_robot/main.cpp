/* 作成日: 2024/9月/11日
   作成者: D1 藤田
   改変者: I3 藤本
   2024ABメイン
   最終更新日: 2024/9/28
   注意: 動作未確認
*/
#include <Arduino.h>
#include <MsTimer2.h>

// 定数定義
constexpr uint8_t BUFFER_SIZE = 50;
constexpr uint8_t DATA_SIZE = 2;
constexpr uint16_t BAUD_RATE = 19200;
constexpr uint8_t WATCHDOG_THRESHOLD = 50;
constexpr uint8_t TIMER_INTERVAL = 8; // ミリ秒

// 入力ビットマスク
#define RF_IN (recvData[0] & 0x08)
#define RR_IN (recvData[0] & 0x04)
#define LF_IN (recvData[0] & 0x02)
#define LR_IN (recvData[0] & 0x01)
#define SF_IN (recvData[0] & 0x80)
#define SR_IN (recvData[0] & 0x40)
#define CF_IN (recvData[0] & 0x20)
#define CR_IN (recvData[0] & 0x10)

// 出力ピン
constexpr uint8_t RF_OUT = 2;
constexpr uint8_t RR_OUT = 4;
constexpr uint8_t LF_OUT = 7;
constexpr uint8_t LR_OUT = 8;
constexpr uint8_t SF_OUT = 17;
constexpr uint8_t SR_OUT = 16;
constexpr uint8_t CF_OUT = 15;
constexpr uint8_t CR_OUT = 14;

// 変数定義
uint8_t watchdogCounter = WATCHDOG_THRESHOLD;
uint8_t recvIndex = 0;
bool colonReceived = false;
uint8_t recvBuffer[BUFFER_SIZE];
uint8_t recvData[DATA_SIZE];

// 関数のプロトタイプ宣言
void Timer2_OVF_vect(); // タイマー割込み関数
void serialEvent();
void IM920Write(const uint8_t *pdata, uint8_t n);
void processReceivedData();

// 初期設定
void setup() {
  Serial.begin(BAUD_RATE);

  // 出力ピンの設定
  pinMode(RF_OUT, OUTPUT);
  pinMode(RR_OUT, OUTPUT);
  pinMode(LF_OUT, OUTPUT);
  pinMode(LR_OUT, OUTPUT);
  pinMode(SF_OUT, OUTPUT);
  pinMode(SR_OUT, OUTPUT);
  pinMode(CR_OUT, OUTPUT);
  pinMode(CF_OUT, OUTPUT);

  // タイマー割込み設定
  MsTimer2::set(TIMER_INTERVAL, Timer2_OVF_vect);
  MsTimer2::start(); // タイマースタート
}

void loop() {}

// シリアルデータ受信イベント
void serialEvent() {
  while (Serial.available()) {
    uint8_t data = Serial.read();
    if (data == ':') {
      colonReceived = true; // 受信開始
    } else if (colonReceived && data == '\r') {
      // データ受信完了
      watchdogCounter = 0;   // ウォッチドッグクリア
      processReceivedData(); // 受信データを処理
      colonReceived = false;
      recvIndex = 0;
    } else if (colonReceived && recvIndex < BUFFER_SIZE) {
      // バッファにデータを追加
      recvBuffer[recvIndex++] = data;
    }
  }
}

// 受信データを処理して2進数に変換
void processReceivedData() {
  for (uint8_t i = 0; i * 3 < recvIndex && i < DATA_SIZE; i++) {
    recvData[i] =
        ((recvBuffer[i * 3] <= '9' ? recvBuffer[i * 3] - '0'
                                   : recvBuffer[i * 3] - 'A' + 10)
         << 4) +
        (recvBuffer[i * 3 + 1] <= '9' ? recvBuffer[i * 3 + 1] - '0'
                                      : recvBuffer[i * 3 + 1] - 'A' + 10);
  }
  IM920Write(recvData, DATA_SIZE);
}

// タイマー割込み
void Timer2_OVF_vect() {
  if (watchdogCounter >= WATCHDOG_THRESHOLD) { // 非常用
    digitalWrite(RF_OUT, LOW);
    digitalWrite(RR_OUT, LOW);
    digitalWrite(LF_OUT, LOW);
    digitalWrite(LR_OUT, LOW);
    digitalWrite(SF_OUT, LOW);
    digitalWrite(SR_OUT, LOW);
    digitalWrite(CF_OUT, LOW);
    digitalWrite(CR_OUT, LOW);
  } else {
    watchdogCounter++;
    // 入力状態に基づいて出力を更新
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

// IM920にデータを送信
void IM920Write(const uint8_t *pdata, uint8_t n) {
  Serial.print("TXDA ");
  for (uint8_t i = 0; i < n; i++) {
    Serial.print(pdata[i] >> 4, HEX);
    Serial.print(pdata[i] & 0x0F, HEX); // 0x0F で下位4ビットをマスク
  }
  Serial.println();
}
