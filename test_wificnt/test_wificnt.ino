// ------------------------------------------------------------
// ライブラリインクルード部 Library include section.
// ------------------------------------------------------------
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// サーバー設定ポート80で接続
WebServer server(80);
// 入出力ピン番号設定
#define OUTPUT_PIN0 0  //出力ピン
// ------------------------------------------------------------
// 定数/変数　定義部　Constant / variable definition section.
// ------------------------------------------------------------ 
//SSID: aterm-09487a-g
//PASS: 1345046b0c5ef
const char* ssid = "aterm-09487a-g"; // ご自分のWi-FiルータのSSIDを記述します。 "your-ssid"
const char* password = "1345046b0c5ef"; // ご自分のWi-Fiルータのパスワードを記述します。"your-password"


// 変数宣言
float data;
// htmlデータを文字列に格納 ※R"(ここに書いた文字列は改行を無視して1行の文字列として扱える)"
String html = R"(
	<!DOCTYPE html><html lang="jp"><head>
	<meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>REMOTE-CONTROLLER</title>
	<style>
		body{font-family: sans-serif; background-color: #fff; max-width: 480px; margin: 0 auto; align-items: center;}
		h1 {color:#333; text-align: center; font-size: 28px; margin: 10px auto;}
		div {display: flex; flex-direction: row; justify-content: center;}
		.btn {height: 70px; width: 100px; color: rgb(44, 43, 43); background-color: #dddde9; font-size: 18px; font-weight: bold; border-radius: 7%; margin: 0 10px; -webkit-appearance: none;}
		td {width: 110px; color:#333; text-align: center; font-size: 18px;}
	</style></head><body>
		<h1>REMOTE-CONTROLLER</h1>
		<div>
		<button class="btn" id="btn0" onclick="getBtnOn(0) ">ON</button>
		<button class="btn" id="btn1" onclick="getBtnOn(1) ">OFF</button>
		</div>
		<div style = "margin-top: 20px;">
		<table border = "1">
			<tr><td>本体温度</td><td><p id="output"></p></td></tr>
		</table>
		</div>
	</body>
	
	<script type="text/javascript">
	// ON/OFFボタン操作処理
	async function getBtnOn(i) {
		let link;
		switch (i) {
		case 0: link = "/get/btn_on"; break;
		case 1: link = "/get/btn_off"; break;
		}
		try {
		const response = await fetch(link);
		if (response.ok) {
			const text = await response.text();
			console.log(text);
		}
		} catch (error) { console.log(error); }
	}
	// データ取得、output表示更新
	async function updateOutput() {
		const output = document.getElementById("output");
		try {
		const response = await fetch("/get/data");
		if (response.ok) {
			output.textContent = await response.text();
		} else { throw new Error(); }
		} catch (error) { console.log(error); }
	}
	setInterval(updateOutput, 1000);
	</script></html>

)";
// 関数 ------------------------------------------------------------
/********* クライアントにWebページ（HTML）を返す関数 *********/
void handleRoot() {
	server.send(200, "text/html", html); //レスポンス200を返し、htmlデータ送信
	Serial.println("200, Root Access!");
}
/********** クライアントにエラーメッセージを返す関数 **********/
void handleNotFound() { //ファイルが見つかりません
	server.send(404, "text/plain", "File not found!");
	Serial.println("404, File not found!");
}
/********** ブラウザボタンON/OFF、温度データのレスポンスを返す関数 **********/
// ブラウザボタンON
void BtnOn() {
	digitalWrite(LED_BUILTIN, HIGH);        // 本体LED ON
	digitalWrite(OUTPUT_PIN0, HIGH);        // GPIO0 HIGH
	server.send(200, "text/html", "ON!");   // レスポンス200を返し、ON！を送信
	Serial.println("200, ON!");
}
// ブラウザボタンOFF
void BtnOff() {
	digitalWrite(LED_BUILTIN, LOW);         // 本体LED OFF
	digitalWrite(OUTPUT_PIN0, LOW);         // GPIO0 LOW
	server.send(200, "text/html", "OFF!");  // レスポンス200を返し、OFF!を送信
	Serial.println("200, OFF!");
}
// ブラウザデータ表示更新
void getData() {
	char buf[10];                           // data文字列格納バッファ
	sprintf(buf, "%.1f℃", data);            // dataの値を文字列として格納
	server.send(200, "text/html", buf);     // レスポンス200を返し、dataを文字列で送信
	Serial.printf("Temp: %.1f'C\n", data);
}
/********************* Webサーバー設定 ********************/
void serverSetting() {
	server.on("/", handleRoot);         // ルートアクセス時の応答関数を設定
	server.onNotFound(handleNotFound);  // 不正アクセス時の応答関数を設定
	server.on("/get/btn_on", BtnOn);    // ボタンON受信処理
	server.on("/get/btn_off", BtnOff);  // ボタンOFF受信処理
	server.on("/get/data", getData);    // データ表示更新受信処理
	server.begin(); // Webサーバー開始
}
// 初期設定 ------------------------------------------------
void setup() {
	// WiFi接続開始
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) { //接続完了してなければ
		delay(500);
		Serial.print(".");
	}
	Serial.print("\nSSID: ");       // SSID表示
	Serial.println(WiFi.SSID());
	Serial.print("IP: ");           // IPアドレス表示
	Serial.println(WiFi.localIP());
	// サーバー設定
	serverSetting();
	// 出力端子設定
	pinMode(LED_BUILTIN, OUTPUT);     // 本体LEDは「LED_BUILTIN」で指定（ピン番号だと32で指定）
	digitalWrite(LED_BUILTIN, HIGH);  // 本体LED ON
	pinMode(OUTPUT_PIN0, OUTPUT);     // LED GP0を出力端子に設定（LEDのWL_GPIO0とは別物）
}
// メイン --------------------------------------------------
void loop() {
	server.handleClient();    //クライアントからのアクセス処理
	data = analogReadTemp();  // 本体温度データ取得
	delay(500);
}