#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "debug.h"
#include "config.h"


void i2cInit(I2C_HandleTypeDef *i2cHandler, I2C_TypeDef *i2cInterface) {
    i2cHandler->Instance = i2cInterface;
    i2cHandler->Init.ClockSpeed = 400000;
    i2cHandler->Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2cHandler->Init.OwnAddress1 = 0;
    i2cHandler->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2cHandler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2cHandler->Init.OwnAddress2 = 0;
    i2cHandler->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2cHandler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(i2cHandler) != HAL_OK)
    {
        debugFatal("Error Config I2C!");
    }
}

bool i2cWrite(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t *data, size_t length) {
    if (HAL_I2C_Master_Transmit(i2cHandler, deviceAddress << 1, data, length, I2C_TIMEOUT) == HAL_OK)
        return true;
    else
        return false;
}

bool i2cRead(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t *data, size_t length) {
    if(HAL_I2C_Master_Receive(i2cHandler, deviceAddress << 1 | 1, data, length, I2C_TIMEOUT) == HAL_OK)
        return true;
    else
        return false;
}



bool i2cWriteMemory(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t memoryAddress, uint8_t *data, size_t length) {
    if (HAL_I2C_Mem_Write(i2cHandler, deviceAddress << 1, memoryAddress, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT) == HAL_OK)
        return true;
    else
        return false;
}

bool i2cReadMemory(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t memoryAddress, uint8_t *data, size_t length) {
    if(HAL_I2C_Mem_Read(i2cHandler, deviceAddress << 1, memoryAddress, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT) == HAL_OK)
        return true;
    else
        return false;
}
