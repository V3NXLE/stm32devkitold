#include "stm32g0xx_hal.h"
#include "ArmControl.hpp
#include "LeakSensor.hpp"
#include "I2C.hpp"

TIM_HandleTypeDef htim2;  // Timer for PWM servo
ArmControl gripperServo(htim2, TIM_CHANNEL_1);
LeakSensor leakSensor;
I2CHandler i2cHandler(0x30);  // STM32 I2C address for both leak sensor and servo control

void SystemClock_Config();
static void MX_GPIO_Init();
static void MX_TIM2_Init();

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();

    gripperServo.Init();
    leakSensor.Init();
    i2cHandler.Init();

    uint8_t receivedData[2];
    uint8_t leakData[3];

    while (true) {
        // **1️⃣ Check the leak sensor**
        uint16_t sensorValue = leakSensor.GetRawValue();
        leakData[0] = 0x01;  // Sensor ID (if multiple sensors exist)
        leakData[1] = (uint8_t)(sensorValue >> 8);  // High byte
        leakData[2] = (uint8_t)(sensorValue & 0xFF); // Low byte

        // **Send leak data to ESP**
        i2cHandler.SendData(leakData, 3);

        // **2️⃣ Check if ESP sends a servo movement command**
        if (i2cHandler.ReceiveData(receivedData, 2)) {
            uint16_t angle = (receivedData[0] << 8) | receivedData[1];  // Convert bytes to angle
            if (angle >= 0 && angle <= 180) {  // Ensure valid angle
                gripperServo.SetAngle(angle);
            }
        }

        HAL_Delay(500);  // Wait before next cycle
    }
}

static void MX_TIM2_Init() {
    __HAL_RCC_TIM2_CLK_ENABLE();
}
