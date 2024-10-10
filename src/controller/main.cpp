// 他のファイルのプログラムを取得する
#include "Controller.h" // Controller.hを取得
#include <Arduino.h>
#include <MsTimer2.h> // MsTimer2.hを取得
#include <digitalWriteFast.h>
#include <liboshima.h> // liboshima.hを取得

// #defineでピン番号に別名をつける
#define NUM_MORTOR_BUTTONS NUM_MOTORS * 2 // モータのボタンの数

// 通信中に光るランプ
Led connectLed(PIN_PC0);

Button buttons[NUM_MORTOR_BUTTONS] = {
    Button(PIN_PD3), Button(PIN_PD4), Button(PIN_PD5), Button(PIN_PD6),
    Button(PIN_PD7), Button(PIN_PB0), Button(PIN_PB1), Button(PIN_PB2),
    Button(PIN_PC2), Button(PIN_PC3)};

// ImSender型のsender変数を宣言し、serial変数で初期化する
IM920SL im(Serial);

void onTimeOut() { connectLed.off(); }

// 1度だけ実行される
void setup() {
  // IM920SLの初期化
  im.begin();

  // 1秒のタイマーを設定
  MsTimer2::set(IM_RECEIVE_TIMEOUT, onTimeOut);
  MsTimer2::start();
}

// 繰り返し実行される
void loop() {
  // ロボットにコントローラーの状態を送信
  // ボタンの状態を取得し、コントローラーの状態を更新
  Controller controller;

  uint8_t pinNum = 0;
  uint8_t motorNum = 0;
  
  while (pinNum < NUM_MORTOR_BUTTONS) {
    if (buttons[pinNum].isPressed()) {
      controller.motors[motorNum] = MotorStateEnum::Forward;
    } else if (buttons[pinNum + 1].isPressed()) {
      controller.motors[motorNum] = MotorStateEnum::Reverse;
    } else {
      controller.motors[motorNum] = MotorStateEnum::Stop;
    }
    pinNum += 2;
    motorNum++;
  }

  // コントローラーの状態をIM920SLを使って送信
  im.send(controller, ImSenderMode::CAREER_SENSE);

  // ロボットからの応答を受信
  char buf[sizeof(CONNECT_SUCCESS)];
  bool colonedReceived = im.receive(buf, ImReceiverMode::NO_WAIT);
  if (colonedReceived) {
    // ランプを点灯
    connectLed.on();
    // タイマーのカウントを最初からやり直す
    MsTimer2::start();
  }
}
