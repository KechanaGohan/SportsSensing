int Squat_count = 0;
bool SquatRotated = false;

void squat_loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);   // 加速度データ取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ); // 角速度データ取得
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);    // 姿勢角データ取得

  float x_angle = atan2(accX, accZ) * 180.0 / PI; // X-Z加速度から角度に換算
  float y_angle = atan2(accY, accZ) * 180.0 / PI; // Y-Z加速度から角度に換算

  //Bluetoothでペアリングしたデバイスにデータを送信
  SerialBT.printf("2, 0, 0, %02d, 0, 0, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", Squat_count, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

  Serial.println("pitch, roll, yaw"); // 姿勢角
  Serial.printf("%7.2f, %7.2f, %7.2f\n", pitch, roll, yaw);   // 姿勢角
  if (SquatRotated == false) {
    if (roll < 45) {
      Squat_count ++;
      SquatRotated = true;
    }
  } else {
    if (roll > 45) {
      SquatRotated = false;
    }
  }

  sprite.fillScreen(BLACK);

  sprite.setCursor(10, 10); //文字表示の左上位置を設定
  sprite.println("Squat count:" + String(Squat_count));

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

void squat() {
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
      //センサーの値が0から始まるため、現在の値になるまで回す
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

  Squat_count = 0;
  SquatRotated = false;

  do {
    squat_loop();
  } while (!M5.BtnA.wasPressed());

  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  M5.Lcd.println("Squat count:" + String(Squat_count));
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
  SerialBT.printf("100, 0, 0, %02d, 0, 0, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", Squat_count, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result


}
