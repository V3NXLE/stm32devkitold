#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include "stm32g0xx_hal.h"

class I2CHandler {
public:
    I2CHandler(uint8_t address);
    void Init();
    void SendData(uint8_t* data, uint8_t size);
    bool ReceiveData(uint8_t* data, uint8_t size);

private:
    I2C_HandleTypeDef hi2c1;
    uint8_t deviceAddress;
};

#endif
