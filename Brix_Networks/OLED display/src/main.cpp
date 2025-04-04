#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 


void setup()   {

    Serial.begin(9600);
  
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
  
    delay(250); // wait for the OLED to power up
    display.begin(i2c_Address, true); // Address 0x3C default
   //display.setContrast (0); // dim display
   
    display.display();
    delay(2000);
  
    // Clear the buffer.
    display.clearDisplay();
  
    // text display tests
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Failure is always an option");
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.print("0x"); 
    display.println(0xDEADBEEF, HEX);
    display.display();
    delay(2000);
    display.clearDisplay();
 
 
  }
  
  void loop() {

    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);      
    display.println("Connectingto");
    display.display();
    delay(2000);
    display.clearDisplay();
  

}

/*
  
  #include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
 
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}


// this is the bitmap, change this variable for your specification.
const unsigned char BRIX_LOGO [] PROGMEM =  {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0x80, 0x3f, 0xc3, 0xff, 0x00, 0xff, 0xde, 0xd8, 
  0x00, 0x1f, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xe0, 0x1f, 0x81, 0xfe, 0x00, 0x7f, 0x06, 0xd8, 
  0x00, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0x80, 0xfe, 0x00, 0xfe, 0x04, 0xd8, 
  0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0x80, 0x7f, 0x00, 0xfe, 0x04, 0xf8, 
  0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf8, 0x1f, 0x80, 0x7f, 0x81, 0xfc, 0x04, 0xf8, 
  0x00, 0x0f, 0xc0, 0x01, 0xfc, 0x0f, 0x80, 0x01, 0xf8, 0x1f, 0x80, 0x3f, 0x83, 0xf8, 0x04, 0xa8, 
  0x00, 0x0f, 0xc0, 0x01, 0xfc, 0x0f, 0x80, 0x01, 0xfc, 0x1f, 0x80, 0x1f, 0xc7, 0xf0, 0x04, 0x88, 
  0x00, 0x0f, 0xc0, 0x00, 0xfc, 0x0f, 0x80, 0x00, 0xfc, 0x1f, 0x80, 0x0f, 0xef, 0xe0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x01, 0xf8, 0x0f, 0x80, 0x00, 0xfc, 0x1f, 0x80, 0x0f, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x01, 0xf8, 0x0f, 0x80, 0x01, 0xfc, 0x1f, 0x80, 0x07, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0x80, 0x03, 0xf8, 0x1f, 0x80, 0x03, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf8, 0x1f, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0x80, 0x00, 0xfe, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xe0, 0x1f, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xc0, 0x1f, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x01, 0xfc, 0x0f, 0xff, 0xfe, 0x00, 0x1f, 0x80, 0x03, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 0xfc, 0x0f, 0x80, 0x7e, 0x00, 0x1f, 0x80, 0x07, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 0xfe, 0x0f, 0x80, 0x7f, 0x00, 0x1f, 0x80, 0x0f, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 0xfe, 0x0f, 0x80, 0x3f, 0x80, 0x1f, 0x80, 0x0f, 0xcf, 0xf0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 0xfe, 0x0f, 0x80, 0x1f, 0x80, 0x1f, 0x80, 0x1f, 0xc7, 0xf0, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 0xfc, 0x0f, 0x80, 0x1f, 0xc0, 0x1f, 0x80, 0x3f, 0x83, 0xf8, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x03, 0xfc, 0x0f, 0x80, 0x0f, 0xe0, 0x1f, 0x80, 0x7f, 0x01, 0xfc, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xfc, 0x0f, 0x80, 0x07, 0xf0, 0x1f, 0x80, 0xfe, 0x01, 0xfe, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0x80, 0x07, 0xf0, 0x1f, 0x81, 0xfc, 0x00, 0xfe, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0x80, 0x03, 0xf8, 0x1f, 0x81, 0xfc, 0x00, 0x7f, 0x00, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xc0, 0x1f, 0xc0, 0x03, 0xfc, 0x1f, 0xc7, 0xfe, 0x00, 0xff, 0xc0, 0x00, 
  0x00, 0x1f, 0xff, 0xf8, 0x00, 0x1f, 0xc0, 0x03, 0xfc, 0x1f, 0xc7, 0xfc, 0x00, 0xff, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1c, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1e, 0x10, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1e, 0x10, 0x07, 0x00, 0xf8, 0x10, 0x02, 0x00, 0xe0, 0x0b, 0x03, 0x08, 0x06, 0x00, 0x00, 
  0x00, 0x1b, 0x10, 0x0f, 0xc0, 0xfc, 0x18, 0xc6, 0x03, 0xf0, 0x1f, 0x83, 0x38, 0x1f, 0x00, 0x00, 
  0x00, 0x1b, 0x10, 0x18, 0xc0, 0x60, 0x18, 0xc6, 0x07, 0x38, 0x1c, 0x03, 0x30, 0x18, 0x00, 0x00, 
  0x00, 0x19, 0x90, 0x18, 0x60, 0x60, 0x19, 0xe6, 0x06, 0x18, 0x1c, 0x03, 0xe0, 0x18, 0x00, 0x00, 
  0x00, 0x19, 0x90, 0x1f, 0xe0, 0x60, 0x1d, 0xec, 0x06, 0x18, 0x18, 0x03, 0xc0, 0x1e, 0x00, 0x00, 
  0x00, 0x18, 0xd0, 0x1f, 0xc0, 0x60, 0x0d, 0x6c, 0x06, 0x18, 0x18, 0x03, 0xe0, 0x07, 0x00, 0x00, 
  0x00, 0x18, 0xf0, 0x18, 0x00, 0x60, 0x0f, 0x3c, 0x06, 0x18, 0x18, 0x03, 0x70, 0x03, 0x80, 0x00, 
  0x00, 0x18, 0x70, 0x18, 0x00, 0x60, 0x07, 0x38, 0x06, 0x18, 0x18, 0x03, 0x30, 0x01, 0x80, 0x00, 
  0x00, 0x18, 0x30, 0x0f, 0xc0, 0x38, 0x07, 0x38, 0x03, 0xf0, 0x18, 0x03, 0x18, 0x1f, 0x00, 0x00, 
  0x00, 0x10, 0x30, 0x07, 0x80, 0x38, 0x06, 0x10, 0x01, 0xe0, 0x08, 0x01, 0x08, 0x1e, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };
  
void setup() 
{
  display.begin(i2c_Address, true);  // here the 0x3c is the I2C address, check your i2c address if u have multiple devices.
  display.clearDisplay();
  delay(2000);

}

void loop() 
{
  display.drawBitmap(0, 0, BRIX_LOGO, 128, 64, SH110X_WHITE);
  display.display();
  delay(1000);

  // display.drawBitmap(0, 0, BRIX_LOGO, 128, 64, SH110X_WHITE);
  // display.display(); 
  // delay(1000);

}


*/
