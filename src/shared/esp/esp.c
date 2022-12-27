#include "esp.h"
#include "debug.h"
#include "string/string.h"

usartHandle *Connection;

int SendCommandAndWait(char *cmd, uint32_t Timeout) {
    usartWriteLine(Connection, cmd);
    char strBuffer[ESP_RESPONSE_BUFFER_LENGTH];

    uint16_t Holder = HAL_GetTick();
    while ((HAL_GetTick() - Holder) <= Timeout)
    {
        usartReadLine(Connection, strBuffer, ESP_RESPONSE_BUFFER_LENGTH, Timeout);
        printf("%s\n", strBuffer);

        if(strCompare(strBuffer, "ERROR") == 0)
            return 0;
        if(strCompare(strBuffer, "OK") == 0)
            return 1;
    }
    return 0;
}

#define SendCommand(CMD) SendCommandAndWait(CMD, ESP_DEFAULT_RESPONSE_DELAY)


void ResetESP(GPIO_TypeDef *ResetGPIO, uint16_t ResetPin) {
    HAL_GPIO_WritePin(ResetGPIO, ResetPin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(ResetGPIO, ResetPin, GPIO_PIN_SET);
    HAL_Delay(1500);
}

//*****************************************************************************************

void espInit(usartHandle *handle, GPIO_TypeDef *ResetGPIO, uint16_t ResetPin) {
    debugInfo("Initialize ESP-AT...");
    Connection = handle;

    ResetESP(ResetGPIO, ResetPin);

    if(!SendCommand("AT"))
        debugInfo("ESP-AT Initialization failed!!!");

    SendCommand("ATE0");
}

bool espWifiConnect(char *SSID, char *Password) {
    debugInfo("Set station Mode ...");
    SendCommand("AT+CWMODE=1");//Set mode to wifi station
    
    debugInfo("Connect to Access point ...");
    char ConnectionString[100];
    strConcat(ConnectionString, 100, 5, "AT+CWJAP=\"", SSID, "\",\"", Password, "\"");
    if(!SendCommandAndWait(ConnectionString, ESP_AP_CONNECTION_DELAY)) {
        debugInfo("Access Point Connection failed!!!");
        return false;
    }
    else
        debugInfo("Access Point Connected!!!");

    return true;
}
