#include "esp.h"
#include "debug.h"

usartHandle *Connection;


int StrCompare(char* String1, char *String2) {
    while(1) {
        if(*String1 == 0 && *String2 == 0)
            return 0;
        
        if(*String1 < *String2)
            return -1;
        else if(*String1 > *String2)
            return 1;
        String1++;
        String2++;
    }
}


int SendCommandAndWait(char *cmd, uint32_t Timeout) {
    usartWriteLine(Connection, cmd);
    char strBuffer[ESP_RESPONSE_BUFFER_LENGTH];

    uint16_t Holder = HAL_GetTick();
    while ((HAL_GetTick() - Holder) <= Timeout)
    {
        usartReadLine(Connection, strBuffer, ESP_RESPONSE_BUFFER_LENGTH);
        printf("%s\n", strBuffer);

        if(StrCompare(strBuffer, "ERROR") == 0)
            return 0;
        if(StrCompare(strBuffer, "OK") == 0)
            return 1;
    }
    return 0;
}

#define SendCommand(CMD) SendCommandAndWait(CMD, ESP_DEFAULT_RESPONSE_DELAY)

//*****************************************************************************************

void espInit(usartHandle *handle) {
    debugInfo("Initialize ESP-AT...");
    Connection = handle;

    if(!SendCommand("AT"))
        debugInfo("ESP-AT Initialization failed!!!");

    SendCommand("ATE0");
}