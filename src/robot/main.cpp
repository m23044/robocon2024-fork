#include <Arduino.h>
#include <MsTimer2.h>
#include <controller/Controller.h>
#include <liboshima.h>

/*
  int型の配列を作成する場合、以下のようにする。
  int array[] = {1, 2, 3, 4, 5};

  NonSpeedAdjustable型の配列を作成する場合、以下のようにする。
  NonSpeedAdjustable motors[] = {
    NonSpeedAdjustable(PIN_PD2, PIN_PD4), NonSpeedAdjustable(PIN_PD7, PIN_PB0),
    NonSpeedAdjustable(PIN_PC3, PIN_PC2), NonSpeedAdjustable(PIN_PC1, PIN_PC0),
    NonSpeedAdjustable(PIN_PC4, PIN_PC5)};

  ここで、NonSpeedAdjustable型はint型と違い複雑な型であるため、int型の様に数字1つで初期化することはできない。

  int i = 1;
  でint型のi変数を作成し、1で初期化する。

  NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
  でNonSpeedAdjustable型のaction1変数を作成し、PIN_PD2とPIN_PD4で初期化する。

  ※NonSpeedAdjustableはモータドライバのことで、2つのピンを指定することでモータを制御することができる。※
*/
NonSpeedAdjustable motors[NUM_MOTORS] = {
    NonSpeedAdjustable(PIN_PD2, PIN_PD4), NonSpeedAdjustable(PIN_PD7, PIN_PB0),
    NonSpeedAdjustable(PIN_PC3, PIN_PC2), NonSpeedAdjustable(PIN_PC1, PIN_PC0),
    NonSpeedAdjustable(PIN_PC4, PIN_PC5)};

// im920SLを使用するための変数を作成する
IM920SL im(Serial);

// 一定時間コントローラーからデータを受信しなかった場合に実行される
void emergencyStop() {
  // 各モータに対して停止命令を送る
  for (auto &motor : motors) {
    motor.stop();
  }
}

// 1度だけ実行される
void setup() {
  // 0.5秒のタイマーを設定
  MsTimer2::set(IM_RECEIVE_TIMEOUT, emergencyStop);
  MsTimer2::start();
  // 初期化
  im.begin();
}

// 繰り返し実行される
void loop() {
  // ボタンの状態を取得する
  Controller controller;
  im.receive(controller, true);

  // タイマーをリセットする
  MsTimer2::start();

  for (uint8_t i = 0; i < NUM_MOTORS; i++) {
    switch (controller.motors[i]) {
    case MotorStateEnum::Forward:
      motors[i].forward();
      break;
    case MotorStateEnum::Reverse:
      motors[i].reverse();
      break;
    case MotorStateEnum::Stop:
      motors[i].stop();
      break;
    }
  }

  // 受信成功したことをコントローラーに知らせる
  im.send(CONNECT_SUCCESS);
}