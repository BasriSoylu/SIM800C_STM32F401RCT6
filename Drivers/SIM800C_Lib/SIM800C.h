/*
 * SIM800C.h
 *
 *  Created on: Apr 15, 2025
 *      Author: SOYLU
 */

#ifndef SIM800C_LIB_SIM800C_H_
#define SIM800C_LIB_SIM800C_H_

#include "main.h"
#include <stdbool.h>
#include <stdint.h>
#include "sim800c_adapter.h"

#define SIM800_RX_BUFFER_SIZE   100
#define SIM800_TX_BUFFER_SIZE   100
#define COMMAND_COUNT (sizeof(sim800cSteps) / sizeof(sim800cSteps[0]))



typedef enum {
	CMD_START,
	CMD_EKO_0,
    CMD_CMEE,
    CMD_CPIN,
	CMD_COMPLATED,
    CMD_DONE,
	CMD_ERROR
}Sim800CommandState;



typedef struct {

	    char rxBuffer[SIM800_RX_BUFFER_SIZE];
	    char txBuffer[SIM800_TX_BUFFER_SIZE];

	    uint16_t rxBufferSize;
	    uint16_t txBufferSize;

	    uint8_t rxChar;
	    uint8_t rxIndex;
	    uint8_t responseReady;

	    uint32_t lastCommandTime;
	    Sim800CommandState commandState;

	    void *sim800cUart;
	    void *debugUart;

	    int (*UartTransmit)(void *huart, const uint8_t *data, uint16_t size, uint32_t timeout);
	    int (*UartReceive_IT)(void *huart, uint8_t *data, uint16_t size);
	    uint32_t (*GetTick)(void);
	    void (*DelayMs)(uint32_t ms);

} SIM800C_Handle;




typedef enum {
    STATE_INIT,          // Başlangıç
    STATE_SEND_COMMAND,  // AT komutu gönderilecek
    STATE_WAIT_RESPONSE, // Cevap bekleniyor
    STATE_PROCESS_OK,    // OK cevabı alındı
    STATE_PROCESS_ERR,   // ERROR cevabı alındı
    STATE_DONE,          // Tüm işlemler tamam
    STATE_ERROR          // Hata durumu
} SIM800C_State;

typedef struct {
    SIM800C_State state;
    uint8_t commandIndex;
} SIM800C_FSM;

typedef struct {
    const char *command;
    const char *successMsg;
} SIM800C_CommandStep;






void SIM800C_Init(SIM800C_Handle *hSim, void *sim800cUart, void *debugUart);
void SIM800C_Loop(SIM800C_Handle *hSim, uint32_t sampleTime);
void SIM800C_RxCpltCallback(SIM800C_Handle *hSim);



#endif /* SIM800C_LIB_SIM800C_H_ */
