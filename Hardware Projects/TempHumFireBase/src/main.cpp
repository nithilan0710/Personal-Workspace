#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <time.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
#define PRIMARY_SSID "CPPMTRAIL"
#define PRIMARY_PASSWORD "1234567890"

// Firebase Credentials
#define API_KEY "AIzaSyAtYD7gBwoKdcwnUaSS1bKIVCNQBMuPOdA"
#define DATABASE_URL "https://temp-humid-monitoring-01-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define USER_EMAIL "projecttrailcppm@gmail.com"
#define USER_PASSWORD "password"
// Sensor & Display Setup
#define DHTPIN 27
#define DHTTYPE DHT22
#define BUZZER 25

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long fetchSettingsPrevMillis = 0;

bool firebaseConnected = false;

// Variables fetched from Firebase
String backupSSID = "";
String backupPassword = "";
float maxTemp = 99.9; // Celsius
float minTemp = 99.5; // Celsius

// Last known values to prevent unnecessary updates
float lastTemp = -1.0;
float lastHumidity = -1.0;

// Function Prototypes
void buzzerAlert();
void fetchSettings();
void fetchBackupWiFi();
bool connectWiFi(bool isPrimary);
String getTimestamp();

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    dht.begin();
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    // Connect to Wi-Fi
    if (!connectWiFi(true)) {
        fetchBackupWiFi();
        if (!connectWiFi(false)) {
            Serial.println("No Wi-Fi available. Restarting...");
            ESP.restart();
        }
    }

    Serial.println("Wi-Fi Connected! Proceeding to Firebase setup...");

    // Initialize Firebase
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    if (Firebase.ready()) {
        firebaseConnected = true;
        Serial.println("Firebase Connected Successfully.");
        fetchSettings();
    } else {
        Serial.println("Firebase Connection Failed.");
    }

    // Time Synchronization
    configTime(19800, 0, "pool.ntp.org");
}

void loop() {
    float tempC = dht.readTemperature(); // Read in Celsius
    float humidity = dht.readHumidity();

    if (isnan(tempC) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor! Retrying...");
        delay(1000);
        tempC = dht.readTemperature();
        humidity = dht.readHumidity();
        if (isnan(tempC) || isnan(humidity)) {
            Serial.println("DHT sensor read failed again. Restarting sensor...");
            dht.begin();  // Reset sensor
            delay(2500);  // Wait for the sensor to stabilize
            return;
        }
    }
    Serial.println("test1");

    // Update LCD only if values change significantly
    if (abs(tempC - lastTemp) > 0.5 || abs(humidity - lastHumidity) > 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(tempC);
        lcd.print((char)223); // Degree symbol
        lcd.print("C ");
        lcd.setCursor(0, 1);
        lcd.print("Hum: ");
        lcd.print(humidity);
        lcd.print(" % ");

        lastTemp = tempC;
        lastHumidity = humidity;
    }

    // Buzzer alert if temp is out of range
    if (tempC > maxTemp || tempC < minTemp) {
        buzzerAlert();
    } else {
        digitalWrite(BUZZER, LOW);
    }

    // Ensure Wi-Fi stays connected
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconnecting to Wi-Fi...");
        WiFi.reconnect();
    }
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    // Ensure Firebase stays connected
    if (!Firebase.ready()) {
        Serial.println("Reconnecting to Firebase...");
        Firebase.begin(&config, &auth);
    }

    // Send data to Firebase every 15 seconds
    if (firebaseConnected && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
        sendDataPrevMillis = millis();
        
        FirebaseJson json;
        json.set("temperature", tempC); // Store temperature in Celsius
        json.set("humidity", humidity);
        json.set("timestamp", getTimestamp());

        int retryCount = 0;
        while (!Firebase.RTDB.updateNode(&fbdo, "/sensors", &json) && retryCount < 3) {
            Serial.println("Retrying Firebase update...");
            retryCount++;
            delay(2000);
        }

        if (retryCount == 3) {
            Serial.println("Failed to update data after retries.");
        } else {
            Serial.println("Data updated successfully.");
        }
    }

    // Fetch updated settings every 30 seconds
    if (firebaseConnected && (millis() - fetchSettingsPrevMillis > 30000 || fetchSettingsPrevMillis == 0)) {
        fetchSettings();
        fetchSettingsPrevMillis = millis();
    }

    delay(2000);
}

void buzzerAlert() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER, HIGH);
        delay(300);
        digitalWrite(BUZZER, LOW);
        delay(100);
    }
}

void fetchBackupWiFi() {
    Serial.println("Fetching backup Wi-Fi credentials from Firebase...");

    if (Firebase.ready()) {
        if (Firebase.RTDB.getString(&fbdo, "/settings/backupSSID")) {
            backupSSID = fbdo.stringData();
            Serial.println("Backup SSID fetched successfully.");
        } else {
            Serial.println("Failed to get backup SSID: " + fbdo.errorReason());
            backupSSID = ""; // Reset in case of failure
        }

        if (Firebase.RTDB.getString(&fbdo, "/settings/backupPassword")) {
            backupPassword = fbdo.stringData();
            Serial.println("Backup Password fetched successfully.");
        } else {
            Serial.println("Failed to get backup password: " + fbdo.errorReason());
            backupPassword = ""; // Reset in case of failure
        }
    }
}

void fetchSettings() {
    Serial.println("Fetching temperature settings from Firebase...");

    if (Firebase.ready()) {
        if (Firebase.RTDB.getFloat(&fbdo, "/settings/maxTemp")) {
            maxTemp = fbdo.floatData(); // Store as Celsius value
            Serial.print("Max Temp set to: ");
            Serial.println(maxTemp);
        } else {
            Serial.println("Failed to get maxTemp: " + fbdo.errorReason());
            maxTemp = 30.0; // Default value in case of failure
        }

        if (Firebase.RTDB.getFloat(&fbdo, "/settings/minTemp")) {
            minTemp = fbdo.floatData(); // Store as Celsius value
            Serial.print("Min Temp set to: ");
            Serial.println(minTemp);
        } else {
            Serial.println("Failed to get minTemp: " + fbdo.errorReason());
            minTemp = 20.0; // Default value in case of failure
        }
        
         Serial.print("Updated Temp Range: ");
         Serial.print(minTemp);
         Serial.print(" - ");
         Serial.println(maxTemp);
     }
}

bool connectWiFi(bool isPrimary) {
     String ssid = isPrimary ? PRIMARY_SSID : backupSSID;
     String password = isPrimary ? PRIMARY_PASSWORD : backupPassword;
    
     lcd.setCursor(0, 0);
     lcd.print("Connecting to");
     lcd.setCursor(0, 1);
     lcd.print(ssid);
     Serial.print("Connecting to ");
     Serial.println(ssid);

     WiFi.begin(ssid.c_str(), password.c_str());
     int attempt = 0;
     while (WiFi.status() != WL_CONNECTED && attempt < 10) {
         Serial.print(".");
         delay(1000);
         attempt++;
     }

     return (WiFi.status() == WL_CONNECTED);
}

String getTimestamp() {
     time_t now;
     struct tm timeinfo;
     if (!getLocalTime(&timeinfo)) {
         return "N/A";
     }
     char timeString[20];
     strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
     return String(timeString);
}
