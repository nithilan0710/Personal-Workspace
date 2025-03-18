#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"

// HX711 circuit wiring
const int Load_Cell_1_Pin= 25;
const int Load_Cell_2_Pin= 26;
const int Load_Cell_3_Pin= 27;
const int Load_Cell_4_Pin= 32;

const int Load_Cell_SCK_Pin = 33;

HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

void Init_Scale();
void Calibrate_Scale_1();
void Calibrate_Scale_2();
void Calibrate_Scale_3();
void Calibrate_Scale_4();

void setup() {
  Serial.begin(115200);
  Init_Scale();
 }

void loop() {
  //Calibrate_Scale_1();
  //Calibrate_Scale_2();
  Calibrate_Scale_3();
  //Calibrate_Scale_4();

}

//calibration factor will be the (reading)/(known weight)

void Init_Scale(){

  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);

  scale1.begin(Load_Cell_1_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 1 Initialize done ");
  delay(100);

  scale2.begin(Load_Cell_2_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 2 Initialize done ");
  delay(100);

  scale3.begin(Load_Cell_3_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 3 Initialize done ");
  delay(100);

  scale4.begin(Load_Cell_4_Pin, Load_Cell_SCK_Pin);
  Serial.println("Scale 4 Initialize done ");
  delay(100);
}

void Calibrate_Scale_1() {

  Serial.println("Scale 1");
  if (scale1.is_ready()) {
    scale1.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(500);
    scale1.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(500);
    long reading = scale1.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 Scale 1 not found.");
  }
  delay(1000);

}

void Calibrate_Scale_2() {

  Serial.println("Scale 2");
  if (scale2.is_ready()) {
    scale2.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(500);
    scale2.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(500);
    long reading = scale2.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 Scale 2 not found.");
  }
  delay(1000);

}

void Calibrate_Scale_3() {

  Serial.println("Scale 3");
  if (scale3.is_ready()) {
    scale3.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(500);
    scale3.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(500);
    long reading = scale3.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 Scale 3 not found.");
  }
  delay(1000);

}

void Calibrate_Scale_4() {

  Serial.println("Scale 4");
  if (scale4.is_ready()) {
    scale4.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(500);
    scale4.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(500);
    long reading = scale4.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 Scale 4 not found.");
  }
  delay(1000);

}
