#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

// HX711 circuit wiring
const int Load_Cell_1_Pin = 25;
const int Load_Cell_2_Pin = 26;
const int Load_Cell_3_Pin = 27;
const int Load_Cell_4_Pin = 32;

const int Load_Cell_SCK_Pin = 33;

float Weight_1 = 0;
float Weight_2 = 0;
float Weight_3 = 0;
float Weight_4 = 0;

#define Scale_1_Callibration_Factor 0
#define Scale_2_Callibration_Factor 96.6
#define Scale_3_Callibration_Factor 98.2736172295
#define Scale_4_Callibration_Factor 0

HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

void Init_Weight_Scale();
void update_Weight_1();
void update_Weight_2();
void update_Weight_3();
void update_Weight_4();
void update_all_Weight();

void setup()
{
  Serial.begin(115200);

  Init_Weight_Scale();

  Serial.println("Readings:");
}

void loop()
{
  update_Weight_2();
  update_Weight_3();

  char values[100]; // Create a buffer to hold the formatted string
  sprintf(values, "Weight 1 = %.2f g\t | Weight 2 = %.2f g\t | Weight 3 = %.2f g\t | Weight 4 = %.2f g", Weight_1, Weight_2, Weight_3, Weight_4);
  Serial.println(values);

  delay(1000);
}

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

  // scale1.set_scale(Scale_1_Callibration_Factor);
  // scale1.tare();
  // delay(100);

  scale2.set_scale(Scale_2_Callibration_Factor);
  scale2.tare();
  delay(100);

  scale3.set_scale(Scale_3_Callibration_Factor);
  scale3.tare();
  delay(100);

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
  update_Weight_1(); 
  update_Weight_2(); 
  update_Weight_3(); 
  update_Weight_4(); 
}

