#include "I2C.hpp"

I2CHandler::I2CHandler(uint8_t address) : deviceAddress(address) {}

void I2CHandler::Init() {
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();  // Enable GPIO clock for I2C pins

    // Configure PB6 (SCL) and PB7 (SDA) for I2C1
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;  // Open-drain mode
    GPIO_InitStruct.Pull = GPIO_PULLUP;      // Enable pull-up resistors
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;  // STM32G0 uses AF6 for I2C1
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure I2C1
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x2000090E;  // 100kHz I2C Timing
    hi2c1.Init.OwnAddress1 = deviceAddress;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        // Handle error
    }
}

void I2CHandler::SendData(uint8_t* data, uint8_t size) {
    HAL_I2C_Master_Transmit(&hi2c1, 0x08 << 1, data, size, HAL_MAX_DELAY);  // Send to ESP at 0x08
}

bool I2CHandler::ReceiveData(uint8_t* data, uint8_t size) {
    return HAL_I2C_Slave_Receive(&hi2c1, data, size, HAL_MAX_DELAY) == HAL_OK;
}
