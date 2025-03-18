#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

// Load cell pin definitions
const int LOADCELL_DOUT_PIN_1 = 16;
const int LOADCELL_SCK_PIN_1 = 4;
const int LOADCELL_DOUT_PIN_2 = 17;
const int LOADCELL_SCK_PIN_2 = 5;
const int LOADCELL_DOUT_PIN_3 = 18;
const int LOADCELL_SCK_PIN_3 = 6;
// const int LOADCELL_DOUT_PIN_4 = 19;
// const int LOADCELL_SCK_PIN_4 = 7;

// Create HX711 instances
HX711 scale1, scale2, scale3, scale4;

// Set the calibration factors (Replace with your actual values)
float calibration_factor_1 = -471.497;  // Example values, update after calibration
float calibration_factor_2 = -471.497;
float calibration_factor_3 = -471.497;
// float calibration_factor_4 = -471.497;

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing Load Cells...");
    // Set CPU frequency to 80 MHz for faster data reading
    rtc_cpu_freq_config_t config;
    rtc_clk_cpu_freq_get_config(&config);
    rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
    rtc_clk_cpu_freq_set_config_fast(&config);
    // Initialize each scale
    scale1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
    scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
    scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
    // scale4.begin(LOADCELL_DOUT_PIN_4, LOADCELL_SCK_PIN_4);

    // Apply calibration factors
    scale1.set_scale(calibration_factor_1);
    scale2.set_scale(calibration_factor_2);
    scale3.set_scale(calibration_factor_3);
    // scale4.set_scale(calibration_factor_4);

    Serial.println("Load Cells Initialized. Ready to measure weights.");
}

void loop() {
    Serial.println("Reading weights:");

    float weight1 = scale1.get_units(10); // Average of 10 readings
    float weight2 = scale2.get_units(10);
    float weight3 = scale3.get_units(10);
    // float weight4 = scale4.get_units(10);

    Serial.print("Weight 1: "); Serial.print(weight1, 2); Serial.println(" g");
    Serial.print("Weight 2: "); Serial.print(weight2, 2); Serial.println(" g");
    Serial.print("Weight 3: "); Serial.print(weight3, 2); Serial.println(" g");
    // Serial.print("Weight 4: "); Serial.print(weight4, 2); Serial.println(" g");

    Serial.println("------------------------");
    delay(2000); // Delay between readings
}
