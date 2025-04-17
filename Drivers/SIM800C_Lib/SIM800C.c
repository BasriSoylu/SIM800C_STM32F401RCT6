/*
 * SIM800C.c
 *
 *  Created on: Apr 15, 2025
 *      Author: SOYLU
 */
#include "SIM800C.h"
#include <string.h>
#include <stdio.h>



static void SIM800C_SendATCommand(SIM800C_Handle *hSim, const char *cmd)
{
    snprintf(hSim->txBuffer, hSim->txBufferSize, "%s\r\n", cmd);
    hSim->UartTransmit(hSim->sim800cUart, (uint8_t *)hSim->txBuffer, strlen(hSim->txBuffer), 1000);
}



static void SIM800C_DebugPrint(SIM800C_Handle *hSim, const char *msg)
{
	hSim->UartTransmit(hSim->debugUart, (uint8_t *)msg, strlen(msg), 1000);
}


static void SIM800C_RxBuff_Clear(SIM800C_Handle *hSim)
{
    memset(hSim->rxBuffer, 0, strlen(hSim->rxBuffer));
    hSim->rxIndex = 0;
}

void SIM800C_Init(SIM800C_Handle *hSim, void *sim800cUart, void *debugUart)
{
	hSim->sim800cUart     = sim800cUart;
	hSim->debugUart       = debugUart;
	hSim->UartTransmit    = SIM800C_UART_Transmit_Adapter;
	hSim->UartReceive_IT  = SIM800C_UART_Receive_IT_Adapter;
	hSim->GetTick         = SIM800C_GetTick_Adapter;
	hSim->DelayMs         = SIM800C_Delay_Adapter;
    hSim->rxBufferSize    = SIM800_RX_BUFFER_SIZE;
    hSim->txBufferSize    = SIM800_TX_BUFFER_SIZE;

    hSim->rxIndex = 0;
    hSim->responseReady   = false;
    hSim->commandState    = CMD_NONE;
    hSim->lastCommandTime = hSim->GetTick();

    hSim->UartReceive_IT(hSim->sim800cUart, &hSim->rxChar, 1);
}


void SIM800C_RxCpltCallback(SIM800C_Handle *hSim)
{
    if (hSim->rxIndex < sizeof(hSim->rxBuffer) - 1) {

        hSim->rxBuffer[hSim->rxIndex++] = hSim->rxChar;

        if (hSim->rxChar == '\n') {

            hSim->rxBuffer[hSim->rxIndex] = '\0';
            hSim->responseReady = true;
        }

    }
    else {
        SIM800C_DebugPrint(hSim, "RX Buffer Overflow!\r\n");
    }

    hSim->UartReceive_IT(hSim->sim800cUart, &hSim->rxChar, 1);

}



void SIM800C_Loop(SIM800C_Handle *hSim)
{

    if ((hSim->GetTick() - hSim->lastCommandTime) > 1000) {

        switch (hSim->commandState) {

			case CMD_NONE:
				SIM800C_SendATCommand(hSim, "ATE0");
				hSim->commandState = CMD_EKO_0;
				break;

            case CMD_CMEE:
                SIM800C_SendATCommand(hSim, "AT+CMEE=2");
                break;

            case CMD_CPIN:
                SIM800C_SendATCommand(hSim, "AT+CPIN?");
                break;

            case CMD_COMPLATED:

                break;


            default:
                break;
        }
        hSim->lastCommandTime =  hSim->GetTick();
    }


	if (hSim->responseReady) {

		hSim->responseReady = false;

		if (strstr(hSim->rxBuffer, "OK")){

			switch (hSim->commandState){

			   case CMD_EKO_0:
				  hSim->commandState = CMD_CMEE;
				  SIM800C_DebugPrint(hSim, "EKO Mode Off!\r\n");
			   break;

			   case CMD_CMEE:
				   hSim->commandState = CMD_CPIN;
				   SIM800C_DebugPrint(hSim, "CMEE Mode On!\r\n");
			   break;

			   case CMD_CPIN:
				   hSim->commandState = CMD_COMPLATED;
				   SIM800C_DebugPrint(hSim, "Sim Card is OK!\r\n");
			   break;

			   default:
				   break;
			}

		}
		else if (strstr(hSim->rxBuffer, "ERROR")){

		   switch (hSim->commandState){

			   case CMD_EKO_0:
				   SIM800C_DebugPrint(hSim, "EKO_0 Mode is Fault! : ");
				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
				   hSim->commandState = CMD_ERROR;
			   break;

			   case CMD_CMEE:
				   SIM800C_DebugPrint(hSim, "CMEE Mode is Fault! : ");
				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
				   hSim->commandState = CMD_ERROR;
			   break;

			   case CMD_CPIN:
				   SIM800C_DebugPrint(hSim, "CPIN is Fault! : ");
				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
				   hSim->commandState = CMD_ERROR;
			   break;

			   default:
				   break;
		   }
		}

       SIM800C_RxBuff_Clear(hSim);
    }


}




