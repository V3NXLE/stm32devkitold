#include "ArmControl.hpp"

ArmControl::ArmControl(TIM_HandleTypeDef& htim, uint32_t channel)
    : htim(htim), pwm_channel(channel) {}

void ArmControl::Init() {
    __HAL_RCC_TIM2_CLK_ENABLE();  // Enable Timer2 clock

    TIM_OC_InitTypeDef sConfigOC = {0};
    htim.Instance = TIM2;
    htim.Init.Prescaler = 80 - 1;  // 80MHz clock, prescaler sets 1MHz timer
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = 20000 - 1;  // 20ms period (50Hz PWM)
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_PWM_Init(&htim);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;  // Default position at 1.5ms (90°)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, pwm_channel);
    HAL_TIM_PWM_Start(&htim, pwm_channel);
}

void ArmControl::SetAngle(float angle) {
    uint32_t pulse = 1000 + (angle / 180.0f) * 1000;  // Convert 0-180° to 1-2ms pulse width
    __HAL_TIM_SET_COMPARE(&htim, pwm_channel, pulse);
}
