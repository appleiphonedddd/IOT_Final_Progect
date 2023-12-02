#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "TridentTD_LineNotify.h"
#include "Adafruit_Keypad.h"
#define LINE_TOKEN "Your Token"
TridentTD_LineNotify Line(LINE_TOKEN);
WiFiClientSecure client;

char SSID[] = "ICMNLAB";
char PASSWORD[] = "ndhuicmn@D307";
Servo myservo;

int servoPin = 16;   // SG90 pin and rotate position
int pos = 0;

int buttonPin = 5;   // Button pin

/**
 * Ultrasonic sensor
 */
int trigPin = 4;
int echoPin = 2;
const int distanceThreshold = 20;

unsigned long lastPasswordChange = 0;
const long oneWeek = 604800000;
String currentPassword = "";
String enteredPassword = "";

//Keyboard
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

//init keyboard
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void activateScanner() 
{
  /**
   *  Enable Scanner 
   */
  Serial.begin(9600);
  Serial.println("~T");
  Serial.println("Scanner Activated!");
  Serial.end();
}

String generatePassword()
{
    String password = "";
    const char chars[] = {'A', 'B', 'C', 'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    for (int i = 0; i < 8; i++) {
        int index = random(8);
        password += chars[index];
    }
    return password;
}

void setup() 
{
  /**
   * Servo and Button
   */
  myservo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));
  currentPassword = generatePassword();
  Serial.println("Current Password: " + currentPassword);

  /**
   * Ultrasonic sensor
   */
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  /**
   * Password
   */
  randomSeed(analogRead(0));
  lastPasswordChange = millis(); // 初始化上次密码更改的时间

  /**
   * Connect to AP
   */
  Serial.print("Connecting Wifi: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  Line.setToken(LINE_TOKEN); // Init Line Token

  /**
   * Keyboard
   */
  customKeypad.begin();
  Serial.println("KeyPad Test...");
}

void loop()
{
  int buttonState = digitalRead(buttonPin); // Read button status

  /**
   * If press button rotate 90 angla after 30 second return origin position 
   */
  if (buttonState == LOW) 
  {
    myservo.write(90);
    delay(1000);       
    myservo.write(0);   
  }

  /**
   * Ultrasonic sensor detect user then enable Scanner
   */
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
    
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < distanceThreshold) 
  {
    activateScanner();
  }
  delay(500);

  /**
   * Check if a week has passed about passwords then send Line group by LINE Notify
   */
  unsigned long currentMillis = millis();
  if (currentMillis - lastPasswordChange >= oneWeek) {
    String password = generatePassword();
    Serial.println(password);
    Line.notify("New Passwords: " + password);
    lastPasswordChange = currentMillis;
  }

  delay(1000);

  /**
   * Enter password by keyboard
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
