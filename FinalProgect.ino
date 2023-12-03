#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "TridentTD_LineNotify.h"
#include "Adafruit_Keypad.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define LINE_TOKEN "Your Token"

TridentTD_LineNotify Line(LINE_TOKEN);
WiFiClientSecure client;

char SSID[] = "ICMNLAB";              // WiFi SSID
char PASSWORD[] = "ndhuicmn@D307";    // WiFi Password

/**
 * MySQL
 */
IPAddress server_addr(192,168,50,188);  
char user[] = "root";
char db_password[] = "611221220";
MySQL_Connection conn((Client *)&client);
//char INSERT_SQL = INSERT INTO `mem_info` (`name`, `id`, `date`) VALUES ('egor', '6112212010', '2023-12-03 08:32:33');

// NTP Client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 8 * 3600); // GMT+8 timezone for Taiwan

Servo myservo;

int servoPin = 16;   // SG90 pin and rotate position
int pos = 0;

int buttonPin = 5;   // Button pin

/**
 * Ultrasonic sensor
 */
int trigPin = 4;     // Trigger pin
int echoPin = 2;     // Echo pin
const int distanceThreshold = 20;  // Threshold distance for triggering action

unsigned long lastPasswordChange = 0;
const long oneWeek = 604800000;
String currentPassword = "";
String enteredPassword = "";

//Keyboard setting
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

String activateScanner() {
    Serial.begin(9600);
    Serial.println("~T");
    Serial.println("Scanner Activated!");

    String barcode = "";
    long start = millis();
    while (millis() - start < 5000) 
    { 
       if (Serial.available()) 
       {
            char ch = Serial.read();
            barcode += ch;
            if (ch == '\n') 
            { 
                break;
            }
        }
    }
    Serial.end();

    barcode.trim();
    return barcode;
}


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

void setup() 
{
  /**
   * Servo and Button
   */
  myservo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);

  /**
   * Generate password for every week
   */
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
  lastPasswordChange = millis(); // Initialize the time of last password change

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
  
  // Connect to MySQL
  Serial.println("Connecting to MySQL...");
  if(conn.connect(server_addr, 3306, user, db_password)) 
  {
    Serial.println("Connected to MySQL!");
  } 
  else 
  {
    Serial.println("Connection to MySQL failed!");
  }

  Line.setToken(LINE_TOKEN); // Init Line Token

  /**
   * Keyboard
   */
  customKeypad.begin();
  Serial.println("KeyPad Test...");

  /**
   * Time for NTP
   */
  timeClient.begin(); 
  
  while(!timeClient.update()) 
  {
    timeClient.forceUpdate();
  }
  
  setTime(timeClient.getEpochTime());
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
     String barcode = activateScanner();  // Enable Scanner
     
     if (barcode != "")
     {
        HTTPClient http;
        http.begin(client, "" + barcode);
        int httpCode = http.GET();

        if (httpCode > 0) 
        {
            String payload = http.getString();
            if (payload == "true")
            {
                myservo.write(90); 
                delay(5000);
                myservo.write(0);
            }
        }
            http.end();
     }

    if (conn.connected()) 
    {
    char query[128];
    
    // Query to check if the barcode exists in the database
    sprintf(query, "SELECT id FROM mem_info WHERE id = '%s'", barcode.c_str());
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query);
    column_names *cols = cur_mem->get_columns();
    
    if (cols->num_fields > 0) 
    {
      if(timeClient.update()) 
      {
          setTime(timeClient.getEpochTime());
      }
      
      // If the barcode exists, update the scan access time
      sprintf(query, "UPDATE mem_info SET date = NOW() WHERE id = '%s'", barcode.c_str());
      cur_mem->execute(query);
      Serial.println("Access time updated.");
      
      String formattedDate = String(year()) + "-" + String(month()) + "-" + String(day());
      String formattedTime = String(hour()) + ":" + String(minute()) + ":" + String(second());

      // Send the formatted date and time via LINE Notify
      LINE.notify("Access granted at: " + formattedDate + " " + formattedTime);

      // Wait for a while before sending the next notification
      delay(10000);

    } 
    
    else 
    {
      Serial.println("Barcode not found in the database.");
    }
    
    delete cur_mem;
  }
  
  else 
  {
    Serial.println("MySQL connection lost. Reconnecting...");
    conn.connect(server_addr, 3306, user, db_password);
  }
  
 
  delay(5000);
     
  }
  delay(500);

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