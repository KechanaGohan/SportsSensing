int Lunge_count;
int startTime = 0;    //カウント開始時刻
int currentTime = 0;  //現在の時刻
int passedTime = 0;   //経過時刻
int COUNTDOWN = 10;   //いくつカウントするか
int count;
bool LungeRotated = false;

void lunge_loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);   // 加速度データ取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ); // 角速度データ取得
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);    // 姿勢角データ取得

  float x_angle = atan2(accX, accZ) * 180.0 / PI; // X-Z加速度から角度に換算
  float y_angle = atan2(accY, accZ) * 180.0 / PI; // Y-Z加速度から角度に換算

  //Bluetoothでペアリングしたデバイスにデータを送信
  SerialBT.printf("4, 0, 0, 0, 0, %02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", Lunge_count, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

  Serial.println("AngleX,Y");         // 角度項目
  Serial.printf("%5.1f, %5.1f\n", x_angle, y_angle);  // 加速度から換算した角度
  if (LungeRotated == false) {
    if (y_angle < 45) {
      Lunge_count ++;
      LungeRotated = true;
    }
  }
  if (LungeRotated == true) {
    if (y_angle > 100) {
      Lunge_count ++;
      LungeRotated = false;
    }
  }

  sprite.fillScreen(BLACK);

  sprite.setCursor(10, 10); //文字表示の左上位置を設定
  sprite.println("Lunge count:" + String(Lunge_count));

  sprite.setCursor(10, 175); //文字表示の左上位置を設定
  sprite.setTextColor(BLUE); //文字色の指定
  sprite.print("Result");
  sprite.fillTriangle(30, 210, 55, 240, 80, 210, BLUE);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

  sprite.setTextColor(WHITE); //文字色の指定
  sprite.setTextSize(3);

  delay(100);
  // スプライトを画面表示する
  sprite.pushSprite(0, 0);
  M5.update();  // ボタン状態更新
}

void Lunge() {
  //開始時刻の記録
  startTime = millis();

  //カウントダウン処理
  //カウントが0になるまで繰り返す
  for (int count = COUNTDOWN; count > 0; count--) {
    do {
      //現在の時刻をcurrentTimeに記録（ループで随時取得し更新）
      currentTime = millis();
      //経過時刻（現在の時刻 - 開始時刻）
      passedTime = currentTime - startTime;

      //各種データの更新
      //センサーの値が0から始まることによって運動回数が1から開始されることを防ぐため、
      M5.IMU.getAccelData(&accX, &accY, &accZ);   // 加速度データ取得
      M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ); // 角速度データ取得
      M5.IMU.getAhrsData(&pitch, &roll, &yaw);    // 姿勢角データ取得

      float x_angle = atan2(accX, accZ) * 180.0 / PI; // X-Z加速度から角度に換算
      float y_angle = atan2(accY, accZ) * 180.0 / PI; // Y-Z加速度から角度に換算

      //Bluetoothでペアリングしたデバイスにデータを送信
      SerialBT.printf("0, %02d, 0, 0, 0, 0, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", count, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
      //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
      //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

      //カーソルの位置を初期化
      M5.Lcd.setCursor(10, 10);
      //現在のカウントを表示
      M5.Lcd.println(count);
      //経過時刻がカウントよりも小さい間ループし続ける
      //経過時刻 >= カウントになったとき、カウントの値を更新する
      //例：count = 10; 0010 < 1000 (true) -> 1000 < 1000 (false)
      //COUNTDOWN - count　だと計算の都合よくないので、+1している
    } while (passedTime < ((COUNTDOWN + 1 - count) * 1000));
    //画面を黒くする（10の場合、次に9を表示するときに0が残り続けるため）
    M5.Lcd.clear(BLACK);
  }

  M5.Lcd.println("Start!");
  delay(1000);
  M5.Lcd.clear(BLACK);

  Lunge_count = 0;
  LungeRotated = false;

  do {
    lunge_loop();
  } while (!M5.BtnA.wasPressed());

  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  M5.Lcd.println("Lunge count:" + String(Lunge_count));
  M5.Lcd.setCursor(10, 175); //文字表示の左上位置を設定
  M5.Lcd.setTextColor(GREEN); //文字色の指定
  M5.Lcd.print("Back to Selection");
  M5.Lcd.fillTriangle(30, 210, 55, 240, 80, 210, GREEN);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

  //各種データの更新
  M5.IMU.getAccelData(&accX, &accY, &accZ);   // 加速度データ取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ); // 角速度データ取得
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);    // 姿勢角データ取得

  float x_angle = atan2(accX, accZ) * 180.0 / PI; // X-Z加速度から角度に換算
  float y_angle = atan2(accY, accZ) * 180.0 / PI; // Y-Z加速度から角度に換算

  //Bluetoothでペアリングしたデバイスにデータを送信
  SerialBT.printf("100, 0, 0, 0, 0, %02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", Lunge_count, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

}
