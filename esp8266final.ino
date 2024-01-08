#include <ESP8266WiFi.h> //import ESP8266 WiFi library
#include <ESP8266HTTPClient.h>//import ESP8266 HTTP Client library
#include <Wire.h>

const char ssid[]     = "123";// change to your WIFI SSID
const char password[] = " 12345678";// change to your WIFI Password
WiFiClient client;   
HTTPClient http;
String serverName  = "http://134.208.2.206:8147/final/connect.php";
String BorrowName = "";
int count = 1;

void(* resetFunc) (void) = 0;
const int relayPin = 5;
bool relayState = LOW; // 使用布林變量來跟踪繼電器的狀態

#define MAX_NAMES 10  // 假設最多追踪 10 個不同的名稱

String names[MAX_NAMES];    // 儲存名稱的陣列
int counts[MAX_NAMES] = {0}; // 對應每個名稱的計數器


String checkName(String name) {
    for (int i = 0; i < MAX_NAMES; i++) {
        if (names[i] == name) {
            // 名稱已存在，增加計數並返回對應的狀態
            counts[i]++;
            return (counts[i] % 2 == 1) ? "借用" : "歸還";
        } else if (names[i] == "") {
            // 新名稱，儲存並設定計數為 1
            names[i] = name;
            counts[i] = 1;
            return "借用"; // 第一次出現，總是 "barrow"
        }
    }
    return "error"; // 如果名稱太多，無法追踪
}


void setup()
{ 
    Serial.begin(9600);
    Serial.println("Starting Test......"); Serial.println("");
    //connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    pinMode(relayPin, OUTPUT);
    WiFi.begin(ssid, password);  
    //try to connect to WIFI 
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    

}
void loop()
{
    if (Serial.available()) { // 檢查是否有來自Arduino的數據
    String receivedData = Serial.readStringUntil('\n'); // 讀取數據直到換行符
    String received = "";
    for(int i=0 ; i<9;i++){
        received = received + receivedData[i];
      }
    Serial.println("Test: " + received);
    Serial.println("Received Data: " + receivedData); // 打印接收到的數據
    delay(500);
    ;
    String br = checkName(received);
    Serial.println(br);
      
      if(WiFi.status()== WL_CONNECTED){
          HTTPClient http;
    
          String serverPath = serverName +"?id="+ String(received) + "&mode="+"%27" +String(br)+"%27";
          // Your Domain name with URL path or IP address with path
          http.begin(client,serverPath); // 使用新的API格式
          Serial.println(serverPath);
          // If you need Node-RED/server authentication, insert user and password below
          //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
          
          // Send HTTP GET request
          int httpResponseCode = http.GET();
          
          if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            delay(1000);
                      
            if (payload == "1") { // 進行字符串比較
                relayState = !relayState; // 切換繼電器狀態
                digitalWrite(relayPin, relayState ? HIGH : LOW);
                Serial.println(relayState ? "Relay ON" : "Relay OFF");
            }
          }else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }
          // Free resources
          http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
    }
    delay(1000); // 延遲1秒以避免連續發送請求
}
