#include <stdio.h>

#include "board.h"

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "i2c.h"
#include "esp/esp.h"
#include "rht.h"
#include "MotionTracking.h"


enum MessageTypes {
    MessageAck = 1,
    MessageData
};

typedef struct {
    enum MessageTypes MessageType;
    uint8_t Data;
} ServerMessage;

/* Main routine*/
int main(void) {
    usartHandle usartESP = {0},
                usartDebug = {0};
    
    I2C_HandleTypeDef i2cHandler = {0};

    TIM_HandleTypeDef timerHandler = {0};
    
    boardInit();
    boardInitUsartDebug(&usartDebug);

    printf("\n\nBoard Start!!!\n");

    boardInitUsartEsp(&usartESP);
    boardInitI2C(&i2cHandler);
    boardInitEsp(&usartESP);
    boardInitTimer(&timerHandler);

    MotionTrackingInit(&i2cHandler);

    espWifiConnect("esp32", "123456789");
    espStartPassThroughUDP("192.168.11.157", 4000, 4000);
    
    //just a test
    usartWrite(&usartESP, (uint8_t *)"012345678901234567890123456789012345", 36);

    while(1){
        ServerMessage ReceptionBuffer;
        if(usartRead(&usartESP, (uint8_t *)&ReceptionBuffer, 2, 500)) {
            printf("Message received: %d, %d\n", ReceptionBuffer.MessageType, ReceptionBuffer.Data);
        }
        
        printf("Temperature data: %d\n", rhtReadTemerature(&i2cHandler));
        printf("Humidity data: %d\n", rhtReadHumidity(&i2cHandler));

        printf("Gyro:\tX: %d\tY:%d\tZ:%d",  MotionTrackingReadGyroX(&i2cHandler), 
                                            MotionTrackingReadGyroY(&i2cHandler), 
                                            MotionTrackingReadGyroZ(&i2cHandler));

        printf("\t\tAccel:\tX: %d\tY:%d\tZ:%d\n",  MotionTrackingReadAccX(&i2cHandler), 
                                             MotionTrackingReadAccY(&i2cHandler), 
                                             MotionTrackingReadAccZ(&i2cHandler));

        boardToggleHealthLED();
    }
}
