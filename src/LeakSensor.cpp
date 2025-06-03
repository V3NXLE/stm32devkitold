#include "LeakSensor.hpp"

#define SENSOR_THRESHOLD 50  // Adjust based on sensor calibration

LeakSensor::LeakSensor() {}

void LeakSensor::Init() {
    __HAL_RCC_ADC_CLK_ENABLE();  // Enable ADC clock

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&hadc1);
    HAL_ADC_Start(&hadc1);
}

uint16_t LeakSensor::GetRawValue() {
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);  // Wait for conversion
    return HAL_ADC_GetValue(&hadc1);
}

bool LeakSensor::IsLeakDetected() {
    return GetRawValue() > SENSOR_THRESHOLD;  // Compare ADC reading with threshold
}
