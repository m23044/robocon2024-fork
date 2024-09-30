// 他のファイルのプログラムを取得する
#include <Arduino.h>
#include <MsTimer2.h> // MsTimer2.hを取得
#include <components/ims/IM920SL.h> // liboshima(大島商船用ライブラリ)のIM920SL.hを取得
#include <digitalWriteFast.h> // digitalWriteFast.hを取得

// #defineでピン番号に別名をつける
#define CONNECT_LED_PIN PIN_PC0 // 受信中のランプのピン番号

// ボタンピン番号の配列
const uint8_t btnPins[] = {PIN_PD3, PIN_PD4, PIN_PD5, PIN_PD6,
                           PIN_PD7, PIN_PB0, PIN_PB1, PIN_PB2,
                           PIN_PC2, PIN_PC3, PIN_PC4, PIN_PC5};

// SerialPort型のserial変数を宣言し、Serial変数で初期化する
SerialPort serial(Serial);

// ImSender型のsender変数を宣言し、serial変数で初期化する
IM920SL im(serial);

// 接続中のランプを消灯する関数
void onDisconnected() { digitalWriteFast(CONNECT_LED_PIN, LOW); }

// 1度だけ実行される
void setup() {
  // 各ピンに対しボタンとして使うための設定を行う
  for (uint8_t i = 0; i < sizeof(btnPins); i++) {
    pinModeFast(btnPins[i], INPUT_PULLUP);
  }
  // ランプを出力に設定
  pinModeFast(CONNECT_LED_PIN, OUTPUT);

  // IM920SLの初期化
  im.begin();

  // 500000マイクロ秒(0.5秒)のタイマーを設定
  MsTimer2::set(IM_RECEIVE_INTERVAL_MILLIS, onDisconnected);
  MsTimer2::start();
}

// 繰り返し実行される
void loop() {
  // ボタンの状態を格納する変数(16ビット)を宣言
  // 0000 0000 0000 0000 <- なんのボタンも押されていない状態
  // 0000 0000 0000 0001 <- btnPins[0]のボタンが押されている状態
  // 1000 0000 0000 0001 <- btnPins[0]とbtnPins[15]のボタンが押されている状態
  // ^^^^ ^^^^ ^^^^ ^^^^
  // |||| |||| |||| ||||
  // |||| |||| |||| |||+---- btnPins[0]の状態
  // |||| |||| |||| ||+----- btnPins[1]の状態
  // |||| |||| |||| |+------ btnPins[2]の状態
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // +---------------------- btnPins[15]の状態
  uint16_t keyCode = 0;

  // ボタンの状態をビット演算で読み取る
  for (uint8_t i = 0; i < sizeof(btnPins); i++) {
    // ボタンが押されているとき1を設定
    keyCode |= !digitalReadFast(btnPins[i]) << i;
  }

  // コントローラーの状態をIM920SLを使って送信
  im.send(keyCode);
  // 100ミリ秒待機
  delay(IM_SEND_INTERVAL);
}

// ロボットから返答があった時に呼び出される関数
void serialEvent() {
  // ランプを消灯
  digitalWriteFast(CONNECT_LED_PIN, HIGH);
  // タイマーのカウントを最初からやり直す
  MsTimer2::start();
}
