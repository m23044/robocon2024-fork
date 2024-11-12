#include <Arduino.h> // setup, loop関数を呼び出してくれるライブラリを取得
#include <controller/Controller.h> // src/controller/Controller.hを取得
#include <liboshima.h> // platformio.iniのlib_depsで指定したライブラリを取得

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
// モータ配列を作成
NonSpeedAdjustable motors[NUM_MOTORS] = {
    NonSpeedAdjustable(PIN_PD2, PIN_PD4), NonSpeedAdjustable(PIN_PD7, PIN_PB0),
    NonSpeedAdjustable(PIN_PC3, PIN_PC2), NonSpeedAdjustable(PIN_PC1, PIN_PC0),
    NonSpeedAdjustable(PIN_PC4, PIN_PC5)};

// im920SLを使用するための変数を作成する
IM920SL im(Serial);

// 一定時間コントローラーからデータを受信しなかった場合に実行される関数
void emergencyStop() {
  // 各モータに対して停止命令を送る
  for (auto &motor : motors) {
    motor.stop();
  }

  /* これと同じ意味
  // 各モータに対して停止命令を送る
  for (int i = 0; i < NUM_MOTOR_BUTTONS; i++) {
    motors[i].stop();
  }
  */
}

// プログラム開始時に1度だけ実行される関数
void setup() {
  // IM920SLのシリアル通信を初期化
  im.beginSerial();
  // データ未受信時にemergencyStop関数を実行するよう設定
  im.onDataNotReceived(emergencyStop);
}

// プログラムが終了するまで繰り返し実行される関数
void loop() {
  // コントローラーの状態を取得するためのインスタンスを作成
  Controller controller;
  // コントローラーからのデータを受信
  im.receive(&controller, ImReceiveMode::WAIT);

  // 各モータの状態を更新
  for (uint8_t motorNum = 0; motorNum < NUM_MOTORS; motorNum++) {
    switch (controller.motorButtons[motorNum]) {
    case MotorButtonState::FORWARD:
      // モータを前進させる
      motors[motorNum].forward();
      break;
    case MotorButtonState::REVERSE:
      // モータを後退させる
      motors[motorNum].reverse();
      break;
    case MotorButtonState::STOP:
      // モータを停止させる
      motors[motorNum].stop();
      break;
    }
  }

  // 受信成功したことをコントローラーに知らせる
  // CAREER_SENSEモードで送信
  im.send(0xFF, ImSendMode::CAREER_SENSE);
}