#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#include <WiFi.h>
#include <time.h>

#include <EEPROM.h>

#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// List of multiple chat IDs
const char* chatIDs[] = {
  "5177265044",  // Your Chat ID
  "1234567890",  // Add another user
  "0987654321"   // Add another user
};

const int chatCount = sizeof(chatIDs) / sizeof(chatIDs[0]); // Number of chat IDs

#define DHTPIN 27
#define DHTTYPE DHT22
#define BUZZER 25

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Function Prototypes
void buzzerAlert();
String getTimestamp();

/************************************ EEPROM Declarations  ***************************/
#define EEPROM_SIZE 200
#define SSID1_ADDR 0
#define PASS1_ADDR 50
#define SSID2_ADDR 100
#define PASS2_ADDR 150

// Function to write WiFi credential to EEPROM
void writeWiFiCredential(int addr, const char *data)
{
  int len = strlen(data);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addr + i, data[i]);
  }
  EEPROM.write(addr + len, '\0'); // Null terminator
}
// Function to read WiFi credential from EEPROM
String readWiFiCredential(int addr)
{
  char data[50]; // Buffer to store read data
  int i = 0;
  while (i < 49)
  {
    data[i] = EEPROM.read(addr + i);
    if (data[i] == '\0')
      break;
    i++;
  }
  data[i] = '\0'; // Ensure null termination
  return String(data);
}
// Function to clear stored WiFi credentials
void clearWiFiCredentials_1_()
{
  Serial.println("Clearing WiFi 1 credentials...");
  for (int i = SSID1_ADDR; i < PASS1_ADDR; i++)
  {
    EEPROM.write(i, '\0'); // Clear SSID 1
  }
  for (int i = PASS1_ADDR; i < SSID2_ADDR; i++)
  {
    EEPROM.write(i, '\0'); // Clear Password 1
  }
  EEPROM.commit();
  Serial.println("WiFi 1 credentials cleared.");
}
void clearWiFiCredentials_2_()
{
  Serial.println("Clearing WiFi 2 credentials...");
  for (int i = SSID2_ADDR; i < PASS2_ADDR; i++)
  {
    EEPROM.write(i, '\0'); // Clear SSID 2
  }
  for (int i = PASS2_ADDR; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, '\0'); // Clear Password 2
  }
  EEPROM.commit();
  Serial.println("WiFi 2 credentials cleared.");
}
// Function to store SSID and Password
void storeWiFiCredentials_1_(const char *ssid, const char *password)
{
  writeWiFiCredential(SSID1_ADDR, ssid);
  writeWiFiCredential(PASS1_ADDR, password);
  EEPROM.commit(); // Save changes to EEPROM
}
void storeWiFiCredentials_2_(const char *ssid, const char *password)
{
  writeWiFiCredential(SSID2_ADDR, ssid);
  writeWiFiCredential(PASS2_ADDR, password);
  EEPROM.commit(); // Save changes to EEPROM
}

/************************************ EEPROM Declarations END ***************************/

/*************************************** WiFi Declarations *****************************/
String default_ssid = "CPPMTRAIL";
String default_password = "1234567890";
// Restore From EEPROM
String ssid_1;
String password_1;
String ssid_2;
String password_2;

