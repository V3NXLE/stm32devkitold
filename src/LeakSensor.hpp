#ifndef LEAK_SENSOR_H
#define LEAK_SENSOR_H

#include "stm32g0xx_hal.h"

class LeakSensor {
public:
    LeakSensor();
    void Init();
    uint16_t GetRawValue();
    bool IsLeakDetected();

private:
    ADC_HandleTypeDef hadc1;
};

#endif
