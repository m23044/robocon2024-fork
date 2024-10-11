// 他のファイルのプログラムを取得する
#include "Controller.h" // Controller.hを取得
#include <liboshima.h>  // liboshima.hを取得

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

// 1度だけ実行される
void setup() {
  // IM920SLの初期化
  im.beginSerial();
  im.attachDataReceived([]() { connectLed.on(); });
  im.attachDataNotReceived([]() { connectLed.off(); });
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
      controller.motors[motorNum] = MotorStateEnum::FORWARD;
    } else if (buttons[pinNum + 1].isPressed()) {
      controller.motors[motorNum] = MotorStateEnum::REVERSE;
    } else {
      controller.motors[motorNum] = MotorStateEnum::STOP;
    }
    pinNum += 2;
    motorNum++;
  }

  // コントローラーの状態をIM920SLを使って送信
  im.send(controller, ImSendMode::CAREER_SENSE);

  // ロボットからの応答を受信
  char buf[sizeof(CONNECT_SUCCESS)];
  im.receive(buf, ImReceiveMode::NO_WAIT);
}
