#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "clock.h"
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
    HAL_Init();
    clockConfig();
    gpioInit();

    usartHandle usartESP = {0},
                usartDebug = {0};

    usartInit(&usartESP, USART1, 115200);
    usartInit(&usartDebug, USART2, 115200);
    usartEnableDebug(&usartDebug);

    printf("Board Start!!!\n");

    espInit(&usartESP, GPIOB, GPIO_PIN_8);
    espWifiConnect("esp32", "123456789");
    espStartPassThroughUDP("192.168.11.157", 4000, 4000);

    //empty usart buffer
    usartFlushBuffer(&usartESP);
    
    //just a test
    usartWrite(&usartESP, (uint8_t *)"012345678901234567890123456789012345", 36);

    while(1){
        ServerMessage ReceptionBuffer;
        if(usartRead(&usartESP, (uint8_t *)&ReceptionBuffer, 2, 500)) {
            printf("Message received: %d, %d\n", ReceptionBuffer.MessageType, ReceptionBuffer.Data);
        }
        gpioToggleHealthLED();
    }
}
