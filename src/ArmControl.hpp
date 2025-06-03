#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include "stm32g0xx_hal.h"

class ArmControl {
public:
    ArmControl(TIM_HandleTypeDef& htim, uint32_t channel);
    void Init();
    void SetAngle(float angle);

private:
    TIM_HandleTypeDef& htim;
    uint32_t pwm_channel;
};

#endif
