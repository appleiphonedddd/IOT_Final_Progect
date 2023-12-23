#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Keypad.h>

// Wi-Fi Credentials
const char* ssid = "ICMNLAB"; // Replace with your actual SSID
const char* password = "ndhuicmn@D307"; // Replace with your actual Wi-Fi password

// MySql Info
const char* serverName = "https://134.208.2.206:8147"; // Replace with your actual server API URL
char user[]         = "root";
char Password[]     = "611221220";

char database[] = "write";
char table[]    = "yyyy";

// Keyboard setup
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 5, 4, 0}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 14, 12, 13}; // Connect to the column pinouts of the keypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char inputCode[16]; // Array to store the entered code
int inputIndex = 0; // Index for the position in the inputCode

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  customKeypad.begin();
  Serial.println("KeyBoard init finish");
}

void loop() 
{
  customKeypad.tick();

  // Handle keypad events
  while (customKeypad.available()) 
  {
    keypadEvent e = customKeypad.read();
    
    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {
      char key = (char)e.bit.KEY;
      
      if (key == '#') 
      { // Use '#' as enter key
        inputCode[inputIndex] = '\0'; // Null-terminate the string
        checkPassword(inputCode);
        inputIndex = 0; // Reset the index
      } 
      
      else if (inputIndex < sizeof(inputCode) - 1) 
      { // Prevent buffer overflow
        inputCode[inputIndex++] = key; // Add key to code
      }
    }
  }
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void checkPassword(const char* code) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;
    WiFiClient wifiClient;
    String serverPath = String(serverName) + "/api.php?password=" + String(inputCode);
    http.begin(wifiClient,serverName); // Specify the URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Data to send with HTTP POST
    //String httpRequestData = "password=" + String(code);

    // Send HTTP POST request
    int httpResponseCode = http.POST(serverPath);

    if (httpResponseCode == 200) 
    {
      Serial.println("connect to Mysql");
      String payload = http.getString();
      Serial.println("Response: " + payload);
    } 
    else 
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end(); // Free resources
  } 
  else 
  {
    Serial.println("Wi-Fi Disconnected.");
  }
}
