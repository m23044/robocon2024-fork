#include <Arduino.h>
#include <MsTimer2.h>
#include <Servo.h>
#include <components/ims/IM920SL.h>
#include <components/motors/NonSpeedAdjustable.h>

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
NonSpeedAdjustable motors[] = {
    NonSpeedAdjustable(PIN_PD2, PIN_PD4), NonSpeedAdjustable(PIN_PD7, PIN_PB0),
    NonSpeedAdjustable(PIN_PC3, PIN_PC2), NonSpeedAdjustable(PIN_PC1, PIN_PC0),
    NonSpeedAdjustable(PIN_PC4, PIN_PC5)};

// im920SLを使用するための変数を作成する
SerialPort serial(Serial);
IM920SL im(serial);

// コントローラーからデータを受信した際に実行される
void serialEvent() {
  // タイマーをリセットする
  MsTimer2::start();

  // ボタンの状態を取得する
  uint16_t keyCode;
  im.receive(keyCode);

  // ボタンの状態に応じてモータを制御する
  // keyCode = 0000 0000 0000 0000
  //           ^^^^ ^^^^ ^^^^ ^^^^
  //           |||| |||| |||| ||||
  //           |||| |||| |||| |||+---- i = 0の時、1か0かを調べる
  //           |||| |||| |||| ||+----- i = 1の時、1か0かを調べる
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //           +---------------------- i = 15の時、1か0かを調べる
  for (uint8_t i = 0; i < sizeof(motors) / sizeof(motors[0]); i++) {
    if (keyCode & (1 << i)) { // ボタンが押されている場合
      if (i % 2 == 0) // 偶数番目のボタンが押されている場合
        motors[i].forward();
      else
        motors[i].reverse();
    } else { // ボタンが押されていない場合
      motors[i].stop();
    }
  }
}

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
  MsTimer2::set(IM_RECEIVE_INTERVAL_MILLIS, emergencyStop);
  MsTimer2::start();
  // IM920SLの初期化
  im.begin();
}

// 繰り返し実行される
void loop() {}
