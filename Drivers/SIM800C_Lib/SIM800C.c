/*
 * SIM800C.c
 *
 *  Created on: Apr 15, 2025
 *      Author: SOYLU
 */
#include "SIM800C.h"
#include <string.h>
#include <stdio.h>


SIM800C_CommandStep sim800cSteps[] = {
    { "AT",        "SIM800C is Started!\r\n" },
    { "ATE0",      "EKO Mode Off!\r\n"       },
    { "AT+CMEE=2", "CMEE Mode On!\r\n"       },
    { "AT+CPIN?",  "Sim Card is OK!\r\n"     }
};



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
    hSim->commandState    = CMD_START;
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




void SIM800C_Loop(SIM800C_Handle *hSim, uint32_t sampleTime)
{
    static SIM800C_FSM fsm = { .state = STATE_INIT, .commandIndex = 0 };

    switch (fsm.state)
    {
        case STATE_INIT:
            fsm.commandIndex = 0;
            fsm.state = STATE_SEND_COMMAND;
            break;

        case STATE_SEND_COMMAND:
            if ((hSim->GetTick() - hSim->lastCommandTime) > sampleTime && fsm.commandIndex < COMMAND_COUNT) {
                SIM800C_SendATCommand(hSim, sim800cSteps[fsm.commandIndex].command);
                hSim->lastCommandTime = hSim->GetTick();
                fsm.state = STATE_WAIT_RESPONSE;
            }
            break;

        case STATE_WAIT_RESPONSE:
            if (hSim->responseReady)
            {
                hSim->responseReady = false;

                if (strstr(hSim->rxBuffer, "OK"))
                {
                    fsm.state = STATE_PROCESS_OK;
                    SIM800C_RxBuff_Clear(hSim);
                }
                else if (strstr(hSim->rxBuffer, "ERROR"))
                {
                    fsm.state = STATE_PROCESS_ERR;
                }

            }
            break;

        case STATE_PROCESS_OK:
            SIM800C_DebugPrint(hSim, sim800cSteps[fsm.commandIndex].successMsg);
            fsm.commandIndex++;
            fsm.state = (fsm.commandIndex >= COMMAND_COUNT) ? STATE_DONE : STATE_SEND_COMMAND;
            break;

        case STATE_PROCESS_ERR:
            SIM800C_DebugPrint(hSim, "---------------- Command Error -------------");
            SIM800C_DebugPrint(hSim, hSim->rxBuffer);
            SIM800C_DebugPrint(hSim, "------------------------------------------------------\r\n");
            SIM800C_RxBuff_Clear(hSim);
            fsm.state = STATE_ERROR;
            break;

        case STATE_DONE:
            SIM800C_DebugPrint(hSim, "All SIM800C Commands Done.\r\n");
            // FSM burada beklemede kalabilir veya tekrar başlatılabilir.
            break;

        case STATE_ERROR:
            //SIM800C_DebugPrint(hSim, "FSM entered ERROR state!\r\n");
            // Hata işleme yapılabilir (reset, retry vs.)
            break;
    }
}


//void SIM800C_Loop(SIM800C_Handle *hSim, uint32_t sampleTime)
//{
//
//    if ((hSim->GetTick() - hSim->lastCommandTime) > sampleTime) {
//
//        switch (hSim->commandState) {
//
//			case CMD_START:
//				SIM800C_SendATCommand(hSim, "AT");
//				break;
//
//			case CMD_EKO_0:
//				SIM800C_SendATCommand(hSim, "ATE0");
//				break;
//
//            case CMD_CMEE:
//                SIM800C_SendATCommand(hSim, "AT+CMEE=2");
//                break;
//
//            case CMD_CPIN:
//                SIM800C_SendATCommand(hSim, "AT+CPIN?");
//                break;
//
//            case CMD_COMPLATED:
//
//                break;
//
//
//            default:
//                break;
//        }
//        hSim->lastCommandTime =  hSim->GetTick();
//    }
//
//
//	if (hSim->responseReady) {
//
//		hSim->responseReady = false;
//
//		if (strstr(hSim->rxBuffer, "OK")){
//
//			switch (hSim->commandState){
//
//			   case CMD_START:
//				  SIM800C_DebugPrint(hSim, "SIM800C is Started!\r\n");
//			      hSim->commandState = CMD_EKO_0;
//			   break;
//
//			   case CMD_EKO_0:
//				  SIM800C_DebugPrint(hSim, "EKO Mode Off!\r\n");
//				  hSim->commandState = CMD_CMEE;
//			   break;
//
//			   case CMD_CMEE:
//				   SIM800C_DebugPrint(hSim, "CMEE Mode On!\r\n");
//				   hSim->commandState = CMD_CPIN;
//			   break;
//
//			   case CMD_CPIN:
//				   SIM800C_DebugPrint(hSim, "Sim Card is OK!\r\n");
//				   hSim->commandState = CMD_COMPLATED;
//			   break;
//
//			   default:
//			   break;
//			}
//
//		}
//		else if (strstr(hSim->rxBuffer, "ERROR")){
//
//		   switch (hSim->commandState){
//
//			   case CMD_START:
//				  SIM800C_DebugPrint(hSim, "SIM800C is Fault! : ");
//				  SIM800C_DebugPrint(hSim, hSim->rxBuffer);
//				   hSim->commandState = CMD_ERROR;
//			   break;
//
//			   case CMD_EKO_0:
//				   SIM800C_DebugPrint(hSim, "EKO_0 Mode is Fault! : ");
//				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
//				   hSim->commandState = CMD_ERROR;
//			   break;
//
//			   case CMD_CMEE:
//				   SIM800C_DebugPrint(hSim, "CMEE Mode is Fault! : ");
//				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
//				   hSim->commandState = CMD_ERROR;
//			   break;
//
//			   case CMD_CPIN:
//				   SIM800C_DebugPrint(hSim, "CPIN is Fault! : ");
//				   SIM800C_DebugPrint(hSim, hSim->rxBuffer);
//				   hSim->commandState = CMD_ERROR;
//			   break;
//
//			   default:
//			   break;
//		   }
//
//		}
//
//       SIM800C_RxBuff_Clear(hSim);
//    }
//
//}




