// 他のファイルのプログラムを取得する
#include "Controller.h" // Controller.hを取得
#include <liboshima.h>  // liboshima.hを取得

// #defineでピン番号に別名をつける
#define NUM_MORTOR_BUTTONS NUM_MOTORS * 2 // モータのボタンの数

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
  im.attachDataReceived([]() { connectLed.on(); });
  // データ未受信時にconnectLedを消灯させるコールバック関数を設定
  im.attachDataNotReceived([]() { connectLed.off(); });
}

// loop関数はプログラムが終了するまで繰り返し実行される
void loop() {
  // ロボットにコントローラーの状態を送信するための準備
  // コントローラーのインスタンスを作成
  Controller controller;

  // ボタンのピン番号とモーター番号を初期化
  uint8_t pinNum = 0;
  uint8_t motorNum = 0;

  // 各ボタンの状態をチェックし、対応するモーターの状態を更新
  while (pinNum < NUM_MORTOR_BUTTONS) {
    if (buttons[pinNum].isPressed()) {
      // ボタンが押されている場合、モーターを前進状態に設定
      controller.motors[motorNum] = MotorStateEnum::FORWARD;
    } else if (buttons[pinNum + 1].isPressed()) {
      // 別のボタンが押されている場合、モーターを後退状態に設定
      controller.motors[motorNum] = MotorStateEnum::REVERSE;
    } else {
      // どちらのボタンも押されていない場合、モーターを停止状態に設定
      controller.motors[motorNum] = MotorStateEnum::STOP;
    }
    // 次のボタンペアに進む
    pinNum += 2;
    // 次のモーターに進む
    motorNum++;
  }

  // コントローラーの状態をIM920SLを使って送信
  im.send(controller, ImSendMode::CAREER_SENSE);

  // ロボットからの応答を受信
  char buf[sizeof(CONNECT_SUCCESS)];
  im.receive(buf, ImReceiveMode::NO_WAIT);
}