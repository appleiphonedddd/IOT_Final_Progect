#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "TridentTD_LineNotify.h"
#include "Adafruit_Keypad.h"
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define LINE_TOKEN "LhSJY6MAXOXW9IxWxLkGTULqRvDk2n4idBV7a5ZDogM"

TridentTD_LineNotify Line(LINE_TOKEN);
WiFiClientSecure client;

char SSID[] = "ICMNLAB2";              // WiFi SSID
char PASSWORD[] = "ndhuicmn@D307";    // WiFi Password
// NTP Client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 8 * 3600); // GMT+8 timezone for Taiwan

Servo myservo;

int servoPin = 15;   // SG90 pin and rotate position
int pos = 0;

unsigned long lastPasswordChange = 0;
const long oneWeek = 604800000;
String currentPassword = "";
String enteredPassword = "";

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 5, 4, 0}; //定義列的腳位
byte colPins[COLS] = {2, 14, 12, 13}; //定義行的腳位

//初始化鍵盤
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String generatePassword()
{
    String password = "";
    const char chars[] = {'A', 'B', 'C', 'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    for (int i = 0; i < 8; i++) {
        int index = random(14);
        password += chars[index];
    }
    return password;
}

void setup() {
  /**
   * Servo and Button
   */
  myservo.attach(servoPin);

  /**
   * Generate password for every week
   */
  randomSeed(analogRead(0));
  currentPassword = generatePassword();
  Serial.println("Current Password: " + currentPassword);

    /**
   * Password
   */
  randomSeed(analogRead(0));
  lastPasswordChange = millis(); // Initialize the time of last password change

  /**
   * Connect to AP
   */
  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  
  Serial.begin(9600);
  customKeypad.begin();
  Serial.println("KeyPad Test...");

  timeClient.begin(); 
  
  while(!timeClient.update()) 
  {
    timeClient.forceUpdate();
  }
  
  setTime(timeClient.getEpochTime());
}

void loop() {
    /**
   * Check and update password weekly, notify via LINE
   */
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastPasswordChange >= oneWeek) 
  {
    String password = generatePassword();
    Serial.println(password);
    Line.notify("New Passwords: " + password);
    lastPasswordChange = currentMillis;
  }

  delay(1000);

  /**
   * Enter password by keyboard and check true or false
   */
  customKeypad.tick();
  
  while(customKeypad.available()) 
  {
      keypadEvent e = customKeypad.read();
      
      if (e.bit.EVENT == KEY_JUST_PRESSED) 
      {
          enteredPassword += (char)e.bit.KEY;
          Serial.print((char)e.bit.KEY);

          if (enteredPassword.length() == currentPassword.length()) 
          {
             if (enteredPassword == currentPassword) 
             {
                Serial.println("Correct Password!");
                myservo.write(90);
                delay(5000);
                myservo.write(0);
             } 
             else 
             {
                Serial.println("Incorrect Password!");
             }
                enteredPassword = ""; // reset input pssword by user
          }
     }
   }
}
