#include <OneWire.h>
#include <DallasTemperature.h>

// Define sensor pins
const int Temperature_Sensor_1_Pin = 14;
const int Temperature_Sensor_2_Pin = 4;
const int Temperature_Sensor_3_Pin = 16;
const int Temperature_Sensor_4_Pin = 13;

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

float Temperature_1 = 0;
float Temperature_2 = 0;
float Temperature_3 = 0;
float Temperature_4 = 0;

void Init_Temperature_Sensor();
void update_Temperature_1();
void update_Temperature_2();
void update_Temperature_3();
void update_Temperature_4();
void update_all_Temperature();

void setup()
{
  Serial.begin(115200);
  Init_Temperature_Sensor();
}

void loop()
{

  // update_Temperature_1();
  // update_Temperature_2();
  // update_Temperature_3();
  // update_Temperature_4();
  update_all_Temperature();

  Serial.print("Sensor 1 Temperature: ");
  Serial.print(Temperature_1);
  Serial.println(" °C");

  Serial.print("Sensor 2 Temperature: ");
  Serial.print(Temperature_2);
  Serial.println(" °C");

  Serial.print("Sensor 3 Temperature: ");
  Serial.print(Temperature_3);
  Serial.println(" °C");

  Serial.print("Sensor 4 Temperature: ");
  Serial.print(Temperature_4);
  Serial.println(" °C");

  Serial.println("-----------------------------");
  delay(1000); // Wait for 2 seconds before the next reading
}

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

  if(Temperature_1 < -100){
    Temperature_1 = 0 ;
  }
}
void update_Temperature_2()
{
  Temp_Sensor2.requestTemperatures();
  Temperature_2 = Temp_Sensor2.getTempCByIndex(0);

  if(Temperature_2 < -100){
    Temperature_2 = 0 ;
  }
}
void update_Temperature_3()
{
  Temp_Sensor3.requestTemperatures();
  Temperature_3 = Temp_Sensor3.getTempCByIndex(0);

  if(Temperature_3 < -100){
    Temperature_3 = 0 ;
  }
}
void update_Temperature_4()
{
  Temp_Sensor4.requestTemperatures();
  Temperature_4 = Temp_Sensor4.getTempCByIndex(0);

  if(Temperature_4 < -100){
    Temperature_4 = 0 ;
  }
}
void update_all_Temperature()
{
  update_Temperature_1();
  update_Temperature_2();
  update_Temperature_3();
  update_Temperature_4();
}
