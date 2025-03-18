#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

#include <WiFi.h>
#include "time.h"

const char *ssid = "PTC-HALL";
const char *password = "Ksr1984@PTC";

/*********************** Temperature Sensror Declarations ************************/
// Define sensor pins
const int Temperature_Sensor_1_Pin = 14;
const int Temperature_Sensor_2_Pin = 4;
const int Temperature_Sensor_3_Pin = 16;
const int Temperature_Sensor_4_Pin = 13;

// Define Temperature sensor Variables
float Temperature_1 = 0;
float Temperature_2 = 0;
float Temperature_3 = 0;
float Temperature_4 = 0;

// Initialize OneWire instances
OneWire oneWire1(Temperature_Sensor_1_Pin);
OneWire oneWire2(Temperature_Sensor_2_Pin);
OneWire oneWire3(Temperature_Sensor_3_Pin);
OneWire oneWire4(Temperature_Sensor_4_Pin);

// Initialize DallasTemperature instances
DallasTemperature Temp_Sensor1(&oneWire1);
DallasTemperature Temp_Sensor2(&oneWire2);
DallasTemperature Temp_Sensor3(&oneWire3);
DallasTemperature Temp_Sensor4(&oneWire4);

// Function Prototypes
void Init_Temperature_Sensor();
void update_Temperature_1();
void update_Temperature_2();
void update_Temperature_3();
void update_Temperature_4();
void update_all_Temperature();
/*********************** Temperature Sensror Declarations End  ************************/

/******************************* Load Cell Declarations *******************************/
// Define HX711 Load cell pins
const int Load_Cell_1_Pin = 25;
const int Load_Cell_2_Pin = 26;
const int Load_Cell_3_Pin = 27;
const int Load_Cell_4_Pin = 32;

const int Load_Cell_SCK_Pin = 33;

// Define Weight sensor Variables
float Weight_1 = 0;
float Weight_2 = 0;
float Weight_3 = 0;
float Weight_4 = 0;

// Define Callibration Factor
#define Scale_1_Callibration_Factor 0
#define Scale_2_Callibration_Factor 96.6
#define Scale_3_Callibration_Factor 98.2736172295
#define Scale_4_Callibration_Factor 0

// Initialize Weight sensor instances
HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

// Function Prototypes
void Init_Weight_Scale();
void update_Weight_1();
void update_Weight_2();
void update_Weight_3();
void update_Weight_4();
void update_all_Weight();
/******************************* Load Cell Declarations END *******************************/

