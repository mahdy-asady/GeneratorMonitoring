#include "rht.h"
#include "debug.h"

#define HTU21D_ADDR             0x40
#define RHT_TEMERATURE_ADDR     0xE3
#define RHT_HUMIDITY_ADDR       0xE5



uint16_t rhtReadValue(I2C_HandleTypeDef *i2cHandler, uint8_t sensor) {
    uint8_t i2cResult[3];
    
    if(!i2cWrite(i2cHandler, HTU21D_ADDR, &sensor, 1)) {
        debugError("I2C Write Error on RTH");
        return 0;
    }

    if(!i2cRead(i2cHandler, HTU21D_ADDR, (uint8_t *)&i2cResult, 3)) {
        debugError("I2C Read Error on RTH");
        return 0;
    }

    return (i2cResult[0] << 8) | (i2cResult[1] & 0xFC);
}

uint16_t rhtReadTemerature(I2C_HandleTypeDef *i2cHandler) {
    return rhtReadValue(i2cHandler, RHT_TEMERATURE_ADDR);
}

uint16_t rhtReadHumidity(I2C_HandleTypeDef *i2cHandler) {
    return rhtReadValue(i2cHandler, RHT_HUMIDITY_ADDR);
}