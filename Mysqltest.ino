/*#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>

char user[] = "root";
char password[] = "611221220";
WiFiClient client;
MySQL_Connection conn(&client);

void setup() {
  Serial.begin(115200);
  WiFi.begin("ICMNLAB2", "ndhuicmn@D307");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  //IPAddress serverIP(192, 168, 51, 251); // 将字符串 IP 地址转换为 IPAddress 对象

  if (conn.connect("192.168.51.251", 3306, "root", "611221220")) 
  {
    Serial.println("Connected to MySQL server.");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  // 在这里执行您的 MySQL 查询和操作
}*/

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>

const char* ssid = "ICMNLAB2";
const char* password = "ndhuicmn@D307";
const char* host = "192.168.51.251";
const char* user = "root";
const char* password_mysql = "611221220";
const char* database = "icmn_lock";

WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  Serial.println("Connecting to MySQL");

  IPAddress serverIp(192, 168, 51, 251); // 替换为你的 MySQL 服务器的 IP 地址

  if (conn.connect(serverIp, 3306, "root", "611221220")) {
    Serial.println("Connected to MySQL");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  // 程序主循环
}
