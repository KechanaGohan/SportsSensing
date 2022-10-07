#include <M5Core2.h>
#include <math.h>
#include "BluetoothSerial.h"

#define M5STACK_MPU6886

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
//TFT_eSpriteクラスをインスタンス化、spriteを使用し画面のちらつき防止
TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);

// 変数宣言
float accX, accY, accZ;      // 加速度格納用
float gyroX, gyroY, gyroZ;  // 角速度格納用
float pitch, roll, yaw;     // 姿勢角格納用
int mode = 0;               // 測定モード選択用
int exercise_select = 1;
int countDown;
int countStart;
int countFinish;


void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  M5.IMU.Init();                      // 6軸センサ初期化
  M5.IMU.SetAccelFsr(M5.IMU.AFS_8G);  // 加速度センサースケール初期値設定 ±8G(2,4,8,16) ※GRAYは「setAccelFsr」（先頭のsが小文字）

  // スプライトの作成
  sprite.setColorDepth(8);
  sprite.setTextSize(3);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());

  M5.Lcd.println("Setup is complete");
  M5.Lcd.println("You can pair it with bluetooth!");
  M5.Lcd.setCursor(10, 175); //文字表示の左上位置を設定
  M5.Lcd.setTextColor(GREEN); //文字色の指定
  M5.Lcd.println("Select Exercise");
  M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）
}

void loop() {
  M5.update();        // ボタン状態更新

  if (M5.BtnA.wasPressed()) {  // ボタンAが押されたら運動変更
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE); //文字色の指定
    M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定

    exercise_select++;             // 運動選択+1
    if (exercise_select == 5) {    // 運動選択値が5なら
      exercise_select = 1;         // 1にセット
    }
    switch (exercise_select) { // 運動選択
      case 1:
        M5.Lcd.print("Walking");

        M5.Lcd.setCursor(5, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(GREEN); //文字色の指定
        M5.Lcd.println("Select");
        M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setCursor(120, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(YELLOW); //文字色の指定
        M5.Lcd.println("Ready");
        M5.Lcd.fillTriangle(135, 210, 160, 240, 185, 210, YELLOW);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setTextColor(WHITE); //文字色の指定
        break;
      case 2: M5.Lcd.print("Squat");
      
        M5.Lcd.setCursor(5, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(GREEN); //文字色の指定
        M5.Lcd.println("Select");
        M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setCursor(120, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(YELLOW); //文字色の指定
        M5.Lcd.println("Ready");
        M5.Lcd.fillTriangle(135, 210, 160, 240, 185, 210, YELLOW);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setTextColor(WHITE); //文字色の指定
        break;
      case 3: M5.Lcd.print("Side plank");
      
        M5.Lcd.setCursor(5, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(GREEN); //文字色の指定
        M5.Lcd.println("Select");
        M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setCursor(120, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(YELLOW); //文字色の指定
        M5.Lcd.println("Ready");
        M5.Lcd.fillTriangle(135, 210, 160, 240, 185, 210, YELLOW);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setTextColor(WHITE); //文字色の指定
        break;
      case 4: M5.Lcd.print("Lunge");
      
        M5.Lcd.setCursor(5, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(GREEN); //文字色の指定
        M5.Lcd.println("Select");
        M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setCursor(120, 175); //文字表示の左上位置を設定
        M5.Lcd.setTextColor(YELLOW); //文字色の指定
        M5.Lcd.println("Ready");
        M5.Lcd.fillTriangle(135, 210, 160, 240, 185, 210, YELLOW);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

        M5.Lcd.setTextColor(WHITE); //文字色の指定
        break;
    }
  }

  if (M5.BtnB.wasPressed()) {
    doExercise(exercise_select);
  }
}

void doExercise(int exercise_select) {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  switch (exercise_select) { // 運動選択
    case 1: walking();
      break;
    case 2: squat();
      break;
    case 3: sidePlank();
      break;
    case 4: Lunge();
      break;
  }
}
