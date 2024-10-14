// 他のファイルのプログラムを取得する
#include "Controller.h" // 同じディレクトリにあるController.hを取得
#include <Arduino.h> // setup, loop関数を呼び出してくれるライブラリを取得
#include <liboshima.h> // platformio.iniのlib_depsで指定したライブラリを取得

// #defineでピン番号に別名をつける
// モータのボタンの数
#define NUM_MORTOR_BUTTONS NUM_MOTORS * 2

// 通信中に光るランプを定義
Led connectLed(PIN_PC0);

// ボタンの配列を定義し、各ボタンに対応するピン番号を設定
Button buttons[NUM_MORTOR_BUTTONS] = {
    Button(PIN_PD3), Button(PIN_PD4), Button(PIN_PD5), Button(PIN_PD6),
    Button(PIN_PD7), Button(PIN_PB0), Button(PIN_PB1), Button(PIN_PB2),
    Button(PIN_PC2), Button(PIN_PC3)};

// ImSender型のim変数を宣言し、Serialで初期化する
IM920SL im(Serial);

// setup関数はプログラム開始時に1度だけ実行される
void setup() {
  // IM920SLの初期化
  im.beginSerial();
  // データ受信時にconnectLedを点灯させるコールバック関数を設定
  im.onDataReceived([]() { connectLed.on(); });
  // データ未受信時にconnectLedを消灯させるコールバック関数を設定
  im.onDataNotReceived([]() { connectLed.off(); });
}

// loop関数はプログラムが終了するまで繰り返し実行される
void loop() {
  // ロボットにコントローラーの状態を送信するための準備
  // Controller型のcontroller変数を宣言
  Controller controller;

  // ボタンのボタン番号とモーター番号を初期化
  uint8_t buttonNum = 0;
  uint8_t motorNum = 0;

  // 各ボタンの状態をチェックし、対応するモーターの状態を更新
  while (buttonNum < NUM_MORTOR_BUTTONS) {
    if (buttons[buttonNum].isPressed()) {
      // ボタンが押されている場合、モーターを前進状態に設定
      controller.motorButtons[motorNum] = MotorButtonState::FORWARD;
    } else if (buttons[buttonNum + 1].isPressed()) {
      // 別のボタンが押されている場合、モーターを後退状態に設定
      controller.motorButtons[motorNum] = MotorButtonState::REVERSE;
    } else {
      // どちらのボタンも押されていない場合、モーターを停止状態に設定
      controller.motorButtons[motorNum] = MotorButtonState::STOP;
    }
    // 次のボタンペアに進む
    buttonNum += 2;
    // 次のモーターに進む
    motorNum++;
  }

  // コントローラーの状態をIM920SLを使って送信（delayあり）
  im.send(controller, ImSendMode::CAREER_SENSE);

  // データを受信し、受信したデータを読み捨てる
  im.receive<NullType>(nullptr, ImReceiveMode::NO_WAIT);
}