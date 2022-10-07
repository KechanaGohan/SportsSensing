# SportsSensing
M5stackCore2を使用した運動促進プロダクト  
![Videotogif](https://user-images.githubusercontent.com/102903015/194477477-780f9adb-0df9-4ed9-b6cf-ea948ea1305a.gif)  
M5stackCore2とProcessingを使用し、楽しく運動ができるプロダクト制作に挑戦しました。  
M5stackCore2内臓のMPU6886で運動情報を取得し、PCにBluetoothシリアル通信でデータを送信します。  
送られたデータはProcessingに反映されます。  
ここではM5stackCore2側の機能にフォーカスして記載します。
# 使用したもの
M5stackCore2  
https://www.switch-science.com/catalog/6530/  
Bluetooth接続可能なPC  
M5stackCore2装着用のレッグバンド  
※レッグバンドはちょうどいいものがなかったため、100均のものを組み合わせて自作しました
# 構成イメージ
M5StackCore2ーー[Bluetoothシリアル通信]ーー＞Python(PC)ーー[OSC通信]ーー＞Processing(PC)
//[Bluetoothシリアル通信]までが担当箇所
![image](https://user-images.githubusercontent.com/102903015/194478515-2e86ee0d-64a2-4807-ba11-ac7b7614bd36.png)
# 使用方法
1)Python,Processingをコード実行し、レッグバンドを太ももに装着します。
2)M5StackCore2を起動し、PCとBluetoothペアリングを行います。
3)M5StackCore2でメニューに従い運動を選択します。
4)10秒カウントダウンが開始するので、レッグバンドにM5StackCore2を装着します。
5)各種運動を行います。
6)任意のタイミングでM5StackCore2の終了ボタンを押します。
7)resultを確認します。
※resultはPCのsキー押下で保存できます。
※M5StackCore2条の選択画面に戻るボタンを押すことで再び運動の選択が可能です(手順3から繰り返し)。
