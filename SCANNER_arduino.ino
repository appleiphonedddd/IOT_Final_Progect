
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);        //  arduino 10,11
SoftwareSerial esp8266Serial(10, 11); // ESP8266連接
String receivedData = "";
String keyData = "";

#include <Wire.h>              // I2C程式庫
#include <LiquidCrystal_I2C.h> // LCD_I2C模組程式庫

// LCD I2C位址，默認為0x27或0x3F，依據背板的晶片不同而有差異，16、2為LCD顯示器大小。
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {13, 12, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void (*resetFunc)(void) = 0;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Scanner Ready");
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0); // 從第一行開始顯示
  lcd.print("studentID:");
}

void loop()
{ // run over and over
  if (mySerial.available())
  {
    char receivedChar = mySerial.read(); // 讀取字符
    Serial.write(receivedChar);          // 將字符打印到串行監視器

    // 檢查是否是終止字符（例如換行符）
    if (receivedChar == '\n')
    {
      // 處理接收到的完整數據
      Serial.println("Received Data: " + receivedData);
      esp8266Serial.begin(9600); // ESP8266串行通信
      esp8266Serial.println(receivedData);
      esp8266Serial.end();
      mySerial.begin(9600);
      lcd.setCursor(2, 1); // 從第二行開始顯示
      for (int k = 0; k < 9; k++)
      {
        lcd.print(receivedData[k]);
      }

      // 初始化ESP8266，連接到Wi-Fi等（如果需要）

      // 清空字符串以便於接收新的數據
      receivedData = "";
      delay(3000);
      lcd.clear();         // 清空整個lcd
      lcd.setCursor(2, 0); // 從第一行開始顯示
      lcd.print("studentID:");
    }
    else
    {
      // 將讀取的字符添加到字符串中
      receivedData += receivedChar;
    }
  }
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    keyData += key;
    Serial.print(key);
    lcd.setCursor(2, 1); // 從第二行開始顯示
    lcd.print(keyData);
  }
  if (keyData.length() > 8)
  {
    esp8266Serial.begin(9600); // ESP8266串行通信
    esp8266Serial.println(keyData);
    esp8266Serial.end();
    Serial.println("key Data: " + keyData);
    keyData = "";
    delay(3000);
    lcd.clear();         // 清空整個lcd
    lcd.setCursor(2, 0); // 從第一行開始顯示
    lcd.print("studentID:");
    resetFunc();
  }
}