bool waitForWiFi(int timeout)
{
  int elapsedTime = 0;
  while (WiFi.status() != WL_CONNECTED && elapsedTime < timeout)
  {
    Serial.print(".");
    delay(100);
    elapsedTime += 100;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  Serial.println("\nWiFi not available.");
  return false;
}
void connectToWiFi(String ssid1, String pass1, String ssid2, String pass2, String def_ssid, String def_pass)
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nChecking WiFi 1...");
    WiFi.begin(ssid1.c_str(), pass1.c_str());
    if (waitForWiFi(1000))
      return;

    Serial.println("\nChecking WiFi 2...");
    WiFi.begin(ssid2.c_str(), pass2.c_str());
    if (waitForWiFi(1000))
      return;

    Serial.println("\nConnecting to Default WiFi...");
    WiFi.begin(def_ssid.c_str(), def_pass.c_str());
    if (waitForWiFi(1000))
      return;

    Serial.println("\nNo WiFi Connected. Retrying...");
  }
}
void Check_Wifi_Status()
{
  // Ensure Wi-Fi stays connected
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi disconnected! Reconnecting...");
    ssid_1 = readWiFiCredential(SSID1_ADDR);
    password_1 = readWiFiCredential(PASS1_ADDR);
    ssid_2 = readWiFiCredential(SSID2_ADDR);
    password_2 = readWiFiCredential(PASS2_ADDR);
    connectToWiFi(ssid_1, password_1, ssid_2, password_2, default_ssid, default_password);
  }
}
void Init_WiFi()
{
  ssid_1 = readWiFiCredential(SSID1_ADDR);
  password_1 = readWiFiCredential(PASS1_ADDR);
  ssid_2 = readWiFiCredential(SSID2_ADDR);
  password_2 = readWiFiCredential(PASS2_ADDR);

  Serial.println("********* Primary SSID 1 ********");
  Serial.println("Stored SSID 1 : " + ssid_1);
  Serial.println("Stored Password 1: " + password_1);
  Serial.println("******* Secondary SSID 2 ********");
  Serial.println("Stored SSID 2 : " + ssid_2);
  Serial.println("Stored Password 2 : " + password_2);
  Serial.println("*");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Connecting  to ");
  lcd.setCursor(0, 1);
  lcd.print("      WiFi      ");

  connectToWiFi(ssid_1, password_1, ssid_2, password_2, default_ssid, default_password);

  Serial.println("");
  Serial.println("WiFi connected.");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Wifi connected ");
  lcd.setCursor(0, 1);
  lcd.print("Successfully..!!");

  delay(200);
}
/************************************ WiFi Declarations END ***************************/

#define API_KEY "AIzaSyAtYD7gBwoKdcwnUaSS1bKIVCNQBMuPOdA"
// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "projecttrailcppm@gmail.com"
#define USER_PASSWORD "password"
// Insert RTDB URL
#define DATABASE_URL "https://temp-humid-monitoring-01-default-rtdb.asia-southeast1.firebasedatabase.app/"
// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid; // Variable to save USER UID
// Database main path (to be updated in setup with the user UID)
String databasePath = "Data_Logged/Sensor_Readings/";
String timePath = "/timestamp";
String formattedTimePath = "/formatted_time";

int update_Cycle_Delay = 1000;
float Minimum_Temperature = 99.9;
float Maximum_Temperature = 99.5;

float Temperature_C = 0.0;
float Humidity = 0.0;

float lastTemp = -1.0;
float lastHumidity = -1.0;

