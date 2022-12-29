#ifndef __RHT_H__
#define __RHT_H__

#include <stdint.h>
#include "i2c.h"

uint16_t rhtReadTemerature(I2C_HandleTypeDef *i2cHandler);

uint16_t rhtReadHumidity(I2C_HandleTypeDef *i2cHandler);

#endif