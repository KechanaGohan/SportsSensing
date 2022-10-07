//Walking用定義
const int PARAMETER = 50;
float storeAccelData[PARAMETER];
float threshold = 0;
int elementCount = 0;
float range = 50.0;
uint8_t countStep = 0;


//閾値を取得する関数
float getDynamicThreshold(float * s) {
  float maxVal = s[0];
  float minVal = s[0];
  //最大値、最小値を取得
  for (int i = 1; i < sizeof(s); i++) {
    maxVal = max(maxVal, s[i]);
    minVal = min(minVal, s[i]);
  }
  //閾値を出力
  return (maxVal + minVal) / 2.0;
}

//ノイズ除去関数
float getFilterdAccelData() {
  static float y[2] = {0};
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  //デジタルローパスフィルタの係数の値を0.8に設定
  y[1] = 0.8 * y[0] + 0.2 * (abs(accX) + abs(accY) + abs(accZ)) * 1000.0;
  y[0] = y[1];
  return y[1];
}


//万歩計のループ処理
void walking_loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);   // 加速度データ取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ); // 角速度データ取得
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);    // 姿勢角データ取得

  float x_angle = atan2(accX, accZ) * 180.0 / PI; // X-Z加速度から角度に換算
  float y_angle = atan2(accY, accZ) * 180.0 / PI; // Y-Z加速度から角度に換算

  //Bluetoothでペアリングしたデバイスにデータを送信
  SerialBT.printf("1, 0, %02d, 0, 0, 0, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", countStep, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

  //0.1秒の遅延を挿入
  delay(100);
  //50回（PARAMETERの定義値）分センサー値を求め、その最大値と最小値を足したものを２分して平均を求める。
  //elementCountはそのセンサー値を取得するためのカウントの変数。
  //ループごとに1ずつ増やしていく。
  elementCount++;
  storeAccelData[elementCount] = getFilterdAccelData();
  //わずかなセンサーの動きは、前の値をそのまま渡すことによって、歩行以外の動作を少しでも切り捨てる。
  //今回は50.0という値をその閾値に設定している。
  if (abs(storeAccelData[elementCount] - storeAccelData[elementCount - 1]) < range) {
    storeAccelData[elementCount] = storeAccelData[elementCount - 1];
    elementCount--;
  }
  //一個前の加速度のデータが閾値よりも大きく、今回の加速度のデータが閾値よりも小さい場合、歩数としてカウント
  if (storeAccelData[elementCount] < threshold && storeAccelData[elementCount - 1] > threshold) {
    countStep++;
  }
  //センサーの値が50個目になったとき、その50回分のデータを利用して、閾値を算出。
  //elementCountの値を0に戻しておく。
  if (elementCount == PARAMETER) {
    threshold = getDynamicThreshold(&storeAccelData[0]);
    elementCount = 0;
    storeAccelData[elementCount] = getFilterdAccelData();
  }

  sprite.fillScreen(BLACK);

  sprite.setCursor(10, 10); //文字表示の左上位置を設定
  sprite.println("Walked " + String(countStep) + " steps");
  Serial.println("Walked " + String(countStep) + " steps");


  sprite.setCursor(10, 175); //文字表示の左上位置を設定
  sprite.setTextColor(BLUE); //文字色の指定
  sprite.print("Result");
  sprite.fillTriangle(30, 210, 55, 240, 80, 210, BLUE);// 塗り潰し三角（x0, y0, x1, y1, x2, y2, 色）

  sprite.setTextColor(WHITE); //文字色の指定
  sprite.setTextSize(3);

  // スプライトを画面表示する
  sprite.pushSprite(0, 0);

  M5.update();  // ボタン状態更新
}

void walking() {
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

  do {
    walking_loop();
  } while (!M5.BtnA.wasPressed());

  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  M5.Lcd.println("Walked " + String(countStep) + " steps");
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
  SerialBT.printf("100, 0, %02d, 0, 0, 0, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", countStep, accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle);
  //運動の内容,カウントダウン,ウォーキング回数,スクワット回数,サイドプランク時間,ランジ回数,accX, accY, accZ, gyroX, gyroY, gyroZ, pitch, roll, yaw, x_angle, y_angle
  //0 運動なし,1 ウォーキング,2 スクワット,3 サイドプランク,4 ランジ,100 result

}