void Init_FireBase()
{
  // Assign Firebase credentials
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign token callback and retry attempts
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  // Initialize Firebase
  Firebase.begin(&config, &auth);

  // Wait for authentication
  Serial.println("\nGetting User UID...");
  while (auth.token.uid == "")
  {
    Serial.print('.');
    delay(100);
  }
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
}
void get_Config_FB()
{

  ssid_1 = readWiFiCredential(SSID1_ADDR);
  password_1 = readWiFiCredential(PASS1_ADDR);
  ssid_2 = readWiFiCredential(SSID2_ADDR);
  password_2 = readWiFiCredential(PASS2_ADDR);

  
  String ssid_1_FB;
  String password_1_FB;
  String ssid_2_FB;
  String password_2_FB;
  // Get Primary_WiFi SSID_1
  if (Firebase.RTDB.getString(&fbdo, "/Configurations/WiFi_Credentials/Primary_WiFi/SSID_1"))
  {
    ssid_1_FB = fbdo.stringData();
    Serial.print("WiFi Primary_WiFi Data: ");
    Serial.println(ssid_1_FB);
  }
  else
  {
    Serial.print("Failed to read Primary_WiFi SSID_1 :  ");
    Serial.println(fbdo.errorReason());
  }
  // Get Primary_WiFi Password_1
  if (Firebase.RTDB.getString(&fbdo, "/Configurations/WiFi_Credentials/Primary_WiFi/Password_1"))
  {
    password_1_FB = fbdo.stringData();
    Serial.print("Password Primary_WiFi Data: ");
    Serial.println(password_1_FB);
  }
  else
  {
    Serial.print("Failed to read Primary_WiFi Password_1 : ");
    Serial.println(fbdo.errorReason());
  }

  // Get Secondary_WiFi SSID_2
  if (Firebase.RTDB.getString(&fbdo, "/Configurations/WiFi_Credentials/Secondary_WiFi/SSID_2"))
  {
    ssid_2_FB = fbdo.stringData();
    Serial.print("WiFi Secondary_WiFi Data: ");
    Serial.println(ssid_2_FB);
  }
  else
  {
    Serial.print("Failed to read Secondary_WiFi SSID_2 :  ");
    Serial.println(fbdo.errorReason());
  }
  // Get Secondary_WiFi Password_2
  if (Firebase.RTDB.getString(&fbdo, "/Configurations/WiFi_Credentials/Secondary_WiFi/Password_2"))
  {
    password_2_FB = fbdo.stringData();
    Serial.print("Password Secondary_WiFi Data: ");
    Serial.println(password_2_FB);
  }
  else
  {
    Serial.print("Failed to read Secondary_WiFi Password_2 : ");
    Serial.println(fbdo.errorReason());
  }

  ssid_1.trim();
  password_1.trim();
  ssid_2.trim();
  password_2.trim();

  ssid_1_FB.trim();
  password_1_FB.trim();
  ssid_2_FB.trim();
  password_2_FB.trim();

  // Compare credentials Primary_WiFi
  if (ssid_1.equals(ssid_1_FB) && password_1.equals(password_1_FB))
  {
    Serial.println("!!!!!!!!!!!!!!!!!!Primary_WiFi credentials match!!!!!!!!!!!!!!!!!!");
  }
  else
  {
    Serial.println("Primary_WiFi credentials do not match!");
    clearWiFiCredentials_1_();
    storeWiFiCredentials_1_(ssid_1_FB.c_str(), password_1_FB.c_str());
  }

  // Compare credentials Secondary_WiFi
  if (ssid_2.equals(ssid_2_FB) && password_2.equals(password_2_FB))
  {
    Serial.println("!!!!!!!!!!!!!!!!!!Secondary_WiFi credentials match!!!!!!!!!!!!!!!!!!!");
  }
  else
  {
    Serial.println("Secondary_WiFi credentials do not match!");
    clearWiFiCredentials_2_();
    storeWiFiCredentials_2_(ssid_2_FB.c_str(), password_2_FB.c_str());
  }

  // Get Update_Cycle_Delay
  if (Firebase.RTDB.getInt(&fbdo, "/Configurations/Update_Cycle_Delay(in_Seconds)"))
  {
    update_Cycle_Delay = fbdo.intData();
    Serial.print("Update Cycle Delay Data: ");
    Serial.println(update_Cycle_Delay);
  }
  else
  {
    Serial.print("Failed to read Update Cycle Delay Data : ");
    Serial.println(fbdo.errorReason());
  }

  // Get Minimum_Temperature
  if (Firebase.RTDB.getFloat(&fbdo, "/Configurations/Temperature/Minimum_Temperature"))
  {
    Minimum_Temperature = fbdo.floatData();
    Serial.print("Minimum_Temperature Data: ");
    Serial.println(Minimum_Temperature);
  }
  else
  {
    Serial.print("Failed to read Minimum_Temperature :");
    Serial.println(fbdo.errorReason());
  }

  // Get Maximum_Temperature
  if (Firebase.RTDB.getFloat(&fbdo, "/Configurations/Temperature/Maximum_Temperature"))
  {
    Maximum_Temperature = fbdo.floatData();
    Serial.print("Maximum_Temperature Data : ");
    Serial.println(Maximum_Temperature);
  }
  else
  {
    Serial.print("Failed to read Maximum_Temperature : ");
    Serial.println(fbdo.errorReason());
  }
}
void Check_temp_Humidity()
{
  if (isnan(Temperature_C) || isnan(Humidity))
  {
    Serial.println("Failed to read from DHT sensor! Retrying...");
    delay(100);
    Temperature_C = dht.readTemperature();
    Humidity = dht.readHumidity();
    if (isnan(Temperature_C) || isnan(Humidity))
    {
      Serial.println("DHT sensor read failed again. Restarting sensor...");
      dht.begin(); // Reset sensor
      delay(2500); // Wait for the sensor to stabilize
      return;
    }
  }
  Serial.println("test1");
}
void Update_LCD()
{
  // Update LCD only if values change significantly
  if (abs(Temperature_C - lastTemp) > 0.5 || abs(Humidity - lastHumidity) > 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(Temperature_C);
    lcd.print((char)223); // Degree symbol
    lcd.print("C ");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(Humidity);
    lcd.print(" % ");

    lastTemp = Temperature_C;
    lastHumidity = Humidity;
  }
}

