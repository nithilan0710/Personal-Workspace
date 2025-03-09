#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Wi-Fi credentials
#define WIFI_SSID "Nithilan"
#define WIFI_PASSWORD "testtest"

// Firebase credentials
#define API_KEY "AIzaSyBtL9DUryU76Cf74vNagVGQjyGLdoezYh8"
#define DATABASE_URL "https://temp-humid-monitoring-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Sensor and display setup
#define DHTPIN 27
#define DHTTYPE DHT22
#define BUZZER 25

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
float maxTemp = 99.9;
float minTemp = 99.5;

void buzzerAlert();

void setup() {
    Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("Firebase connected");
        signupOK = true;
    } else {
        Serial.printf("Firebase Signup Error: %s\n", config.signer.signupError.message.c_str());
    }
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    lcd.init();
    lcd.clear();         
    dht.begin();
    //lcd.begin(16,2,0x27);
    lcd.backlight();
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void loop() {
    float tempF = dht.readTemperature(true);
    float humidity = dht.readHumidity();

    if (isnan(tempF) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempF);
    // lcd.print(223);
    lcd.print(" ");

    lcd.print((char)223);
    lcd.print("F ");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print(" % ");

    if (tempF > maxTemp || tempF < minTemp) {
        buzzerAlert();
    } else {
        digitalWrite(BUZZER, LOW);
    }
    FirebaseJson json;  // Create a JSON object

    json.set("temperature", tempF);  // Add temperature data
    json.set("humidity", humidity);  // Add humidity data
    
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
        sendDataPrevMillis = millis();
        if (Firebase.RTDB.updateNode(&fbdo, "/sensors", &json)) {
            Serial.println("Temperature updated");
        } else {
            Serial.println("Failed to update temperature: " + fbdo.errorReason());
        }
        
        if (Firebase.RTDB.updateNode(&fbdo, "/sensors", &json)) {
            Serial.println("Humidity updated");
        } else {
            Serial.println("Failed to update humidity: " + fbdo.errorReason());
        }
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
