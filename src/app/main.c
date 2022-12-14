#include <stdio.h>

#include "config.h"
#include "board.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "esp/esp.h"


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
    
    boardInit(&usartDebug, &usartESP, &i2cHandler, &timerHandler);

    printf("\n\nBoard Start!!!\n");

    espWifiConnect(WIFI_SSID, WIFI_PASS);
    espStartPassThroughUDP(SERVER_ADDR, SERVER_PORT, UDP_LISTEN_PORT);
    
    //just a test
    usartWrite(&usartESP, (uint8_t *)"012345678901234567890123456789012345", 36);

    while(1){
        /*ServerMessage ReceptionBuffer;
        if(usartRead(&usartESP, (uint8_t *)&ReceptionBuffer, 2, 500)) {
            printf("Message received: %d, %d\n", ReceptionBuffer.MessageType, ReceptionBuffer.Data);
        }

        printf("Buttons: %d, %d\n\n", boardReadDigit1(), boardReadDigit2());
        
*/
        boardToggleHealthLED();
        HAL_Delay(500);
    }
}