void buzzerAlert()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}
String getTimestamp()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    return "N/A";
  }
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}

struct tm timeinfo;
unsigned long getEpochTime()
{
  time_t now;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return 0;
  }
  time(&now);
  return now;
}
String getFormattedTime()
{
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", &timeinfo);
  return String(buffer);
}

void send_FireBase()
{
  FirebaseJson json;

  unsigned long timestamp = getEpochTime();
  String formattedTime = getFormattedTime();

  Serial.print("Timestamp: ");
  Serial.println(timestamp);
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  // Store temperature and humidity correctly
  json.set("Temperature", String(Temperature_C));
  json.set("Humidity", String(Humidity));

  // Ensure databasePath is valid
  String parentPath = String(databasePath) + "/" + formattedTime + "/";

  // Send data to Firebase
  Serial.printf("Pushing time data... %s\n",
                Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "Success" : fbdo.errorReason().c_str());
}

void setup()
{
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);
  delay(100);

  lcd.init();
  lcd.backlight();
  delay(100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Welcome   ");

  dht.begin();
  delay(100);

  Init_WiFi();
  delay(100);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  configTime(19800, 0, "pool.ntp.org");
  delay(50);
  configTime(19800, 0, "pool.ntp.org");
  delay(50);
  configTime(19800, 0, "pool.ntp.org");
  delay(50);
  configTime(19800, 0, "pool.ntp.org");
  delay(50);

  Init_FireBase();
  delay(100);

  get_Config_FB();

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Root certificate for Telegram
  bot.sendMessage(CHAT_ID, "Bot started successfully!", "");

}

void loop()
{
  Serial.println("******************************************* LOOP BEGINS ****************************************");
  Check_Wifi_Status();

  get_Config_FB();

  Temperature_C = dht.readTemperature(); // Read in Celsius
  Humidity = dht.readHumidity();

  Check_temp_Humidity();

  Update_LCD();

  if (Temperature_C < Minimum_Temperature)
  {
    buzzerAlert();
    bot.sendMessage(CHAT_ID, "<b>Alert! Low Temperature.</b>", "HTML");
    Serial.println("Alert Sent!");
  }
  else if(Temperature_C > Maximum_Temperature){
    buzzerAlert();
    bot.sendMessage(CHAT_ID, "<b>Alert! High Temperature.</b>", "HTML");
    Serial.println("Alert Sent!");
  }
  else
  {
    digitalWrite(BUZZER, LOW);
  }

 //  Check_Wifi_Status();

  send_FireBase();
  Serial.println("******************************************* LOOP END ****************************************");
  delay(update_Cycle_Delay);
}