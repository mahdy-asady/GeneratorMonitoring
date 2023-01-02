#ifndef __I2C_H__
#define __I2C_H__

#include <stdbool.h>
#include "stm32f1xx_hal.h"


void i2cInit(I2C_HandleTypeDef *i2cHandler, I2C_TypeDef *I2C);

bool i2cWrite(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t *data, size_t length);

bool i2cRead(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t *data, size_t length);



bool i2cWriteMemory(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t memoryAddress, uint8_t *data, size_t length);

bool i2cReadMemory(I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint8_t memoryAddress, uint8_t *data, size_t length);


#endif