/***************************** OLED DISPLAY Declarations *****************************/
#define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// https://diyusthad.com/image2cpp
// this is the bitmap, change this variable for your specification.
const unsigned char BRIX_LOGO[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1f,
    0xff,
    0xff,
    0x80,
    0x3f,
    0xff,
    0xff,
    0x80,
    0x3f,
    0xc3,
    0xff,
    0x00,
    0xff,
    0xde,
    0xd8,
    0x00,
    0x1f,
    0xff,
    0xff,
    0xe0,
    0x1f,
    0xff,
    0xff,
    0xe0,
    0x1f,
    0x81,
    0xfe,
    0x00,
    0x7f,
    0x06,
    0xd8,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x1f,
    0x80,
    0xfe,
    0x00,
    0xfe,
    0x04,
    0xd8,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x1f,
    0x80,
    0x7f,
    0x00,
    0xfe,
    0x04,
    0xf8,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x1f,
    0x80,
    0x7f,
    0x81,
    0xfc,
    0x04,
    0xf8,
    0x00,
    0x0f,
    0xc0,
    0x01,
    0xfc,
    0x0f,
    0x80,
    0x01,
    0xf8,
    0x1f,
    0x80,
    0x3f,
    0x83,
    0xf8,
    0x04,
    0xa8,
    0x00,
    0x0f,
    0xc0,
    0x01,
    0xfc,
    0x0f,
    0x80,
    0x01,
    0xfc,
    0x1f,
    0x80,
    0x1f,
    0xc7,
    0xf0,
    0x04,
    0x88,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfc,
    0x0f,
    0x80,
    0x00,
    0xfc,
    0x1f,
    0x80,
    0x0f,
    0xef,
    0xe0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x01,
    0xf8,
    0x0f,
    0x80,
    0x00,
    0xfc,
    0x1f,
    0x80,
    0x0f,
    0xff,
    0xc0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x01,
    0xf8,
    0x0f,
    0x80,
    0x01,
    0xfc,
    0x1f,
    0x80,
    0x07,
    0xff,
    0xc0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x0f,
    0x80,
    0x03,
    0xf8,
    0x1f,
    0x80,
    0x03,
    0xff,
    0x80,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x1f,
    0x80,
    0x01,
    0xff,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xe0,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x1f,
    0x80,
    0x00,
    0xfe,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x0f,
    0xff,
    0xff,
    0xe0,
    0x1f,
    0x80,
    0x01,
    0xff,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x0f,
    0xff,
    0xff,
    0xc0,
    0x1f,
    0x80,
    0x01,
    0xff,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x01,
    0xfc,
    0x0f,
    0xff,
    0xfe,
    0x00,
    0x1f,
    0x80,
    0x03,
    0xff,
    0x80,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfc,
    0x0f,
    0x80,
    0x7e,
    0x00,
    0x1f,
    0x80,
    0x07,
    0xff,
    0xc0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfe,
    0x0f,
    0x80,
    0x7f,
    0x00,
    0x1f,
    0x80,
    0x0f,
    0xff,
    0xe0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfe,
    0x0f,
    0x80,
    0x3f,
    0x80,
    0x1f,
    0x80,
    0x0f,
    0xcf,
    0xf0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfe,
    0x0f,
    0x80,
    0x1f,
    0x80,
    0x1f,
    0x80,
    0x1f,
    0xc7,
    0xf0,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0xfc,
    0x0f,
    0x80,
    0x1f,
    0xc0,
    0x1f,
    0x80,
    0x3f,
    0x83,
    0xf8,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x03,
    0xfc,
    0x0f,
    0x80,
    0x0f,
    0xe0,
    0x1f,
    0x80,
    0x7f,
    0x01,
    0xfc,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xfc,
    0x0f,
    0x80,
    0x07,
    0xf0,
    0x1f,
    0x80,
    0xfe,
    0x01,
    0xfe,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf8,
    0x0f,
    0x80,
    0x07,
    0xf0,
    0x1f,
    0x81,
    0xfc,
    0x00,
    0xfe,
    0x00,
    0x00,
    0x00,
    0x0f,
    0xff,
    0xff,
    0xf0,
    0x1f,
    0x80,
    0x03,
    0xf8,
    0x1f,
    0x81,
    0xfc,
    0x00,
    0x7f,
    0x00,
    0x00,
    0x00,
    0x1f,
    0xff,
    0xff,
    0xc0,
    0x1f,
    0xc0,
    0x03,
    0xfc,
    0x1f,
    0xc7,
    0xfe,
    0x00,
    0xff,
    0xc0,
    0x00,
    0x00,
    0x1f,
    0xff,
    0xf8,
    0x00,
    0x1f,
    0xc0,
    0x03,
    0xfc,
    0x1f,
    0xc7,
    0xfc,
    0x00,
    0xff,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0x00,
    0x00,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xc0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x18,
    0x10,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1c,
    0x10,
    0x00,
    0x00,
    0x20,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1e,
    0x10,
    0x00,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1e,
    0x10,
    0x07,
    0x00,
    0xf8,
    0x10,
    0x02,
    0x00,
    0xe0,
    0x0b,
    0x03,
    0x08,
    0x06,
    0x00,
    0x00,
    0x00,
    0x1b,
    0x10,
    0x0f,
    0xc0,
    0xfc,
    0x18,
    0xc6,
    0x03,
    0xf0,
    0x1f,
    0x83,
    0x38,
    0x1f,
    0x00,
    0x00,
    0x00,
    0x1b,
    0x10,
    0x18,
    0xc0,
    0x60,
    0x18,
    0xc6,
    0x07,
    0x38,
    0x1c,
    0x03,
    0x30,
    0x18,
    0x00,
    0x00,
    0x00,
    0x19,
    0x90,
    0x18,
    0x60,
    0x60,
    0x19,
    0xe6,
    0x06,
    0x18,
    0x1c,
    0x03,
    0xe0,
    0x18,
    0x00,
    0x00,
    0x00,
    0x19,
    0x90,
    0x1f,
    0xe0,
    0x60,
    0x1d,
    0xec,
    0x06,
    0x18,
    0x18,
    0x03,
    0xc0,
    0x1e,
    0x00,
    0x00,
    0x00,
    0x18,
    0xd0,
    0x1f,
    0xc0,
    0x60,
    0x0d,
    0x6c,
    0x06,
    0x18,
    0x18,
    0x03,
    0xe0,
    0x07,
    0x00,
    0x00,
    0x00,
    0x18,
    0xf0,
    0x18,
    0x00,
    0x60,
    0x0f,
    0x3c,
    0x06,
    0x18,
    0x18,
    0x03,
    0x70,
    0x03,
    0x80,
    0x00,
    0x00,
    0x18,
    0x70,
    0x18,
    0x00,
    0x60,
    0x07,
    0x38,
    0x06,
    0x18,
    0x18,
    0x03,
    0x30,
    0x01,
    0x80,
    0x00,
    0x00,
    0x18,
    0x30,
    0x0f,
    0xc0,
    0x38,
    0x07,
    0x38,
    0x03,
    0xf0,
    0x18,
    0x03,
    0x18,
    0x1f,
    0x00,
    0x00,
    0x00,
    0x10,
    0x30,
    0x07,
    0x80,
    0x38,
    0x06,
    0x10,
    0x01,
    0xe0,
    0x08,
    0x01,
    0x08,
    0x1e,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

/**************************** OLED DISPLAY Declarations END *****************************/

/******************************** Time NTP Server Declarations **************************/
const char *ntpServer = "pool.ntp.org";
// Tamil Nadu (IST) offset: UTC +5:30 = 5 hours 30 minutes = 19800 seconds
const long gmtOffset_sec = 19800; // 5 hours and 30 minutes offset for IST
const int daylightOffset_sec = 0; // No daylight saving time for IST

void Init_Time()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void printLocalTime_Now()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void Oled_Disp_Time()
{

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Failed to obtain time");
    display.display();

    return;
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(&timeinfo, "%I:%M"); // Hour , Minutes , Seconds
  display.setTextSize(1.5);
  display.println(&timeinfo, "%A, %d");  // Day , date
  display.println(&timeinfo, "%B , %Y"); // Month , Year
  display.display();
}

void Oled_Disp_Container_1()
{
  /*
  Serial.println("Container 1 : ");
  Serial.print(" Temperature 4 : ");
  Serial.print(Temperature_4);
  Serial.print(" °C | ");
  Serial.print(" Weight 4 : ");
  Serial.print(Weight_4);
  Serial.println(" gm");


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(&timeinfo, "%H:%M"); // Hour , Minutes , Seconds
  display.setTextSize(1.5);
  display.println(&timeinfo, "%A, %d");  // Day , date
  display.println(&timeinfo, "%B , %Y"); // Month , Year
  display.display();
*/

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(&timeinfo, "%H:%M"); // Hour , Minutes , Seconds
  display.setTextSize(1.5);
  display.println(&timeinfo, "%A, %d");  // Day , date
  display.println(&timeinfo, "%B , %Y"); // Month , Year
  display.display();
  return;
}
void Oled_Disp_Container_2()
{
  return;
}
void Oled_Disp_Container_3()
{
  return;
}
void Oled_Disp_Container_4()
{
  return;
}

/****************************** Time NTP Server Declarations END ************************/

/******************************** WiFi Declarations **************************/

void Init_WiFi()
{

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Connectingto");
  display.println(ssid);
  display.display();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");

  delay(200);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println();
  display.println("   WiFi   ");
  display.print("Connected!");
  display.display();
}

/****************************** WiFi Declarations END ************************/

/****************************** Serial Monitor Declarations  *****************************/
// Function Prototypes
void serial_Disp_Food_Container_1();
void serial_Disp_Food_Container_2();
void serial_Disp_Food_Container_3();
void serial_Disp_Food_Container_4();

/**************************** Serial Monitor Declarations END *****************************/

void setup()
{

  display.begin(i2c_Address, true); // here the 0x3c is the I2C address, check your i2c address if u have multiple devices.
  display.clearDisplay();
  delay(50);

  display.drawBitmap(0, 0, BRIX_LOGO, 128, 64, SH110X_WHITE);
  display.display();
  delay(50);

  Serial.begin(115200);

  Serial.println("BRIX Networks ");
  delay(50);

  Init_Temperature_Sensor();
  Serial.println("Temperature Sensor Initialize DONE ");
  delay(50);

  Init_Weight_Scale();
  Serial.println("Load Cell Initialize DONE ");
  delay(50);

  Init_WiFi();
  delay(50);

  Init_Time();
  delay(50);

  Oled_Disp_Time();
}

void loop()
{

  update_all_Temperature();
  update_all_Weight();

  // float Temperature_1 = 0;
  // float Temperature_2 = 0;
  // float Temperature_3 = 0;
  // float Temperature_4 = 0;

  // float Weight_1 = 0;
  // float Weight_2 = 0;
  // float Weight_3 = 0;
  // float Weight_4 = 0;

  serial_Disp_Food_Container_1();
  serial_Disp_Food_Container_2();
  serial_Disp_Food_Container_3();
  serial_Disp_Food_Container_4();

  delay(1000);

  Oled_Disp_Time();

  Oled_Disp_Container_1();
  delay(1000);

  Oled_Disp_Container_1();
  delay(1000);

  Oled_Disp_Container_1();
  delay(1000);

  Oled_Disp_Container_1();
  delay(1000);
}

void serial_Disp_Food_Container_1()
{
  Serial.println("Container 1 : ");
  Serial.print(" Temperature 1 : ");
  Serial.print(Temperature_1);
  Serial.print(" °C | ");
  Serial.print(" Weight 1 : ");
  Serial.print(Weight_1);
  Serial.println(" gm");
}
void serial_Disp_Food_Container_2()
{
  Serial.println("Container 2 : ");
  Serial.print(" Temperature 2 : ");
  Serial.print(Temperature_2);
  Serial.print(" °C | ");
  Serial.print(" Weight 2 : ");
  Serial.print(Weight_2);
  Serial.println(" gm");
}
void serial_Disp_Food_Container_3()
{
  Serial.println("Container 3 : ");
  Serial.print(" Temperature 3 : ");
  Serial.print(Temperature_3);
  Serial.print(" °C | ");
  Serial.print(" Weight 3 : ");
  Serial.print(Weight_3);
  Serial.println(" gm");
}
void serial_Disp_Food_Container_4()
{
  Serial.println("Container 4 : ");
  Serial.print(" Temperature 4 : ");
  Serial.print(Temperature_4);
  Serial.print(" °C | ");
  Serial.print(" Weight 4 : ");
  Serial.print(Weight_4);
  Serial.println(" gm");
}

/*********************** Temperature Sensor Function Definition ************************/

void Init_Temperature_Sensor()
{
  Temp_Sensor1.begin();
  Serial.println("Temperature Sensor 1 Initialize done ");
  delay(10);

  Temp_Sensor2.begin();
  Serial.println("Temperature Sensor 2 Initialize done ");
  delay(10);

  Temp_Sensor3.begin();
  Serial.println("Temperature Sensor 3  Initialize done ");
  delay(10);

  Temp_Sensor4.begin();
  Serial.println("Temperature Sensor 4 Initialize done ");
  delay(10);
}
void update_Temperature_1()
{
  Temp_Sensor1.requestTemperatures();
  Temperature_1 = Temp_Sensor1.getTempCByIndex(0);

  if (Temperature_1 < -100)
  {
    Temperature_1 = 0;
  }
}
void update_Temperature_2()
{
  Temp_Sensor2.requestTemperatures();
  Temperature_2 = Temp_Sensor2.getTempCByIndex(0);

  if (Temperature_2 < -100)
  {
    Temperature_2 = 0;
  }
}
void update_Temperature_3()
{
  Temp_Sensor3.requestTemperatures();
  Temperature_3 = Temp_Sensor3.getTempCByIndex(0);

  if (Temperature_3 < -100)
  {
    Temperature_3 = 0;
  }
}
void update_Temperature_4()
{
  Temp_Sensor4.requestTemperatures();
  Temperature_4 = Temp_Sensor4.getTempCByIndex(0);

  if (Temperature_4 < -100)
  {
    Temperature_4 = 0;
  }
}
void update_all_Temperature()
{
  update_Temperature_1();
  update_Temperature_2();
  update_Temperature_3();
  update_Temperature_4();
}

/*********************** Temperature Sensror Function Definition END ************************/

/******************************** Load Cell Function Definition ******************************/

void Init_Weight_Scale()
{

  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");

  scale1.begin(Load_Cell_1_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 1 Initialize done ");
  delay(50);

  scale2.begin(Load_Cell_2_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 2 Initialize done ");
  delay(50);

  scale3.begin(Load_Cell_3_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 3 Initialize done ");
  delay(50);

  scale4.begin(Load_Cell_4_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 4 Initialize done ");
  delay(50);

  // scale1.set_scale(Scale_1_Callibration_Factor);
  // scale1.tare();
  // delay(100);

  scale2.set_scale(Scale_2_Callibration_Factor);
  scale2.tare();
  delay(50);

  scale3.set_scale(Scale_3_Callibration_Factor);
  scale3.tare();
  delay(50);

  // scale4.set_scale(Scale_4_Callibration_Factor);
  // scale4.tare();
  // delay(100);
}
void update_Weight_1()
{
  // Weight_1 = scale1.get_units();
  Weight_1 = scale1.get_units(10);
  if (Weight_1 < 1)
  {
    Weight_1 = 0;
  }
}
void update_Weight_2()
{
  // Weight_2 = scale2.get_units();
  Weight_2 = scale2.get_units(10);
  if (Weight_2 < 1)
  {
    Weight_2 = 0;
  }
}
void update_Weight_3()
{
  // Weight_3 = scale3.get_units();
  Weight_3 = scale3.get_units(10);
  if (Weight_3 < 1)
  {
    Weight_3 = 0;
  }
}
void update_Weight_4()
{
  // Weight_4 = scale4.get_units();
  Weight_4 = scale4.get_units(10);
  if (Weight_4 < 1)
  {
    Weight_4 = 0;
  }
}
void update_all_Weight()
{
  // update_Weight_1();
  update_Weight_2();
  update_Weight_3();
  // update_Weight_4();
}

/****************************** Load Cell Function Definition END ****************************/
