#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
#define WIFI_SSID "CPPMTRAIL"
#define WIFI_PASSWORD "1234567890"

// Firebase Credentials
#define API_KEY "AIzaSyAtYD7gBwoKdcwnUaSS1bKIVCNQBMuPOdA"
#define DATABASE_URL "https://temp-humid-monitoring-01-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupSuccess = false;
float maxTemp = 99.9;
float minTemp = 99.5;

void fetchSettings();

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Serial.println("\nConnected to Wi-Fi! IP: " + WiFi.localIP().toString());

    // Configure Firebase
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("Firebase connected!");
        signupSuccess = true;
    } else {
        Serial.printf("Firebase signup failed: %s\n", config.signer.signupError.message.c_str());
    }

    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Fetch settings from Firebase
    fetchSettings();
}

void loop() {
    // Periodically fetch settings every 10 seconds
    delay(10000);
    fetchSettings();
}

void fetchSettings() {
    Serial.println("\nFetching temperature settings from Firebase...");
    Serial.println("Firebase ready: " + String(Firebase.ready()));
    if (Firebase.ready() && signupSuccess) {
        if (Firebase.RTDB.getFloat(&fbdo, "/settings/maxTemp")) {
            maxTemp = fbdo.floatData();
            Serial.print("Max Temp: ");
            Serial.println(maxTemp);
        } else {
            Serial.print("Failed to get maxTemp: ");
            Serial.println(fbdo.errorReason());
        }

        if (Firebase.RTDB.getFloat(&fbdo, "/settings/minTemp")) {
            minTemp = fbdo.floatData();
            Serial.print("Min Temp: ");
            Serial.println(minTemp);
        } else {
            Serial.print("Failed to get minTemp: ");
            Serial.println(fbdo.errorReason());
        }
    } else {
        Serial.println("Firebase not ready. Skipping fetch.");
    }
}
